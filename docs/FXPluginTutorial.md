# Making an audio fx plugin

## Anatomy

### Engine, Editor and SharedState

A plugin in mostly harmless is divided into 3 distinct areas:

- `Engine`: Contains all audio-thread related functions.
- `Editor`: Contains all gui related functions.
- `SharedState`: Owns any data needed by both the `Engine` and `GUI` classes.

`SharedState` can loosely be thought of as the Model, and `Engine` and `Editor` can loosely be thought of as views
into `SharedState`.
<br>The reasoning behind this is to force a separation of concerns between different parts of the plugin, by making
the `Engine` and `GUI`
completely independent of one another - any comms between the two should take place through `SharedState`, via instances
of `marvin::containers::fifos::SPSC`.

The framework provides mostly-abstract base classes for the user to implement for each of these areas of the
plugin, `ISharedState`, `IEngine`, and `IEditor`.

`ISharedState` requires some extra variables passed from the internal framework, for various useful tasks (running a
function on the message thread, and requesting a param flush).
These extras are encapsulated in `SharedStateContext`, which we'll touch on in the next section.

### PluginEntry

As the user needs to inform the framework of their custom derived `SharedState`, `Engine` and `Editor` classes, we also
provide an interface to specify them - `IPluginEntry`, and a macro (`MH_REGISTER_PLUGIN_ENTRY(EntryType)`) to register
the user's
custom `PluginEntry` with the framework.

The framework does *not* enforce that derived classes must take a pointer to an `ISharedState` to the `Engine`
and `Editor` constructors,
and that is instead left up to the user to implement - the reasoning being to avoid annoying downcasts every time the
custom `SharedState` class is needed within the other derived types.

However, the function prototypes in `IPluginEntry` for `createEngine` and `createEditor` both take an `ISharedState*`
parameter, which the user can cast to their custom type,
and pass to their derived `Engine` and `Editor` classes.

As mentioned above, `ISharedState` needs an instance of `SharedStateContext` passed to its constructor. This is passed
by rvalue ref to
`IPluginEntry`'s `createSharedState` function, but it is the user's responsibility to forward this arg to
their `SharedState` class' base constructor.

## Setting it up

As mentioned above, in order to create a plugin, the user needs to implement `ISharedState`, `IEngine`, `IEditor`,
and `IPluginEntry`.

### SharedState

Lets start with the basic boilerplate for our custom `SharedState` class.

```cpp
// SharedState.h
#include <mostly_harmless/core/mostlyharmless_ISharedState.h>
namespace myplugin { 
    class SharedState final : public mostly_harmless::core::ISharedState { // [1] 
    public: 
        explicit SharedState(mostly_harmless::core::SharedStateContext&& context); // [2]
        
    private: 
        void loadState(std::string_view toLoad) override; // [3]
        void saveState(std::ostringstream& dest) override;
    };
}
```

[1] Here we create a class called `SharedState`, deriving from the framework's `ISharedState` interface.

[2] As explained
earlier, we take an rvalue ref to
a `SharedStateContext`, to be passed to the base class' constructor.

[3] We also override some pure virtual functions from
the base, `loadState` and `saveState`.
These will be called when a host restore's the plugin state, or needs to store the plugin's state respectively.

Next we'll define our functions in a source file.

```cpp
// SharedState.cpp
namespace myplugin {
    std::vector<mostly_harmless::Parameter<float>> createParams() { // [1]
        std::vector<mostly_harmless::Parameter<float>> params;
        // Leave this empty for now
        return params;
    }

    SharedState::SharedState(mostly_harmless::core::SharedStateContext&& context) : mostly_harmless::core::ISharedState(std::move(context), createParams()) { // [2]
    }

    void SharedState::loadState(std::string_view toLoad) { // [3]
    }

    void SharedState::saveState(std::ostringstream& dest) {
    }
}
```

[1] `ISharedState` also takes a `std::vector<Parameter<float>>` as its second constructor arg - this is to register any
parameters that may exist with the host internally.
For this purpose we create a TU scoped free function to create the params, and for the time being, return an empty
vector.

[2] Here we pass our `context` arg, and a newly created vector of params (as explained above) to our base class'
constructor.

[3] Finally, we implement our `loadState` and `saveState` functions we declared in our header, and leave them empty for
now.

We'll return to our `SharedState` class in a little while once the rest of the boilerplate has been set up. For now,
let's move on to the `Engine` class.

### Engine

```cpp
// Engine.h
#include "SharedState.h"
#include <mostly_harmless/core/mostlyharmless_IEngine.h>
namespace myplugin { 
    class IEngine final : public mostly_harmless::core::IEngine { // [1]
    public: 
        explicit Engine(SharedState* sharedState); // [2]
        void initialise(double sampleRate, std::uint32_t minBlockSize, std::uint32_t maxBlockSize) override; // [3]
        void process(marvin::containers::BufferView<float> buffer, std::optional<mostly_harmless::TransportState> transport) override; // [4]
        void reset() override; // [5]
    private: 
        SharedState* m_sharedState{ nullptr }; // [2]
    };
}
```

[1] We create an `Engine` class, deriving from `IEngine`.

[2] We take a single argument to our constructor, a pointer to our previously set-up `SharedState` class. We also
declare a member variable to store the passed pointer in.

[3] We override `initialise`, a pure virtual from `IEngine`, which is called to set up audio processing, before the
first call to `process`. This can be called by any thread by the host,
but is guaranteed to not happen concurrently with `process` or `reset`.

[4] We override `process`, a pure virtual from `IEngine`, which is called on the audio thread by the host to perform
some audio processing.

[5] We override `reset`, a pure virtual from `IEngine`, which is called to clear any existing internal state by the
host, to prepare to be able to play again. This can be called by any thread by the host,
but is guaranteed to not happen concurrently with `process` or `initialise`.

Next we'll implement all of the above in a source file.

```cpp 
#include "Engine.h"
namespace myplugin { 
    Engine::Engine(SharedState* sharedState) : m_sharedState(sharedState) { // [1]
    
    }
    
    void Engine::initialise(double sampleRate, std::uint32_t minBlockSize, std::uint32_t maxBlockSize) { // [2]
    
    }
    
    void Engine::process(marvin::container::BufferView<float> buffer, std::optional<mostly_harmless::TransportState> transport) { // [3]
    
    }
    
    void Engine::reset() { // [4]
    
    }
}
```

[1] Here we store the passed `SharedState` instance in our `m_sharedState` member, for access later on.

[2] We implement `initialise`, which takes the host's sample rate (`sampleRate`), the smallest block size the host can
pass (`minBlockSize`),
and the largest block size the host can pass (`maxBlockSize`).

[3] We implement `process`, which takes a non-owning-view into the buffer passed by the host (`buffer`), and an optional
representing the transport state, if it is available (`transport`).

[4] We implement `reset`, as detailed above.

Next, let's implement our `Editor` class.

### Editor

The editor class is a bit more complex than all of the above. Mostly harmless doesn't come with a gui framework bolted
on, ala JUCE, and instead expects the user to
implement whatever gui framework they like, via the hooks in the `IEditor` class.

That being said, the framework provides abstractions over `IEditor` for a webview gui, with varying degrees of
opinionated-ness. More frameworks are planned here, such as Slint, but
for now, Webviews are the only fully featured option out of the box.

A full javascript / html / css / trendy web framework tutorial here is obviously out of scope, so we'll instead assume
that some gui project exists within our current project, and we'll focus on
the C++ side setup to call into this imaginary gui project. Down the line, a tutorial is planned for the full setup, but
for now, the `Gain` example will have to suffice, which uses React and Vite.

Disclaimers aside, let's talk specifics on the levels of abstraction mostly harmless provides:

- `IEditor` is the aforementioned interface, with no additional functionality.
- `WebviewBase` handles cross platform window management, and creates a webview for the user to use as they see fit. It
  does *not* handle bidirectional parameter communications for the user, and exists to serve as an unopinionated
  abstraction layer over `IEditor`.
- `WebviewEditor` derives from `WebviewBase`, and takes things a step further. It establishes bindings to javascript
  events for bidirection parameter comms for the user, and handles the majority of the plumbing between C++ and
  Javascript.

In this tutorial, we'll use `WebviewEditor` for simplicity.

```cpp
// Editor.h
#include "SharedState.h"
#include <mostly_harmless/gui/mostlyharmless_WebviewEditor.h>
namespace myplugin { 
    class Editor final : public mostly_harmless::gui::WebviewEditor {  // [1]
    public: 
        explicit Editor(SharedState* sharedState); // [2]
        void initialise() override; // [3]
    private: 
        constexpr static auto m_windowWidth{ 500 }; // [4]
        constexpr static auto m_windowHeight{ 500 };    
        const mostly_harmless::gui::Colour m_backgroundColour{ 0xFF000000 };
    };
}
```

[1] We declare an `Editor` class, deriving from `WebviewEditor`, as discussed above.

[2] Our constructor again takes a pointer to our `SharedState` class. Because we're deriving from `WebviewEditor`, which
actually *does* require an `ISharedState*` passed to its constructor, we in this case don't keep this pointer as a
member.

[3] We implement the `initialise` function, which is *not* pure virtual, but is where we need to inject any more
functionality for after the webview has been created.

[4] We define some compile time constants for our plugin window's width, height and background colour as members.

Next lets implement all of this in a source file.

```cpp 
#include "Editor.h"
namespace myplugin { 
    Editor::Editor(SharedState* sharedState) : 
     mostly_harmless::gui::WebviewEditor(sharedState, m_width, m_height, m_backgroundColour) { // [1]
        setOptions({.enableDebug = true }); // [2]
    }
    
    void Editor::initialise() { 
        mostly_harmless::gui::WebviewEditor::initialise(); // [3]
        m_internalWebview->navigate("http://localhost:5173"); // [4]
    }
}
```

[1] Here we call our `WebviewEditor` class' constructor, which takes our pointer to an `ISharedState`, our desired
window width and height, and our desired window's background colour.

[2] For now, we just enable inspect-element functionality on the webview - more options are available here, which we'll
discuss later.

[3] `WebviewEditor`'s `initialise` function is *not* pure virtual, so we still need to call it before doing our own
custom behaviour in `initialise`.

[4] For now, we load our gui from localhost - in a later tutorial, we'll talk through instead serving the webview from
ram.

We're nearly done with our boilerplate, the last thing we need to do is register our derived classes with the framework.

### PluginEntry

As discussed earlier, `IPluginEntry` is our interface for doing so.

```cpp
// PluginEntry.h
#include "SharedState.h"
#include "Engine.h"
#include "Editor.h"
#include <mostly_harmless/core/mostlyharmess_IPluginEntry.h>

namespace myplugin { 
    struct PluginEntry final : public mostly_harmless::core::IPluginEntry { 
    public:    
        std::unique_ptr<mostly_harmless::core::ISharedState> createState(mostly_harmless::core::SharedStateContext&& context) override; 
        std::unique_ptr<mostly_harmless::Core::IEngine> createEngine(mostly_harmless::core::ISharedState* sharedState) override;
        std::unique_ptr<mostly_harmless::core::IEditor> createEditor(mostly_harmless::core::ISharedState* sharedState) override; 
    };
}

```

Our header is relatively self explanatory, so let's implement those functions in our source file.

```cpp
// PluginEntry.cpp
#include "PluginEntry.h"
namespace myplugin { 
    SharedState* asUserState(mostly_harmless::core::ISharedState* sharedState) { // [1]
        return static_cast<SharedState*>(sharedState);
    }
    
    std::unique_ptr<mostly_harmless::core::ISharedState> PluginEntry::createState(mostly_harmless::core::SharedStateContext&& context) { 
        return std::make_unique<SharedState>(std::move(context)); // [2]
    } 
    
    std::unique_ptr<mostly_harmless::Core::IEngine> PluginEntry::createEngine(mostly_harmless::core::ISharedState* sharedState) { 
        return std::make_unique<Engine>(asUserState(sharedState)); // [3]
    }
    
    std::unique_ptr<mostly_harmless::core::IEditor> PluginEntry::createEditor(mostly_harmless::core::ISharedState* sharedState) { 
        return std::make_unique<Editor>(asUserState(sharedState)); // [4]
    }
    
}
MH_REGISTER_PLUGIN_ENTRY(myplugin::PluginEntry); // [5]
```

[1] We declare a TU scoped helper function to avoid verbose static casts, for downcasting our `ISharedEditor` points to
our user `SharedState` class.

[2] We create our user `SharedState` class, forwarding it the `context` arg by move.

[3] We create our `Engine` class, passing it a downcast-to-user-state of `sharedState`.

[4] We create our `Editor` class, passing it a downcast-to-user-state of `sharedState`.

[5] Finally, we call a macro to register this `PluginEntry` class with the framework. Internally this defines a free
function, `createPluginEntry`, which returns our user `PluginEntry` type.
The internal framework class then uses its hooks to create `SharedState`, `Engine` and `Editor` classes, and forwards
relevant function calls to the appropriate places within there classes.

## Getting it building

Mostly harmless uses cmake, and provides some CMake helpers to make setting up your plugin a little more painless.

Familiarity with CMake is assumed here, and some typical options have been ommitted for the sake of brevity (macOS arch
to build, windows runtime linking type, codesigning, etc).

```cmake 
cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 20)
project(MyPlugin VERSION 0.0.1)
include(FetchContent)
FetchContent_Declare(mostly_harmless
        GIT_REPOSITORY https://github.com/SLM-Audio/mostly-harmless.git
        GIT_TAG main
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(mostly_harmless) # [1]

mostly_harmless_add_plugin(MyPlugin # [2]
        ID "yourcompany.yourplugin"
        VENDOR "Your Company"
        FORMATS CLAP AU VST3 Standalone # [3]
        AU_BUNDLE_ID "com.yourcompany.yourplugin"
        AU_BUNDLE_VERSION "1"
        MANUFACTURER_CODE "Yrco"
        SUBTYPE_CODE "Ypi1"
        AU_TYPE "aufx"
        FEATURES "audio-effect"
)

target_sources(MyPlugin PRIVATE # [4]
        ${CMAKE_CURRENT_SOURCE_DIR}/source/PluginEntry.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/SharedState.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/Engine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/Editor.cpp
)

target_include_directories(MyPlugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source) # [5]
```

[1] We pull in mostly harmless via fetch content - in real life, you probably want to pin this to a specific SHA or tag.

[2] Mostly harmless provides `mostly_harmless_add_plugin`, which will set up the specified plugin targets for you, and
automatically handle the internal linking, etc.

[3] This is where we specify what formats we want to build. Note that AU requires some extra arguments that the other
targets don't - see README.md for more info on this.

[4] We specify what source files the plugin should compile.

[5] We set the `source` directory as an include directory for convenience.

At this point, you should be at a stage where your plugin is compiling!






