//
// Created by titto on 2022/12/29.
//

#include "QuickJs.h"
#include <quickjs/quickjs.h>
#define countof(x) (sizeof(x) / sizeof((x)[0]))

// 原始的 C 函数
static int fib(int n) {
    if (n <= 0) return 0;
    else if (n == 1) return 1;
    else return fib(n - 1) + fib(n - 2);
}

// 包一层，处理类型转换
static JSValue js_fib(JSContext *ctx, JSValueConst this_val,
                      int argc, JSValueConst *argv) {
    int n, res;
    if (JS_ToInt32(ctx, &n, argv[0])) return JS_EXCEPTION;
    res = fib(n);
    return JS_NewInt32(ctx, res);
}

// 将包好的函数定义为 JS 模块下的 fib 方法
static const JSCFunctionListEntry js_fib_funcs[] = {
    JS_CFUNC_DEF("fib", 1, js_fib ),
};

// 模块初始化时的回调
static int js_fib_init(JSContext *ctx, JSModuleDef *m) {
    return JS_SetModuleExportList(ctx, m, js_fib_funcs, countof(js_fib_funcs));
}

// 最终对外的 JS 模块定义
JSModuleDef *js_init_module_fib(JSContext *ctx, const char *module_name) {
    JSModuleDef *m;
    m = JS_NewCModule(ctx, module_name, js_fib_init);
    if (!m) return NULL;
    JS_AddModuleExportList(ctx, m, js_fib_funcs, countof(js_fib_funcs));
    return m;
}