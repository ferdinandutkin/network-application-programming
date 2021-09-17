import server;


#include <regex>
#include <iostream>
#include <format>



int main() {

	try {
		server s = { 1111 };

		s.start();

		s.accept();

		std::cout << s.get_client_address();



		while (true)
		{

			auto message = s.recieve<22>();


			std::cout << std::format("recieved: {}", message.empty() ? "empty message" : message) << std::endl;

			if (message.empty()) {
				break;
			}

			s << message;

			std::cout << std::format("sent: {}", message.empty() ? "empty message" : message) << std::endl;
		}

	}
	catch (std::exception& e) {
		std::cout << e.what();
	}

}
	

