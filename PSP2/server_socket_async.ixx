module;

import server_socket;
import client_socket_async;


#include <string>
#include <functional>
#include <WinSock2.h>
#include <thread>

export module server_socket_async;



export class server_socket_async :  public server_socket {


public:
	using server_socket::server_socket;
	using server_socket::accept;


	 

	server_socket_async(SOCKET wrapped) : server_socket{ wrapped } {
	}

	server_socket_async(server_socket base) : server_socket{ base.operator SOCKET() } {

	}

	server_socket_async() : server_socket{} {
	}

	std::thread accept(std::function<void(client_socket_async)> on_completed) const {

		return std::thread{ [=] {

			auto res = server_socket::accept().operator SOCKET();
			auto socket = client_socket_async{res};

			
			on_completed(socket);

		}
		};
		
	}
};