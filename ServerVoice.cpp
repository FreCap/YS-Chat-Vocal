
#include "ServerVoice.h"

#define CHECK_ERROR(x) if((error = x) != ERROR_ok) { goto on_error; }
TTInstance* ttInst;

/*
int ServerVoice::createChannel(const wstring& wname, const wstring& wpassword) {
	wstring name(wname.begin(), wname.end());
	wstring password(wpassword.begin(), wpassword.end());
	return ServerVoice::createChannel(name, password);
}*/

int ServerVoice::createChannel(const string& name, const string& password) {
	unsigned int error;
	INT32 newChannelID;

	TTCHAR nameA[TT_STRLEN] = {0};
	name.copy(nameA, 0,name.length());

	TTCHAR passwordA[TT_STRLEN] = {0};
	password.copy(passwordA, 0,password.length());

/*
	TTCHAR nameA[TT_STRLEN] = {0};
	for(int i = 0; i<name.length(); i++){
		nameA[i] = nameC[i];
	}
	
	char passwordC[password.length() + 1];
	TTCHAR passwordA[TT_STRLEN] = {0};
	for(int i = 0; i<password.length(); i++){
		passwordA[i] = passwordC[i];
	}*/
	
	Channel chan = {0};
    chan.nParentID = TT_GetRootChannelID(ttInst);
	strncpy(chan.szName, nameA, TT_STRLEN);
	strncpy(chan.szPassword, passwordA, TT_STRLEN);
	chan.nMaxUsers = 1000;
	chan.codec.nCodec = SPEEX_CODEC;
	chan.codec.speex.bStereoPlayback = FALSE;
	chan.codec.speex.bUseJitterBuffer = FALSE;
	chan.codec.speex.nBandmode = 2;
	chan.codec.speex.nMSecPerPacket = 40;
	chan.codec.speex.nQuality = 4;
	chan.uChannelType = CHANNEL_DEFAULT;
	
	
	BOOL b = TT_DoMakeChannel(ttInst, &chan);
	
    return b;

on_error:
    printf("Error creating channel: %d\n", error);
	return 0;
}


void ServerVoice::deleteChannel(INT32 channelID) {
	
	BOOL b = TT_DoRemoveChannel(ttInst, channelID);

}

void ServerVoice::moveClient(INT32 clientID, INT32 newChannelID) {
	
	BOOL b = TT_DoMoveUser(ttInst, clientID, newChannelID);
        
}

// ################ CALLBACKS SECTION ########################


void ServerVoice::onClientConnected(INT32 clientID, const string& clientNickname) {
	function<void(ThriftServer::ClientVoiceToTextClient client)> func = boost::bind(&funcVoiceToText::onClientConnected, _1, (int32_t) clientID, clientNickname);
	TSWorker::TSCallbackTasks.push(func);
}

void ServerVoice::onClientDisconnected(INT32 clientID) {
	function<void(ThriftServer::ClientVoiceToTextClient client)> func = boost::bind(&funcVoiceToText::onClientDisconnected, _1, (int32_t) clientID );
	TSWorker::TSCallbackTasks.push(func);
}

void ServerVoice::onClientJoined(INT32 clientID, INT32 ChannelID) {
	function<void(ThriftServer::ClientVoiceToTextClient client)> func = boost::bind(&funcVoiceToText::onClientJoined, _1, (int32_t) clientID, ChannelID);
	TSWorker::TSCallbackTasks.push(func);
}

void ServerVoice::onClientLeft(INT32 clientID, INT32 ChannelID) {
	function<void(ThriftServer::ClientVoiceToTextClient client)> func = boost::bind(&funcVoiceToText::onClientLeft, _1, (int32_t) clientID, ChannelID);
	TSWorker::TSCallbackTasks.push(func);
}


void ServerVoice::onChannelCreated(INT32 channelID, const string& channelName) {
	function<void(ThriftServer::ClientVoiceToTextClient client)> func = boost::bind(&funcVoiceToText::onChannelCreated, _1, (int32_t) channelID, channelName );
	TSWorker::TSCallbackTasks.push(func);
}
/*
void ServerVoice::onChannelEdited(INT32 invokerClientID, INT32 channelID) {
	function<void(ThriftServer::ClientVoiceToTextClient client)> func = boost::bind(&funcVoiceToText::onChannelEdited, _1, (int32_t) serverID, invokerClientID, (int32_t) channelID );
	TSWorker::TSCallbackTasks.push(func);
}
*/
void ServerVoice::onChannelDeleted(INT32 channelID) {
	function<void(ThriftServer::ClientVoiceToTextClient client)> func = boost::bind(&funcVoiceToText::onChannelDeleted, _1, (int32_t) channelID);
	TSWorker::TSCallbackTasks.push(func);
}

int ServerVoice::start() {

    TTMessage msg;
    int wait_ms, cmd_id;
    struct stat s_file;

    ttInst = TT_InitTeamTalkPoll();

    int i = stat ( "lame_enc.dll", &s_file );
    if ( i != 0 )
        cout << "lame_enc.dll not found, so audio will be stored to .wav instead of .mp3" << endl;

    BOOL b =TT_Connect(ttInst, TT_serverDNS.c_str(), TT_serverTCPport, TT_serverUDPport, 0, 0);
    if(!b){
		//TODO de-init all
	}

    //wait for connect event
    wait_ms = 10000;
    if(!TT_GetMessage(ttInst, &msg, &wait_ms) || msg.wmMsg == WM_TEAMTALK_CON_FAILED || msg.wmMsg != WM_TEAMTALK_CON_SUCCESS){
		//TODO error
	}
	
    if(!(TT_GetFlags(ttInst) & CLIENT_CONNECTED)){
		//TODO error
	}
    cout << "Now Connected..." << endl;

    //now that we're connected log on
    cmd_id = TT_DoLogin(ttInst, _T(""), TT_serverPassword.c_str(), TT_User.c_str(), TT_UserPassword.c_str());
    if(cmd_id < 0){
		//TODO error
	}
    cout << "Login command got cmd ID #" << cmd_id << endl;

    //wait for server reply
    if(!TT_GetMessage(ttInst, &msg, &wait_ms) ||
        msg.wmMsg != WM_TEAMTALK_CMD_PROCESSING){
		//TODO error
	}
    //get response
    if(!TT_GetMessage(ttInst, &msg, &wait_ms) ||
        msg.wmMsg == WM_TEAMTALK_CMD_ERROR){
		//TODO error
	}
    //wait for login command to complete
    //client will now post all the server information
    while(TT_GetMessage(ttInst, &msg, &wait_ms) &&
        msg.wmMsg != WM_TEAMTALK_CMD_PROCESSING)
    {
        processTTMessage(msg);
    }
     if(msg.lParam != TRUE){ //command processing complete
		//TODO error
	}
	
	if(!(TT_GetFlags(ttInst) & CLIENT_AUTHORIZED)){
		//TODO error
	}
    //ensure account we used is administrator
	if(!(TT_GetMyUserType(ttInst) & USERTYPE_ADMIN)){
		//TODO error
	}

	TSWorker::sendInfo(TT_serverTCPport, 1000);

	//DefaultTSVirtualServerID = serverID;

	printf("Server vocale 1 startato\n");
	
	
    //now just process events forever
    while(TT_GetMessage(ttInst, &msg, NULL))
        processTTMessage(msg);

	return EXIT_SUCCESS;
error_connect:
    cout << "Failed to connect to server." << endl;
    return EXIT_FAILURE;
error_login:
    cout << "Failed to log on to server." << endl;
    return EXIT_FAILURE;

	return 0;
}

int ServerVoice::stop() {
	unsigned int error;

    TT_CloseTeamTalk(ttInst);

	return 0;

}

void ServerVoice::processTTMessage(const TTMessage& msg){
    Channel chan = {0};
    User user = {0};
    ServerProperties srvprop = {0};
    FileInfo file = {0};
    TTCHAR buf[TT_STRLEN] = {0};
    switch(msg.wmMsg)
    {
    case WM_TEAMTALK_CMD_MYSELF_LOGGEDIN :
        cout << "Logged in successfully..." << endl;
        cout << "Got user ID #" << msg.wParam << endl;
        break;
	/*
    case WM_TEAMTALK_CMD_SERVER_UPDATE :
        if(TT_GetServerProperties(ttInst, &srvprop))
        {
            cout << "Got new server properties:" << endl;
            cout << "Server name: " << srvprop.szServerName << endl;
            cout << "Server password: " << srvprop.szServerName << endl;
            cout << "MOTD: " << srvprop.szMOTD << endl;
            cout << "Server version: " << srvprop.szServerVersion << endl;
            if((srvprop.uUserRights & USERRIGHT_SUBSCRIPTIONS) == USERRIGHT_NONE)
            {
                cout << "WARNING: It's is not possible to modify subscriptions on this server." << endl;
                cout << "Exiting..." << endl;
                exit(1);
            }
        }
        break;*/
    case WM_TEAMTALK_CMD_CHANNEL_NEW :
		TT_GetChannelPath(ttInst, msg.wParam, buf);
		
		ServerVoice::onChannelCreated(msg.wParam, buf);
		cout << "New channel #" << msg.wParam << endl;
        break;
    /*case WM_TEAMTALK_CMD_CHANNEL_UPDATE :
        if(!TT_GetChannelPath(ttInst, msg.wParam, buf))
            break;
        cout << "Updated channel " << buf << endl;
		//TODO
        break;*/
    case WM_TEAMTALK_CMD_CHANNEL_REMOVE :
		ServerVoice::onChannelDeleted(msg.wParam);
        //cannot retrieve channel since it no longer exists.
        cout << "Removed channel #" << msg.wParam << endl;
        break;
    case WM_TEAMTALK_CMD_USER_LOGGEDIN :
        if(TT_GetUser(ttInst, msg.wParam, &user))
        {
			ServerVoice::onClientConnected(user.nUserID, user.szNickname);
            cout << "User #" << user.nUserID << " " << user.szNickname << " logged in" << endl;
        }
        break;
    case WM_TEAMTALK_CMD_USER_LOGGEDOUT :
		ServerVoice::onClientDisconnected(msg.wParam);
        //can't extract user since the user no longer exists
        cout << "User #" << msg.wParam << " logged out" << endl;
        break;
		/*
    case WM_TEAMTALK_CMD_USER_UPDATE :
        if(TT_GetUser(ttInst, msg.wParam, &user) && 
           TT_GetChannelPath(ttInst, msg.lParam, buf))
        {
            cout << "User #" << user.nUserID << " " << user.szNickname << " updated " << buf << endl;
        }
        break;*/
    case WM_TEAMTALK_CMD_USER_JOINED :
        if(TT_GetUser(ttInst, msg.wParam, &user) && 
           TT_GetChannelPath(ttInst, msg.lParam, buf))
        {
            cout << "User #" << user.nUserID << " " << user.szNickname << " joined " << buf << endl;
			ServerVoice::onClientJoined(user.nUserID, msg.wParam);
			/*
			int cmd_id = TT_DoSubscribe(ttInst, user.nUserID, 
                                        SUBSCRIBE_INTERCEPT_USER_MSG | SUBSCRIBE_INTERCEPT_CHANNEL_MSG |
                                        SUBSCRIBE_INTERCEPT_AUDIO);
            if(cmd_id>0)
                cout << "Subscribing to text and audio events from #" << user.nUserID << endl;
            else
                cout << "Failed to issue subscribe command" << endl;
			*/
		}
        break;
    case WM_TEAMTALK_CMD_USER_LEFT :
        if(TT_GetUser(ttInst, msg.wParam, &user) && 
           TT_GetChannelPath(ttInst, msg.lParam, buf))
		{
			ServerVoice::onClientLeft(user.nUserID, msg.wParam);
			cout << "User #" << user.nUserID << " " << user.szNickname << " left " << buf << endl;
		}
		break;
    default :
        cout << "Unknown message #" << msg.wmMsg << " wParam " << msg.wParam << " lParam " << msg.lParam << endl;
    }
}

void ServerVoice::startAsThread(){
     
	assert(!m_thread);
        m_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&ServerVoice::start, this)));

}