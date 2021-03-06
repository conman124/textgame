#ifndef I_HAS_DESCRIPTION
#define I_HAS_DESCRIPTION

#include <string>

namespace Game {
class IHasDescription {
public:
	virtual std::string describe() const = 0;
	virtual ~IHasDescription() {};
};
}

#endif