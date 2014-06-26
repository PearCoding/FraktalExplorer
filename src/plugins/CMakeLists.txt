CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

SET(FE_PLUGINS_Root ${FE_Root}/plugins)

add_subdirectory(${FE_PLUGINS_Root}/Bell)
add_subdirectory(${FE_PLUGINS_Root}/Bird)
add_subdirectory(${FE_PLUGINS_Root}/BlockEyes)
add_subdirectory(${FE_PLUGINS_Root}/DoubleHead)
add_subdirectory(${FE_PLUGINS_Root}/EulerPacman)
add_subdirectory(${FE_PLUGINS_Root}/Mandelbrot)
add_subdirectory(${FE_PLUGINS_Root}/XHole)