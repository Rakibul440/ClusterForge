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
void serverInfo(int);
void clientInfo(int);
void receivedData();


int main(){

    cout << BOLD << WHITE << "[+] STARTING SERVER..." << RESET << endl;
    int server = createServer();

    int clientSocket = accept(server,nullptr,nullptr);
    if(clientSocket == -1){
        cout << BOLD << RED << "[?] ACCEPT FAILED!" << RESET << endl;
        return 1;
    }
    cout << BOLD << GREEN << "[+] CLIENT CONNECTED!" << RESET << endl;

    cout << BOLD << CYAN << "\n[+] SERVER INFO!" << RESET << endl;
    serverInfo(clientSocket);
    clientInfo(clientSocket);
    

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


void serverInfo(int clientsocket){
    sockaddr_in serverIP;
    socklen_t ipsize = sizeof(serverIP);
    getsockname(clientsocket,(struct sockaddr*)&serverIP,&ipsize);
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET,&serverIP.sin_addr,ip,INET_ADDRSTRLEN);
    cout << BOLD << WHITE <<"[+] SERVER IP : " << ip << RESET << endl;
	cout << BOLD << WHITE << "[+] SERVER PORT : " << ntohs(serverIP.sin_port) << RESET << endl;
}


void clientInfo(int clientsocket){

    cout << BOLD << WHITE << "[+] CLIENT INFO "<< RESET << endl;

    sockaddr_in clientIP;
    socklen_t ipsize = sizeof(clientIP);
    getpeername(clientsocket,(struct sockaddr*)&clientIP,&ipsize);
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET,&clientIP.sin_addr,ip,INET_ADDRSTRLEN);
    cout << BOLD << WHITE <<"[+] CLIENT IP : " << ip << RESET << endl;
	cout << BOLD << WHITE << "[+] CLIENT PORT : " << ntohs(clientIP.sin_port) << RESET << endl;
}
