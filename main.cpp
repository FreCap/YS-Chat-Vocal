/* 
 * File:   main.cpp
 * Author: a
 *
 * Created on 13 dicembre 2011, 21.10
 */



#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "ServerVoice.h"
#include "ServerTextToVoice.h"

#include "configFile.h"

//typedef VoiceToText<10> VoiceToTextManager; // 10 è il numero di client OLD


 void myPrinter(const char* text, int n){            
            printf("\n%s %d\n", text, n);
        }

int main() {
	
	 /*
	uint64 dsa = 213123;
	int32_t asd;
	
	asd = (int32_t) dsa;
	printf("\n%d\n", dsa);
	
   
    char *someText="test";        
    
    
    
   boost::function<void(int asd)> functionWithSavedArgs = boost::bind(&myPrinter, someText, _1);
   
    functionWithSavedArgs(123);
   
    	boost::this_thread::sleep(boost::posix_time::seconds(300));

    boost::this_thread::sleep(boost::posix_time::seconds(300));
    */
	
	ConfigFile cf("config.ys");

	chatServerDNS.assign(cf.Value("ChatServer","chatServerDNS"));
	chatServerPort = cf.Value("ChatServer","chatServerPort");

	thisServerDNS.assign(cf.Value("VoiceServer","thisServerDNS"));
	portTS = cf.Value("VoiceServer","portTS");
	portThrift = cf.Value("VoiceServer","portThrift");

	TT_serverDNS.assign(cf.Value("TTServer","TT_serverDNS"));
	TT_serverTCPport = cf.Value("TTServer","TT_serverTCPport");
	TT_serverUDPport = cf.Value("TTServer","TT_serverUDPport");
	TT_serverPassword.assign(cf.Value("TTServer","TT_serverPassword"));
	TT_User.assign(cf.Value("TTServer","TT_User"));
	TT_UserPassword.assign(cf.Value("TTServer","TT_UserPassword"));
	
    TSWorker::start(1);
   
	boost::this_thread::sleep(boost::posix_time::seconds(2));

    
    // Buahaha
    
	thriftAsThread();
          
          
	ServerVoice serverVoice;
	serverVoice.startAsThread();

	boost::this_thread::sleep(boost::posix_time::seconds(2));

        
    serverVoice.createChannel("asdua","asdas");
       
    //serverVoice.showChannels();
        
                
	boost::this_thread::sleep(boost::posix_time::seconds(300));

       
        
        
}

