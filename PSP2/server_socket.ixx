module;

#include <memory>
#include <WinSock2.h>
#include <iostream>
#include <variant>

export module server_socket;

import socket_base;
import client_socket;
import wsa_exception;


export enum connection_number {
	max = SOMAXCONN
};


export template <ip_protocol protocol>
class server_socket : public socket_base<protocol> {

public:
	using socket_base<protocol>::socket_base;

	void listen(std::variant<connection_number, int> backlog = connection_number::max) const {
		int backlog_value;
		if (std::holds_alternative<int>(backlog)) {
			backlog_value = SOMAXCONN_HINT(std::get<int>(backlog));
		}
		else {
			backlog_value = std::to_underlying(std::get<connection_number>(backlog));
		}
		if (::listen(this->_wrapped, backlog_value) == SOCKET_ERROR) {
			throw wsa_exception("listen");

		}
	}


	client_socket<protocol> accept() const {

	
		if (client_socket<protocol> client = { ::accept(*this, nullptr, nullptr) }; client != INVALID_SOCKET) {
			return client;
		}
		else throw wsa_exception("accept");
	}

};