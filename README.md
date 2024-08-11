# mostly harmless
## About
> mostly harmless is a minimal audio plugin framework for C++20. It's first and foremost a wrapper around the CLAP API, capable of building CLAP, VST3, Audio Unit and Standalone targets.

## Features
> ### dsp
>While the framework itself doesn't provide anything in the way of audio helpers ala something like JUCE, it ships with our sibling dsp library [marvin](https://github.com/MeijisIrlnd/marvin), 
should you need some convenience helpers and basic audio features.

> ### gui
> The goal of the framework was to provide the bare minimum we (as in SLM Audio) need to write a plugin - and as such we don't provide any kind of gui framework of our own. 
> We do, however, provide a thin wrapper around [choc](https://github.com/Tracktion/choc)'s WebView, and plan to implement similar minimal wrappers around a couple of other frameworks.<br>
> We also provide an interface with hooks for you to implement your framework of choice, in `mostly_harmless::gui::IEffect`.

## Getting Started
> ### Building
> Firstly, we'd recommend taking a look in the `examples` directory for a real-world use case, in the absence of actual tutorials (which are planned). <br>
> mostly harmless is built with CMake, and supports Windows and macOS (Linux support is on the roadmap). 
> It can be consumed either as a git submodule, or as a FetchContent dependency. 
> #### Adding to your project
> ```cmake 
> include(FetchContent)
> FetchContent_Declare(mostly_harmless
> GIT_REPOSITORY "https://github.com/SLM-Audio/mostly-harmless.git
> GIT_TAG main 
> GIT_SHALLOW ON)
> 
> FetchContent_MakeAvailable(mostly_harmless)
>```
> #### Adding a plugin target
> We provide a convenience function to set up your plugin targets, `mostly_harmless_add_plugin`. This function will create CLAP, VST3, AU and Standalone targets, and set up linkage with mostly harmless and marvin.
> ```cmake 
> mostly_harmless_add_plugin(YourTarget 
> ID "com.whatever"
> NAME "YourPluginName"
> SOURCES ${YOUR_SOURCES}
> FEATURES "audio-effect")
>```
> #### The actual plugin code
> We've included the boilerplate for various projects in the `boilerplate` directory - these should serve as a good starting point for your plugin sources. 
> As previously mentioned, you can also check out the `examples` directory for more inspiration.

### Documentation





