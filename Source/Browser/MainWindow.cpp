//
// Created by titto on 2022/12/29.
//

#include "MainWindow.h"
#include "../Script/RunTime.h"
#include "../Script/ScriptPlugin.h"
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include <RmlUi/Config/Config.h>
#include <RmlUi_Backend.h>
#include <Shell.h>
#include <iostream>
#include <string>

int main(int argc, char** argv) {

	Rml::Script::ScriptPlugin* script_plugin = Rml::Script::GetInstance();
	Rml::RegisterPlugin(script_plugin);

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
//    Rml::Debugger::Initialise(context);

    // Fonts should be loaded before any documents are loaded.
    Shell::LoadFonts();

    // Load and show the demo document.
    if (Rml::ElementDocument* document = context->LoadDocument("/home/titto/CProjects/RmlUi5.0/Samples/assets/demo.rml"))
        document->Show();

    qjs::Context* js_context = Rml::Script::GetContext();
    try
    {
		js_context->global()["log"] = [](const Rml::String& str){
			std::cout << str << std::endl;
		};
        js_context->eval("const ele = document.getElementById(\"title_bar\");"
						 "log('ID' + ':' + ele.getId());"
						 "log(document.getTitle());"
						 "const ele2 = document.createElement(document, 'div');"
						 "log('ele2' + ele2.getId())");
    }catch(qjs::exception)
    {
        auto exc = js_context->getException();
        std::cerr << (std::string) exc << std::endl;
        if((bool) exc["stack"])
            std::cerr << (std::string) exc["stack"] << std::endl;
        return 1;
    }

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
