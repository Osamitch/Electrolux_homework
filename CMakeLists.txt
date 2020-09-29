
cmake_minimum_required(VERSION 3.7)
project(Electrolux_homework C)
add_library(ellux_usart_api INTERFACE)
target_compile_features(ellux_usart_api PUBLIC c_std_99)
#Add header(s)
target_include_directories(ellux_usart_api {CMAKE_CURRENT_SOURCE_DIR}/Inc)
# For future use
# target_sources()

add_subdirectory(Test)