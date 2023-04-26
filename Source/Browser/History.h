//
// Created by titto on 2023/4/18.
//

#ifndef RMLUI_HISTORY_H
#define RMLUI_HISTORY_H

#include "RmlUi/Core/Types.h"
#include "RmlUi/Config/Config.h"

#include "../Script/Glue.h"



namespace Rml {
namespace Browser {

using His = struct His {
    String title;
    String icon;
    String url;
    String time;
};

class History {
public:
    History();

    void Collect(His s);

    void Remove(const String& url);

    void Get(Vector<His>& history) { history = history_; }

    void Dump();

    void Load();

private:
    Vector<His> history_;
};

}
}

template <>
struct qjs::js_traits<Rml::Browser::His> {
    /// @throws exception
    Rml::Browser::His unwrap(JSContext* ctx, JSValueConst v)
    {
        Rml::Browser::His map;
        return map;
    }

    // transform Star to JsObject directly, so we can use Star as JsObject
    static JSValue wrap(JSContext* ctx, Rml::Browser::His s) noexcept {
        JSValue obj = JS_NewObject(ctx);
        Value v {ctx, JS_DupValue(ctx, obj)};
        v["title"] = s.title;
        v["icon"] = s.icon;
        v["url"] = s.url;
        v["time"] = s.time;
        return v.v;
    }
};

namespace Rml {
namespace Browser {

static void HistoryGlue(qjs::Context* context) {
    context->global()["CHistoryGet"] = [context]() -> qjs::Value {
		History his;
        Vector<His> history;
        his.Get(history);
        JSValue obj = qjs::js_traits<Vector<His>>::wrap(context->ctx, history);
        qjs::Value v {context->ctx, JS_DupValue(context->ctx, obj)};
        return v;
    };
}

static void RegisterHistoryGlueFunc() {
    RegisterGlueFunc(HistoryGlue);
}


}
}



#endif // RMLUI_HISTORY_H
