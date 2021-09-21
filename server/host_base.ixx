module;
#include <string>
#include <functional>
#include <thread>
import socket_base;

export module host_base;

export template <ip_protocol protocol, template<ip_protocol> typename socket>
class socket_container {
protected:
	socket<protocol> _socket;
};


export template <ip_protocol protocol, template<ip_protocol> typename socket>
class host_base : public socket_container<protocol, socket> {

};


export template <template<ip_protocol> typename socket>
class host_base<ip_protocol::tcp, socket> : public socket_container<ip_protocol::tcp, socket>{

};

export template <template<ip_protocol> typename socket>
class host_base<ip_protocol::udp, socket> : public socket_container<ip_protocol::udp, socket> {

	template<size_t length>
	std::string recieve(socket_address from) const  {
		return this->_socket.recieve(from);
	}


	template<size_t length>
	std::string recieve(ip_address from_address, unsigned short from_port) const  {
		return recieve(socket_address{ from_address, from_port });
	}


	void send(std::string message, socket_address from) const {
		this->_socket.send(message, from);
	}



	void send(std::string message, ip_address to_address, unsigned short to_port) const   {
		send(message, socket_address{ to_address, to_port });
	}




	template<size_t length>
	std::thread recieve(socket_address address, std::function<void(std::string)> on_completed) const {
		return this->_socket.recieve(address, on_completed);
	}


	template<size_t length>
	std::thread recieve(ip_address to_address, unsigned short to_port, std::function<void(std::string)> on_completed) const {
		return recieve(socket_address{ to_address, to_port }, on_completed);
	}



	std::thread send(std::string message, socket_address to, std::function<void()> on_completed) const {
		return this->_socket.send(message, to, on_completed);
	}

	std::thread send(std::string message, ip_address to_address, unsigned short to_port, std::function<void()> on_completed) const {
		return send(message, socket_address{ to_address, to_port }, on_completed);
	}



};