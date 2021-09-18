import client;
import ip;

#include <regex>
#include <iostream>
#include <format>
#include <functional>
#include <thread>


int extract_digit(const std::string& source) {
    static std::regex match_digits{ R"(\d+)" };
    std::smatch match_result;
    std::regex_search(source, match_result, match_digits);

    int digit = std::stoi(match_result.str());

    return digit;
}


int main()
{


    try {
        client c = { 1234 };
        c.connect(1111, ip_address::loopback(), [&] {

            int i{};
            const char format_string[] = "Hello from client {:03}!";
            while (true) {

                auto message = (i < 100) ? std::format(format_string, i) : std::string(22, '\0');

                c << message;
                std::cout << message << std::endl;

                c.recieve<22>([&](std::string recieved) {
                    i = extract_digit(recieved);
                    i++;
                    }).join();

            }

            }).join();
        
    }
  

    catch (std::exception& e) {
        std::cout << e.what();
    }
   
}

 