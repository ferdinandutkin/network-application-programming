module;

#include <optional>
#include <string>
export module client;


import server_socket;
import client_socket;
import ip;
export class client {
private:

	client_socket _socket;

public:


	client(unsigned short port) : _socket{ socket_type::stream, ip_protocol::tcp }  {
		_socket.bind(ip_address::loopback(), port);
	}


	void connect(unsigned short port, ip_address address = ip_address::loopback()) const {
		_socket.connect({ address, port });
	}

	void connect(socket_address server_address) const {	 
		_socket.connect(server_address);
	}


	friend const client& operator << (const client& client, const std::string& message) {
		client.send(message);
		return client;
	}

	void send(std::string message) const {
		if (_socket) {
			_socket.send(message);
		}
		else throw std::exception("?????");
	}


	template <size_t length>
	std::string recieve() const {
		if (_socket) {
			return _socket.recieve<length>();
		}
		else throw std::exception("?????");
	}
};