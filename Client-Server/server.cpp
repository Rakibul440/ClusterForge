#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

void startServer();
int main(){

	cout << "[+] STARTING SERVER ..." << endl;

	startServer();

	return 0;
}

void startServer(){

	// creating server
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);


	// specifying the address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;


	// binding socket
	bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));

	// listening socket
	listen(serverSocket,5);
	int clientSocket = accept(serverSocket, nullptr,nullptr);

	// receive data
	char buffer[1024] = {0};

	recv(clientSocket,buffer,sizeof(buffer),0);
	cout << "[+] MESSAGE FROM CLIENT : " << endl;
	cout << buffer << endl;

	// close socket
	close(serverSocket);

}