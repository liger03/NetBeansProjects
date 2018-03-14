/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TWEETER.cpp
 * Author: john_
 *
 * Created on March 9, 2018, 2:13 PM
 */

#include <cstdlib>
#include <string>
#include <iostream>
#include "User.h"

using namespace std;


int main(int argc, char** argv) 
{
    string fname;
    if(argc<2)
    {
        cout<<"Please enter a filename:";
        cin>>fname;
        cout<<endl;
    }
    else
    {
        fname=argv[2];
    }
    User testUser(fname);
    testUser.readLine();

    return 0;
}

