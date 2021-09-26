import server;


import ip;
import socket_base;

#include <regex>
#include <iostream>
#include <format>
#include <functional>
#include <thread>



int main()
{

	try
	{

		udp_server server = { 1111 };

		constexpr int message_len = 25;

		while (true)
		{
			bool done{};

			while (not done)
			{


				socket_address client_address;

				server.recieve<message_len>(client_address, [&](std::string message) {
					const bool is_empty = message.empty();

					std::cout << std::format("received: {}", is_empty ? "empty message" : message) << std::endl;

					if (is_empty) {
						done = true;
						return;
					}

					server.send(message, client_address, [&]
					{
							done = is_empty;
					}).join();

				

					std::cout << std::format("sent: {}", message) << std::endl;
					}).join();

			}

		}
		
	}
	catch (exception& e) {
		std::cout << e.what();
	}



}