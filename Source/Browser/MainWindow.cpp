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

bool continue_running = false;

int StartWindow() {
    continue_running = false;
	Rml::Script::ScriptPlugin* script_plugin = Rml::Script::GetInstance();
	Rml::RegisterPlugin(script_plugin);

    const int window_width = 1550;
    const int window_height = 760;

    // Initializes the shell which provides common functionality used by the included samples.
    if (!Shell::Initialize())
        return -1;

    // Constructs the system and render interfaces, creates a window, and attaches the renderer.
    if (!Backend::Initialize("Rml Browser", window_width, window_height, true))
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

    qjs::Context* js_context = Rml::Script::GetContext();
    js_context->global()["log"] = [](const Rml::String& str){
      std::cout << str << std::endl;
    };


    // Load and show the demo document.
	Rml::String rml("/home/titto/CProjects/RmlUi5.0/Samples/web/chromium-intro/index.rml");
    Rml::ElementDocument* document = context->LoadDocument(rml);
    if (document)
        document->Show();

    js_context->global()["reload"] = [&](){
        Backend::RequestExit();
        Rml::Script::GetContext(true);
        continue_running = true;
        std::cout << "--Reload--" << std::endl;
    };

    bool running = true;
    while (running)
    {
        // Handle input and window events.
        running = Backend::ProcessEvents(context, &Shell::ProcessKeyDownShortcuts);

        // This is a good place to update your game or application.
        // Always update the context before rendering.
        context->Update();

        // Prepare the backend for taking rendering commands from RmlUi and then render the context.
        Backend::BeginFrame();
        context->Render();
        Backend::PresentFrame();
    }

    // Shutdown RmlUi.
    Rml::Shutdown();

    Backend::Shutdown();
    Shell::Shutdown();
    return 0;
}

int main(int argc, char** argv) {
	do
	{
        StartWindow();
	} while (continue_running);
}
