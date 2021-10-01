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

	using host_base<protocol, client_socket_async>::receive;
	using host_base<protocol, client_socket_async>::send;

	client(port_t port) : client{} {
		this->_socket.bind(ip_address::loopback(), port);
	}

	client() {
		auto type = protocol == ip_protocol::tcp ? socket_type::stream : socket_type::datagram;
		this->_socket = client_socket_async<protocol>{ type };
	}

	void connect(port_t port, ip_address address = ip_address::loopback()) const  requires (protocol == ip_protocol::tcp) {
		this->_socket.connect({ address, port });
	}

	void connect(socket_address server_address) const  requires (protocol == ip_protocol::tcp) {
		this->_socket.connect(server_address);
	}


	std::thread connect(socket_address server_address, std::function<void()> on_completed) const  requires (protocol == ip_protocol::tcp) {
		return this->_socket.connect(server_address, on_completed);
	}


	std::thread  connect(unsigned short port, ip_address address, std::function<void()> on_completed) const  requires (protocol == ip_protocol::tcp) {
		return this->_socket.connect({ address, port }, on_completed);
	}




	friend const client& operator << (const client& client, const std::string& message)  requires (protocol == ip_protocol::tcp) {
		client.send(message);
		return client;
	}

	void send(std::string message) const  requires (protocol == ip_protocol::tcp) {
		
		this->_socket.send(message);
		
	}


	std::thread send(std::string message, std::function<void()> on_completed) const requires (protocol == ip_protocol::tcp) {
		
		 return this->_socket.send(message, on_completed);

	}



	template <size_t length>
	std::string receive() const requires (protocol == ip_protocol::tcp) {
		 
		return this->_socket.receive<length>();
		 
	}



	template<size_t length> 
	std::thread receive(std::function<void(std::string)> on_completed) const requires (protocol == ip_protocol::tcp) {
		 
		return this->_socket.receive<length>(on_completed);
	 
	}

};

export using tcp_client = client<ip_protocol::tcp>;
export using udp_client = client<ip_protocol::udp>;
