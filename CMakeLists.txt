
cmake_minimum_required(VERSION 3.7)
Project(Electrolux_homework C)
set(CMAKE_C_STANDARD C99)

add_library(ELLUX_USART_API STATIC)
#Add header(s)
target_include_directories(ELLUX_USART_API {CMAKE_CURRENT_SOURCE_DIR}/Inc)
# For future use
target_sources()

add_subdirectory(Test)