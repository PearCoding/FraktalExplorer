CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(FraktalExplorer-Main)

SET(MAIN_Root ${FE_Root}/main)

#SETS
configure_file(${MAIN_Root}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
configure_file(${MAIN_Root}/../../settings.xml.in ${CMAKE_BINARY_DIR}/settings.xml)

SET(FE_MAIN_Src
  ${MAIN_Root}/algebra_parser.cpp
  ${MAIN_Root}/algorithmdialog.cpp
  ${MAIN_Root}/algebra_parser.cpp
  ${MAIN_Root}/algorithmplugin.cpp
  ${MAIN_Root}/databankdialog.cpp
  ${MAIN_Root}/fraktalwidget.cpp
  ${MAIN_Root}/helpdialog.cpp
  ${MAIN_Root}/lsystem_parser.cpp
  ${MAIN_Root}/main.cpp
  ${MAIN_Root}/mainwindow.cpp
  ${MAIN_Root}/plugin.cpp
  ${MAIN_Root}/pluginmanager.cpp
  ${MAIN_Root}/renderthread.cpp
  ${MAIN_Root}/xmlparser.cpp)

SET(FE_MAIN_Hdr
  ${MAIN_Root}/config.h.in
  ${MAIN_Root}/algebra_parser.h
  ${MAIN_Root}/algorithmdialog.h
  ${MAIN_Root}/algebra_parser.h
  ${MAIN_Root}/algorithmplugin.h
  ${MAIN_Root}/databankdialog.h
  ${MAIN_Root}/fraktale.h
  ${MAIN_Root}/fraktalwidget.h
  ${MAIN_Root}/helpdialog.h
  ${MAIN_Root}/lsystem_parser.h
  ${MAIN_Root}/mainwindow.h
  ${MAIN_Root}/plugin.h
  ${MAIN_Root}/plugininterface.h
  ${MAIN_Root}/pluginmanager.h
  ${MAIN_Root}/renderthread.h
  ${MAIN_Root}/xmlparser.h)

SET(FE_MAIN_Fms
  ${MAIN_Root}/ui/algorithmdialog.ui
  ${MAIN_Root}/ui/databankdialog.ui
  ${MAIN_Root}/ui/helpdialog.ui
  ${MAIN_Root}/ui/mainwindow.ui)
SOURCE_GROUP("Form Files" FILES ${FE_MAIN_Fms})

SET(FE_MAIN_Res
  ${FE_Root}/../Images.qrc)
SOURCE_GROUP("Resource Files" FILES ${FE_MAIN_Res})

QT5_WRAP_CPP(FE_MAIN_Hdr_Moc ${FE_MAIN_Hdr})
QT5_WRAP_UI(FE_MAIN_Fms_Hdr ${FE_MAIN_Fms})
QT5_ADD_RESOURCES(FE_MAIN_Res_Rcc ${FE_MAIN_Res})

SOURCE_GROUP("Generated Files" FILES ${FE_MAIN_Hdr_Moc} ${FE_MAIN_Fms_Hdr} ${FE_MAIN_Res_Rcc})

add_executable(${FE_TARGET} ${FE_MAIN_Src} ${FE_MAIN_Hdr}
		 ${FE_MAIN_Hdr_Moc} ${FE_MAIN_Fms_Hdr} ${FE_MAIN_Res_Rcc})
target_link_libraries(${FE_TARGET} Qt5::Widgets)
target_link_libraries(${FE_TARGET} Qt5::Concurrent)
target_link_libraries(${FE_TARGET} Qt5::Xml)
set_target_properties(${FE_TARGET} PROPERTIES VERSION ${FE_Version})
set_target_properties(${FE_TARGET} PROPERTIES COMPILE_DEFINITIONS "${DEFS}" COMPILE_DEFINITIONS_DEBUG "${DEFS_DEBUG}")
add_dependencies(${FE_TARGET} fe_bell_plugin fe_bird_plugin fe_blockeyes_plugin fe_doublehead_plugin
							 fe_eulerpacman_plugin fe_mandelbrot_plugin fe_xhole_plugin)