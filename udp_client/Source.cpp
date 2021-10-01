import client;
import ip;
import socket_base;

#include <regex>
#include <iostream>
#include <format>
#include <functional>
#include <thread>


int extract_digit(const std::string& source) {
	static std::regex match_digits{ R"(\d+)" };
	std::smatch match_result;
	std::regex_search(source, match_result, match_digits);

	const int digit = std::stoi(match_result.str());

	return digit;
}




int main()
{


	try
	{
		socket_address server_address{ ip_address::loopback(), 1111 };

		udp_client client;


		int i{};
		constexpr char format_string[] = "Hello from client {:05}!";

		constexpr int message_len = 25;

		const auto empty_message = std::string(message_len, '\0');
		while (true) {

			auto message = (i < 10000) ? std::format(format_string, i) : empty_message;

			std::cout << message << std::endl;

			client.send(message, server_address, [&]
			{
					client.receive<message_len>(server_address, [&] (std::string received)
					{
							i = extract_digit(received);
							i++;
					}).join();

			}).join();

			
			

			 


			

		}
	}
	catch (std::exception& e) {
		std::cout << e.what();
	}










};