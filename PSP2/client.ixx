#include <optional>
#include <string>
export module client;


import server_socket;
import client_socket;
import ip;
export class client {


	client_socket _socket;


	client(unsigned short port) : _socket{ socket_type::stream, ip_protocol::tcp } {
		_socket.bind(ip_address::loopback(), port);
	}


	void connect(socket_address server_address) {	 
		_socket.connect(server_address);
	}

	void send(std::string message) {
		if (_socket) {
			_socket.send(message);
		}
		else throw std::exception("?????");
	}


	template <size_t length>
	std::string recieve() {
		if (_socket) {
			return _socket.recieve<length>();
		}
		else throw std::exception("?????");
	}
};