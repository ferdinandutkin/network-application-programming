module;

#include <memory>
#include <WinSock2.h>
#include <iostream>


export module socket_base;

import wsa_exception;
import wsa_wrapper;
import endian;
import wsa_provider;
import ip;



export class socket_address {

	sockaddr_in _wrapped;
public:


	socket_address() : _wrapped{} {
	}

	socket_address(sockaddr_in wrapped) : _wrapped{ wrapped } {
	}
	socket_address(ip_address address, unsigned short port) {
		_wrapped = { PF_INET,  to_big_endian(port), {.S_un = {.S_addr = address.as_big_endian().operator uint32_t()}} };
	}


	friend std::ostream& operator << (std::ostream& os, const socket_address& a) {
		os << a.get_ip() << ':' << a.get_port();
		return os;
	}

	


	operator sockaddr_in() const {
		return _wrapped;
	}

	ip_address get_ip() const {
		return {_wrapped.sin_addr.S_un.S_addr};
	}

	unsigned short get_port() const {
		return _wrapped.sin_port;
	}


};

export enum class socket_type {
	stream = SOCK_STREAM, datagram = SOCK_DGRAM, row = SOCK_RAW, reliable_message_datagram = SOCK_RDM, sequenced_packets = SOCK_SEQPACKET
};

export enum class ip_protocol {
	tcp = IPPROTO::IPPROTO_TCP, udp = IPPROTO::IPPROTO_UDP
};




export template <ip_protocol protocol>

class socket_base {


	std::shared_ptr<wsa_wrapper> _wsa;

protected:


	SOCKET _wrapped{};

	socket_base() : _wsa{ wsa_provider::get_instance() } {

	}


	
public:


	operator SOCKET() const {
		return _wrapped;
	}

	socket_base(SOCKET wrapped) : socket_base{} {
		_wrapped = wrapped;
	}

	socket_base(socket_type type) : socket_base{} {
		if (SOCKET sock = ::socket(PF_INET, std::to_underlying(type), std::to_underlying(protocol)); sock != INVALID_SOCKET) {

			_wrapped = sock;

		}
		else throw wsa_exception("invalid socket");

	}


	 

 


	template<size_t length>
	std::string recieve(socket_address& from) const requires (protocol == ip_protocol::udp) {
		char response[length + 1]{};

		int from_len = sizeof sockaddr_in;

		if (::recvfrom(this->_wrapped, response, std::size(response), 0, reinterpret_cast<sockaddr*>(&from), &from_len) == SOCKET_ERROR) {
			throw wsa_exception(__func__);
		}
		
		return { response };
	}

 

	void send(std::string message, socket_address to) const requires (protocol == ip_protocol::udp) {
		if (::sendto(this->_wrapped, message.c_str(), message.size(), 0, reinterpret_cast<sockaddr*>(&to), sizeof sockaddr) == SOCKET_ERROR) {
			std::cout << WSAGetLastError();
			throw wsa_exception(__func__);
		}
	}



	void send(std::string message, ip_address to_address, unsigned short to_port) const requires (protocol == ip_protocol::udp) {
		send(message, socket_address{ to_address, to_port });
	}





	void bind(ip_address address, unsigned short port) const {
		bind(socket_address{ address, port });
	}


	socket_address get_address() const {
		sockaddr sockaddr{};
		int size = sizeof sockaddr;
		getpeername(_wrapped, &sockaddr, &size);

		return {*reinterpret_cast<sockaddr_in*>(&sockaddr)};
	}

	 

	void bind(socket_address address) const {
		if (::bind(_wrapped, reinterpret_cast<sockaddr*>(&address), sizeof sockaddr_in) == SOCKET_ERROR) {
			throw wsa_exception(__func__);
		}
 
	}


	virtual ~socket_base() {
		//::closesocket(_wrapped);
		//blyat)))))))))))))))
	 
	}
};