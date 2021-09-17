import server;

#include <iostream>

#include <string>



int main() {
	server s = { 1111 };

	s.start();

	s.accept();

	std::cout << s.get_client_address();

	s.send( "hui" );

	
}
