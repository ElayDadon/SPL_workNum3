 #pragma once
#include <string>
#include "../include/ConnectionHandler.h"
using namespace std;
// TODO: implement the STOMP protocol
class StompProtocol
{
private:
public:
    string parserLogin(string userName,string password);
    string parserJoin(string msg);
    vector<string> parserReport(string msg);
    string parserLogout();
    bool process_answer(string answer);
    
};
