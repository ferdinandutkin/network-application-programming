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

	client_socket() : socket_base{} {
	}


	void send(std::string message) const {
		if (::send(_wrapped, message.c_str(), message.size(), 0) == SOCKET_ERROR) {
			throw wsa_exception(__func__);
		}
	}
	 

	void connect(socket_address address) const {
		if (::connect(_wrapped, reinterpret_cast<sockaddr*>(&address), sizeof address) == SOCKET_ERROR) {
			throw wsa_exception(__func__);
		} 
	}

	template<size_t length> //можно сделать и для других типов но там паддинг систмнозависимый
	std::string recieve() const {
		char response[length]{};
		if (::recv(_wrapped, response, std::size(response), 0) == SOCKET_ERROR) {
			throw wsa_exception(__func__);
		}
		return { response };
	}

};