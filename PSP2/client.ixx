module;

#include <optional>
#include <string>
#include <thread>
#include <functional>

export module client;


import server_socket;
import client_socket_async;
import ip;
export class client {
private:

	client_socket_async _socket;

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


	std::thread connect(socket_address server_address, std::function<void()> on_completed) const {
		return _socket.connect(server_address, on_completed);
	}


	std::thread  connect(unsigned short port, ip_address address, std::function<void()> on_completed) const {
		return _socket.connect({ address, port }, on_completed);
	}




	friend const client& operator << (const client& client, const std::string& message) {
		client.send(message);
		return client;
	}

	void send(std::string message) const {
		
		_socket.send(message);
		
	}


	std::thread send(std::string message, std::function<void()> on_completed) const {
		
		 return _socket.send(message, on_completed);

	}



	template <size_t length>
	std::string recieve() const {
		 
		return _socket.recieve<length>();
		 
	}



	template<size_t length> 
	std::thread recieve(std::function<void(std::string)> on_completed) const {
		 
		return _socket.recieve<length>(on_completed);
	 
	}

};