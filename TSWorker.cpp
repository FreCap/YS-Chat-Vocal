#include "TSWorker.h"
    
void TSWorker::operator()()  
{  

    printf("Started TSWorker %d\n", n_thread);

    shared_ptr<TTransport> socket(new TSocket(chatServerDNS, chatServerPort));
    shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

    ThriftServer::ClientVoiceToTextClient client(protocol);
    transport->open();

    boost::function<void(ThriftServer::ClientVoiceToTextClient client)> func;

    while(TSWorker::TSCallbackTasks.wait_and_pop(func)){

        func(client);

    }

}  
       
void TSWorker::sendInfo(int portTS, int virtualServerMaxClients){
	
	// virtualServerId unused, da vedere per uno sviluppo futuro
	
    shared_ptr<TTransport> socket(new TSocket(chatServerDNS, chatServerPort));
    shared_ptr<TTransport> transport(new TBufferedTransport(socket));//TBufferedTransport TFramedTransport
    shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

    ThriftServer::ClientVoiceToTextClient client(protocol);
    transport->open();
    printf("MANDO INFO\n");
    client.info(VoiceServerId ,virtualServerMaxClients,thisServerDNS ,portTS, portThrift);
    
    transport->close();

}
    
void TSWorker::start(int nThread)
{

    for (int i = 0; i < nThread; i++) {
        TSWorker worker(i);
        boost::thread workerThread(worker);  
    }

}

concurrentQueue<boost::function<void(ThriftServer::ClientVoiceToTextClient client)> > TSWorker::TSCallbackTasks(idConcurrent);

