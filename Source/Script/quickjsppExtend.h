//
// Created by titto on 2023/1/1.
//

#ifndef RMLUI_QUICKJSPPEXTEND_H
#define RMLUI_QUICKJSPPEXTEND_H

#include <json/json.h>
#include "quickjspp.hpp"
#include "RmlUi/Config/Config.h"
#include "RmlUi/Core/Variant.h"
#include "RmlUi/Core/Types.h"
#include "RmlUi/Core/Property.h"
#include "Dom/Ownership.h"
#include "Browser/Collections.h"
#include "core/http/http_headers.h"
#include "Dom/JsDocumentElement.h"

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

    static JSValue wrap(JSContext* ctx, UniqueReleaserPtr val) noexcept {
		T* raw_ptr = val.get();
		Rml::Script::GetOwnershipMgr<T>()->ShiftOwner(std::move(val));
		return js_traits<T*>::wrap(ctx, raw_ptr);
	}

    /// @throws exception
    static UniqueReleaserPtr unwrap(JSContext* ctx, JSValueConst v)
    {
        UniqueReleaserPtr ptr = nullptr;
        if (JS_IsNull(v)) {
            return ptr;
        }
        auto shared_ptr = js_traits<std::shared_ptr<T>>::unwrap(ctx, v);
        return Rml::Script::GetOwnershipMgr<T>()->GetOwner(shared_ptr.get());
    }
};


/** Conversion traits for Variant*.
 */
template <>
struct js_traits<Rml::Variant*> {
    /// @throws exception
    static double unwrap(JSContext* ctx, JSValueConst v)
    {
        double r;
        if (JS_ToFloat64(ctx, &r, v))
            throw exception{ctx};
        return r;
    }

    static JSValue wrap(JSContext* ctx, Rml::Variant* v) noexcept {
		if (v == nullptr) {
            JSValue obj = JS_NewString(ctx, "");
            Value val {ctx, JS_DupValue(ctx, obj)};
			return val.v;
		}
		auto type = v->GetType();
		switch (type)
		{
		case Rml::Variant::Type::STRING:
			return js_traits<Rml::String>::wrap(ctx, v->Get<Rml::String>());
		}
	}
};

template <>
struct js_traits<Rml::Browser::Star> {
    /// @throws exception
    Rml::Browser::Star unwrap(JSContext* ctx, JSValueConst v)
    {
        Rml::Browser::Star map;
        return map;
    }

    // transform Star to JsObject directly, so we can use Star as JsObject
    static JSValue wrap(JSContext* ctx, Rml::Browser::Star s) noexcept {
        JSValue obj = JS_NewObject(ctx);
        Value v {ctx, JS_DupValue(ctx, obj)};
        v["title"] = s.title;
        v["icon"] = s.icon;
        v["url"] = s.url;
        return v.v;
    }
};

template <>
struct js_traits<tit::net::HttpHeaders> {
    /// @throws exception
    tit::net::HttpHeaders unwrap(JSContext* ctx, JSValueConst v)
    {
        tit::net::HttpHeaders map;
        return map;
    }

    static JSValue wrap(JSContext* ctx, tit::net::HttpHeaders s) noexcept {
        JSValue obj = JS_NewObject(ctx);
        Value v {ctx, JS_DupValue(ctx, obj)};
		auto h = s.headers();
        for (auto it = h.begin(); it != h.end(); ++it) {
            v[it->first.c_str()] = it->second.c_str();
        }
        return v.v;
    }
};

template <>
struct js_traits<Json::Value> {
    /// @throws exception
    Json::Value unwrap(JSContext* ctx, JSValueConst v)
    {
        Json::Value map;
        return map;
    }

    static JSValue wrap(JSContext* ctx, Json::Value s) noexcept {
        Json::FastWriter writer;
        std::string jsonString = writer.write(s);
        JSValue obj = JS_ParseJSON(ctx, jsonString.c_str(), jsonString.size(), "<eval>");
        Value v {ctx, JS_DupValue(ctx, obj)};
        return v.v;
    }
};

template <>
struct js_traits<Rml::Script::ElementProperty> {

    static JSValue wrap(JSContext* ctx, Rml::Script::ElementProperty s) noexcept {
        JSValue obj = JS_NewObject(ctx);
        Value v {ctx, JS_DupValue(ctx, obj)};
		v["inherit"] = s.inherit;
		v["list"] = s.list;
        return v.v;
    }
};

template <>
struct js_traits<Rml::Script::UnitProperty> {

    static JSValue wrap(JSContext* ctx, Rml::Script::UnitProperty s) noexcept {
        JSValue obj = JS_NewObject(ctx);
        Value v {ctx, JS_DupValue(ctx, obj)};
        v["source"] = s.source;
        v["list"] = s.list;
        return v.v;
    }
};

template <>
struct js_traits<const Rml::PropertySource*> {
    const Rml::PropertySource* unwrap(JSContext* ctx, JSValueConst v)
    {
        Rml::PropertySource* obj;
        return obj;
    }
    static JSValue wrap(JSContext* ctx, const Rml::PropertySource* s) noexcept {
        JSValue obj = JS_NewObject(ctx);
        Value v {ctx, JS_DupValue(ctx, obj)};
		if (s != nullptr)
		{
			v["path"] = s->path;
			v["rule"] = s->rule_name;
			v["lineNumber"] = s->line_number;
		}
        return v.v;
    }
};


//template <>
//struct js_traits<const Rml::Property*> {
//
//    static JSValue wrap(JSContext* ctx, const Rml::Property* s) noexcept {
//        JSValue obj = JS_NewString(ctx, s->ToString().data());
//        Value v {ctx, JS_DupValue(ctx, obj)};
//        return v.v;
//    }
//};

}

#endif // RMLUI_QUICKJSPPEXTEND_H
