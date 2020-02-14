#ifndef I_HAS_NAME
#define I_HAS_NAME

#include <string>
#include <vector>

namespace Game {
class IHasName {
public:
	virtual std::string name() const = 0;
	virtual std::vector<std::string> synonyms() const { return std::vector<std::string>(); };
	virtual ~IHasName();
};
}

#endif