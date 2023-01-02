//
// Created by titto on 2023/1/2.
//

#ifndef RMLUI_JSDOCUMENTELEMENT_H
#define RMLUI_JSDOCUMENTELEMENT_H

#include "RmlUi/Core/ElementDocument.h"

namespace Rml {
namespace Script {

class JsDocumentElement : public Rml::ElementDocument
{
public:
    JsDocumentElement(const String& tag);
    void LoadInlineScript(const String& content, const String& source_path, int source_line) override;
    void LoadExternalScript(const String& source_path) override;
};

}
}

#endif // RMLUI_JSDOCUMENTELEMENT_H
