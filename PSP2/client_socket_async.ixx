module;

import client_socket;


#include <string>
#include <functional>
#include <WinSock2.h>
#include <thread>

export module client_socket_async;

export class client_socket_async :  public client_socket {


public:
	using client_socket::client_socket;
	using client_socket::send;
	using client_socket::connect;
	using client_socket::recieve;

	 
	client_socket_async(SOCKET wrapped) : client_socket{ wrapped } {
	}

 
	client_socket_async(client_socket base) : client_socket{ base.operator SOCKET() } {

	}

	client_socket_async() : client_socket{} {
	}




	std::thread send(std::string message, std::function<void()> on_completed) const {

		return std::thread{ [=] {
			this->client_socket::send(message);
			on_completed();

			} };
	}
 

	std::thread connect(socket_address address, std::function<void()> on_completed) const {

		return std::thread{ [=] {
			this->client_socket::connect(address);
			on_completed();

			} };
		
	}

	std::thread connect(ip_address address, unsigned short port, std::function<void()> on_completed) const {
		return connect({ address, port }, on_completed);
	}


	template<size_t length> 
	std::thread recieve(std::function<void(std::string)> on_completed) const {
		return std::thread{ [=] {
		   auto res = this->client_socket::recieve<length>();
		   on_completed(res);

		   } };
	}


};