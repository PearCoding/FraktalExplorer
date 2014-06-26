CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(FraktalExplorer-Plugin-EulerPacman)

add_library(fe_eulerpacman_plugin MODULE ${FE_PLUGINS_Root}/EulerPacman/eulerpacman.cpp)
target_link_libraries(fe_eulerpacman_plugin Qt5::Widgets)
set_target_properties(fe_eulerpacman_plugin PROPERTIES VERSION ${FE_Version})
set_target_properties(fe_eulerpacman_plugin PROPERTIES COMPILE_DEFINITIONS "${DEFS}" COMPILE_DEFINITIONS_DEBUG "${DEFS_DEBUG}")
