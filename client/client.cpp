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


    try {

        tcp_client c = { 1234 };

        constexpr int message_len = 22;

        const auto empty_message = std::string(message_len, '\0');
        c.connect(1111, ip_address::loopback(), [&] {

            int i{};
            constexpr char format_string[] = "Hello from client {:03}!";
            while (true) {

                auto message = (i < 100) ? std::format(format_string, i) : empty_message;

                c << message;
                std::cout << message << std::endl;

                c.receive<message_len>([&](std::string received) {
                    i = extract_digit(received);
                    i++;
                    }).join();

            }

            }).join();
        
    }
  

    catch (std::exception& e) {
        std::cout << e.what();
    }
   
}

 