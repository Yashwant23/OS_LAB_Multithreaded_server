#include <iostream>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<semaphore.h>
#include<mutex>
#include<bits/stdc++.h>


using namespace std;
sem_t sem;
sem_t sem2;
mutex mtx;
mutex mtx2;
int nc=0;
int active=0;
queue<int> threadPool;

void handleClient(int clientSocket) {


    char buffer[1024];
    int valread;
    
    //assignment of client ID
   int ClientID=++nc;
   active++;
   sem_wait(&sem2);
   std::cout<<"Connected to Client : "<<ClientID<<endl;
    while (true) {
    
    //Retriving request from client.
        valread = read(clientSocket, buffer, 1024);
        if (valread <= 0) {
            break;
        }
        //Decrimenting the sem so that no two thread are using same resource.
        sem_wait(&sem);
        mtx.lock();
        buffer[valread] = '\0';
        //while(active || pri[ClientID]){  pri[ClientID]=!pri[ClientID-1]; }
        
        
        cout <<ClientID<< " Client message: " << buffer << endl;
        //active=true;
        char data[] = "Hello from server";
        std::cout<<"What do you want to send to clinet : "<<ClientID<<" :";
        std::cin.getline(data , 200);
        //Sending the data to the client.
    send(clientSocket, data, strlen(data), 0);
    cout << "Data sent to client:"<<ClientID<<" " << data << endl;

    mtx.unlock();
    sem_post(&sem);
   
         
    }
    sem_post(&sem2);
    close(clientSocket);
    //nc--;
    active--;
    int topClient;
        if(active<2 && threadPool.size()>0) 
        {
        topClient=threadPool.front();
	threadPool.pop();
	thread clientThread(handleClient, topClient);
        clientThread.detach();
        
        }
        
    std::cout<<"Client "<<ClientID<<" left the server ***\n ";
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int opt = 1;
    int addrlen = sizeof(serverAddr);
	sem_init(&sem , 0, 1);
	sem_init(&sem2 , 0 , 2);
	//sem_post(sem);
  //Creatinon of Socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == 0) {
        cerr << "Failed to create server socket." << endl;
        exit(EXIT_FAILURE);
    }

    
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        cerr << "Failed to set socket options." << endl;
        exit(EXIT_FAILURE);
    }

//Binding the socket with adrress  
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Failed to bind server socket." << endl;
        exit(EXIT_FAILURE);
    }

 //listing the client upto 10
    if (listen(serverSocket, 10) < 0) {
        cerr << "Failed to listen for connections." << endl;
        exit(EXIT_FAILURE);
    }
	std::cout<<"Server listing on Port 8080....\n";
    
    while (true) {
    
    //Accepting the client 
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, (socklen_t*)&addrlen);
        if (clientSocket < 0) {
            cerr << "Failed to accept incoming connection." << endl;
            continue;
        }
        
        //Adding Client to queue
      	threadPool.push(clientSocket);
      	
      	
      	int topClient;
        if(active<2 && threadPool.size()>0) 
        {
        topClient=threadPool.front();
		threadPool.pop();
		thread clientThread(handleClient, topClient);
        clientThread.detach();
        
        }
        
    }

    return 0;
}

