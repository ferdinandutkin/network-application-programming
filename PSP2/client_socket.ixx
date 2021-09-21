module;

#include <memory>
#include <WinSock2.h>
#include <iostream>
#include <variant>

export module client_socket;



import socket_base;
import wsa_exception;



export template <ip_protocol protocol>
class client_socket : public socket_base<protocol> {



public:
	using socket_base<protocol>::socket_base;
	

	client_socket(SOCKET wrapped) : socket_base<protocol>{ wrapped } {
	}

	client_socket() : socket_base<protocol>{} {
	}


	void send(std::string message) const {
		if (::send(this->_wrapped, message.c_str(), message.size(), 0) == SOCKET_ERROR) {
			throw wsa_exception(__func__);
		}
	}



	
	friend const client_socket& operator << (const client_socket& socket, const std::string& message) {	
		socket.send(message);
		return socket;
	}


	void connect(socket_address address) const {
		if (::connect(this->_wrapped, reinterpret_cast<sockaddr*>(&address), sizeof address) == SOCKET_ERROR) {
			throw wsa_exception(__func__);
		}
	}

	void connect(ip_address address, unsigned short port) const {
		connect({ address, port });
	}


	template<size_t length> //можно сделать и для других типов но там паддинг систмнозависимый
	std::string recieve() const {
		char response[length + 1]{};
		if (::recv(this->_wrapped, response, std::size(response), 0) == SOCKET_ERROR) {
			throw wsa_exception(__func__);
		}
		return { response };
	}

};