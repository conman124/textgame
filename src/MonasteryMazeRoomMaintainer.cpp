#include "MonasteryMazeRoomMaintainer.h"

#include <iostream>

namespace Game {
std::shared_ptr<MonasteryMazeRoomMaintainer::IFullRoom> MonasteryMazeRoomMaintainer::constructRoom(const RoomIdType& id) const {
	return std::make_shared<MonasteryMazeRoom>(id);
}

std::string MonasteryMazeRoomMaintainer::normalizeId(const std::string& id ) const {
	if(std::string{"eternity"}.find(id) != 0) {
		return "";
	}
	return id;
}

MonasteryMazeRoom::MonasteryMazeRoom(std::string id_in)
	: id{id_in}
{

}

std::vector<MonasteryMazeRoomMaintainer::ExitDescriptor> MonasteryMazeRoom::exitDescriptors() const {
	auto ret = std::vector<MonasteryMazeRoomMaintainer::ExitDescriptor>{};
	for(char a = 'a'; a <= 'z'; ++a) {
		ret.push_back(MonasteryMazeRoomMaintainer::ExitDescriptor{std::string{a}, id + a});
	}

	return ret;
}

void MonasteryMazeRoom::innerDescribe(ConsoleOutputter& writer) const {
	writer.write("You have made it to: {}", id);
}

}