cmake_minimum_required(VERSION 3.20)
project(GuitarPedalApp VERSION 0.1.0 LANGUAGES C CXX)

# Disable JUCE examples, extras and helper tools
set(JUCE_BUILD_EXAMPLES     OFF CACHE BOOL "" FORCE)
set(JUCE_BUILD_EXTRAS       OFF CACHE BOOL "" FORCE)
set(JUCE_BUILD_HELPER_TOOLS OFF CACHE BOOL "" FORCE)

# Pull in JUCE framework
add_subdirectory(external/JUCE)

# Load JUCE CMake helper macros
include("${CMAKE_CURRENT_SOURCE_DIR}/external/JUCE/extras/Build/CMake/JUCEUtils.cmake")

# Create your GUI application
juce_add_gui_app(GuitarPedalApp
    PRODUCT_NAME "GuitarPedalApp"
)

# Generate the amalgamated JuceHeader.h
juce_generate_juce_header(GuitarPedalApp)

# Link in the audio-utils module for AudioAppComponent, etc.
target_link_libraries(GuitarPedalApp
    PRIVATE
        juce::juce_audio_utils
)

# Point to your source files
target_sources(GuitarPedalApp
    PRIVATE
        src/main.cpp
)

# Make your headers discoverable
target_include_directories(GuitarPedalApp
    PRIVATE
        include
)

# Use C++17
target_compile_features(GuitarPedalApp
    PRIVATE
        cxx_std_17
)
