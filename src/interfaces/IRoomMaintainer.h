#ifndef I_ROOM_MAINTAINER
#define I_ROOM_MAINTAINER

#include "IRoom.h"

#include <algorithm>
#include <map>
#include <memory>
#include <vector>

namespace Game {
template <typename TRoomIdType>
class IRoomMaintainer {
public:
	typedef TRoomIdType RoomIdType;

	class ExitDescriptor {
	public:
		ExitDescriptor(std::string name_in, RoomIdType id_in, bool hidden_in = false);

		const std::string name;
		const RoomIdType id;
		const bool hidden;
	};

	class IFullRoom : public IRoom {
	public:
		virtual std::vector<ExitDescriptor> exitDescriptors() const = 0;
	protected:
		virtual std::vector<std::string> exitNames(bool includeHidden = false) const final;
		virtual void innerDescribe() const override = 0;
	};

	std::shared_ptr<IFullRoom> getRoom(RoomIdType id);

protected:
	virtual std::shared_ptr<IFullRoom> constructRoom(RoomIdType id) const = 0;
	virtual RoomIdType normalizeId(RoomIdType id) const;

private:
	std::map<RoomIdType, std::shared_ptr<IFullRoom>> cache;
};

template <typename TRoomIdType>
IRoomMaintainer<TRoomIdType>::ExitDescriptor::ExitDescriptor(std::string name_in, TRoomIdType id_in, bool hidden_in)
	: name(name_in)
	, id(id_in)
	, hidden(hidden_in)
{

}

template <typename TRoomIdType>
std::vector<std::string> IRoomMaintainer<TRoomIdType>::IFullRoom::exitNames(bool includeHidden) const {
	auto exits = exitDescriptors();
	auto ret = std::vector<std::string>{};

	for( auto& exit : exits ) {
		if(!exit.hidden || includeHidden) {
			ret.push_back(exit.name);
		}
	}

	return ret;
}

template <typename TRoomIdType>
std::shared_ptr<typename IRoomMaintainer<TRoomIdType>::IFullRoom> IRoomMaintainer<TRoomIdType>::getRoom(TRoomIdType id) {
	auto normalized = normalizeId(id);
	auto it = cache.find(normalized);
	if(it == cache.end()) {
		return constructRoom(normalized);
	}
	return it->second;
}

template <typename TRoomIdType>
TRoomIdType IRoomMaintainer<TRoomIdType>::normalizeId(TRoomIdType id) const {
	return id;
}


}

#endif