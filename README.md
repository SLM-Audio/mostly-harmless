# mostly harmless

## About

mostly harmless is a minimal audio plugin framework for C++20. It's first and foremost a wrapper around the CLAP API,
capable of building CLAP, VST3, Audio Unit and Standalone targets.
> If you're compiling VST3 targets, the responsibility to ensure you're adhering to the Steinberg VST3 License agreement
> is firmly on you!

The framework is in active development, alongside some of our own plug-ins, and there are some notable missing features,
so if you're thinking of giving it a go, please bare in mind that we're still
working on it.<br>
If you have any questions, the best way to reach me is
via [The Audio Programmer Discord](https://discord.gg/v5Rs8h6aGF), I'm @Meijis.

## Documentation

Documentation is built automatically, and hosted [here](https://mostly-harmless.pages.dev). For convenience, it also
includes the documentation for marvin.

## Features

### dsp

While the framework itself doesn't provide anything in the way of audio helpers ala something like JUCE, it ships with
our sibling dsp library [marvin](https://github.com/MeijisIrlnd/marvin),
should you need some convenience helpers and basic audio features.

### gui

The goal of the framework was to provide the bare minimum we (as in SLM Audio) need to write a plugin - and as such we
don't provide any kind of gui framework of our own.
We do, however, provide a thin wrapper around [choc](https://github.com/Tracktion/choc)'s WebView, and plan to implement
similar minimal wrappers around a couple of other frameworks.<br>
We also provide an interface with hooks for you to implement your framework of choice,
in `mostly_harmless::gui::IEditor`.

## Getting Started

### First Steps

Firstly, we'd recommend taking a look in the `examples` directory for a real-world use case, and the FX Plugin Tutorial
in the docs directory,
which talks you through some of the framework's core ideas, and how to set up a simple gain plugin project.
We also provide a (wip) python script to generate the boilerplate needed for a new project for you, found in the root
directory.<br>
Usage is

`python3 create.py <name> <dest_path> <fx | instrument> [no_gui | web | raw_gui]`

where `<>` denotes a required field, and `[]` denotes an optional field. The final field, for gui, if not provided, will
default to `no_gui`.<br>
I realise it's a little janky at the moment having positional args instead of the usual `--some_flag=something`
approach, and plan to refactor it to be a little more user friendly in future.

### Building

mostly harmless is built with CMake, and supports Windows and macOS (Linux support is on the roadmap).
It can be consumed either as a git submodule, or as a FetchContent dependency.

### Adding to your project

 ```cmake 
 include(FetchContent)
FetchContent_Declare(mostly_harmless
        GIT_REPOSITORY "https://github.com/SLM-Audio/mostly-harmless.git
 GIT_TAG main 
 GIT_SHALLOW ON)
 
 FetchContent_MakeAvailable(mostly_harmless)
```

### Adding a plugin target

We provide a convenience function to set up your plugin targets, `mostly_harmless_add_plugin`.
This function will create targets for the specified formats, and link them to the necessary dependencies. Note that if
you specify the AU format, a few more arguments become neccessary - the arguments are detailed below.

 ```cmake 
 mostly_harmless_add_plugin(YourTarget
        FORMATS CLAP # REQUIRED - At least one of: CLAP, VST3, AU or Standalone
        ID "com.whatever" # REQUIRED - The ID for your plugin
        FEATURES "audio-effect" # REQUIRED - The CLAP features your plugin uses. [1]
        VERSION "1.0.0" # OPTIONAL - Your plugin's verson.
        DESCRIPTION "We are trapped in the belly of this horrible machine" # OPTIONAL 
        #                                            - A description for your plugin
        URL "https://slm-audio.com" # OPTIONAL - A link to your website.
        MANUAL_URL "https://slm-audio.com/manual" # OPTIONAL - A link to your manual.
        SUPPORT_URL "https://slm-audio.com/hr" # OPTIONAL - A link to your support page.

        NEEDS_NOTE_IN_BUS FALSE # OPTIONAL, DEFAULTS TO FALSE - Whether your plugin accepts midi/note input.
        NEEDS_NOTE_OUT_BUS FALSE # OPTIONAL, DEFAULTS TO FALSE - Whether your plugin produces midi/note output.
        NEEDS_AUDIO_IN_BUS TRUE # OPTIONAL, DEFAULTS TO TRUE - Whether your plugin accepts audio input.
        NEEDS_AUDIO_OUT_BUS TRUE # OPTIONAL, DEFAULTS TO TRUE - Whether your plugin produces audio output.

        VENDOR "YourCompany" # REQUIRED FOR AU, OPTIONAL OTHERWISE - The name of your company.
        AU_BUNDLE_ID "com.yourcompany.plugin" # REQUIRED FOR AU, UNUSED OTHERWISE - A company id for the AU bundle.
        AU_BUNDLE_VERSION "1" # REQUIRED FOR AU, UNUSED OTHERWISE - A version for the AU bundle.
        MANUFACTURER_CODE "Brat" # REQUIRED FOR AU, UNUSED OTHERWISE - Your unique 4 character manufacturer code.
        SUBTYPE_CODE "Bpi1" # REQUIRED FOR AU, UNUSED OTHERWISE - A 4 character identifier for your plugin.
        AU_TYPE "aufx" # REQUIRED FOR AU, UNUSED OTHERWISE - The AU plugin type for your plugin. [2]
)
```

`mostly_harmless_add_plugin` will create several targets based on the arguments provided to the `FORMATS` field.
Internally, we create a static library named based on the value you provide to the `YourTarget` argument, with linkage
to the library dependencies, and then link our format targets to that -
so after the call to `mostly_harmless_add_plugin`, the usual cmake rules and techniques apply to the shared
target.<br>
For example, to add some sources to the target:

```cmake
target_sources(YourTarget PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/source/Plugin.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/PluginEditor.cpp
)
```

[1] - See [here](https://github.com/free-audio/clap/blob/main/include/clap/plugin-features.h) for a full list of
available features.<br>
[2] - These are the au specific 4 letter idents you get when you scan with auval - options are as follows:

```
    aufx - audio effect
    aufc - format converter
    augn - audio generator
    aumi - midi processor
    aumx - mixer 
    aumu - music device
    aumf - music effect
    auol - offline effect
    auou - output
    aupn - panner
```

### Binary Data

We also provide a helper to embed assets into your binary, (essentially sugar around `CMRC`) and a corresponding cmake
function to call
this, `mostly_harmless_add_binary_data`.

```cmake 
mostly_harmless_add_binary_data(YourTarget
        TARGET_NAME BinaryData
        ROOT ${CMAKE_CURRENT_SOURCE_DIR}/resources
        BINARY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources/VonDutch128.mp3
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/JamesJoyceReads_TheJungleBook.wav
)
```

This will create a static library called `BinaryData` containing your resources, and add the resulting header to your
include path.
The value you pass to the `TARGET_NAME` parameters will dictate the name of the generated static library, the resulting
filename (prefixed by `mostly_harmless`),
and the namespace the binary resources are contained in (also prefixed by `mostly_harmless`). <br><br>
The generated header provides a free function, `getNamedResource()` to access binary resources through, which takes a (
relative to root) path to a resource, and returns a `std::optional<BinaryResource>`,
where `BinaryResource` is defined as:

```cpp
struct BinaryResource final { 
    const char* data;
    size_t size;
};
```

If the queried resource isn't found, `getNamedResource()` will return `std::nullopt`.<br>
As mentioned above, the pathing to the resource is relative to the value you passed to the `ROOT` parameter in CMake.
For example, for a dir structure

```
project
    resources
        images
            BottomText.png
        text 
            Placeholder.txt
    CMakeLists.txt
```

if `ROOT` is set to `${CMAKE_CURRENT_SOURCE_DIR}/resources`, then to access `BottomText.png`:

```cpp
auto resourceOpt = mostly_harmless::BinaryData::getNamedResource("images/BottomText.png");
```

<br>
A full usage example then: 

```cpp
#include <mostlyharmless_BinaryData.h>
int main() { 
    auto bottomText = mostly_harmless::BinaryData::getNamedResource("images/BottomText.png");
    assert(bottomText);
    auto [imgData, imgSize] = *bottomText; // const char*, size_t
    ...
    auto placeholder = mostly_harmless::BinaryData::getNamedResource("text/Placeholder.txt");
    assert(placeholder);
    auto [textData, textSize] = *placeholder;
    ...
    return 0;
}
```

You're not limited to a single library of binary resources either, which could be useful for separating resources of a
different category or purpose into different headers, etc etc etc.

### The actual plugin code

We've included the boilerplate for various projects in the `boilerplate` directory - these should serve as a good
starting point for your plugin sources.
As previously mentioned, you can also check out the `examples` directory for more inspiration.

## Dependencies

mostly harmless depends on the following libraries:

- [marvin](https://github.com/MeijisIrlnd/marvin) (and its dependencies)
- [clap](https://github.com/free-audio/clap), [clap-helpers](https://github.com/free-audio/clap-helpers), [clap-wrapper](https://github.com/free-audio/clap-wrapper)
- [choc](https://github.com/Tracktion/choc)
- [cmrc](https://github.com/vector-of-bool/cmrc)
- [fmtlib](https://github.com/fmtlib/fmt)
- [json](https://github.com/nlohmann/json)
- [catch2](https://github.com/catchorg/Catch2)

## Credits

This framework wouldn't exist without the advice, moral support and help
of [mthierman](https://github.com/mthierman), [Ben Vining](https://github.com/benthevining), [Signalsmith](https://signalsmith-audio.co.uk/)
and the legions of incredible people in [The Audio Programmer Discord](https://discord.gg/v5Rs8h6aGF).<b>

SLM Audio Brat Summer 2024, all rights reserved.




