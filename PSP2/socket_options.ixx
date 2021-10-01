module;

#include <chrono>

export module socket_options;

using namespace std::literals::chrono_literals;

export struct socket_options {
	

	bool broadcast{};
	std::chrono::milliseconds send_timeout;
	std::chrono::milliseconds receive_timeout;

	 

	 
};