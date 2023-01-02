//
// Created by titto on 2023/1/2.
//

#include "JsDocumentElement.h"

namespace Rml {

namespace Script {

JsDocumentElement::JsDocumentElement(const String& tag) : ElementDocument(tag)
{
}

void JsDocumentElement::LoadInlineScript(const String& context, const String& source_path, int source_line)
{
    Log::Message(Log::LT_DEBUG, "script: %s", context.data());
}

void JsDocumentElement::LoadExternalScript(const String& source_path)
{

}

}

}