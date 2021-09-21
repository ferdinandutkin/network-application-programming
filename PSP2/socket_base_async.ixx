module;

import socket_base;

#include <string>
#include <functional>
#include <thread>


export module socket_base_async;



export template <ip_protocol protocol>
class socket_base_async : public socket_base<protocol> {

};

export template <>
class socket_base_async<ip_protocol::udp> : socket_base<ip_protocol::udp> {

public:

	template<size_t length>
	std::thread recieve(socket_address address, std::function<void(std::string)> on_completed) const {
		return std::thread{ [=] {
		   auto res = this->socket_base<ip_protocol::udp>::recieve<length>(address);
		   on_completed(res);

		   } };
	}


	template<size_t length>
	std::thread recieve(ip_address to_address, unsigned short to_port, std::function<void(std::string)> on_completed) const {
		return recieve(socket_address{ to_address, to_port }, on_completed);
	}



	std::thread send(std::string message, socket_address to, std::function<void()> on_completed) const {

		return std::thread{ [=] {
			this->socket_base::send(message, to);
			on_completed();

			} };
	}

	std::thread send(std::string message, ip_address to_address, unsigned short to_port, std::function<void()> on_completed) const {
		return send(message, socket_address{ to_address, to_port }, on_completed);
	}




};