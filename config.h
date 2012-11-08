/* 
 * File:   config.h
 * Author: root
 *
 * Created on 24 dicembre 2011, 21.30
 */

#ifndef CONFIG_H
#define	CONFIG_H
#include <string>

const int idConcurrent = 123;

const int VoiceServerId = 1;

const int DefaultTSVirtualServerID = 1;// in teoria dovrebbe essere solo un int x poi essere definito

const int maxClients = 32;


extern std::string chatServerDNS;
extern int chatServerPort;

extern std::string thisServerDNS;
extern int portTS;
extern int portThrift;

// TT ZONE

extern std::string TT_serverDNS;
extern int TT_serverTCPport;
extern int TT_serverUDPport;
extern std::string TT_serverPassword;
extern std::string TT_User;
extern std::string TT_UserPassword;

#endif	/* CONFIG_H */