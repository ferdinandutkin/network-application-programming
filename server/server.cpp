import server;

import socket_base;

#include <iostream>
#include <format>



int main() {

	try {
		tcp_server s = { 1111 };
		 
		constexpr int message_len = 22;

		s.start();



		while (true) {
			s.accept();

			std::cout << s.get_client_address() << std::endl;



			bool done{};

			while (not done)
			{


				s.recieve<message_len>([&](std::string message) {
					const bool empty = message.empty();

					std::cout << std::format("received: {}", empty ? "empty message" : message) << std::endl;

					if (empty) {
						done = true;
						return;
					}

					s << message;

					done = empty;

					std::cout << std::format("sent: {}", message) << std::endl;
					}).join();

			}
		}
		

	}
	catch (std::exception& e) {
		std::cout << e.what();
	}

}
	

