import socket_base;
import socket_options;
import server;
import wsa_exception;
#include <iostream>
#include <string>
#include <chrono>


bool get_request_from_client(const udp_server& socket, const std::string& name, socket_address& from) {
	constexpr int wsa_timeout_code = 10060;
	try {
		auto msg = socket.receive<5>(from);

		if ( msg == name)
			return true;
	}
	catch (wsa_exception& exception) {
		if (exception.error_code == wsa_timeout_code)
			return false;
		throw;
	}

	return false;
}

void put_answer_to_client(const udp_server& socket, const std::string& message, const socket_address& to) {

	socket.send(message, to);
}
int main() {
	using namespace std::literals::chrono_literals;

	const port_t server_port = 344;
	const std::string server_name = "Hello";


	const udp_server server = { server_port, ip_address::any()};

	const socket_options options = { .send_timeout = 600ms, .receive_timeout = 600ms, };

 
	
	while(true)
	{
		socket_address client_address;

		if (get_request_from_client(server, server_name, client_address))
		{
			std::cout << "called";
			
			put_answer_to_client(server, server_name, client_address);
		}
		 
	}


}
