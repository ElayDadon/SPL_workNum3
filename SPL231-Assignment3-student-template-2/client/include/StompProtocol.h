 #pragma once
#include "../include/event.h"
#include "../include/json.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <regex>
#include "../include/ConnectionHandler.h"
#include <sstream>
using namespace std;
// TODO: implement the STOMP protocol
class StompProtocol
{
private:
//<id,command>
map<string,string> whatCommand;
public:
    string parserLogin(string userName,string password);
    string parserJoin(string msg);
    vector<string> parserReport(string msg);
    string parserLogout();
    bool process_answer(string answer);
    
};
