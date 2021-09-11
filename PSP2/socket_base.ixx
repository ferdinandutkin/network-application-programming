#include <memory>
#include <WinSock2.h>
#include <iostream>
export module socket_base;
import wsa_wrapper;
import endian;
import wsa_provider;
import ip;



export class socket_address {

	sockaddr_in _wrapped;
public:
	socket_address(ip_address address, unsigned short port) {
		_wrapped = { PF_INET, to_big_endian(port), {.S_un = {.S_addr = address.as_big_endian().operator size_t()}} };
	}

	operator sockaddr_in() const {
		return _wrapped;
	}


};

export enum class socket_type {
	stream = SOCK_STREAM, datagram = SOCK_DGRAM, row = SOCK_RAW, reliable_message_datagram = SOCK_RDM, sequenced_packets = SOCK_SEQPACKET
};

export enum class ip_protocol {
	tcp = IPPROTO::IPPROTO_TCP, udp = IPPROTO::IPPROTO_UDP
};



export class socket_base {


	std::shared_ptr<wsa_wrapper> _wsa;



protected:

	SOCKET _wrapped;

	socket_base() : _wsa{ wsa_provider::get_instance() } {

	}


	
public:


	operator SOCKET() const {
		return _wrapped;
	}

	socket_base(SOCKET wrapped) : socket_base{} {
		_wrapped = wrapped;
	}

	socket_base(socket_type type, ip_protocol protocol) : socket_base{} {
		if (SOCKET sock = ::socket(PF_INET, std::to_underlying(type), std::to_underlying(protocol)); sock != INVALID_SOCKET) {

			_wrapped = sock;

		}
		else throw std::runtime_error("invalid socket");

	}


	void bind(ip_address address, unsigned short port) {
		bind(socket_address{ address, port });
	}


	 

	void bind(socket_address address) {
		if (::bind(_wrapped, reinterpret_cast<sockaddr*>(&address), sizeof sockaddr_in) == SOCKET_ERROR) {
			std::cerr << "RIP" << WSAGetLastError() << std::endl;
		}
 
	}
};