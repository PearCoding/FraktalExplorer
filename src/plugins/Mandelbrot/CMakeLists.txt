CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(FraktalExplorer-Plugin-Mandelbrot)

add_library(fe_mandelbrot_plugin MODULE ${FE_PLUGINS_Root}/Mandelbrot/mandelbrot.cpp)
target_link_libraries(fe_mandelbrot_plugin Qt5::Widgets)
set_target_properties(fe_mandelbrot_plugin PROPERTIES VERSION ${FE_Version})
set_target_properties(fe_mandelbrot_plugin PROPERTIES COMPILE_DEFINITIONS "${DEFS}" COMPILE_DEFINITIONS_DEBUG "${DEFS_DEBUG}")
