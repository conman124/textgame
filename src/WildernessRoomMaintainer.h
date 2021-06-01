#ifndef WILDERNESS_ROOM_MAINTAINER_H
#define WILDERNESS_ROOM_MAINTAINER_H

#include "interfaces/IRoomMaintainer.h"

#include <memory>
#include <string>
#include <tuple>
#include <vector>

namespace Game {

class WildernessRoomMaintainer : public IRoomMaintainer<std::pair<int, int>> {
protected:
	std::shared_ptr<IFullRoom> constructRoom(const RoomIdType& id) const override;
};

class WildernessRoom : public IRoomMaintainer<std::pair<int,int>>::IFullRoom {
public:
	WildernessRoom(std::pair<int,int> locationIn);
	std::vector<IRoomMaintainer<std::pair<int, int>>::ExitDescriptor> exitDescriptors() const override;
	
	// TODO better encapsulation
	std::pair<int,int> location;
protected:
	std::string innerDescribe() const override;
};
}

#endif