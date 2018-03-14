/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <cstdlib>
#include <string>
#include <iostream>
#include "User.h"

using namespace std;

User::User(string fname)
{
    file.open(fname);
    if(!file)
    {
        cout<<"Cannot find file: "<<fname<<endl;
        exit(0);
    }

}
void User::readLine()
{
    string command;
    getline(file, command, ' ');
    if(command=="Handle")    //Case for handling "HANDLE" command
    {
        file.ignore(10, '@');  //ignore whitespace up to the @ of the user's handle
        file>>name;
        cout<<"Handle set to "<<name<<endl;
        return;
    }
    if(command=="Start")     //Case for handling "START" command
    {
        file.ignore(10, '#');
        Twit newTwit = readTwit();

    }
    if(command=="Follow")    //Case for handling "FOLLOW" command
    {
        file.ignore();
    }
    if(command=="Read")      //Case for handling "READ" command
    {
        file.ignore();
    }
    if(command=="Exit")      //Case for handling "EXIT" command
    {

    }
}
Twit User::readTwit()
{
    Twit output;
    file>>output.topic;
    

}