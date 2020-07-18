/*
 * File: UDPProxy.cpp 
 */

#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<iostream>
#include<sys/socket.h>
#include<Logger.h>
#include<UDPProxy.h>

UDPProxy::UDPProxy(unsigned short rxport, unsigned short txport, const char* ipaddr, unsigned short msgsize) {
	mRxPort  = rxport;
	mTxPort  = txport;
	mIpAddr  = ipaddr;
	mMsgSize = msgsize;
}

void UDPProxy::CreateRx() {
	struct sockaddr_in serveraddress; 

	// Creating socket file descriptor 
	if ( (mRxSockFD = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) { 
		std::cout <<"Socket creation failed" <<std::endl; 
		exit(EXIT_FAILURE); 
	} 

	// Set address to zero
	memset(&serveraddress, 0, sizeof(serveraddress)); 

	// Filling server information 
	serveraddress.sin_family = AF_INET; // IPv4 
	serveraddress.sin_port = htons(mRxPort);
	if (inet_pton(AF_INET, (const char *)mIpAddr.c_str(), (void *)&serveraddress.sin_addr.s_addr) != 1) {
		std::cout <<"Address assign failed, error: " <<strerror(errno) <<std::endl; 
		exit(EXIT_FAILURE); 
	}

	// Bind the socket with the server address 
	if ( bind(mRxSockFD, (const struct sockaddr *)&serveraddress, sizeof(serveraddress)) == -1 ) {
		std::cout <<"Bind failed, port(" <<mRxPort <<") error: " <<strerror(errno) <<std::endl;
		exit(EXIT_FAILURE); 
	}
}

void UDPProxy::CreateTx() {
	// Creating socket file descriptor 
	if ( (mTxSockFD = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) { 
		std::cout <<"Socket creation failed" <<std::endl; 
		exit(EXIT_FAILURE); 
	} 

	memset(&mTxServerAddress, 0, sizeof(mTxServerAddress)); 

	// Filling server information 
	mTxServerAddress.sin_family = AF_INET; // IPv4 
	mTxServerAddress.sin_port = htons(mTxPort); 
	if (inet_pton(AF_INET, (const char *)mIpAddr.c_str(), (void *)&mTxServerAddress.sin_addr.s_addr) != 1) {
		std::cout <<"Address assign failed, error: " <<strerror(errno) <<std::endl; 
		exit(EXIT_FAILURE); 
	}
}

void UDPProxy::StartProxyProcess() {
	int nrecvdbytes = 0;
	int nsentbytes = 0;
	unsigned char buff[mMsgSize];

	char logmsg[LOGMSGSIZE];

	struct sockaddr_in clientaddr; 
	int clientaddrlen = sizeof(clientaddr);

	while(true) {
		memset(&buff, 0, sizeof(buff)); 
		nrecvdbytes = recvfrom(mRxSockFD, (void *)buff, sizeof(buff), 0, (struct sockaddr *)&clientaddr, &clientaddrlen);
		if (nrecvdbytes < 0) {
			std::cout <<"Failed to recv message, error: " <<strerror(errno) <<std::endl;
			exit(EXIT_FAILURE); 
		}

		sprintf(logmsg, "Msg Received: %s of bytes(%d)", reinterpret_cast<char*>(buff), nrecvdbytes);
		Logger::LogMsg(logmsg);

		nsentbytes = sendto(mTxSockFD, (const void *)buff, (size_t)nrecvdbytes, 0x800, (const struct sockaddr *)&mTxServerAddress, (socklen_t)sizeof(mTxServerAddress));
		if (nsentbytes != nrecvdbytes) {
			std::cout <<"Failed to send message, error: " <<strerror(errno) <<std::endl;
			exit(EXIT_FAILURE); 
		}
	}
}
