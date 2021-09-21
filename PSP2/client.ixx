module;

#include <optional>
#include <string>
#include <thread>
#include <functional>

export module client;


import server_socket;
import client_socket_async;
import socket_base;
import host_base;
import ip;

export template<ip_protocol protocol>
class client : public host_base<protocol, client_socket_async> {
 

public:


	client(unsigned short port)   {
		this->_socket = { socket_type::stream };
		this->_socket.bind(ip_address::loopback(), port);
	}


	void connect(unsigned short port, ip_address address = ip_address::loopback()) const {
		this->_socket.connect({ address, port });
	}

	void connect(socket_address server_address) const {	 
		this->_socket.connect(server_address);
	}


	std::thread connect(socket_address server_address, std::function<void()> on_completed) const {
		return this->_socket.connect(server_address, on_completed);
	}


	std::thread  connect(unsigned short port, ip_address address, std::function<void()> on_completed) const {
		return this->_socket.connect({ address, port }, on_completed);
	}




	friend const client& operator << (const client& client, const std::string& message) {
		client.send(message);
		return client;
	}

	void send(std::string message) const {
		
		this->_socket.send(message);
		
	}


	std::thread send(std::string message, std::function<void()> on_completed) const {
		
		 return this->_socket.send(message, on_completed);

	}



	template <size_t length>
	std::string recieve() const {
		 
		return this->_socket.recieve<length>();
		 
	}



	template<size_t length> 
	std::thread recieve(std::function<void(std::string)> on_completed) const {
		 
		return this->_socket.recieve<length>(on_completed);
	 
	}

};

export using tcp_client = client<ip_protocol::tcp>;
export using udp_client = client<ip_protocol::udp>;
