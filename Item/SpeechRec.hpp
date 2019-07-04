#ifndef _SPEECHREC_
#define _SPEECHREC_

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <map>
#include <unistd.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unordered_map>
#include "speech.h"
#include "base/http.h"//直接使用百度提供的httpclient
#define SPEECH_FILE "temp_file/demo.wav"
#define PLAY_FILE "temp_file/play.mp3"
#define CMD_ETC "command.etc"
//访问百度语音识别类
class SpeechDiv{
};
//图灵机器人交互式类
class InterRobot{
};
//SpeechRec 完成核心逻辑
class SpeechRec{
private:
SpeechRec sr;
InterRobot robot;
std::unordered_map<std::string, std::string> command_set;
public:
//加载命令执行配置文件
Jarvis()
{
char buffer[256];
std::ifstream in(CMD_ETC);
if(!in.is_open()){
std::cerr << "open file error" <<std::endl;
exit(1);
}
std::string sep = ":";
while(in.getline(buffer, sizeof(buffer))){
std::string str = buffer;
std::size_t pos = str.find(sep);
if(std::string::npos == pos){
std::cerr << "Load Etc Error" << std::endl;
exit(2);
}
std::string k = str.substr(0, pos);
std::string v = str.substr(pos+sep.size());
k+="。";
command_set.insert(std::make_pair(k, v));
}
std::cout << "Load command etc ... done" << std::endl;
in.close();
;
record += ">/dev/null 2>&1"; //不显示输出结果或者消息
std::cout << "...请讲话...";
fflush(stdout);
if(Exec(record, false)){
sr.ASR(err_code, message);
if(err_code == 0){
return true;
}
std::cout << "语音识别失败..." << std::endl;
}
else{
std::cout << "录制失败..." << std::endl;
}
return false;
}
//使用百度语音合成接口，文本转语音，在使用cvlc进行本地播放
bool TTSAndPlay(std::string message)
{
//cvlc命令行式的播放
std::string play = "cvlc --play-and-exit ";//播放完毕退出：--play-and-exit
play += PLAY_FILE;
play += " >/dev/null 2>&1";
sr.TTS(message); //语音识别
Exec(play, false); //执行播放
#endif
