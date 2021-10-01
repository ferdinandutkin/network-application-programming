module;

#include <memory>
#include <WinSock2.h>
#include <iostream>
#include <chrono>

export module socket_base;

import wsa_exception;
import wsa_wrapper;
import endian;
import wsa_provider;
import socket_options;
import ip;

export using port_t = unsigned short;


export class socket_address {

	sockaddr_in _wrapped;
public:


	socket_address() : _wrapped{} {
	}

	socket_address(sockaddr_in wrapped) : _wrapped{ wrapped } {
	}
	socket_address(ip_address address, port_t port) {
		_wrapped = { PF_INET,  to_big_endian(port), {.S_un = {.S_addr = address.as_big_endian().operator uint32_t()}} };
	}


	friend std::ostream& operator << (std::ostream& os, const socket_address& a) {
		os << a.operator std::string();
		return os;
	}
	

	operator std::string() const {
		return get_ip().operator std::string() + ':' + std::to_string(get_port());
	}

	


	operator sockaddr_in() const {
		return _wrapped;
	}

	ip_address get_ip() const {
		return {_wrapped.sin_addr.S_un.S_addr};
	}

	port_t get_port() const {
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

	void set_socket_option(int level, int name, auto value) const {
		if (::setsockopt(_wrapped, level, name, reinterpret_cast<const char*>(&value), sizeof value) < 0)
			throw wsa_exception(__func__);
	}
	
	void set_broadcast_option(bool value) const {
		set_socket_option(SOL_SOCKET, SO_BROADCAST, value? 1 : 0);
	 		
	}

	void set_receive_timeout_option(std::chrono::milliseconds timeout) const {
		timeval tv;
		tv.tv_sec = timeout.count(); //?????????
		set_socket_option(SOL_SOCKET, SO_RCVTIMEO, tv);

	}

	void set_send_timeout_option(std::chrono::milliseconds timeout) const  {
		timeval tv;
		tv.tv_sec = timeout.count(); //?????????
		set_socket_option(SOL_SOCKET, SO_SNDTIMEO, tv);
	}

	void set_socket_options(socket_options options) const {
		set_broadcast_option(options.broadcast);
		set_send_timeout_option(options.send_timeout);
		set_receive_timeout_option(options.receive_timeout);
	}

	
public:

	void set_options(socket_options options) const {
		set_socket_options(options);
	}

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

	socket_base(socket_type type, socket_options options) : socket_base{type} {
		set_options(options);
	}
	 

 


	template<size_t length>
	std::string receive(socket_address& from) const requires (protocol == ip_protocol::udp) {
		char response[length + 1]{};

		int from_len = sizeof sockaddr_in;

		if (::recvfrom(this->_wrapped, response, std::size(response), 0, reinterpret_cast<sockaddr*>(&from), &from_len) == SOCKET_ERROR) {
			throw wsa_exception(__func__);
		}
		
		return { response };
	}

 

	void send(std::string message, socket_address to) const requires (protocol == ip_protocol::udp) {
		if (::sendto(this->_wrapped, message.c_str(), message.size(), 0, reinterpret_cast<sockaddr*>(&to), sizeof sockaddr) == SOCKET_ERROR) {
			throw wsa_exception(__func__);
		}
	}



	void send(std::string message, ip_address to_address, unsigned short to_port) const requires (protocol == ip_protocol::udp) {
		send(message, socket_address{ to_address, to_port });
	}





	void bind(ip_address address, port_t port) const {
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