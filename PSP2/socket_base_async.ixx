module;

import socket_base;

#include <string>
#include <functional>
#include <thread>


export module socket_base_async;



export template <ip_protocol protocol>
class socket_base_async : public socket_base<protocol> {
	public:

		using socket_base<protocol>::receive;
		using socket_base<protocol>::send;
		using socket_base<protocol>::socket_base;


	template<size_t length>
	std::thread receive(socket_address& from, std::function<void(std::string)> on_completed) const requires (protocol == ip_protocol::udp) {
		return std::thread{ [=, &from] {
		   std::string res = this->socket_base<protocol>::receive<length>(from);
		   on_completed(res);

		   } };
	}


	 



	std::thread send(std::string message, socket_address& to, std::function<void()> on_completed) const requires(protocol == ip_protocol::udp) {

		return std::thread{ [=] {
			this->socket_base<protocol>::send(message, to);
			on_completed();

			} };
	}

	 

};
