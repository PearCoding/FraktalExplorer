CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(FraktalExplorer-Plugin-XHole)

add_library(fe_xhole_plugin MODULE ${FE_PLUGINS_Root}/XHole/xhole.cpp)
target_link_libraries(fe_xhole_plugin Qt5::Widgets)
set_target_properties(fe_xhole_plugin PROPERTIES VERSION ${FE_Version})
set_target_properties(fe_xhole_plugin PROPERTIES COMPILE_DEFINITIONS "${DEFS}" COMPILE_DEFINITIONS_DEBUG "${DEFS_DEBUG}")
