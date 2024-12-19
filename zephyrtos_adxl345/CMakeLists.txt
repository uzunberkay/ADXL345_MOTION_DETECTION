cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(zephyrtos)





target_include_directories(app PUBLIC   ${CMAKE_CURRENT_SOURCE_DIR}/src/app_libs/gpio_settings)
target_sources            (app PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/src/app_libs/gpio_settings/gpio_settings.c)



target_include_directories(app PUBLIC   ${CMAKE_CURRENT_SOURCE_DIR}/src/app_libs/adxl345)
target_sources            (app PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/src/app_libs/adxl345/adxl345.c)


target_include_directories(app PUBLIC   ${CMAKE_CURRENT_SOURCE_DIR}/src/app_libs/utils)


target_sources            (app PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/src/app_libs/motion_detection/adxl345_motion_example.c)