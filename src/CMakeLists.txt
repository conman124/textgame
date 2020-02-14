project("textgame")

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Og")
set(CMAKE_CXX_FLAGS_RELEASE "-g -O3")

add_executable(textgame main.cpp MonasteryMazeRoomMaintainer.cpp interfaces/IRoom.cpp)