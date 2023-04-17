set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message("IMGUI_DIR: ${IMGUI_DIR}")
message("SDL2_Lib: ${SDL2_LIBRARIES}")
add_library(IMGUI   imgui_demo.cpp
                    imgui_draw.cpp
                    imgui_tables.cpp
                    imgui_widgets.cpp
                    imgui.cpp
                    backends/imgui_impl_opengl3.cpp
                    backends/imgui_impl_glfw.cpp)

target_include_directories( IMGUI
                            PUBLIC ${IMGUI_DIR}
                            PUBLIC ${IMGUI_DIR}/backends)


set(USE_GLES ON) 

# Link GLFW
target_link_libraries(IMGUI PUBLIC glfw)
# target_include_directories(IMGUI PUBLIC ${SDL2_INCLUDE_DIRS})

if(USE_GLES)
    target_compile_definitions(IMGUI PUBLIC -DIMGUI_IMPL_OPENGL_ES2)

    if(RASPBIAN AND USE_BROADCOM_DRIVER)
        target_link_directories(IMGUI PUBLIC /opt/vc/lib)
        target_link_libraries(IMGUI PUBLIC brcmGLESv2 brcmEGL)
    else()
        target_link_libraries(IMGUI PUBLIC GLESv2)
    endif()
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(IMGUI PUBLIC ${OPENGL_LIBRARIES})
    message("OpenGL LIBS: ${OPENGL_LIBRARIES}")
endif()

# if(BUILD_EXAMPLES)
#     add_executable(example_sdl_opengl3)
#     target_sources(example_sdl_opengl3 PUBLIC ${IMGUI_DIR}/examples/example_sdl_opengl3/main.cpp)
#     target_link_libraries(example_sdl_opengl3 IMGUI)
#     set_target_properties(example_sdl_opengl3 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
# endif()
