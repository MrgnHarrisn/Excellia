#include "Server.h"
#include "Client.h"

#include <iostream>

int main()
{
	
	char a;
	std::cin >> a;

	if (a == 's') {
		Server server;
		server.start();
	}
	else {
		Client client;
	}

	return 0;
}