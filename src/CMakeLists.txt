cmake_minimum_required(VERSION 3.16)
project("textgame")

set(CMAKE_CXX_FLAGS "-std=c++20 -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Og -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "-g -O3")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-fsanitize=address")

find_package(Boost 1.71 REQUIRED COMPONENTS fiber)
find_package( Threads )

include_directories( ${Boost_INCLUDE_DIRS} )
include_directories( /usr/include/readline )

add_executable(textgame 
	main.cpp
	interfaces/IRoom.cpp
	io/CommandLoop.cpp
	io/ConsoleOutputter.cpp
	MonasteryMazeRoomMaintainer.cpp
	TimerManager.cpp
)
target_link_libraries(textgame ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} readline fmt)