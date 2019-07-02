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
};
#endif
