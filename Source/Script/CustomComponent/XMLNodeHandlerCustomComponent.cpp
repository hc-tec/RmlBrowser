//
// Created by titto on 2023/5/7.
//

#include "XMLNodeHandlerCustomComponent.h"

#include <sstream>
#include <random>

#include "RmlUi/Core/XMLParser.h"

namespace Rml {

namespace Script {

static UnorderedMap<String, String> slots_map_;
static UnorderedMap<String, CustomComponentAssert> components_map_;
static Vector<CustomComponentAssert> use_components_list_;
static Queue<Element*> need_parse_els_;

static std::string generate_random_string(int length) {
    std::string chars {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"};
    std::default_random_engine rng(std::random_device{}());
    std::uniform_int_distribution<std::string::size_type> dist(0, chars.size()-1);

    std::string result(length, 0);
    for (int i = 0; i < length; ++i) {
        result[i] = chars[dist(rng)];
    }

    return result;
}


Rml::Element* XMLNodeHandlerCustomComponent::ElementStart(XMLParser* parser, const String& name, const XMLAttributes& attributes)
{
    Element* res = XMLNodeHandlerDefault::ElementStart(parser, name, attributes);
    if (components_map_.find(name) == components_map_.end()) return res;
    need_parse_els_.push(res);
    String id = "rue" + generate_random_string(4);
    res->SetAttribute("r_id", id);
    CustomComponentAssert assert = components_map_[name];
    ReplaceScript(assert, id);
    use_components_list_.push_back(std::move(assert));
    return res;
}


bool XMLNodeHandlerCustomComponent::ElementData(XMLParser* parser, const String& data, XMLDataType type)
{
    Element* cur = parser->GetParseFrame()->element;

//    CustomComponentAssert& assert = use_components_list_[use_components_list_.size()-1];
//	String component_data = assert.rml;

//	bool res = XMLNodeHandlerDefault::ElementData(parser, component_data, XMLDataType::InnerXML);
	return XMLNodeHandlerDefault::ElementData(parser, data, XMLDataType::InnerXML);
}

void XMLNodeHandlerCustomComponent::ReplaceScript(CustomComponentAssert& assert, const String& id) {
    String old_str = "__rue__";
    size_t pos = assert.script.find(old_str);
    while (pos != String::npos) {
        assert.script.replace(pos, old_str.length(), id);
        pos = assert.script.find(old_str, pos + id.length());
    }

    pos = assert.rml.find(old_str);
    while (pos != String::npos) {
        assert.rml.replace(pos, old_str.length(), id);
        pos = assert.rml.find(old_str, pos + id.length());
    }
}

void XMLNodeHandlerCustomComponent::RegisterCustomComponent(String name, CustomComponentAssert assert) {
	assert.name = name;
	if (components_map_.find(name) != components_map_.end()) return;
    XMLParser::RegisterNodeHandler(name, MakeShared<XMLNodeHandlerCustomComponent>());
    components_map_.insert({std::move(name), std::move(assert)});
}



String XMLNodeHandlerCustomComponent::GetSlot(const String& id) {
    auto it = slots_map_.find(id);
	if (it != slots_map_.end()) {
		return it->second;
	}
	return "";
}

UnorderedMap<String, CustomComponentAssert>& XMLNodeHandlerCustomComponent::GetComponentAssets() { return components_map_; }

Vector<CustomComponentAssert>& XMLNodeHandlerCustomComponent::GetUseComponentAssets() { return use_components_list_; }

void XMLNodeHandlerCustomComponent::ClearUseComponentAssets() {
	use_components_list_.clear();
}

void XMLNodeHandlerCustomComponent::SetSlotData(String id, String rml) {
	slots_map_.insert({std::move(id), std::move(rml)});
}

bool XMLNodeHandlerCustomComponent::NeedParse() { return !need_parse_els_.empty(); }

Element* XMLNodeHandlerCustomComponent::GetParseEl() {
	Element* el = need_parse_els_.front();
	need_parse_els_.pop();
	return el;
}

}
}