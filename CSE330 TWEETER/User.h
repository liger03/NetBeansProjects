/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   User.h
 * Author: john_
 *
 * Created on March 9, 2018, 3:40 PM
 */

#ifndef USER_H
#define USER_H

#include <fstream>
#include <string>

class Twit              //a Twit is the patented totally-legit message package of TWEETER
{
public:
    std::string topic;
    std::string message;
};

class User
{
public:
    User(std::string fname); //create a user and designate a thread for it to read
    void readLine();  //will read one command and perform it
    Twit readTwit();
private:
    std::ifstream file;
    std::string name;
};

#endif /* USER_H */

