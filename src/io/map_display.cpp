#include "map_display.h"

#include <algorithm>
#include <exception>
#include <fmt/core.h>
#include <vector>

typedef std::vector<std::vector<IMapDescriptor>> MapVector;

void validateMap(const MapVector& map) {
	// TODO don't validate the map everytime before display?
	// TODO use a standard assert function
	auto assert = [](bool cond, std::string reason) { if(!cond) { throw std::logic_error(reason); }};

	size_t size = map.size();
	assert(std::all_of(map.begin(), map.end(), [size](auto v) { return v.size() == size; }), "sizes don't match");

	for(size_t i = 0; i < size; ++i) {
		for(size_t j = 0; j < size; ++j) {
			if(j + 1 < size) {
				// validate south
				assert(map[i][j].s == map[i][j+1].n, "south");

				if(i > 1) {
					// validate southwest
					assert(map[i][j].sw == map[i-1][j+1].ne, "southwest");
				}
				if(i + 1 < size) {
					// validate southeast
					assert(map[i][j].se == map[i+1][j+1].nw, fmt::format("southeast {} {} {} {}", i, j, map[i][j].se, map[i+1][j+1].nw));
				}
			}
			
			if(i + 1 < size) {
				// validate east
				assert(map[i][j].e == map[i+1][j].w, "east");
			}
		}
	}
}

std::string getConnectionsRow(const MapVector& map, size_t i) {
	(void)map;
	(void)i;

	std::string ret;
	// TODO write this
	return ret;
}

std::string getRoomsRow(const MapVector& map, size_t i) {
	// TODO add connections
	std::string ret;
	for(size_t j = 0; j < map.size(); ++j) {
		ret += map[i][j].style(map[i][j].character);
	}
	return ret + "\n";
}

std::string displayMap(const IMap& map) {
	auto size = map.size();
	MapVector vec{size, std::vector<IMapDescriptor>{size}};

	for(size_t i = 0; i < size; ++i) {
		for(size_t j = 0; j < size; ++j) {
			vec[i][j] = map.getDescriptor(i, j);
		}
	}

	validateMap(vec);

	std::string ret;

	for(size_t i = 0; i < size; ++i) {
		if(i > 0) {
			ret += getConnectionsRow(vec, i);
		}
		ret += getRoomsRow(vec, i);
		// (check index for 0, then) write connections between previous row and this row. write newline
		// write this row. write newline
	}
	return ret;
}