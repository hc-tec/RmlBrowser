//
// Created by titto on 2023/4/20.
//

#include "RmlUi/Core/Utils.h"

namespace Rml {

String Absolutepath(const String& source, const String& base)
{
	String joined_path;
	::Rml::GetSystemInterface()->JoinPath(joined_path, StringUtilities::Replace(base, '|', ':'), StringUtilities::Replace(source, '|', ':'));
	return joined_path;
}

}