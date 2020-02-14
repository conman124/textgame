#ifndef I_ROOM
#define I_ROOM

#include "IHasDescription.h"

#include <string>
#include <vector>

namespace Game {
class IRoom : public IHasDescription {
public:
	virtual void describe() const override final;
	virtual ~IRoom() { };
protected:
	virtual void innerDescribe() const = 0;
	virtual std::vector<std::string> exitNames(bool includeHidden = false) const = 0;
private:
	void printExits() const;
};
}

#endif