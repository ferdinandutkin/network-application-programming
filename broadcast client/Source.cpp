import client;
import socket_base;
import socket_options;
import wsa_exception;
import socket_base;
import host_base;
import ip;
#include <iostream>
#include <string>
#include <WinSock2.h>
#include  <format>
#include  <chrono>

 

bool get_server(const udp_client& client, const std::string& name, const port_t& port, socket_address& address) {
	constexpr int wsa_timeout_code = 10060;
	try {
		socket_address server_address{ ip_address::broadcast(), port };
		client.send(name, server_address);
		const std::string res = client.receive<5>(server_address);
		address = server_address;
		return  res == name;
	}
	catch (wsa_exception& exception) {
		if (exception.error_code == wsa_timeout_code)
			return false;
		throw;
	}
}
int main() {
	using namespace std::literals::chrono_literals;
	try
	{
		 
		const udp_client client;

		const port_t server_port = 2000;
		const std::string server_name = "Hello";

		const socket_options options = { .broadcast = true};

		client.set_options(options);

		socket_address server_address;
		if (get_server(client, server_name, server_port, server_address))
		{
			std::cout << std::format("Got server {} ({})", server_name, server_address.operator std::string()) << std::endl;
		}
		
	}
	catch (std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}

	 


}