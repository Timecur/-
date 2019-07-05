#ifndef _VOICEINTER_HPP_
#define _VOICEINTER_HPP_
#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <unistd.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unordered_map>
#include "speech.h"
#include "base/http.h"
#include "json/json.h"

#define SPEECH_FILE "temp_file/demo.wav"
#define PLAY_FILE "temp_file/play.mp3"
#define CMD_ETC "command.etc"

// 访问百度语音识别类
class SpeechRec{
  public:
    // 建立客户端
    SpeechRec(){
      sclient = new aip::Speech(app_id, api_key, secret_key);
    }

    // 识别本地文件
    void ASR(int &err_code, std::string &message){

      std::cout << "......正在识别" << std::endl;

      // 语音文件
      std::string file_content;
      aip::get_file_content(SPEECH_FILE, &file_content);

      std::map<std::string, std::string> options;
      options["dev_pid"] = "1536"; // 识别语言类型
      options["lan"] = "ZH"; //中文

      // recognize，无可选参数：aip::null,有参数：options
      Json::Value result = sclient->recognize(file_content, "wav", 16000, options);

      // 检测是否正确返回
      err_code = result["err_code"].asInt(); // err_code 错误码
      if(err_code == 0){
        // 识别成功，取result数组中结果
        message = result["result"][0].asString();
      }
      else{
        message = "...识别错误...";
      }
    }
   
    void VoiceSynthesis(std::string& text){
      std::ofstream ofile;
      std::string ret;
      std::map<std::string, std::string> options;
      options["spd"] = "5";
      options["per"] = "4";
      options["vol"] = "15";

      ofile.open(PLAY_FILE, std::ios::out | std::ios::binary);
      Json::Value result = sclient->text2audio(text, options, ret);
      if(ret.empty()){
        std::cerr << "语音合成错误: " <<  result.toStyledString() << std::endl;
      }
      else{
        ofile << ret;
      }
      ofile.close();
    }


    ~SpeechRec(){
      delete sclient;
      sclient = nullptr;
    }
  private:
    std::string app_id = "16713718";
    std::string api_key = "n0MYBllGLGQQgzGU29S5YvL5";
    std::string secret_key = "atQYVeXZlIIgGf89yz94Zlz0WCV2YXZs";
    aip::Speech *sclient;
};

// 图灵机器人交互式类
class InterRobot{
  public:
    InterRobot(){}

    std::string MakeJson(const std::string& message){

      Json::Value root;
      Json::Value item1;
      Json::Value item2;
      Json::Value item3;

      item1["apiKey"] = api_key;
      item1["userId"] = user_id;
      
      root["userInfo"] = item1;
      
      item2["text"] = message;
      item3["inputText"] = item2;
      root["perception"] = item3;
      root["reqType"] = 0;

      Json::StreamWriterBuilder wb;
      std::ostringstream os; 
      std::unique_ptr<Json::StreamWriter> jsonWriter(wb.newStreamWriter());
      jsonWriter->write(root, &os);
      std::string str = os.str();
      return str;
    }
 

  // 发出Http post请求
  // 需要序列化、反序列化方法
  std::string PostReq(std::string& data){

    std::string response;
    std::cout << data << std::endl;
    // 通过http的post请求，获取
    //int post(string url,map<std::string, std::string> const * params, \
    // Json::Value const & data, map<std::string, std::string> const * headers, string * response) const
    int res = rclient.post(url, nullptr, data, nullptr, &response);
    if(res != CURLcode::CURLE_OK){
      return "";
    }
    return response;
  }

  std::string Parse_Json(std::string& ret){

    JSONCPP_STRING errs;
    Json::Value root;
    Json::CharReaderBuilder readerBuilder;

    std::unique_ptr<Json::CharReader> const jsonReader(readerBuilder.newCharReader());
    bool res = jsonReader->parse(ret.data(), ret.data()+ret.size(), &root, &errs);
    
    if(!res || !errs.empty()){
      std::cerr << "json parse error: " << errs << std::endl;
      return "";
    }  
    // 从results的vlaues中的text取出回应消息
    Json::Value results = root["results"][0];
    Json::Value values = results["values"];
    return values["text"].asString();
  }

  std::string Talk(std::string &message){

    // 序列化
    std::string json_str = MakeJson(message);

    // 通过http发送自己的语音信息
    // 获取机器人的回应消息
    std::string ret = PostReq(json_str);

    // 反序列化
    std::string text = Parse_Json(ret);

    return text;
  }

~InterRobot(){}

private:
  // 图灵机器人接入地址
  std::string url = "http://openapi.tuling123.com/openapi/api/v2";
  // apikey
  std::string api_key = "46445cb9c37540118891adf6a03af36d";
  // 用户ID
  std::string user_id = "1";
  // 使用百度语音识别所带的Http Client发起相关请求
  aip::HttpClient rclient;

};

// 语音交互核心逻辑
class VoiceInter{
  public:

    VoiceInter(){
      char buffer[256];
      std::ifstream in(CMD_ETC);
      if(!in.is_open()){
        std::cerr << "open file error" << std::endl;
        exit(1);
      }
      std::string sep = ":";
      while(in.getline(buffer, sizeof(buffer))){
        std::string str = buffer;
        std::size_t pos = str.find(sep);
        if(pos){
          std::string key = str.substr(0, pos);
          std::string val = str.substr(pos+sep.size());
          key += "。";
          commandSet.insert(std::make_pair(key, val));
        }
        else{
          std::cout << "Load ECT_FILE Failed!!!" << std::endl;
        }
      }
      in.close();
    }

    bool IsCommand(std::string &message){
      if(this->commandSet.count(message) > 0){
        return true;
      }
      return false;
    }

    // 执行指定命令
    bool Exec(std::string command, bool flag){
      /*
       * FILE * popen(const char *command , const char *type );
       * int pclose(FILE *stream);
       * 通过创建管道，调用fork()产生一个子进程，执行shell以命令开启一个进程
       */
      FILE* fp = popen(command.c_str(), "r");
      if(fp == NULL){
        // 返回值错误
        std::cerr << "popen error!" << std::endl;
        return false;
      }
      // 执行预设命令 
      if(flag){
        char c;
        while(fread(&c, 1, 1, fp) > 0){
          std::cout << c;
        }
      }
      pclose(fp);
      return true;
    }

    // 进行录音、语音识别
    bool Record_ASR(std::string &message){

      // 生成语音文件
      std::string record = "arecord -t wav -c 1 -r 16000 -d 5 -f S16_LE ";
      record += SPEECH_FILE;
      // dev/null linux下垃圾回收文件
      // 2>&1 标准错误流内容输出到标准输出流
      record += " >/dev/null 2>&1 ";// 不显示输出结果或消息 

      // 录音
      std::cout << "已开始录音，请讲话>--------";
      fflush(stdout);
      // 刷新标准输出缓冲区
      int err_code = -1;
      if(Exec(record, false)){
        // 进行语音识别，若成功将结果返回给message，失败返回错误码
        sr.ASR(err_code, message); 
        if(err_code == 0){
          return true;
        }
        std::cout << "语音识别失败！！！" << std::endl; 
      } 
      else{
        std::cout << "录音失败！！！" << std::endl;
      }
      return false;
    }

    void PlayVoice(std::string voice_file){
      std::string cmd = "cvlc --play-and-exit ";
      cmd += voice_file;
      Exec(cmd, false);
    }
    

    // 运行
    void Run(){

      volatile bool quit = false;
      std::string message;
      while(!quit){
        message = "";
        // 录音，语音识别
        bool ret = Record_ASR(message);
        if(ret){
          std::string response;
          // 用户语音文本
          std::cout << "我 >  " <<  message << std::endl;

          // 查看是否命令集中的命令
          if(IsCommand(message)){
            if(message == "退出。"){
              std:: cout << "再见。"<< std::endl;
            }
            else{
              Exec(message, true);
            }
            quit = true;
          }
          else{
            std::string response = robot.Talk(message);
            std::cout << "Robot > " << response << std::endl;
            sr.VoiceSynthesis(response);
            PlayVoice(PLAY_FILE);
          }
        } 
      }
    }

  private:
    // 语音识别对象
    SpeechRec sr;
    // 图灵机器人
    InterRobot robot;
    // 指令集
    std::unordered_map<std::string, std::string> commandSet;
};
