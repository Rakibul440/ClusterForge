#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"


#define BOLD "\033[1m"

int createClient();
void sendData(int);

int main(){
	cout << BOLD << CYAN << "[+] STARTING CLIENT..." << RESET << endl;
    int client = createClient();
    if(client == -1){
        cout << BOLD << RED << "[?] COULDN'T CREATE CLIENT!" << RESET << endl;
        return 1;
    }
    sendData(client);

    cout << BOLD << CYAN << "\n[*] CLOSING CONNECTION..." << RESET << endl;
    close(client);
    cout << BOLD << RED << "[*] CONNECTION CLOSED!" << RESET << endl;

    return 0;
}

int createClient(){
    
    int clientSocket = socket(AF_INET,SOCK_STREAM,0);
    cout << GREEN << "[+] SOCKET CREATED!" << RESET << endl;

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    int status = connect(clientSocket,(struct sockaddr*)&serverAddress,sizeof(serverAddress));
    
    cout << BOLD << WHITE << "[+] CONNECTING TO SERVER ..." << endl;
	if(status==1){
		cout << BOLD << RED << "[?] CONNECTION FAILED!" << endl;
		return -1;
	}
	cout << BOLD << GREEN <<"[-] CONNECTED SUCCESSFULLY!" << RESET << endl;

    cout << "\n";
    return clientSocket;
}

void sendData(int client){
    while(true){
        cout << "[+] ENTER YOUR MESSAGE : ";

        string message;
        getline(cin,message);
        int bytesSent = send(client,message.c_str(),message.size(),0);

        if(bytesSent==-1){
			cout << BOLD << RED << "[?] SERVER STOPPED, CAN NOT SEND!" << RESET << endl;
			break;
		}
    }
}