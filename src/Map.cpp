#include "Map.h"

Map::Map(int x, int y, const Game::IRoomMaintainer<std::pair<int,int>>& maintainerIn)
	: center{x, y}
	, start{x - size() / 2, y - size() / 2}
	, maintainer{&maintainerIn}
{

}

size_t Map::size() const {
	return 9;
}

IMapDescriptor Map::getDescriptor(size_t x, size_t y) const {
	auto exits = maintainer->getRoom(std::make_pair(start.first + x, start.second + y))->exitDescriptors();

	auto getExit = [&exits](std::string name) { return std::find_if(exits.begin(), exits.end(), [name](auto ed) { return ed.name == name; }); };
	auto hasExit = [&exits, &getExit](std::string name) { return (getExit(name) != exits.end()) ? true : false; };

	return {
		.style{},
		.character = '*',
		.n = hasExit("north"),
		.ne = hasExit("northeast"),
		.e = hasExit("east"),
		.se = hasExit("southeast"),
		.s = hasExit("south"),
		.sw = hasExit("southwest"),
		.w = hasExit("west"),
		.nw = hasExit("northwest")
	};
}