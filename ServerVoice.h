#ifndef SERVERVOICE_H
#define SERVERVOICE_H

#ifdef WINDOWS
#include <Windows.h>
#else
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <stdio.h>


#include <sys/stat.h>
#include <TeamTalk4.h>
#include <iostream>
#include <sstream>
#include <assert.h>

#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>

#include "TClass/ClientVoiceToText.h"
#include "funcVoiceToText.h"

#include "concurrentQueue.h"

#include "TSWorker.h"


#ifdef _WIN32
#define SLEEP(x) Sleep(x)
#else
#define SLEEP(x) usleep(x*1000)
#endif





#ifdef UNICODE
#define cout wcout
#define cin wcin
#define string wstring
#define ostringstream wostringstream
#define istringstream wistringstream
#define _T(x) L ## x
#else
#define _T
#endif



using namespace boost;
using namespace std;


class ServerVoice {
	public:

        int start();
		int stop();
               
		void static moveClient(INT32 clientID, INT32 newChannelID);
		void static deleteChannel(INT32 channelID);
		int  static createChannel(const string& name, const string& password);
		//int  static createChannel(const wstring& wname, const wstring& wpassword);
		void static showChannels();
                
		void startAsThread();

		void processTTMessage(const TTMessage& msg);
		std::string get_str(const std::string& input);
		int get_int(int value);
                
	private:

	    boost::shared_ptr<boost::thread> m_thread;

		static void onClientConnected(INT32 clientID, const string& clientNickname);

		static void onClientDisconnected(INT32 clientID);

		static void onClientJoined(INT32 clientID, INT32 ChannelID);
		
		static void onClientLeft(INT32 clientID, INT32 ChannelID);

		static void onChannelCreated(INT32 channelID, const string& channelName);

		//static void onChannelEdited(INT32 invokerClientID, INT32 channelID);

		static void onChannelDeleted(INT32 channelID);

};



#endif