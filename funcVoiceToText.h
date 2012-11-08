/* 
 * File:   funcVoiceToText.h
 * Author: Francesco Capponi
 *
 * Created on 31 gennaio 2012, 16.43
 */

#ifndef FUNCVOICETOTEXT_H
#define	FUNCVOICETOTEXT_H

#include <stdio.h>
#include <sys/types.h>
#include <string>
#include <typeinfo>


#include "TClass/ClientVoiceToText.h"
#include "config.h"

using namespace boost;



class funcVoiceToText
{    
    
	public:   
    
		static void info(ThriftServer::ClientVoiceToTextClient client, const int32_t max_clients, const std::string& DNS, const int32_t port_TS, const int32_t port_Thrift) {
			client.info(VoiceServerId, max_clients, DNS, port_TS, port_Thrift);
			printf("info\n");
		}

		static void onClientConnected(ThriftServer::ClientVoiceToTextClient client,  const int32_t clientID, const std::string& clientNickname) {
			client.onClientConnected(VoiceServerId, clientID, clientNickname);
			printf("onClientConnected\n");
		}

		static void onClientDisconnected(ThriftServer::ClientVoiceToTextClient client,  int32_t clientID) {
			client.onClientDisconnected(VoiceServerId, clientID);
			printf("onClientDisconnected\n");
		}

		static void onClientJoined(ThriftServer::ClientVoiceToTextClient client,  const int32_t clientID, const int32_t ChannelID) {
			client.onClientJoined(VoiceServerId, clientID, ChannelID);
			printf("onClientMoved\n");
		}

		static void onClientLeft(ThriftServer::ClientVoiceToTextClient client,  const int32_t clientID, const int32_t ChannelID) {
			client.onClientLeft(VoiceServerId, clientID, ChannelID);
			printf("onClientMoved\n");
		}		
		
		static void onChannelCreated(ThriftServer::ClientVoiceToTextClient client, const int32_t channelID, const std::string& channelName) {
			client.onChannelCreated(VoiceServerId, channelID, channelName);
			printf("onChannelCreated\n");
		}
/*
		static void onChannelEdited(ThriftServer::ClientVoiceToTextClient client,  const int32_t invokerClientID, const int32_t channelID) {
			client.onChannelEdited(VoiceServerId, invokerClientID, channelID, NICKNAME);
			//TODO decidere se è direttamente da eliminare o fare un'eventuale verifica del nome del channel
			printf("onChannelEdited\n");
		}
*/
		static void onChannelDeleted(ThriftServer::ClientVoiceToTextClient client, const int32_t channelID) {
			client.onChannelDeleted(VoiceServerId, channelID);
			//TODO decidere se è direttamente da eliminare o fare un'eventuale verifica del nome del channel
			printf("onChannelDeleted\n");
		}
   
	private:     
		
	/*	static std::string getNicknameby( const int32_t clientID){
			
			unsigned int error;
			char* name;
			if((error = ts3server_getClientVariableAsString( , clientID, CLIENT_NICKNAME, &name)) != ERROR_ok) {  
				printf("Error querying client nickname: %d\n", error);
			}
			if(error){
				return "";
			}
			printf("%u - %s\n", clientID, name);
			ts3server_freeMemory(name); 
			std::string clientNick(name);
			return clientNick;
		}*/
		
};  


#endif	/* FUNCVOICETOTEXT_H */

