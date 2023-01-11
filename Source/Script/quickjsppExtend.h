//
// Created by titto on 2023/1/1.
//

#ifndef RMLUI_QUICKJSPPEXTEND_H
#define RMLUI_QUICKJSPPEXTEND_H

#include "quickjspp.hpp"
#include "RmlUi/Config/Config.h"
#include "RmlUi/Core/Variant.h"
#include "RmlUi/Core/Types.h"
#include "Dom/Ownership.h"

namespace qjs {

template <>
struct js_property_traits<char *>
{
    static void set_property(JSContext * ctx, JSValue this_obj, char * name, JSValue value)
    {
        int err = JS_SetPropertyStr(ctx, this_obj, name, value);
        if(err < 0)
            throw exception{ctx};
    }

    static JSValue get_property(JSContext * ctx, JSValue this_obj, char * name) noexcept
    {
        return JS_GetPropertyStr(ctx, this_obj, name);
    }
};

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

/** Conversion traits for SmallUnorderedMap.
 */
using SmallUnorderedMap = Rml::SmallUnorderedMap< Rml::String, Rml::Variant >;
template <>
struct js_traits<SmallUnorderedMap> {
    /// @throws exception
    SmallUnorderedMap unwrap(JSContext* ctx, JSValueConst v)
    {
        SmallUnorderedMap map;
        return map;
    }

	// transform SmallUnorderedMap to JsObject directly, so we can use SmallUnorderedMap as JsObject
    static JSValue wrap(JSContext* ctx, SmallUnorderedMap i) noexcept {
		JSValue obj = JS_NewObject(ctx);
		Value v {ctx, JS_DupValue(ctx, obj)};
		for (auto& [key, val] : i) {
			switch (val.GetType())
			{
                case Rml::Variant::INT:
                    v[key.data()] = val.Get<int>();
					break;
                case Rml::Variant::INT64:
                    v[key.data()] = val.Get<int64_t>();
                    break;
                case Rml::Variant::UINT:
                    v[key.data()] = val.Get<unsigned int>();
                    break;
//                case Rml::Variant::UINT64:
//                    v[key.data()] = val.Get<uint64_t>();
                    break;
                case Rml::Variant::BOOL:
                    v[key.data()] = val.Get<bool>();
                    break;
                case Rml::Variant::FLOAT:
                    v[key.data()] = val.Get<float>();
                    break;
                case Rml::Variant::DOUBLE:
                    v[key.data()] = val.Get<double>();
                    break;
                case Rml::Variant::CHAR:
                    v[key.data()] = val.Get<char>();
                    break;
                case Rml::Variant::STRING:
                    v[key.data()] = val.Get<Rml::String>();
                    break;
				default:
					break;
			}
		}
		return v.v;
	}
};


/** Conversion traits for UniqueReleaserPtr<T>.
 */
template <class T>
struct js_traits<Rml::UniqueReleaserPtr<T>> {
    using UniqueReleaserPtr = Rml::UniqueReleaserPtr<T>;
    /// @throws exception
    static UniqueReleaserPtr unwrap(JSContext* ctx, JSValueConst v)
    {
        UniqueReleaserPtr ptr = nullptr;
        if (JS_IsNull(v)) {
            return ptr;
        }
        auto shared_ptr = js_traits<std::shared_ptr<T>>::unwrap(ctx, v);
        return std::move(Rml::Script::GetOwnership<T, UniqueReleaserPtr>()->GetOwner(shared_ptr.get()));
    }

    static JSValue wrap(JSContext* ctx, UniqueReleaserPtr val) noexcept {
		T* raw_ptr = val.get();
		Rml::Script::GetOwnership<T, UniqueReleaserPtr>()->ShiftOwner(std::move(val));
		return js_traits<T*>::wrap(ctx, raw_ptr);
	}
};

/** Conversion traits for std::unique_ptr<T>.
 */
//template <class T>
//struct js_traits<std::unique_ptr<T>> {
//    /// @throws exception
//    static std::unique_ptr<T> unwrap(JSContext* ctx, JSValueConst v)
//    {
//        std::unique_ptr<T> ptr = nullptr;
//        if (JS_IsNull(v)) {
//            return ptr;
//        }
//        auto class_id = JS_GetClassID(v);
//        T* opaque = reinterpret_cast<T*>(JS_GetOpaque2(ctx, v, class_id));
//        return std::move(Rml::Script::GetOwnership<T>()->GetOwner(opaque));
//    }
//
//    static JSValue wrap(JSContext* ctx, std::unique_ptr<T> val) noexcept {
//        T* raw_ptr = val.get();
//        Rml::Script::GetOwnership<T>()->ShiftOwner(val);
//        return js_traits<T*>::wrap(ctx, raw_ptr);
//    }
//};




}

#endif // RMLUI_QUICKJSPPEXTEND_H
