#include <exception>
#include <WinSock2.h>
#include <string>
#include <system_error>


export module wsa_exception;


 
export class wsa_exception : public std::exception {

private:
    std::string _message;
public:

    const char* what() const noexcept override {
        return _message.c_str();
    }

    const int error_code;

    wsa_exception() : error_code{ WSAGetLastError() } {

        _message = std::system_category().message(error_code);
    }

    wsa_exception(std::string prefix) : wsa_exception{} {

        _message = prefix + _message;
    }

    wsa_exception(const char* prefix) : wsa_exception{ std::string{prefix} } {

        
    }

};