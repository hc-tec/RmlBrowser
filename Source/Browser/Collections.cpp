//
// Created by titto on 2023/4/17.
//

#include "Collections.h"

#include <fstream>

#include <json/json.h>
#include "log/logging.h"

namespace Rml {
namespace Browser {

void Collections::Collect(Star s) {
	stars_.push_back(std::move(s));
	Dump();
}

void Collections::Remove(const String& url) {

}

void Collections::Dump() {
    Json::Value v;
	for (auto& it : stars_) {
        Json::Value t;
		t["title"] = it.title;
        t["icon"] = it.icon;
        t["url"] = it.url;
		v.append(t);
	}
    std::ofstream os;             //实例化输出类
    os.open("stars.json", std::ios::out); //打开并指定文件以输出方式打开（往文件里写）
    if (!os.is_open())               //检查文件打开情况
        LOG(DEBUG) << "open file failed";
    Json::StyledWriter sw;      //定义格式化输出
    os << sw.write(v);       //输出到文件
    os.close();                 //关闭文件

}

void Collections::Load() {
    std::ifstream ifs("stars.json");
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj);
	for (auto& it : obj) {
		Star s = {
			.title = it["title"].asString(),
            .icon = it["icon"].asString(),
            .url = it["url"].asString(),
		};
		stars_.push_back(s);
	}
}

Collections::Collections() {
	Load();
}

}
}