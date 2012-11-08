#ifndef TSWORKER_H
#define	TSWORKER_H

#include "config.h"

#include <stdio.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>

#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

#include "concurrentQueue.h"

#include "TClass/ClientVoiceToText.h"



using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace boost;


class TSWorker
{    
    
public:   
     
    static concurrentQueue<boost::function<void(ThriftServer::ClientVoiceToTextClient client)> > TSCallbackTasks;
    
    TSWorker(int n): n_thread(n) {};
    
    void operator()();
       
    static void sendInfo(int portTS, int virtualServerMaxClients);
    
    static void start(int nThread);
   
private:     
    int n_thread; 
};  

#endif	/* TSWORKER_H */
