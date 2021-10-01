module;

import client_socket;
import socket_base;
import socket_base_async;

#include <WinSock2.h>
#include <string>
#include <functional>
#include <thread>

export module client_socket_async;

 



export template <ip_protocol protocol>
class client_socket_async :  public client_socket<protocol>, public socket_base_async<protocol> {


public:

	using socket_base<protocol>::get_address;
	using socket_base<protocol>::bind;
	using socket_base<protocol>::set_options;

	using client_socket<protocol>::client_socket;
	using client_socket<protocol>::connect;
	using client_socket<protocol>::receive;
	using client_socket<protocol>::send;
 

	using socket_base_async<protocol>::send;
	using socket_base_async<protocol>::receive;

	 
	client_socket_async(SOCKET wrapped) : client_socket<protocol>{ wrapped }, socket_base_async<protocol>{ wrapped } {
		this->socket_base_async<protocol>::_wrapped = this->client_socket<protocol>::_wrapped;
	}

 
	client_socket_async(client_socket<protocol> base) : client_socket_async<protocol>{ base.operator SOCKET() } {

	}

	client_socket_async() : client_socket<protocol>{} {
		this->socket_base_async<protocol>::_wrapped = this->client_socket<protocol>::_wrapped;
	}

 
	client_socket_async(socket_type type) : client_socket<protocol>{ type } {
		this->socket_base_async<protocol>::_wrapped = this->client_socket<protocol>::_wrapped;
	}


	std::thread send(std::string message, std::function<void()> on_completed) const {

		return std::thread{ [=] {
			this->client_socket::send(message);
			on_completed();

			} };
	}




	std::thread connect(socket_address address, std::function<void()> on_completed) const {

		return std::thread{ [=] {
			this->client_socket<protocol>::connect(address);
			on_completed();

			} };
		
	}

	std::thread connect(ip_address address, unsigned short port, std::function<void()> on_completed) const {
		return connect({ address, port }, on_completed);
	}


	template<size_t length> 
	std::thread receive(std::function<void(std::string)> on_completed) const {
		return std::thread{ [=] {
		   auto res = this->client_socket<protocol>::receive<length>();
		   on_completed(res);

		   } };
	}





};