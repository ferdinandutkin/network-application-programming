#include <optional>
#include <string>
export module server;


import server_socket;
import client_socket;
import ip;

export class server {
private:
	server_socket _socket;
	std::optional<client_socket> _client;
public:

	server(unsigned short port) : _socket{ socket_type::stream, ip_protocol::tcp }, _client{} { 
		_socket.bind(ip_address::loopback(), port);
	}

	void start() {
		_socket.listen(connection_number::max);

	}

	void accept() {
		_client =  _socket.accept();

	}

	void send(std::string message) {
		if (_client) {
			_client.value().send(message);
		}
		else throw std::exception("?????");
	}
};