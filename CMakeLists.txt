cmake_minimum_required(VERSION 3.30)
project(synth-63731)
set(CMAKE_CXX_STANDARD 23)
# define the location of the header files
include_directories("libraries/imgui/backends"
        "libraries/imgui"
        "libraries/sdl/include"
        "libraries/portaudio/include"
)
add_executable(synth-63731 main.cpp
        audio/Oscillator.cpp
        audio/Envelope.cpp
        audio/LowPassFilter.cpp
        audio/Delay.cpp
        AudioGenerator.cpp
        AudioParam.cpp
        ./gui/MainWindow.cpp
        ./libraries/imgui/imgui.cpp
        ./libraries/imgui/imgui_demo.cpp
        ./libraries/imgui/imgui_draw.cpp
        ./libraries/imgui/imgui_widgets.cpp
        ./libraries/imgui/imgui_tables.cpp
        ./libraries/imgui/backends/imgui_impl_sdl3.cpp
        ./libraries/imgui/backends/imgui_impl_sdlrenderer3.cpp)
if (APPLE)
    set(CMAKE_INSTALL_RPATH
            "${CMAKE_SOURCE_DIR}/../libraries/sdl/lib/macos/SDL3.framework")
    target_link_libraries(synth-63731 PRIVATE
            "${CMAKE_SOURCE_DIR}/libraries/sdl/lib/macos/SDL3.framework"
            "-framework CoreFoundation"
            "-framework CoreFoundation"
            "-framework AudioToolbox"
            "-framework CoreAudio"
            "-framework Carbon"
            "${CMAKE_SOURCE_DIR}/libraries/portaudio/lib/macos/libportaudio.a")

elseif (WIN32)
    target_link_libraries(synth-63731 PRIVATE
            "${CMAKE_SOURCE_DIR}/libraries/sdl/lib/win32/sdl3.dll"
            "${CMAKE_SOURCE_DIR}/libraries/portaudio/lib/win32/portaudio_x64.dll"
    )

    # post-build command to copy the DLLs
    add_custom_command(TARGET synth-63731 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/libraries/sdl/lib/win32/sdl3.dll"
            "$<TARGET_FILE_DIR:synth-63731>"
            "${CMAKE_CURRENT_SOURCE_DIR}/libraries/portaudio/lib/win32/portaudio_x64.dll"
            "$<TARGET_FILE_DIR:synth-63731>"
    )
elseif (UNIX)
    target_link_libraries(synth-63731 PRIVATE
            "-ldl"
            "-ljack"
            "${CMAKE_SOURCE_DIR}/libraries/sdl/lib/linux-x86_64/libSDL3.a"
            "${CMAKE_SOURCE_DIR}/libraries/portaudio/lib/linux-x86_64/libportaudio.a"
    )
endif ()
