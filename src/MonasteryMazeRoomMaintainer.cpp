#include "MonasteryMazeRoomMaintainer.h"

#include <fmt/core.h>

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

std::string MonasteryMazeRoom::innerDescribe() const {
	return fmt::format("You have made it to: {}", id);
}

}