//
// Created by titto on 2023/4/17.
//

#ifndef RMLUI_COLLECTIONS_H
#define RMLUI_COLLECTIONS_H

#include "RmlUi/Core/Types.h"

namespace Rml {
namespace Browser {

using Star = struct Star {
	String title;
    String icon;
    String url;
};

class Collections {
public:

    Collections();

	void Collect(Star s);

    void Remove(const String& url);

	void Get(Vector<Star>& stars) {
		stars = stars_;
	}

	void Dump();

	void Load();

private:
	Vector<Star> stars_;
};

}
}

#endif // RMLUI_COLLECTIONS_H
