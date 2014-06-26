CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(FraktalExplorer-Plugin-DoubleHead)

add_library(fe_doublehead_plugin MODULE ${FE_PLUGINS_Root}/DoubleHead/doublehead.cpp)
target_link_libraries(fe_doublehead_plugin Qt5::Widgets)
set_target_properties(fe_doublehead_plugin PROPERTIES VERSION ${FE_Version})
set_target_properties(fe_doublehead_plugin PROPERTIES COMPILE_DEFINITIONS "${DEFS}" COMPILE_DEFINITIONS_DEBUG "${DEFS_DEBUG}")
