# CMakeList.txt : CMake project for acidwarp_gui, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (acidwarp_gui)

# Add source to this project's executable.
add_executable (acidwarp_gui 
"acidwarp_gui.cpp" 
"acidwarp_gui.hpp" 
"display.cpp" 
"display.hpp"
"handy.hpp"
 "draw.cpp"
 "draw.hpp"
imgui/imconfig.h
imgui/imgui.cpp
imgui/imgui.h
imgui/imgui_demo.cpp
imgui/imgui_draw.cpp
imgui/imgui_impl_sdl2.cpp
imgui/imgui_impl_sdl2.h
imgui/imgui_impl_sdlrenderer.cpp
imgui/imgui_impl_sdlrenderer.h
imgui/imgui_internal.h
imgui/imgui_tables.cpp
imgui/imgui_widgets.cpp
imgui/imstb_rectpack.h
imgui/imstb_textedit.h
imgui/imstb_truetype.h "timer.cpp" "palette.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET acidwarp_gui PROPERTY CXX_STANDARD 20)
endif()

include_directories("SDL/include")

set( THIRD_PARTY_DLLS
   "${PROJECT_SOURCE_DIR}/SDL/lib/x64/SDL2.dll"
   "${PROJECT_SOURCE_DIR}/SDL/lib/x64/SDL2.lib"
   "${PROJECT_SOURCE_DIR}/SDL/lib/x64/SDL2main.lib"
   "${PROJECT_SOURCE_DIR}/SDL/lib/x64/SDL2test.lib"
)

foreach( file_i ${THIRD_PARTY_DLLS})
add_custom_command(TARGET acidwarp_gui
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${file_i} "${CMAKE_CURRENT_BINARY_DIR}"
)
endforeach( file_i )

add_custom_command(TARGET acidwarp_gui
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/SDL/lib/x64/SDL2.dll" "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Copy dll file to ${CMAKE_CURRENT_BINARY_DIR} directory" VERBATIM
)

target_link_directories(acidwarp_gui PUBLIC "SDL/lib/x64")
target_link_libraries(acidwarp_gui PUBLIC "SDL2" "SDL2main")
target_include_directories(acidwarp_gui PUBLIC "SDL/include")