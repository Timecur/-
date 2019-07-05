LDFLAGS=-std=c++11
INCLUDE=-I ../aip-cpp-sdk/
LIB=-ljsoncpp -lcurl -lcrypto -lpthread

#.PHONY:all
#all:testJson ParseJson

#testJson:testJson.cc
#	g++ -o testJson testJson.cc $(LDFLAGS)

#ParseJson:ParseJson.cc
#	g++ -o ParseJson ParseJson.cc $(LDFLAGS)

VoiceInter:VoiceInter.cc
	g++ -o VoiceInter VoiceInter.cc $(LDFLAGS) $(LIB) $(INCLUDE)

clean:
	rm -f VoiceInter
