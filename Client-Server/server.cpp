#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <arpa/inet.h> // for inet_ntop();

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


	// Print Server IP - (OPTIONAL)
	cout << "\n[-] SERVER INFO [-]" << endl;

	sockaddr_in serverIP;
	socklen_t ipSize = sizeof(serverIP);
	getsockname(clientSocket, (struct sockaddr*) &serverIP, &ipSize);
	char IP[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &serverIP.sin_addr,IP,INET_ADDRSTRLEN);
	cout << "[+] SERVER IP : " << IP << endl;
	cout << "[+] SERVER PORT : " << ntohs(serverIP.sin_port) << endl;

	// Print Client Address - (OPTIONAL)
	sockaddr_in clientIP;
	socklen_t clientIpSize = sizeof(clientIP);
	getpeername(clientSocket, (struct sockaddr*) &clientIP, &clientIpSize);
	char peerIP[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientIP.sin_addr,peerIP,INET_ADDRSTRLEN);
	cout << "[+] CLIENT IP : " << peerIP << endl;
	cout << "[+] CLIENT PORT : " << ntohs(clientIP.sin_port) << endl;

	cout << "\n";


	while(true){

		// receive data
		char buffer[1024] = {0};
		int bytesReceived = recv(clientSocket,buffer,sizeof(buffer),0);

		if(bytesReceived<=0){
			cout << "\n[?] CLIENT DISCONNECTED!\n";
			break;
		}
		
		cout << "[+] MESSAGE FROM CLIENT( " << peerIP << " ) : ";
		cout << buffer << endl;
	}


	// close socket
	close(serverSocket);

}