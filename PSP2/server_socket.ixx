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
export class server_socket : public socket_base {

public:
	using socket_base::socket_base;

	void listen(std::variant<connection_number, int> backlog = connection_number::max) const {
		int backlog_value;
		if (std::holds_alternative<int>(backlog)) {
			backlog_value = SOMAXCONN_HINT(std::get<int>(backlog));
		}
		else {
			backlog_value = std::to_underlying(std::get<connection_number>(backlog));
		}
		if (::listen(_wrapped, backlog_value) == SOCKET_ERROR) {
			throw wsa_exception("listen");

		}
	}


	client_socket accept() const {
		if (client_socket client = { ::accept(*this, nullptr, nullptr) }; client != INVALID_SOCKET) {
			return client;
		}
		else throw wsa_exception("accept");
	}

};