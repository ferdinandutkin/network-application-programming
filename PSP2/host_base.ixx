module;
#include <string>
#include <functional>
#include <thread>
import socket_base;
import socket_options;

export module host_base;

export template <ip_protocol protocol, template<ip_protocol> typename socket>
class socket_container {
protected:
	socket<protocol> _socket;
public:
	void set_options(socket_options options) const {
		this->_socket.set_options(options);
	}

};


export template <ip_protocol protocol, template<ip_protocol> typename socket>
class host_base : public socket_container<protocol, socket> {


};


export template <template<ip_protocol> typename socket>
class host_base<ip_protocol::tcp, socket> : public socket_container<ip_protocol::tcp, socket>{\

public:
	void receive() = delete;
	void send() = delete;

	 
	 
};

export template <template<ip_protocol> typename socket>
class host_base<ip_protocol::udp, socket> : public socket_container<ip_protocol::udp, socket> {


public:
	template<size_t length>
	std::string receive(socket_address& from) const {
		return this->_socket.receive<length>(from);
	}


	template<size_t length>
	std::thread receive(socket_address& from, std::function<void(std::string)> on_completed) const {
		return this->_socket.receive<length>(from, on_completed);
	}

	
	void send(std::string message, socket_address to) const {
		this->_socket.send(message, to);
	}

	void send(std::string message, ip_address ip_to, unsigned short port_to) {
		this->_socket.send(message, socket_address{ ip_to, port_to });
	}

	std::thread send(std::string message, socket_address to, std::function<void()> on_completed) const {
		return this->_socket.send(message, to, on_completed);
	}

	std::thread send(std::string message, ip_address ip_to, unsigned short port_to) const {
		return this->_socket.send(message, socket_address{ ip_to, port_to });
	}






};