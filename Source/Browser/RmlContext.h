//
// Created by titto on 2023/5/5.
//

#ifndef RMLUI_RMLCONTEXT_H
#define RMLUI_RMLCONTEXT_H

#include <RmlUi/Core.h>
#include <RmlUi/Core/Context.h>
#include <RmlUi_Backend.h>
#include "RenderScheduler.h"

namespace Rml {
namespace Browser {

const int window_width = 1550;
const int window_height = 760;

static const char* BROWSER_ID = "Rml-browser";

static Rml::Context* context_;

class RmlContext {
public:

    ~RmlContext();

    static Context* Get() {
		if (context_ == nullptr)
		{
			context_ = Rml::CreateContext(BROWSER_ID, Rml::Vector2i(window_width, window_height));
			Backend::RegisterContext(context_, RenderScheduler::Get());
		}
		return context_;
	}

	static void Render() {
        RenderScheduler::Get()->go([&](){
          RMLUI_ASSERT(context_)
          // This is a good place to update your game or application.
          // Always update the context before rendering.
          context_->Update();

          // Prepare the backend for taking rendering commands from RmlUi and then render the context.
          Backend::BeginFrame();
          context_->Render();
          Backend::PresentFrame();
          Render();
        });
    }

private:

};

}
}

#endif // RMLUI_RMLCONTEXT_H
