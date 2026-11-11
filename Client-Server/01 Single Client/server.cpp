#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <arpa/inet.h>

// colors on terminal
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"


#define BOLD "\033[1m"



using namespace std;

int createServer();
string serverInfo(int);
string clientInfo(int);
void receivedData(int,string);


int main(){

    cout << BOLD << WHITE << "[+] STARTING SERVER..." << RESET << endl;
    int server = createServer();

    int clientSocket = accept(server,nullptr,nullptr);
    if(clientSocket == -1){
        cout << BOLD << RED << "[?] ACCEPT FAILED!" << RESET << endl;
        return 1;
    }
    cout << BOLD << GREEN << "[+] CLIENT CONNECTED!" << RESET << endl;

    cout << BOLD << CYAN << "\n[+] SERVER INFO" << RESET << endl;
    serverInfo(clientSocket);
    string clientIp = clientInfo(clientSocket);

    receivedData(clientSocket,clientIp);

    cout << BOLD << CYAN << "\n[*] CLOSING CONNECTION..." << RESET << endl;
    close(server);
    cout << BOLD << RED << "[*] CONNECTION CLOSED!" << RESET << endl;

    return 0;
}

int createServer(){
    cout << GREEN << "[+] SOCKET CREATED!" << RESET << endl;

    int serverSocket = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket,(struct sockaddr*)&serverAddress,sizeof(serverAddress));

    listen(serverSocket,5);
    cout << "[+] SERVER LISTENING..." << endl;
    cout << "[+] WAITING FOR CLIENT..." << endl;



    return serverSocket;
}

void receivedData(int clientSocket, string ip){
    while (true)
    {
        char buffer[1024] = {0};
        int bytesReceived = recv(clientSocket,buffer,sizeof(buffer),0);
        		if(bytesReceived<=0){
			cout << BOLD << RED << "\n[?] CLIENT DISCONNECTED!" << RESET << endl;
			break;
		}
		
		cout << "[+] MESSAGE FROM CLIENT ( " << ip << " ) : ";
		cout << buffer << endl;
    }
    
}


string serverInfo(int clientsocket){
    sockaddr_in serverIP;
    socklen_t ipsize = sizeof(serverIP);
    getsockname(clientsocket,(struct sockaddr*)&serverIP,&ipsize);
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET,&serverIP.sin_addr,ip,INET_ADDRSTRLEN);
    cout << BOLD << WHITE <<"[+] SERVER IP : " << ip << RESET << endl;
	cout << BOLD << WHITE << "[+] SERVER PORT : " << ntohs(serverIP.sin_port) << RESET << endl;
    cout << "\n";
    return string(ip);
}


string clientInfo(int clientsocket){

    cout << BOLD << CYAN << "[+] CLIENT INFO "<< RESET << endl;

    sockaddr_in clientIP;
    socklen_t ipsize = sizeof(clientIP);
    getpeername(clientsocket,(struct sockaddr*)&clientIP,&ipsize);
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET,&clientIP.sin_addr,ip,INET_ADDRSTRLEN);
    cout << BOLD << WHITE <<"[+] CLIENT IP : " << ip << RESET << endl;
	cout << BOLD << WHITE << "[+] CLIENT PORT : " << ntohs(clientIP.sin_port) << RESET << endl;

    cout << "\n";
    return string(ip);
}
