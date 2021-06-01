#ifndef IMAP_H
#define IMAP_H

#include "../io/Style.h"

struct IMapDescriptor {
	Style style;
	char character = ' ';
	bool n: 1 = false,
	     ne: 1 = false,
		 e: 1 = false,
		 se: 1 = false,
		 s: 1 = false,
		 sw: 1 = false,
		 w: 1 = false,
		 nw: 1 = false;
};

class IMap {
public:
	virtual size_t size() const = 0;
	virtual IMapDescriptor getDescriptor(size_t x, size_t y) const = 0;
};

#endif