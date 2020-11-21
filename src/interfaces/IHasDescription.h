#ifndef I_HAS_DESCRIPTION
#define I_HAS_DESCRIPTION

#include <string>

class ConsoleOutputter;

namespace Game {
class IHasDescription {
public:
	virtual void describe(ConsoleOutputter& writer) const = 0;
	virtual ~IHasDescription() {};
};
}

#endif