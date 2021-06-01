#ifndef MAP_H
#define MAP_H

#include "interfaces/IMap.h"
#include "interfaces/IRoomMaintainer.h"

class Map : public IMap {
public:
	Map(int x, int y, const Game::IRoomMaintainer<std::pair<int,int>>& maintainerIn);

	size_t size() const override;
	IMapDescriptor getDescriptor(size_t x, size_t y) const override;
private:
	std::pair<int, int> center;
	std::pair<int, int> start;
	const Game::IRoomMaintainer<std::pair<int, int>>* maintainer;
};

#endif