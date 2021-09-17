module;


#include <memory>



export module wsa_provider;


import wsa_wrapper;


export class wsa_provider {

    static std::shared_ptr<wsa_wrapper> _instance;

public:

    static std::shared_ptr<wsa_wrapper> get_instance() {
        if (not _instance) {
            _instance = std::make_shared<wsa_wrapper>();
        }
        return _instance;
    };

};


std::shared_ptr<wsa_wrapper> wsa_provider::_instance;
 