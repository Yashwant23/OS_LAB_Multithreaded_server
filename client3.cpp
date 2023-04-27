#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main(int argc, char *argv[]) {

	// Sock is int variable to store sokect descriptor.
    int sock = 0;
    
    //Valread is int var to check reciving data from server.
    int valread;
    
    
    //serv_addr is a variable from sockaddr data structure of socket libarry to store address and port of 
    //socket to connect with.
    struct sockaddr_in serv_addr;
    //to store data recieved from server,
    char buffer[1024] = {0};
    
    //Adrre and port of server to connect with
    const char* server_ip = "127.0.0.1";  
    int server_port = 8080;               

    //Making the socket using socket() from libary with ipv4 , domian and TCP network
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Socket creation error" << endl;
        return -1;
    }
    //Verifying weather server is running or not.
    serv_addr.sin_family = AF_INET;
    
    serv_addr.sin_port = htons(server_port);
    
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        cout << "Invalid address/ Address not supported" << endl;
        return -1;
    }
    //Requesting connection with the server .
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "Connection Failed" << endl;
        return -1;
    }
	cout<<"Connected to server on port 8080....\n";
    //Communication with the server code.
    while(true){
     char data[]= "Hello from client";
    std::cout<<"What do you want to send to the server : ";
    std::cin.getline(data, 200);
    send(sock, data, strlen(data), 0);
    cout << "Data sent to server: " << data << endl;

    valread = read(sock, buffer, 1024);
    cout << "Data received from server: " << buffer << endl;
	}

	//Closing the server.
    close(sock);
    return 0;
}

