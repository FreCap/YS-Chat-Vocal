//thrift  --gen cpp --gen java VoiceToText.thrift

namespace cpp ThriftServer
namespace java sn.thrift
namespace php ThriftServer
namespace perl ThriftServer

service ClientVoiceToText {

	void info ( 1:i32 server_id, 2:i32 max_clients, 3:string DNS, 4:i32 port_TS, 5:i32 port_Thrift )
		
	// callbacks	
	bool onClientConnected( 1:i32 server_id, 2:i32 clientID, 3:string clientNick )
	bool onClientDisconnected ( 1:i32 server_id, 2:i32 clientID )
	bool onClientJoined( 1:i32 server_id, 2:i32 clientID, 3:i32 ChannelID )
	bool onClientLeft( 1:i32 server_id, 2:i32 clientID, 3:i32 ChannelID )
	bool onChannelCreated( 1:i32 server_id, 2:i32 channelID, 3:string channelName )		
	//bool onChannelEdited( 1:i32 server_id, 2:i32 channelID )		
	bool onChannelDeleted( 1:i32 server_id, 2:i32 channelID )
	
}
