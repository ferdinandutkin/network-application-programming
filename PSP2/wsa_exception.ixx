module;
#define _CRT_SECURE_NO_WARNINGS

#include <exception>

#include <WinSock2.h>
#include <string>
#include <system_error>


export module wsa_exception;


 
export class wsa_exception : public std::exception {

private:
    std::string _message;

    static std::string format_message(int error_code) {
        char error[256];
        auto len = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr,
            static_cast<DWORD>(error_code), MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), error, sizeof error,
            nullptr);
        if (len == 0)
            return "N/A";

        while (len && (error[len - 1] == '\r' || error[len - 1] == '\n'))
            --len;
        return { error, len };

    }
public:

    const char* what() const noexcept override {
        return _message.c_str();
    }

    const int error_code;

    wsa_exception() : error_code{ WSAGetLastError() } {        
        _message = format_message(error_code);
    }


   
    wsa_exception(std::string prefix) : wsa_exception{} {

        _message = prefix + ": " + _message;
    }

    wsa_exception(const char* prefix) : wsa_exception{ std::string{prefix} } {

        
    }

};