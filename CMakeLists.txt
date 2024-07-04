cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 20)
project(tcp_chat)

find_package(Boost REQUIRED)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

set(SOURCE 
        include/my_chat/session_type.hpp
        include/my_chat/server_type.hpp
        include/server.hpp
        src/server.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE} main.cpp)
target_link_libraries(${PROJECT_NAME} ${BOOST_LIBRARIES})

