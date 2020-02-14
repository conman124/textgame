#ifndef MONASTERY_MAZE_ROOM_MAINTAINER_H
#define MONASTERY_MAZE_ROOM_MAINTAINER_H

#include "interfaces/IRoomMaintainer.h"

#include <string>

namespace Game {

class MonasteryMazeRoomMaintainer : public IRoomMaintainer<std::string> {
protected:
	virtual std::shared_ptr<IFullRoom> constructRoom(RoomIdType id) const override;
	virtual RoomIdType normalizeId(RoomIdType) const override;
};

class MonasteryMazeRoom : public IRoomMaintainer<std::string>::IFullRoom {
public:
	MonasteryMazeRoom(std::string name);
	virtual std::vector<IRoomMaintainer<std::string>::ExitDescriptor> exitDescriptors() const override;
protected:
	virtual void innerDescribe() const override;
private:
	std::string id;
};
}

#endif