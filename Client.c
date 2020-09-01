#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h> 

typedef struct Node	
{
    int id;
    int value;
    bool isVisited;
    struct Node* right;
    struct Node* down;
    struct Node* left;
    struct Node* up;
}node_t;

void LounchClient();
void ClientInterractive(int clientSocket, int idMessege);
int ResieveRequestInt(int clientSocket);
void RequestToRecieve(int clientSocket);
void ResponseToRequset(int clientSocket);

int main()
{
    printf("<Client> \n");
    LounchClient();
    return 0;
}

void LounchClient()
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in setConnect;
    setConnect.sin_family = AF_INET;
    setConnect.sin_port = ntohs(8080);
    setConnect.sin_addr.s_addr = INADDR_ANY;

    int statusOfConnection = connect(clientSocket, (struct sockaddr *)&setConnect, sizeof(setConnect));
    if(statusOfConnection == -1)
    {
        ClientInterractive(clientSocket, 1);
    }
    else
    {
        ClientInterractive(clientSocket, 2);
    }
}

void ClientInterractive(int clientSocket, int idMessege)
{
    switch (idMessege)
    {
    case 1:
        printf("status:\nError of connection \n");
        break;

    case 2:
        printf("server is connected \n");
        break;
    }

    for(int i = 0; i < 2; i++ )
    {
        RequestToRecieve(clientSocket);
        ResponseToRequset(clientSocket);
    }

    int fullSizeMatrix = ResieveRequestInt(clientSocket);


    for (int endOfMatrix = 0; endOfMatrix < fullSizeMatrix; endOfMatrix++)
    {
        RequestToRecieve(clientSocket);
        ResponseToRequset(clientSocket);
    }
    ResieveRequestInt(clientSocket);

    if (fullSizeMatrix == 0)
    {
        printf("Ne baluysya \n");
        return;
    }

    printf("Your Snake \n");

    for (int endOfMatrix = 0; endOfMatrix < fullSizeMatrix; endOfMatrix++)
    {
        int resieveRequestInt = ResieveRequestInt(clientSocket);
        printf("[%d]", resieveRequestInt);
    }
}

int ResieveRequestInt(int clientSocket)
{
    int messegeFromServer;
    recv(clientSocket, &messegeFromServer, sizeof(messegeFromServer), 0);
    return messegeFromServer;
}

void RequestToRecieve(int clientSocket)
{
    char messegeFromServer[250];
    recv(clientSocket, messegeFromServer, sizeof(messegeFromServer), 0);
    printf("%s", messegeFromServer);
}

void ResponseToRequset(int clientSocket)
{
    int answer;
    scanf("%d", &answer);
    send(clientSocket, &answer, sizeof(answer), 0);
}

