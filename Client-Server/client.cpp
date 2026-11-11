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
	int status = connect(clientSocket, (struct sockaddr*) &serverAddress,sizeof(serverAddress));

	cout << "[+] CONNECTING TO SERVER ..." << endl;
	if(status==1){
		cout << "[?] CONNECTION FAILED!" << endl;
		return;
	}
	cout << "[-] CONNECTION SUCCESSFULL!" << endl;

	while(true){
		cout << "[-] STATUS : " << status << endl;
		// sending data
		cout << "[+] ENTER YOUR MESSAGE : ";

		string sms;
		getline(cin,sms);
		int bytesSent = send(clientSocket,sms.c_str(),sms.size(),0);

		if(bytesSent==-1){
			cout << "[?] SERVER STOPPED, CAN NOT SEND!" << endl;
			break;
		}
	}

	// close
	close(clientSocket);
}