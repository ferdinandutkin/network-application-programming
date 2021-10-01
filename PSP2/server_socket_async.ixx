module;

import server_socket;
import socket_base_async;
import client_socket_async;
import socket_base;
import socket_base_async;


#include <string>
#include <functional>
#include <WinSock2.h>
#include <thread>

export module server_socket_async;



export template<ip_protocol protocol>
class server_socket_async : public server_socket<protocol>, public socket_base_async<protocol> {


public:
	using socket_base<protocol>::get_address;
	using socket_base<protocol>::bind;
	using socket_base<protocol>::set_options;

	using server_socket<protocol>::server_socket;
	using server_socket<protocol>::accept;


	using socket_base_async<protocol>::send;
	using socket_base_async<protocol>::receive;





	server_socket_async(SOCKET wrapped) : server_socket<protocol>{ wrapped } {
		this->socket_base_async<protocol>::_wrapped = this->server_socket<protocol>::_wrapped;

	}

	server_socket_async(server_socket<protocol> base) : server_socket_async<protocol>{base.operator SOCKET()} {

	}

	server_socket_async() : server_socket<protocol>{} {
		this->socket_base_async<protocol>::_wrapped = this->server_socket<protocol>::_wrapped;
	}

	server_socket_async(socket_type type) : server_socket<protocol>{type} {
		this->socket_base_async<protocol>::_wrapped = this->server_socket<protocol>::_wrapped;

	}

	std::thread accept(std::function<void(client_socket_async<protocol>)> on_completed) const requires (protocol == ip_protocol::tcp) {

		return std::thread{ [=] {

			auto res = server_socket::accept().operator SOCKET();
			auto socket = client_socket_async{res};


			on_completed(socket);

		}
		};

	}
};