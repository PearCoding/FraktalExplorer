CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(FraktalExplorer-Plugin-BlockEyes)

add_library(fe_blockeyes_plugin MODULE ${FE_PLUGINS_Root}/BlockEyes/blockeyes.cpp)
target_link_libraries(fe_blockeyes_plugin Qt5::Widgets)
set_target_properties(fe_blockeyes_plugin PROPERTIES VERSION ${FE_Version})
set_target_properties(fe_blockeyes_plugin PROPERTIES COMPILE_DEFINITIONS "${DEFS}" COMPILE_DEFINITIONS_DEBUG "${DEFS_DEBUG}")
