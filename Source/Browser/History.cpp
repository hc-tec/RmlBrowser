//
// Created by titto on 2023/4/18.
//

#include "History.h"

#include <fstream>

#include <co/time.h>
#include <json/json.h>

#include "log/logging.h"

namespace Rml {
namespace Browser {

void History::Collect(His s) {
	if (s.title.substr(0, 3) == "Rml") return;
	s.time = now::str("%Y-%m-%d %H:%M").c_str();
    history_.push_back(std::move(s));
    Dump();
}

void History::Remove(const String& url) {

}

void History::Dump() {
    Json::Value v;
    for (auto& it : history_) {
        Json::Value t;
        t["title"] = it.title;
        t["icon"] = it.icon;
        t["url"] = it.url;
        t["time"] = it.time;
        v.append(t);
    }
    std::ofstream os;             //实例化输出类
    os.open("history.json", std::ios::out); //打开并指定文件以输出方式打开（往文件里写）
    if (!os.is_open())               //检查文件打开情况
        LOG(DEBUG) << "open file failed";
    Json::StyledWriter sw;      //定义格式化输出
    os << sw.write(v);       //输出到文件
    os.close();                 //关闭文件

}

void History::Load() {
    std::ifstream ifs("history.json");
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj);
    for (auto& it : obj) {
        His s = {
            .title = it["title"].asString(),
            .icon = it["icon"].asString(),
            .url = it["url"].asString(),
            .time = it["time"].asString(),
        };
        history_.push_back(s);
    }
}

History::History() {
    Load();
}

}
}