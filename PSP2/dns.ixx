module;

import socket_base;
import wsa_provider;
import wsa_wrapper;
import ip;
import endian;
import wsa_exception;
#include <string>
#include <WS2tcpip.h>
#include <memory>

export module dns;


export class dns {

private:
	static std::shared_ptr<wsa_wrapper> _wsa;
public:
	static ip_address get_host_by_name(std::string name) {
		const addrinfo hints{};
		addrinfo* result;

		if (::getaddrinfo(name.c_str(), nullptr, &hints, &result) == SOCKET_ERROR) {
			throw wsa_exception(__func__);

		}
		
		while (result and result->ai_family != AF_INET) {
			result = result->ai_next;
		}
		if (result->ai_family != AF_INET) {
			throw std::exception("oof");
		}
		
		auto addr = *reinterpret_cast<sockaddr_in*>(result->ai_addr);

		::freeaddrinfo(result);

		return { addr.sin_addr.S_un.S_addr };


	}
};

std::shared_ptr<wsa_wrapper> dns::_wsa {wsa_provider::get_instance()};