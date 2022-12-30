//
// Created by titto on 2022/12/29.
//

#include "MainWindow.h"

#include <string>
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include <RmlUi_Backend.h>
#include <Shell.h>
//#include <quickjs/quickjs-libc.h>
#include <quickjspp.hpp>
//static int eval_buf(JSContext *ctx, const char *buf, int buf_len,
//                    const char *filename, int eval_flags) {
//    JSValue val;
//    int ret;
//
//    if ((eval_flags & JS_EVAL_TYPE_MASK) == JS_EVAL_TYPE_MODULE) {
//        /* for the modules, we compile then run to be able to set
//           import.meta */
//        val = JS_Eval(ctx, buf, buf_len, filename,
//                      eval_flags | JS_EVAL_FLAG_COMPILE_ONLY);
//        if (!JS_IsException(val)) {
//            js_module_set_import_meta(ctx, val, 1, 1);
//            val = JS_EvalFunction(ctx, val);
//        }
//    } else {
//        val = JS_Eval(ctx, buf, buf_len, filename, eval_flags);
//    }
//    if (JS_IsException(val)) {
//        js_std_dump_error(ctx);
//        ret = -1;
//    } else {
//        ret = 0;
//    }
//    JS_FreeValue(ctx, val);
//    return ret;
//}

int main(int argc, char** argv)

{

//    JSRuntime *rt;
//    JSContext *ctx;
//    rt = JS_NewRuntime();
//    ctx = JS_NewContextRaw(rt);
//    JS_SetModuleLoaderFunc(rt, NULL, js_module_loader, NULL);
//    JS_AddIntrinsicBaseObjects(ctx);
//    JS_AddIntrinsicDate(ctx);
//    JS_AddIntrinsicEval(ctx);
//    JS_AddIntrinsicStringNormalize(ctx);
//    JS_AddIntrinsicRegExp(ctx);
//    JS_AddIntrinsicJSON(ctx);
//    JS_AddIntrinsicProxy(ctx);
//    JS_AddIntrinsicMapSet(ctx);
//    JS_AddIntrinsicTypedArrays(ctx);
//    JS_AddIntrinsicPromise(ctx);
//    JS_AddIntrinsicBigInt(ctx);
//    js_std_add_helpers(ctx, argc, argv);
//
//    extern JSModuleDef *js_init_module_fib(JSContext *ctx, const char *name);
//    js_init_module_fib(ctx, "fib.so");
//
//    std::string buf("import { fib } from 'fib.so';console.log(fib(10));");
//    size_t buf_len = buf.size();
//    const char *filename = "test.js";
//
//    eval_buf(ctx, buf.data(), buf_len, filename, JS_EVAL_TYPE_MODULE);
//
//    JS_FreeContext(ctx);
//    JS_FreeRuntime(rt);

    const int window_width = 1024;
    const int window_height = 768;

    // Initializes the shell which provides common functionality used by the included samples.
    if (!Shell::Initialize())
        return -1;

    // Constructs the system and render interfaces, creates a window, and attaches the renderer.
    if (!Backend::Initialize("Load Document Sample", window_width, window_height, true))
    {
        Shell::Shutdown();
        return -1;
    }

    // Install the custom interfaces constructed by the backend before initializing RmlUi.
    Rml::SetSystemInterface(Backend::GetSystemInterface());
    Rml::SetRenderInterface(Backend::GetRenderInterface());

    // RmlUi initialisation.
    Rml::Initialise();

    // Create the main RmlUi context.
    Rml::Context* context = Rml::CreateContext("main", Rml::Vector2i(window_width, window_height));
    if (!context)
    {
        Rml::Shutdown();
        Backend::Shutdown();
        Shell::Shutdown();
        return -1;
    }

    // The RmlUi debugger is optional but very useful. Try it by pressing 'F8' after starting this sample.
    Rml::Debugger::Initialise(context);

    // Fonts should be loaded before any documents are loaded.
    Shell::LoadFonts();

    // Load and show the demo document.
    if (Rml::ElementDocument* document = context->LoadDocument("/home/titto/CProjects/RmlUi5.0/Samples/assets/demo.rml"))
        document->Show();

    bool running = true;
//    while (running)
//    {
//        // Handle input and window events.
//        running = Backend::ProcessEvents(context, nullptr);
//
//        // This is a good place to update your game or application.
//
//        // Always update the context before rendering.
//        context->Update();
//
//        // Prepare the backend for taking rendering commands from RmlUi and then render the context.
//        Backend::BeginFrame();
//        context->Render();
//        Backend::PresentFrame();
//    }

    // Shutdown RmlUi.
    Rml::Shutdown();

    Backend::Shutdown();
    Shell::Shutdown();

    return 0;
}
