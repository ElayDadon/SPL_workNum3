#include <stdlib.h>
#include "../include/ConnectionHandler.h"
#include <thread>
#include "../include/data.h"
#include <sstream>
#include <vector>
#include "../include/StompProtocol.h"
//map<string, std::map<string, Game>>
int main(int argc, char* argv[])
{
    // TODO: implement the STOMP client
    bool logged = false;
    StompProtocol protocol;
    ConnectionHandler* connectionHandler;
    while (1) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        int len = line.length();
        std::stringstream ss(line);
        std::vector<std::string> words_by_spaces;
        std::string curr_word;
        while (getline(ss, curr_word, ' ')) {
            words_by_spaces.push_back(curr_word);
        }
        //words_by_spaces=<login,1.1.1.1:2000,menu,films> example
        if (words_by_spaces[0] == "login") {
            if (logged)
                cout << "The client is already logged in, log out before trying again" << std::endl;
            else {
                std::string ip_port = words_by_spaces[1];
                int dotdot = ip_port.find(':');
                std::string ip = ip_port.substr(0, dotdot);
                std::string port_instring = ip_port.substr(dotdot + 1);
                short port;
                try {
                    port = std::stoi(port_instring);
                }
                catch (std::exception& e) {
                    std::cout << "Error: port is not a number" << std::endl;
                }
                connectionHandler = new ConnectionHandler(ip, port);
                if (!connectionHandler->connect()) {
                    std::cerr << "Cannot connect to " << ip << ":" << port << std::endl;
                    continue;
                }
                std::string frame = protocol.parserLogin(words_by_spaces[2], words_by_spaces[3]);
                if (!connectionHandler->sendFrameAscii(frame, '\0')) {
                    std::cout << "failed to send, try again in a couple minutes" << std::endl;
                    continue;
                }
                std::string answer;

                if (!connectionHandler->getFrameAscii(answer, '\0')) {
                    std::cout << "failed to recieve" << std::endl;
                    continue;
                }
                logged = protocol.process_answer(answer);
            }
        }

        else {
            //logout
            //
            if (words_by_spaces[0] == "logout") {
                if (!logged)
                    cout << "you have to be logged in order to use this command: " << words_by_spaces[0] << std::endl;
                else {
                    //protocol.parserLogout -
                    string logout_frame = protocol.parserLogout();
                    if (!connectionHandler->sendFrameAscii(logout_frame, '\0')) {
                        std::cout << "failed to send, try again in a couple minutes" << std::endl;
                        continue;
                    }
                    std::string answer;

                    if (!connectionHandler->getFrameAscii(answer, '\0')) {
                        std::cout << "failed to recieve" << std::endl;
                        continue;
                    }
                    logged = protocol.process_answer(answer);
                }
            }
            //join germany_spain
            else if (words_by_spaces[0] == "join") {
                if (!logged)
                    cout << "you have to be logged in order to use this command: " << words_by_spaces[0] << std::endl;
                else {
                    string Join_parsed = protocol.parserJoin(words_by_spaces[1]);
                    if (!Join_parsed.empty()) {
                        if (!connectionHandler->sendFrameAscii(Join_parsed, '\0')) {
                            std::cout << "failed to send, try again in a couple minutes" << std::endl;
                            continue;
                        }
                        std::string answer;

                        if (!connectionHandler->getFrameAscii(Join_parsed, '\0')) {
                            std::cout << "failed to recieve" << std::endl;
                            continue;
                        }
						
                        logged = protocol.process_answer(answer);
                    }
                }
            }
            else if (words_by_spaces[0] == "exit") {
                if (!logged)
                    cout << "you have to be logged in order to use this command: " << words_by_spaces[0] << std::endl;
                else {
                                
                }
            }
            else if (words_by_spaces[0] == "report") {
                if (!logged)
                    cout << "you have to be logged in order to use this command: " << words_by_spaces[0] << std::endl;
                else {
                }
            }
            else if (words_by_spaces[0] == "summary") {
                if (!logged)
                    cout << "you have to be logged in order to use this command: " << words_by_spaces[0] << std::endl;
                else {
                }
            }
            else {
                std::cout << "invalid command was given: " + words_by_spaces[0] + ", expected values are:login, logout, join, exit, report, summary " << std::endl;
            }
        }
    }
    return 0;
}