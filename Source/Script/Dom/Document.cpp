//
// Created by titto on 2022/12/30.
//

#include "Document.h"

#include "RmlUi/Core/Context.h"
#include "RmlUi/Core/ElementDocument.h"
#include "JsDocumentElement.h"

namespace Rml {

namespace Script {

void Document::Glue(qjs::Context::Module& m)
{
    m.class_<ElementDocument>("Document")
        .constructor<const String&>("Document")
        .base<Rml::Element>()
		.fun<&ElementDocument::SetTitle>("setTitle")
        .fun<&ElementDocument::GetTitle>("getTitle")
        .fun<&ElementDocument::GetSourceURL>("getSourceURL")
        .fun<&ElementDocument::CreateElement>("createElement")
        .fun<&ElementDocument::Hide>("hide")
        .fun<&ElementDocument::Show>("show");

    m.class_<Rml::Property>("Property")
        .constructor<>("Property")
        .fun<&Rml::Property::ToString>("asString")
        .fun<&Rml::Property::GetSource>("getSource");

    m.function("getPropString", [](const Rml::Property* prop){
		return prop->ToString();
	});
    m.function("getPropSource", [](const Rml::Property* prop){
      return prop->source;
    });
//    m.function("getSourceRule", [](const Rml::PropertySource* source){
//      return source->rule_name;
//    });
//    m.function("getSourcePath", [](const Rml::PropertySource* source){
//      return source->path;
//    });
//    m.function("getSourceLineNumber", [](const Rml::PropertySource* source){
//      return source->line_number;
//    });
}

}

}