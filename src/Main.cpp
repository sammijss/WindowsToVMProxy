/*
 * File: Main.cpp 
 */
#include<thread>
#include<fstream>
#include<iostream>
#include<stdlib.h>
#include<UDPProxy.h>

static std::string ParseLine(std::string line) {
	if (line.find("=") != std::string::npos) {
		return (line.substr(line.find("=") + 1));
	}
}

int main() {
	unsigned short rxport;
	unsigned short txport;
	std::string    ipaddr;
	unsigned short msgsize;

	std::string line;
	std::ifstream conffile;
	conffile.open("config.cfg");

	if (conffile.is_open()) { /* ok, proceed with output */
		while(std::getline(conffile, line)) {
			if (line.find("RXPORT") != std::string::npos) {
				rxport = atoi(ParseLine(line).c_str());

			} else if (line.find("TXPORT") != std::string::npos) {
				txport = atoi(ParseLine(line).c_str());

			} else if (line.find("IPADDRESS") != std::string::npos) {
				ipaddr = ParseLine(line);

			} else if (line.find("MSGSIZE") != std::string::npos) {
				msgsize = atoi(ParseLine(line).c_str());
			}
		}

		std::cout <<"RXPORT:'" <<rxport <<"', TXPORT:'" <<txport <<"', IPADDRESS:'" <<ipaddr <<"'" <<std::endl;

		UDPProxy proxyobject(rxport, txport, ipaddr.c_str(), msgsize);
		proxyobject.CreateRx();
		proxyobject.CreateTx();
		std::thread proxyobjectthread(&UDPProxy::StartProxyProcess, proxyobject);
		proxyobjectthread.join();
		conffile.close();
	}

	std::cerr <<"Failed to open 'config.cfg' file." <<std::endl;
	return(0);
}
