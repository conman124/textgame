#include "WildernessRoomMaintainer.h"

#include <fmt/core.h>

namespace Game {
std::shared_ptr<WildernessRoomMaintainer::IFullRoom> WildernessRoomMaintainer::constructRoom(const RoomIdType& location) const {
	return std::make_shared<WildernessRoom>(location);
}

WildernessRoom::WildernessRoom(std::pair<int,int> locationIn)
	: location{locationIn}
{

}

std::string exitName(int xOff, int yOff) {
	switch(xOff) {
		case -1:
			switch(yOff) {
				case -1: return "northwest";
				case 0: return "west";
				case 1: return "southwest";
			}
		case 0:
			switch(yOff) {
				case -1: return "north";
				case 1: return "south";
			}
		case 1:
			switch(yOff) {
				case -1: return "northeast";
				case 0: return "east";
				case 1: return "southeast";
			}
	}
	return "invalid";
}

std::vector<WildernessRoomMaintainer::ExitDescriptor> WildernessRoom::exitDescriptors() const {
	auto ret = std::vector<WildernessRoomMaintainer::ExitDescriptor>{};
	for(int i = -1; i <= 1; ++i) {
		for(int j = -1; j <= 1; ++j) {
			if(i == 0 && j == 0) { continue; }
			if(std::get<0>(location) + i < 0) { continue; }
			if(std::get<0>(location) + i > 20) { continue; }
			if(std::get<1>(location) + j < 0) { continue; }
			if(std::get<1>(location) + j > 20) { continue; }

			if(std::get<0>(location) + i > 8 || std::get<0>(location) > 8) { 
				if(std::get<1>(location) == 10 && j == 1) {
					continue;
				}
				if(std::get<1>(location) == 11 && j == -1) {
					continue;
				}
			}

			ret.push_back({exitName(i, j), std::make_pair<int, int>(std::get<0>(location) + i, std::get<1>(location) + j)});		
		}
	}

	return ret;
}

std::string WildernessRoom::innerDescribe() const {
	if(std::get<0>(location) < 8) {
		return "Forest";
	}

	if(std::get<1>(location) <= 10 ) {
		return "Mountain";
	}

	return "Desert";

}

}