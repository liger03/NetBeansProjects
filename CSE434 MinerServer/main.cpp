/*
 * Server program of the socket project's "bitcoin" miner
 */

#include <cstdlib>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

class MinerID
{
public: 
    string username;
    string ip;
    string port;
    int initialCoins;
    socket *comSocket;
};

int parseToken(string input);

int main(int argc, char** argv) 
{
    string input;
    int numberOfMiners=0;
    vector<MinerID> setOfMiners;
    while(true)
    {
        cin>>input;
        switch(parseToken(input))
        {
            case 0:
                cout<<"Number of miners: "<<numberOfMiners<<endl<<endl;
                for(int i=0; i<numberOfMiners; i++)
                {
                    cout<<"For miner "<<i<<": "<<setOfMiners[i].username<<endl;
                    cout<<"IP address: "<<setOfMiners[i].ip<<endl;
                    cout<<"Port number: "<<setOfMiners[i].port<<endl;
                    cout<<"Initial amount of coins: "<<setOfMiners[i].initialCoins<<endl<<endl;
                }
                break;
            case 1:
                MinerID newMiner;
                cin>>newMiner.username;
                cin>>newMiner.ip;
                cin>>newMiner.port;
                cin>>newMiner.initialCoins;
                setOfMiners.push_back(newMiner);
                if ((comSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
                {
                    cout<<"ERROR: Cannot create miner(socket creation failed)"<<endl;
                    break;
                }
                cout<<"TODO: register"<<endl;
                break;
            case 2:
                cout<<"TODO: deregister"<<endl;
                break;
            case 3:
                cout<<"TODO: save:"<<endl;
                break;
            default:
                cout<<"Invalid command: "<<input<<endl;
                break;
        }
        cin.ignore();
        cin.clear();
    }
    return 0;
}

int parseToken(string input)
{
    if(strcmp(input.c_str(), "query")==0)
        return 0;
    if(strcmp(input.c_str(), "register")==0)
        return 1;
    if(strcmp(input.c_str(), "deregister")==0)
        return 2;
    if(strcmp(input.c_str(), "save")==0)
        return 3;
    return 4;
}

