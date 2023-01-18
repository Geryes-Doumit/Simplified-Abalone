#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "artificial_intelligence.h"
#include "game_basics.h"
#include "global_variables.h"
#include "interpret_input.h"
#include "movement.h"
#include "validity_check.h"
#include "network.h"
#include "ihm.h"

int winner = 0;

/**
 * \fn void setup_comm(char isServer, char* argv2)
 * \brief Setup the communication between the server and the client
 * \details Sets up a client or server based on the command line arguments
 *
 * \param isServer a boolean value indicating whether the instance is a server or a client
 * \param argv2 the ip and port concatenated if the instance is the client or only the port if the instance is the server
 */
void setup_comm(char isServer, char *argv2)
{
    char *ip;
    int port;

    if (isServer)
    {
        port = atoi(argv2);
    }
    else
    {
        char *split = strtok(argv2, ":");
        if (split)
        {
            memcpy(&ip, &split, sizeof(split));
        }
        split = strtok(NULL, ":");

        if (split)
        {
            port = atoi(split);
        }
        printf("Initialized client. IP=%s, port=%i\n", ip, port);
    }

    struct sockaddr_in sa;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        return perror("Error while creating socket\n");
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = isServer ? INADDR_ANY : inet_addr(ip);
    sa.sin_port = htons(port);

    if (isServer)
    {
        /* Liberation du socket */
        int reuse = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuse, sizeof(reuse)) < 0)
            perror("setsockopt(SO_REUSEADDR) failed");

        /* Liaison entre une socket et une adresse/port de donnée */
        bind(sock, (const struct sockaddr *)&sa, sizeof(sa));

        /* Le serveur peut écouter la requête "client" */
        if (listen(sock, 2) == -1)
            return perror("Error in listen()");
        printf("En attente de connexion :\n");
        /* Acceptation du socket client*/
        struct sockaddr_in client;
        socklen_t addrClient = sizeof(client);
        int clientSock = accept(sock, (struct sockaddr *)&client, &addrClient);
        if (clientSock == -1)
            return perror("Error while accepting client socket from server");
        printf("Connexion acceptee\n");

        manage_turns(1, clientSock);
        close(clientSock);
        close(sock);
    }
    else // client
    {
        /* Connexion du client au serveur */
        if (connect(sock, (struct sockaddr *)&sa, sizeof(sa)) == -1)
            return perror("connect");
        printf("Connected to server %s:%d\n", ip, port);

        manage_turns(0, sock);
        close(sock);
    }
}

/**
 * \fn void manage_turns(char isServer, int ownSock, int otherSock)
 * \brief Run a game in network mode, manages the rotation between the servers and the clients turns
 * \details Sets up the game settings
 * \details Manages the turns and ending of the game with sockets
 *
 * \param isServer a boolean value indicating whether the instance is a server or a client
 * \param otherSock the socket id used by the instance
 */
void manage_turns(char isServer, int otherSock)
{
    player = 1;
    initialize_matrix();
    print_board();
    int i1, j1, i2, j2;

    if (!isServer)
    {
        send_move(isServer, otherSock);
    }
    while (!finished)
    {
        int coords[4];
        char received[6];
        if (recv(otherSock, &received, 6 * sizeof(char), 0) == -1)
            return perror("Error while receiving socket\n");
        printf("(%s) Received data: %s\n", isServer ? "server" : "client", received);
        input_as_array(received, coords);
        i1 = coords[0];
        j1 = coords[1];
        i2 = coords[2];
        j2 = coords[3];
        //printf("Received (%s) and playing: %i, %i, %i, %i\n", isServer ? "server" : "client", i1, j1, i2, j2);
        make_turn(i1, j1, i2, j2);

        if (finished)
            break;

        send_move(isServer, otherSock);
    }
    printf("Game finished, winner: J%i\n", winner);
}

/**
 * \fn void make_turn(int i1, int j1, int i2, int j2)
 * \brief Manages a specific turn
 * \details Checks if a move is valid or not, if not stops the game
 *
 * \param i1 the first coordinate row index
 * \param j1 the first column index
 * \param i2 the second coordinate row index
 * \param j2 the second column index
 */
void make_turn(int i1, int j1, int i2, int j2)
{
    if (is_move_allowed(i1, j1, i2, j2) == 1)
    {
        shift_piece(i1, j1, i2, j2);
        print_board();
        player = player == 1 ? 2 : 1;
    }
    else if (is_move_allowed(i1, j1, i2, j2) == 0)
    {
        printf("move not allowed\n");
        finished = 1;
        winner = player == 1 ? 2 : 1;
    }
    else
    {
        printf("winning move\n");
        finished = 1;
        winner = player;
    }
}

/**
 * \fn void send_move(int isServer, int otherSock)

 * \brief Sends the next move through the socket
 *
 * \param isServer a boolean value indicating whether the instance is a server or a client
 * \param otherSock the socket id used by the instance
 */
void send_move(int isServer, int otherSock)
{
    int coords[4];
    ai_make_move();
    char aiMoveAsString[6];
    aiMoveAsString[0] = ai_move[0];
    aiMoveAsString[1] = ai_move[1];
    aiMoveAsString[2] = ai_move[2];
    aiMoveAsString[3] = ai_move[3];
    aiMoveAsString[4] = ai_move[4];
    aiMoveAsString[5] = '\0';

    char coordsAsString[6];
    ai_to_string_input(aiMoveAsString, coordsAsString);
    input_as_array(coordsAsString, coords);

    int i1 = coords[0];
    int j1 = coords[1];
    int i2 = coords[2];
    int j2 = coords[3];
    //printf("playing (%s) : %i, %i, %i, %i\n", isServer ? "server" : "client", i1, j1, i2, j2);
    make_turn(i1, j1, i2, j2);
    coordsAsString[5] = '\0';

    send(otherSock, coordsAsString, 6 * sizeof(char), 0);
    printf("(%s) Sent data: %s\n", isServer ? "server" : "client", coordsAsString);
}