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

int Interactive(int seclientSocket, int idRequest);
void LounchSnake(node_t *startItem, int clientSocket);
void LounchServer();
void CreateMatrix(int clientSocket);
void LinkAdresses(node_t *head);
void ShowMatrix(node_t *head, int clientSocket);
bool IsParityDefinition(int rows, int columns);
node_t* FindStartItem(node_t* head, bool IsParityDefinition, int rows, int columns);

int main()
{
	printf("<Server> \n");

	LounchServer();
	return 0;
}

void LounchServer()
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in setConnect;
	setConnect.sin_family = AF_INET;
	setConnect.sin_port = htons(8080);
	setConnect.sin_addr.s_addr = inet_addr("127.0.0.1");


	bind(serverSocket, (struct sockaddr *)&setConnect, sizeof(setConnect));

	listen(serverSocket, 1);

	int clientSocket = accept(serverSocket, NULL, NULL);

	if (clientSocket == -1)
	{
		Interactive(serverSocket, 8);
	}

	while(true)
	{
		CreateMatrix(clientSocket);
	}
}

void CreateMatrix(int clientSocket)
{
	int rows = Interactive(clientSocket, 1);
	int columns = Interactive(clientSocket, 2);
	int fullSizeOfMatrix = rows * columns;

	send(clientSocket, &fullSizeOfMatrix, sizeof(fullSizeOfMatrix), 0);

	if (rows == 0 || columns != rows)
	{
		Interactive(clientSocket, 7);
		return;
	}

	Interactive(clientSocket, 3);

	node_t * head = NULL;
	head = (node_t *)malloc (sizeof(node_t));
	node_t* pointerRows = head;
	node_t* pointerColumns = pointerRows;

	for (int i = 0; i < rows  ; i++)
	{
		pointerRows->isVisited = false;

		pointerRows->value = Interactive(clientSocket, 4);

		pointerRows->right = NULL;
		pointerRows->down = NULL;
		pointerRows->left = NULL;
		pointerRows->up = NULL;

		for (int j = 0; j < columns - 1; j++)
		{
			pointerColumns->right = (node_t*)malloc(sizeof(node_t));

			pointerColumns->right->value = Interactive(clientSocket, 4);

			pointerColumns->right->isVisited = false;

			pointerColumns->right->right = NULL;
			pointerColumns->right->down = NULL;
			pointerColumns->right->left = NULL;
			pointerColumns->right->up = NULL;

			pointerColumns = pointerColumns->right;
		}

		pointerRows->down = (node_t*)malloc(sizeof(node_t));

		pointerRows = pointerRows->down;
		pointerColumns = pointerRows;

		if(i == rows - 1)
		{
			pointerRows->down = NULL;
		}
	}

	LinkAdresses(head);
	ShowMatrix(head, clientSocket);
	LounchSnake(FindStartItem(head, IsParityDefinition(rows, columns), rows, columns), clientSocket);
}
void LinkAdresses(node_t* head)
{
	node_t* pointerRows = head;
	node_t* pointerRows2 = pointerRows->down;
	node_t* pointerColumns = pointerRows;
	node_t* pointerColumns2 = pointerRows2;

	while(pointerRows2->down != NULL)
	{
		while(pointerColumns->right != NULL)
		{
			pointerColumns->right->left = pointerColumns;
			pointerColumns2->right->left = pointerColumns2;

			pointerColumns->right->down = pointerColumns2->right;
			pointerColumns2->right->up = pointerColumns->right;

			pointerColumns = pointerColumns->right;
			pointerColumns2 = pointerColumns2->right;
		}
		pointerRows2->up = pointerRows;

		pointerRows2 = pointerRows2->down;
		pointerRows = pointerRows->down;

		pointerColumns = pointerRows;
		pointerColumns2 = pointerRows2;
	}
}

void ShowMatrix(node_t *head, int clientSocket)
{
	node_t* pointerRows = head;
	node_t* pointerColumns = pointerRows;

	Interactive(clientSocket, 5);

	while (pointerRows->down != NULL)
	{
		while(pointerColumns != NULL)
		{
			printf("[%d]", pointerColumns->value);
			pointerColumns = pointerColumns->right;
		}
		pointerRows = pointerRows->down;
		pointerColumns = pointerRows;
		printf("\n");
	}
	
	Interactive(clientSocket, 6);
}

bool IsParityDefinition(int rows, int columns)
{
	return(rows % columns) != 0;
}

node_t* FindStartItem(node_t* head, bool IsParityDefinition, int rows, int columns)
{
    node_t* pointer = head;

    if (IsParityDefinition == true)
    {
        for (int i = 1; i < rows / 2; i++)
        {
            pointer = pointer->right;
            pointer = pointer->down;
        }
    }

    if (IsParityDefinition == false)
    {
        rows = rows - 1;
        columns = columns - 1;

        for (int i = 1; i < rows / 2 + 1; i++)
        {
            pointer = pointer->right;
            pointer = pointer->down;
        }
    }
	pointer->isVisited = true;
    return pointer;
}

void LounchSnake(node_t* startItem, int clientSocket)
{
	node_t* pointer = startItem;
	pointer->isVisited = true;
	send(clientSocket, &pointer->value, sizeof(pointer->value), 0);
	printf("[%d]", pointer->value);
	pointer = pointer->right;
	
	while(true)
	{
		while(pointer->left->isVisited)
		{
			pointer->isVisited = true;
			send(clientSocket, &pointer->value, sizeof(pointer->value), 0);
			pointer = pointer->down;
		}
		
		while(pointer->up->isVisited)
		{
			pointer->isVisited = true;
			send(clientSocket, &pointer->value, sizeof(pointer->value), 0);
			if(pointer->left == NULL)
			{
				return;
			}
			pointer = pointer->left;
		}
		
		while(pointer->right->isVisited)
		{
			pointer->isVisited = true;
			send(clientSocket, &pointer->value, sizeof(pointer->value), 0);
			pointer = pointer->up;
		}
		
		while(pointer->down->isVisited)
		{
			pointer->isVisited = true;
			send(clientSocket, &pointer->value, sizeof(pointer->value), 0);
			if(pointer->right == NULL)
			{
				return;
			}
			pointer = pointer->right;
		}
	}
}

int Interactive(int clientSocket, int idRequest)
{
	char requestOfRows[] = "Insert quantity of rows \n";
	char requestOfColumns[] = "Insert quantity of Columns \n";
	char requestOfValues[] = "Insert values \n";
	char request[] = "";

	int rows;
	int columns;
	int value;

	switch (idRequest)
	{
		case 1:
			send(clientSocket, requestOfRows, sizeof(requestOfRows), 0);
			recv(clientSocket, &rows, sizeof(rows), 0);
			return rows;
		case 2:
			send(clientSocket, requestOfColumns, sizeof(requestOfColumns), 0);
			recv(clientSocket, &columns, sizeof(columns), 0);
			return columns;
		case 3:
			send(clientSocket, requestOfValues, sizeof(requestOfValues), 0);
			break;
		case 4:
			send(clientSocket, request, sizeof(request), 0);
			recv(clientSocket, &value, sizeof(value), 0);
			return value;
		case 5:
			printf("Matrix from Client \n");
			break;
		case 6:
			printf("---------------------------------------- \n");
			break;
		case 7:
			printf("Client is Baluetsya \n");
			break;
		case 8:
			printf("Client has connect \n ");
			break;
	}
	return -1;
}