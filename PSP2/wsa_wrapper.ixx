module;




#pragma comment(lib, "WS2_32.lib")
#include <WinSock2.h>
#include <exception>
#include <memory>


export module wsa_wrapper;
import wsa_exception;




export struct wsa_wrapper {


    WSADATA data;

    wsa_wrapper() {
       
        if (WSAStartup(MAKEWORD(2, 0), &data) != 0) {
            throw wsa_exception("wsa startup");
        }
    }




    ~wsa_wrapper() noexcept {

        WSACleanup();

    }

  


};