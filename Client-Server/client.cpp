#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

void startClient();

int main(){

	cout << "[+] STARTING CLIENT..." << endl;

	startClient();

	return 0;
}

void startClient(){


	// creating soecket
	int clientSocket = socket(AF_INET,SOCK_STREAM,0);

	// specifying address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// sending connection request
	connect(clientSocket, (struct sockaddr*) &serverAddress,sizeof(serverAddress));

	while(true){
		// sending data
		cout << "[+] ENTER YOUR MESSAGE : ";
		// const char* sms;
		// cin >> *sms;

		string sms;
		getline(cin,sms);
		send(clientSocket,sms.c_str(),sms.size(),0);
		// send(clientSocket,sms,strlen(sms),0);
	}

	// close
	close(clientSocket);
}