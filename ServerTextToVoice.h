/* 
 * File:   ServerTextToVoice.h
 * Author: root
 *
 * Created on 23 dicembre 2011, 23.44
 */

#ifndef SERVERTEXTTOVOICE_H
#define	SERVERTEXTTOVOICE_H


#include "TClass/ClientTextToVoice.h"
#include <protocol/TBinaryProtocol.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <server/TNonblockingServer.h>


using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;


using boost::shared_ptr;

using namespace ::ThriftServer;

 boost::shared_ptr<boost::thread> m_thread;

 
class ClientTextToVoiceHandler : virtual public ClientTextToVoiceIf {
	public:
		ClientTextToVoiceHandler() {
			// Your initialization goes here
		}

		int32_t channelCreate(const int32_t server_id, const std::string& nome, const std::string& password) {
			printf("channelCreate\n");
			return ServerVoice::createChannel(nome, password);
		}

		bool channelDelete(const int32_t server_id, const std::string& nome) {
			// Your implementation goes here
			printf("channelDelete\n");
		}

		bool disconnectClient(const int32_t server_id, const std::string& clientID) {
			// Your implementation goes here
			printf("disconnectClient\n");
		}

};



void startThriftServer(){
    
    int port = 9090;

    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    shared_ptr<ClientTextToVoiceHandler> handler(new ClientTextToVoiceHandler());
    shared_ptr<TProcessor> processor(new ClientTextToVoiceProcessor(handler));
    TNonblockingServer server(processor, protocolFactory, port);
    server.serve();

}


int thriftAsThread() {
   
    assert(!m_thread);
    m_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&startThriftServer)));

    return 0;
}


#endif	/* SERVERTEXTTOVOICE_H */

