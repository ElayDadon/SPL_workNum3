#include "../include/event.h"
#include "../include/json.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <regex>
#include "../include/StompProtocol.h"
using namespace std;
int id = 0;
int sub_id=0;
int receipt=0;
string users;
//login 1.1.1.1:2000 meni films
//IP,port,userName, password
string StompProtocol::parserLogin(string userName,string password){
        string output = "CONNECT\naccept-version:1.2\nhost:stop.cs.bgu.ac.il\n";
        // string userName;
        // string password;
        // regex pattern("([\\d.]+:\\d+)");
        // smatch m;
        // int position = m.position();
        // string username_andpassword = msg.substr(position+m.length());
        // int position_of_username = username_andpassword.find(' ');
        // userName = username_andpassword.substr(0,position_of_username);
        // password = username_andpassword.substr(position_of_username+1);
        // users = userName;
        output +="login:"+userName+"\n"+"passcode:"+password+"\n\n";

        return output;
}
string StompProtocol::parserLogout(){
string output  ="DISCONNECT\n";
string receipt_string ="receipt:"+to_string(receipt);
output += receipt_string+"\n\n";
receipt++;
return output;

}
string StompProtocol::parserJoin(string destination){
    string output = "SUBSCRIBE\n";
    output +="destination:"+destination+"\n";
    output += "id:"+to_string(id)+"\n"+"receipt:"+to_string(receipt)+"\n\n";
    sub_id++;
    receipt++;
    return output;
}
//report {file}
vector<string> StompProtocol::parserReport(string msg){
    vector<string> outputs;
    int position_of_space = msg.find(' ');
    string Tojson = msg.substr(position_of_space+1);
    names_and_events event = parseEventsFile(Tojson);
    string output = "SEND\n";
    string dest = "destination:/"+event.team_a_name+"_"+event.team_b_name+"\n";
    output += dest; 
    string output_for_outputs=output;
    vector<Event> s = event.events;
    for(Event e:s){
        output_for_outputs +="user:"+users+"\n";
        output_for_outputs +="event name:"+e.get_name()+"\n";
        output_for_outputs += "time:"+e.get_time();
        output_for_outputs +="\n";
        map<string,string> s = e.get_game_updates();
        string activities = "";
        for(auto it=s.begin();it!=s.end();++it){
            activities +="  "+it->first+": "+it->second+"\n";
        }
        output_for_outputs += "general game updates:";
        output_for_outputs += "\n";
        output_for_outputs += activities;
        output_for_outputs += "\n";
        output_for_outputs += "team a updates:";
        string activities2 = "";
        map<string,string> t =e.get_team_a_updates();
                for(auto it=t.begin();it!=t.end();++it){
            activities2 +="  "+it->first+": "+it->second+"\n";
        }
        output_for_outputs += activities2;
        output_for_outputs += "\n";
                output_for_outputs += "team b updates:";
        string activities3 = "";
        map<string,string> r =e.get_team_b_updates();
                for(auto it=r.begin();it!=r.end();++it){
            activities3 +="  "+it->first+": "+it->second+"\n";
        }
        output_for_outputs += activities3;
        output_for_outputs += "\n";
        output_for_outputs += "description:"+e.get_discription();
        output_for_outputs += "\n";
        outputs.push_back(output_for_outputs);
        output_for_outputs = output;
    }
    return outputs;
}
bool StompProtocol::process_answer(string answer){
    return true;
}
