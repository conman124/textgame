#ifndef MONASTERY_MAZE_ROOM_MAINTAINER_H
#define MONASTERY_MAZE_ROOM_MAINTAINER_H

#include "interfaces/IRoomMaintainer.h"

#include <memory>
#include <string>
#include <vector>

namespace Game {

class MonasteryMazeRoomMaintainer : public IRoomMaintainer<std::string> {
protected:
	virtual std::shared_ptr<IFullRoom> constructRoom(const RoomIdType& id) const override;
	virtual RoomIdType normalizeId(const RoomIdType& id) const override;
};

class MonasteryMazeRoom : public IRoomMaintainer<std::string>::IFullRoom {
public:
	MonasteryMazeRoom(std::string name);
	virtual std::vector<IRoomMaintainer<std::string>::ExitDescriptor> exitDescriptors() const override;
protected:
	virtual std::string innerDescribe() const override;
private:
	std::string id;
};
}

#endif