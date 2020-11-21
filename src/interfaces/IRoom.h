#ifndef I_ROOM
#define I_ROOM

#include "../io/ConsoleOutputter.h"
#include "IHasDescription.h"

#include <string>
#include <vector>

namespace Game {
class IRoom : public IHasDescription {
public:
	virtual void describe(ConsoleOutputter& writer) const override final;
	virtual ~IRoom() { };
protected:
	virtual void innerDescribe(ConsoleOutputter& writer) const = 0;
	virtual std::vector<std::string> exitNames(bool includeHidden = false) const = 0;
private:
	void printExits(ConsoleOutputter& writer) const;
};
}

#endif