//thrift  --gen cpp --gen java TextToVoice.thrift

namespace cpp ThriftServer
namespace java sn.thrift
namespace php ThriftServer
namespace perl ThriftServer

service ClientTextToVoice {
	
	// server_id deprecated. da decidere se debba diventare VirtualTSServerID	

	//return l'id del channel
	i32 channelCreate ( 1:i32 server_id, 2:string nome, 3:string password )
	bool channelDelete ( 1:i32 server_id, 2:string nome )

	bool disconnectClient ( 1:i32 server_id, 2:string clientID )
	//old bool add_client ( 1:i32 server_id, 2:string channel_name, 3:string client_name )


}
