cmake_minimum_required(VERSION 3.14)
project(docman)

add_executable(docman main.cpp)
target_include_directories(docman PRIVATE third_parties)
set_target_properties(docman PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON)

# 对于 Windows，链接到 ws2_32
if(WIN32)
    target_link_libraries(docman ws2_32)
endif()
