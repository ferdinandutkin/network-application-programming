module;

#include <optional>
#include <string>
export module server;

import socket_base;
import host_base;
import server_socket_async;
import client_socket_async;
import ip;




template<ip_protocol protocol>
class server_base : public host_base<protocol, server_socket_async> {

};

 

template<> 
class server_base<ip_protocol::tcp> : public host_base<ip_protocol::tcp, server_socket_async> {
protected:
	std::optional<client_socket_async<ip_protocol::tcp>> _client;
};

export template<ip_protocol protocol>
class server  : public server_base<protocol> {

	
public:

	using host_base<protocol, server_socket_async>::recieve;
	using host_base<protocol, server_socket_async>::send;

	server(unsigned short port){ 

		auto type = protocol == ip_protocol::tcp ? socket_type::stream : socket_type::datagram;

		this->_socket = { type };
		this->_socket.bind(ip_address::loopback(), port);
	}

	void start() const requires (protocol == ip_protocol::tcp) {
		this->_socket.listen(connection_number::max);

	}

	void accept() requires (protocol == ip_protocol::tcp) {
		this->_client = this->_socket.accept();

	}

	socket_address get_client_address() const requires (protocol == ip_protocol::tcp)  {

		if (this->_client) {
			return this->_client.value().get_address();
		}
		else throw std::exception("?????");
	
	}


	template<size_t length> //можно сделать и для других типов но там паддинг систмнозависимый
	std::thread recieve(std::function<void(std::string)> on_completed) const requires (protocol == ip_protocol::tcp)  {
		if (this->_client) {
			return this->_client.value().recieve<length>(on_completed);
		}
		else throw std::exception("?????");
	
	}


	template<size_t length> //можно сделать и для других типов но там паддинг систмнозависимый
	std::string recieve() const requires (protocol == ip_protocol::tcp)  {
		if (this->_client) {
			return this->_client.value().recieve<length>();
		}
		else throw std::exception("?????");
	}
 
	std::thread send(std::string message, std::function<void()> on_completed)  const requires (protocol == ip_protocol::tcp) {
		if (this->_client) {
			return this->_client.value().send(message, on_completed);
		}
		else throw std::exception("?????");
	}

	friend const server& operator << (const server& server, const std::string& message) requires (protocol == ip_protocol::tcp) {
	    server.send(message);
		return server;
	}

	void send(std::string message) const requires (protocol == ip_protocol::tcp)  {
		if (this->_client) {
			this->_client.value().send(message);
		}
		else throw std::exception("?????");
	}
};


export using tcp_server = server<ip_protocol::tcp>;
export using udp_server = server<ip_protocol::udp>;