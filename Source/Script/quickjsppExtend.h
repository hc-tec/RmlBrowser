//
// Created by titto on 2023/1/1.
//

#ifndef RMLUI_QUICKJSPPEXTEND_H
#define RMLUI_QUICKJSPPEXTEND_H

#include "quickjspp.hpp"

namespace qjs {

/** Conversion traits for float.
 */
template <>
struct js_traits<float> {
	/// @throws exception
	static double unwrap(JSContext* ctx, JSValueConst v)
	{
		double r;
		if (JS_ToFloat64(ctx, &r, v))
			throw exception{ctx};
		return r;
	}

	static JSValue wrap(JSContext* ctx, double i) noexcept { return JS_NewFloat64(ctx, i); }
};

}

#endif // RMLUI_QUICKJSPPEXTEND_H
