#ifndef __NETWORK_H__
#define __NETWORK_H__


void setup_comm(char isServer, char* argv2);
void manage_turns(char isServer, int otherSock);
void make_turn(int i1, int j1, int i2, int j2);
void send_move(int isServer, int otherSock);

#endif
