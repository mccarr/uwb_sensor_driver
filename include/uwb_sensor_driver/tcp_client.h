#include<iostream>    //cout
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<string>  //string
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<netdb.h> //hostent
#include<unistd.h> //for the sleep function
#include<vector> //for the vector iteration
#include<sstream> //for getting the ints from the server


class tcp_client
{
private:
    int sock;
    std::string address;
    int port;
    struct sockaddr_in server;
     
public:
    tcp_client ();
    bool conn(std::string, int);
    bool send_data(std::string data);
    std::string receive(int);
};
