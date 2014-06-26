CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(FraktalExplorer-Plugin-Bell)

add_library(fe_bell_plugin MODULE ${FE_PLUGINS_Root}/Bell/bell.cpp)
target_link_libraries(fe_bell_plugin Qt5::Widgets)
set_target_properties(fe_bell_plugin PROPERTIES VERSION ${FE_Version})
set_target_properties(fe_bell_plugin PROPERTIES COMPILE_DEFINITIONS "${DEFS}" COMPILE_DEFINITIONS_DEBUG "${DEFS_DEBUG}")
