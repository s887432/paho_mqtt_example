cmake_minimum_required(VERSION 3.10)

project(mqtt_pub)

#set_property(TARGET zbar_app PROPERTY C_STANDARD 99)
#set(CMAKE_C_FLAGS "-std=c99")

SET(CMAKE_C_COMPILER   arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
#SET(CMAKE_C_COMPILER   gcc)
#SET(CMAKE_CXX_COMPILER g++)

aux_source_directory(. DIR_SRCS)

add_executable(${PROJECT_NAME} ${DIR_SRCS})
target_link_libraries(${PROJECT_NAME} paho-mqtt3c)
