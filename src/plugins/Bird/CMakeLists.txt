CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(FraktalExplorer-Plugin-Bird)

add_library(fe_bird_plugin MODULE ${FE_PLUGINS_Root}/Bird/bird.cpp)
target_link_libraries(fe_bird_plugin Qt5::Widgets)
set_target_properties(fe_bird_plugin PROPERTIES VERSION ${FE_Version})
set_target_properties(fe_bird_plugin PROPERTIES COMPILE_DEFINITIONS "${DEFS}" COMPILE_DEFINITIONS_DEBUG "${DEFS_DEBUG}")
