module;

#include <optional>
#include <string>
export module server;


import server_socket_async;
import client_socket_async;
import ip;

export class server {
private:
	server_socket_async _socket;
	std::optional<client_socket_async> _client;
public:

	server(unsigned short port) : _socket{ socket_type::stream, ip_protocol::tcp }, _client{} { 
		_socket.bind(ip_address::loopback(), port);
	}

	void start() const {
		_socket.listen(connection_number::max);

	}

	void accept() {
		_client =  _socket.accept();

	}

	socket_address get_client_address() const {

		if (_client) {
			return _client.value().get_address();
		}
		else throw std::exception("?????");
	
	}


	template<size_t length> //можно сделать и для других типов но там паддинг систмнозависимый
	std::thread recieve(std::function<void(std::string)> on_completed) const {
		if (_client) {
			return _client.value().recieve<length>(on_completed);
		}
		else throw std::exception("?????");
	
	}


	template<size_t length> //можно сделать и для других типов но там паддинг систмнозависимый
	std::string recieve() const {
		if (_client) {
			return _client.value().recieve<length>();
		}
		else throw std::exception("?????");
	}
 
	std::thread send(std::string message, std::function<void()> on_completed) const {
		if (_client) {
			return _client.value().send(message, on_completed);
		}
		else throw std::exception("?????");
	}

	friend const server& operator << (const server& server, const std::string& message) {
		server.send(message);
		return server;
	}

	void send(std::string message) const {
		if (_client) {
			_client.value().send(message);
		}
		else throw std::exception("?????");
	}
};