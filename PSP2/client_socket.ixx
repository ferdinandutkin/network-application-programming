#include <memory>
#include <WinSock2.h>
#include <iostream>
#include <variant>

export module client_socket;

import socket_base;
import wsa_exception;
 

 
export class client_socket :  public socket_base {
protected:

	

public:
	using socket_base::socket_base;

	client_socket(SOCKET wrapped) : socket_base{ wrapped } {
	}


	void send(std::string message) {
		if (::send(_wrapped, message.c_str(), message.size(), 0) == SOCKET_ERROR) {
			throw wsa_exception("send");
		}
	}
	 

	void connect(socket_address address) const {
		if (::connect(_wrapped, reinterpret_cast<sockaddr*>(&address), sizeof address) == SOCKET_ERROR) {
			throw wsa_exception("listen");
		}
	}

};