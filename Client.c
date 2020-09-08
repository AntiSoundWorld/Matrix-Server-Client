#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>

typedef struct UserIPAddress
{
    char ipAddress[256];
    int port;
} ipAdderess_t;

ipAdderess_t *SetUserAddress();
int LounchClient();
bool Interractive(int id);
void ServerInterractive(int clientSocket);
int ResieveRequestInt(int clientSocket);
void RequestToRecieve(int clientSocket);
void ResponseToRequset(int clientSocket);
bool DoRepetitionsExist();

    int main()
{
    Interractive(1);

    int clientSocket = LounchClient(SetUserAddress());

    do
    {
        ServerInterractive(clientSocket);
    } while (DoRepetitionsExist());

    return 0;
}

ipAdderess_t *SetUserAddress()
{
    Interractive(0);

    char userAnswer[256];
    scanf("%s", userAnswer);

    char *colon = strchr(userAnswer, ':');

    ipAdderess_t *ipAdderess = malloc(sizeof(ipAdderess_t));

    for (int i = 0; userAnswer[i] != colon[0]; i++)
    {
        ipAdderess->ipAddress[i] = userAnswer[i];
    }

    ipAdderess->port = atoi(colon + 1);

    return ipAdderess;
}

int LounchClient(ipAdderess_t *ipAdderess)
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in setConnect;
    setConnect.sin_family = AF_INET;
    setConnect.sin_port = ntohs(ipAdderess->port);
    setConnect.sin_addr.s_addr = inet_addr(ipAdderess->ipAddress);

    int statusOfConnection = connect(clientSocket, (struct sockaddr *)&setConnect, sizeof(setConnect));
    if(statusOfConnection == -1)
    {
        Interractive(2);
        return 0;
    }
    else
    {
        Interractive(3);
    }

    return clientSocket;
}

void ServerInterractive(int clientSocket)
{
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
        Interractive(4);
        return;
    }

    Interractive(5);

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
    char buffer[250];
    recv(clientSocket, buffer, sizeof(buffer), 0);
    size_t sizeOfMessege = strlen(buffer);
    char messege[sizeOfMessege];
    strcpy(messege, buffer);

    printf("%s", messege);
}

void ResponseToRequset(int clientSocket)
{
    int answer;
    scanf("%d", &answer);
    send(clientSocket, &answer, sizeof(answer), 0);
}

bool Interractive(int id)
{
    switch (id)
    {
        case 0:
            printf("Insert IP Address: \n");
            break;
        case 1:
            printf("<Client> \n");
            break;
        case 2:
            printf("status:\nError of connection \n");
            break;
        case 3:
            printf("server is connected \n");
            break;
        case 4:
            printf("Ne baluysya \n");
            break;
        case 5:
            printf("Your Snake \n");
            break;
        case 6:
            printf("\nDo you want try again? <Y/N>\n");
            break;
        case 7:
            printf("\nyou don`t answer the question\n");
            break;
    }
    return NULL;
}
bool DoRepetitionsExist()
{
    Interractive(6);

    char userRetry[4];
    scanf("%s", userRetry);

    bool DoesUserRetry;

    if (userRetry[0] == 'Y' && 'y')
    {
        return DoesUserRetry = true;
    }
    else if (userRetry[0] == 'N' && 'n')
    {
        return DoesUserRetry = false;
    }
    else
    {
        Interractive(7);
        return DoRepetitionsExist();
    }
}
