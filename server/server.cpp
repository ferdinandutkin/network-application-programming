import server;

import socket_base;

#include <iostream>
#include <format>



int main() {

	try {
		server<ip_protocol::tcp> s = { 1111 };

		s.start();



		while (true) {
			s.accept();

			std::cout << s.get_client_address() << std::endl;



			bool done{};

			while (not done)
			{


				s.recieve<22>([&](std::string message) {


					bool empty = message.empty();

					std::cout << std::format("recieved: {}", empty ? "empty message" : message) << std::endl;

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
	

