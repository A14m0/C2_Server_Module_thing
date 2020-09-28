#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>

int type = 69; //type MODULE
char *name = "StrangeSecure Server Resolver";
int id = 420;

extern "C"{

    // module entrypoint
    void entrypoint(){
        int sock, sock2, rc;
        struct sockaddr_in server, client, connection;
        char buff[100];
        socklen_t clientLen;

        // create the listening UDP socket
        if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
            printf("Socket failed creation\n");
            return;
        }

        // initialize all the things
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = htonl(INADDR_ANY);
        server.sin_port = htons(12000);

        clientLen = (socklen_t) sizeof(client);

        // turn on broadcast
        setsockopt(sock, SOL_SOCKET, SO_BROADCAST, "1", 1);

        rc = bind(sock, (struct sockaddr *)&server, sizeof(server));
        recvfrom(sock, buff, 100, 0, (struct sockaddr *)&client, &clientLen);

        // loop over this so we can ignore general UDP broadcasts that happen
        // to be sent over our target port
        while(strncmp(buff, "hola", 4)){
            recvfrom(sock, buff, 100, 0, (struct sockaddr *)&client, &clientLen);
        }
        
        printf("[Module] Got good signal over UDP\n");

        // now we initialize the second socket 
        if((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            printf("[Module] Socket 2 failed creation\n");
            return;
        }

        // set up all the things again
        connection.sin_family = AF_INET;
        connection.sin_addr.s_addr = client.sin_addr.s_addr;
        connection.sin_port = htons(1337);

        // connect to the agent
        if(connect(sock2, (struct sockaddr *)&connection, clientLen) == -1){
            printf("Failed to connect to client\n");
            perror("Reason");
        } else printf("Successfully conencted to client\n");

        // we are done here. clean up
        close(sock);
        close(sock2);
        return;
    }
}
