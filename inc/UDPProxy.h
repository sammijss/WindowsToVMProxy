/*
 * File: UDPProxy.h
 */

#ifndef UDPPROXY_H
#define UDPPROXY_H

#include<string.h>
#include<arpa/inet.h>

class UDPProxy {
	private:
		unsigned short mRxPort;
		unsigned short mTxPort;
		std::string    mIpAddr;	/* For Rx and Tx both IP Address will be same because running on same machine */

		unsigned short mMsgSize;
		int            mRxSockFD;
		int            mTxSockFD;

		struct sockaddr_in mTxServerAddress; 
	public:
		UDPProxy(unsigned short rxport, unsigned short txport, const char* ipaddr, unsigned short msgsize);

		void CreateRx();
		
		void CreateTx();

		void StartProxyProcess();
};

#endif
