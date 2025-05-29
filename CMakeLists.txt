cmake_minimum_required(VERSION 3.20)
project(GuitarPedalApp VERSION 0.1.0 LANGUAGES C CXX)

set(JUCE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(JUCE_BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
set(JUCE_BUILD_HELPER_TOOLS OFF CACHE BOOL "" FORCE)

add_subdirectory(external/JUCE)

include("${CMAKE_CURRENT_SOURCE_DIR}/external/JUCE/extras/Build/CMake/JUCEUtils.cmake")

juce_add_gui_app(GuitarPedalApp
    PRODUCT_NAME "GuitarPedalApp"
)

juce_generate_juce_header(GuitarPedalApp)

target_link_libraries(GuitarPedalApp
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_processors
        juce::juce_core
        juce::juce_data_structures
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
)

target_sources(GuitarPedalApp
    PRIVATE
        src/main.cpp
        src/dsp/Overdrive.cpp
        src/dsp/CustomReverb.cpp
)

target_include_directories(GuitarPedalApp
    PRIVATE
        src
        src/effects
)

target_compile_features(GuitarPedalApp
    PRIVATE
        cxx_std_17
)

