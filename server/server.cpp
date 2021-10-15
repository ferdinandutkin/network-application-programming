import server;

import socket_base;

#include <iostream>
#include <format>



int main() {

	try {
		tcp_server s = { 1111 };
		 
		constexpr int message_len = 24;

		s.start();



		while (true) {
			s.accept();

			std::cout << s.get_client_address() << std::endl;



			bool done{};


			while (true)
			{

				try
				{

					    auto message = s.receive<message_len>();
					 
						const bool empty = message.empty();

						std::cout << std::format("received: {}", empty ? "empty message" : message) << std::endl;

						if (empty) {
							break;
						}
					
					    s << message;


						std::cout << std::format("sent: {}", message) << std::endl;
						

				}
				catch (std::exception& e)
				{
					std::cout << e.what() << std::endl;
					break;
				}
			
			}
		}
		

	}
	catch (std::exception& e) {
		std::cout << e.what();
	}

}
	

