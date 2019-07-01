// 反序列化测试
#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <json/json.h>
void ParseJson(std::string &s)
{
  JSONCPP_STRING errs;
  Json::Value root;
  Json::CharReaderBuilder readerBuilder;
  std::unique_ptr<Json::CharReader> const jsonReader(readerBuilder.newCharReader());
  bool res = jsonReader->parse(s.data(), s.data()+s.size(), &root, &errs);
  if(!res || !errs.empty()){
    std::cerr << "json parse error: " << errs << std::endl;
    return;
  }
  std::cout << "Name: " << root["Name"].asString() << std::endl;
  std::cout << "Age: " << root["Age"].asInt() << std::endl;
  std::cout << "Lang: " << root["Lang"].asString() << std::endl;
}

int main()
{
  //模拟json 串
  std::string s = "{\"Age\" : 26,\"Lang\" : \"c++\",\"Name\" : \"zhangsan\"}";
  std::cout << s << std::endl;
  ParseJson(s);
  return 0;
}
