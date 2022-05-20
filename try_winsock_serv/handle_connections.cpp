//
// Created by AS on 4/15/2022.
//
#include "handle_connections.h"
#include "includes.h"
#include <sstream>
#include <vector>
#include <clocale>

std::map<int, SOCKET> SAT;
std::map<int, SOCKET> CLIENT;
int sat_counter = 0;
int client_counter = 0;

/** Condition tracker **/
[[noreturn]] void condition_tracker() {
    int sat_counter_loc = 0;
    int client_counter_loc = 0;
    if (SAT.empty()) std::cout << "No satellites connected.\n";
    if (CLIENT.empty()) std::cout << "No clients connected.\n";
    while(true) {
        if (sat_counter_loc != sat_counter or client_counter_loc != client_counter) {
            sat_counter_loc = sat_counter;
            client_counter_loc = client_counter;
            std::cout << std::string(100, '\n');
            if (SAT.empty()) std::cout << "No satellites connected.\n";
            if (CLIENT.empty()) std::cout << "No clients connected.\n";
            if (!SAT.empty()) {
                std::cout << sat_counter_loc << " Satellites connected:\n";
                for (const auto &satellite: SAT) {
                    std::cout << satellite.first << "\n";
                }
            }
            if (!CLIENT.empty()) {
                std::cout << client_counter_loc << " Clients connected:\n";
                for (const auto &client: CLIENT) {
                    std::cout << client.first << "\n";
                }
            }
        }
    }
}


/** Function used to send Message string to client **/
void send_to_client(const SOCKET& connection, const std::string& msg) {
    int msg_size = msg.size();
    send(connection, (char*)&msg_size, sizeof(int), 0);
    send(connection, msg.c_str(), msg_size, 0);
    Sleep(10);
}

///** Function used to send Message string to satellite **/
//void send_to_satellite(int sat_ID, int command) {
//    if (SAT.find(sat_ID) == SAT.end()) std::cout << "There is no Satellite with this ID or it is not chosen\n";
//    SOCKET sat = SAT.find(sat_ID)->second;
//    send(sat, (char*)&sat_ID, sizeof(int), 0);
//}

/** Function used to get Satellite ID from client + stress test **/
int id_from_client(const SOCKET& client) {
    int next_msg_size;
    char c;
    int i;
    recv(client, (char *) &next_msg_size, sizeof(int), 0);
    char *next_msg = new char[next_msg_size + 1];
    next_msg[next_msg_size] = '\0';
    recv(client, next_msg, next_msg_size, 0);
    if (std::string(next_msg) == "back") {
        send_to_client(client, "You left successfully");
        delete[] next_msg;
        return 0;
    }
    if (std::string(next_msg) == "disconnect") {
        return 999;
    }
    if (std::string(next_msg) != "back") {
        std::stringstream ss(next_msg);
        ss >> i;
        if (ss.fail() or ss.get(c)) {
            send_to_client(client, "Not an integer, try again or type 'back'");
            delete[] next_msg;
            return id_from_client(client);
        }
        if (SAT.find(i) == SAT.end()) {
            send_to_client(client, "There is no such ID, try again or type 'back'");
            delete[] next_msg;
            return id_from_client(client);
        }
    }
    delete[] next_msg;
    return i;
}

//bool impulse_from_client(SOCKET client) {
//    int msg_size;
//    char c;
//    double i;
//    recv(client, (char*)&msg_size, sizeof(int), 0);
//    char* msg = new char[msg_size+1];
//    msg[msg_size] = '\0';
//    recv(client, msg, msg_size, 0);
//    std::string str_msg = std::string(msg);
//    if (str_msg == "back") {
//        send_to_client(client, "You left successfully");
//        delete[] msg;
//        return false;
//    }
//    else {
//        std::vector<std::string> arr;
//        std::string temp;
//        std::stringstream ss(msg);
//        std::string token;
//        while (ss >> token)
//        {
//            arr.push_back(token);
//        }
//        for (const auto& s : arr) {
//            std::stringstream si(s);
//            si >> i;
//            if (si.fail() or si.get(c)) {
//                //message
//                return false;
//            }
//        }
//        std::cout << arr[0] << " " << arr[1] << " " << arr[2] << " ";
//        return true;
//    }
//
//}


/** Function that converts message to command ID. Returns 1 if the command doesn't exist **/
int commands(const char *message) {
    std::string msg(message);
    if (msg == "disconnect") return 0;
    if (msg == "refresh") return 3;
    if (msg == "get coords") return 4;
    if (msg == "broadcast coords") return 5;
    if (msg == "stop broadcast") return 6;
    if (msg == "add impulse") return 7;
    //if (msg == "help") return 8;
    return 1;
}


/** Client handler - receives and sends requests from/to client **/
void ClientHandler(const int& client_ID) {
    SOCKET client = (CLIENT.find(client_ID))->second;
    int msg_size;
    int connected = 1;
    int satellite_id = 0;
    while(connected) {
        recv(client, (char*)&msg_size, sizeof(int), 0);
        char* msg = new char[msg_size + 1];
        //char* msg = new char[11 + 1];
        //std::cout<<std::endl<<msg_size<<std::endl;
        msg[msg_size] = '\0';
        recv(client, msg, msg_size, 0);
        //std::cout << msg << std::endl;
        int command_id = commands(msg);
        switch(command_id) {
            case 0: { //Disconnection check
                connected = 0;
                break;
            }
            case 1: { //No such command error
                send_to_client(client, "Error: there is no such command");
                break;
            }
            case 3: { //Refresh block
                std::stringstream refresh_str;
                refresh_str.setf(std::ios::fixed);
                refresh_str << "ID:\n";
                if (SAT.empty()) {
                    send_to_client(client, "There is no Satellites online right now");
                    break;
                }
                for (auto & satellite : SAT) {
                    refresh_str << satellite.first << "\n";
                    //std::cout << satellite.first << "\n";
                }
                // refresh_str << "Type ID of chosen Satellite (if you want to return, write 'back'):";
                send_to_client(client, refresh_str.str());

                /** Processing next message **/
                satellite_id = id_from_client(client);
                if (satellite_id == 999) {
                    connected = 0;
                    break;
                }
                if (satellite_id == 0) break;
                std::stringstream sat_connect_str;
                refresh_str.setf(std::ios::fixed);
                sat_connect_str << "You have successfully connected to satellite " << satellite_id;
                send_to_client(client, sat_connect_str.str());
                break;
            }
            case 4: { // Get coords
                if (satellite_id == 0) send_to_client(client, "Satellite is not chosen. Connect to Satellite and try again");
                auto sat_ = SAT.find(satellite_id)->second;
                send(sat_, (char*)&client_ID, sizeof(int), 0);
                send(sat_, (char*)&command_id, sizeof(int), 0);
                break;
            }
            case 5: { // Get coords continuously
                if (satellite_id == 0) send_to_client(client, "Satellite is not chosen. Connect to Satellite and try again");
                auto sat_ = SAT.find(satellite_id)->second;
                send(sat_, (char*)&client_ID, sizeof(int), 0);
                send(sat_, (char*)&command_id, sizeof(int), 0);
                break;
            }
            case 6: { // Stop getting coords continuously
                if (satellite_id == 0) send_to_client(client, "Satellite is not chosen. Connect to Satellite and try again");
                auto sat_ = SAT.find(satellite_id)->second;
                send(sat_, (char*)&client_ID, sizeof(int), 0);
                send(sat_, (char*)&command_id, sizeof(int), 0);
                break;
            }
            case 7: {
                if (satellite_id == 0) send_to_client(client, "Satellite is not chosen. Connect to Satellite and try again");
                auto sat_ = SAT.find(satellite_id)->second;
                send_to_client(client, "Beta: enter impulse in 'px py pz' format:");
                int nmsg_size;
                recv(client, (char*)&nmsg_size, sizeof(int), 0);
                char* nmsg = new char[nmsg_size+1];
                nmsg[nmsg_size] = '\0';
                recv(client, nmsg, nmsg_size, 0);

                send(sat_, (char*)&client_ID, sizeof(int), 0);
                send(sat_, (char*)&command_id, sizeof(int), 0);

                send(sat_, (char*)&nmsg_size, sizeof(int), 0);
                send(sat_, nmsg, nmsg_size, 0);

                send_to_client(client, "Adding impulse to Satellite");
                delete[] nmsg;
                break;
            }
            //case 8: { //Available commands
            //    send_to_client(client, "help - Get available commands\nrefresh - Get information about Satellites online, connect to them\ndisconnect - Disconnect from server\nget coords - Get Satellite coordinates\nbroadcast coords - Broadcast Satellite coordinates\nstop broadcast - Stop broadcasting coordinates\nadd impulse - Transmit impulse to Satellite\n");
            //    break;
            //}
            default: {
                break;
            }
        }
        delete[] msg;
    }
    //std::cout << "Successfully disconnected" << std::endl;
    CLIENT.erase(client_ID);
    client_counter -= 1;
    Sleep(100);
}

/** Satellite handler - receives and sends requests from/to satellite **/
[[noreturn]] void SatHandler(const int& sat_ID) {
    SOCKET sat_ = SAT.find(sat_ID)->second;
    int msg_size;
    int connected = 1;
    int client_ID;
    while(connected) {
        //Asks for client ID
        recv(sat_, (char*)&client_ID, sizeof(int), 0);
        //std::cout << "Got client_ID: " << client_ID << "\n";
        /**Getting message from satellite**/
        recv(sat_, (char*)&msg_size, sizeof(int), 0);
        //std::cout << "Got msg_size: " << msg_size << "\n";
        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';

        recv(sat_, msg, msg_size, 0);
        //std::cout << "Got msg: " << std::string(msg) << "\n";
        //Sends message from satellite to client
        send_to_client(CLIENT.find(client_ID)->second, std::string(msg));
        //std::cout << "Sent msg to client\n";
        delete[] msg;
    }
    --sat_counter;
}

/** Connects new unit to the server, distinguishes if it is a client or satellite **/
[[noreturn]] void NewClients(const SOCKET& sListen, const SOCKADDR_IN& addr){
    SOCKET newConnection;
    int sizeofaddr = sizeof(addr);

    std::thread counter(condition_tracker);
    counter.detach();

    while(true){
        newConnection = accept(sListen, (SOCKADDR *) &addr, &sizeofaddr);

        if (newConnection == 0) {
            std::cout << "Error #2\n";
        }

        else {
            char connection_type[2] ;
            int unit_ID;

            //receive of connection type:
            // 0 = sat, 1 = client (0 or 1 are sent in a msg from client bellow)
            recv(newConnection, connection_type, sizeof(connection_type), 0);
            recv(newConnection, (char*)&unit_ID, sizeof(int), 0);
            //std::cout << "connection type is: " << connection_type[0] << std::endl;
            //std::cout << "unit id is: " << unit_ID << std::endl;

            //depends on connection type we create a new SatHandler/ClientHandler thread:

            if (connection_type[0] == '0'){
                //stress-case feature - not done yet (after 50 connection server should be considered as over-lasted)
                SAT[unit_ID] = newConnection;
                ++sat_counter;
                //std::cout<<"Sat connected successfully " << std::endl;
                std::thread newThread(SatHandler, unit_ID);
                newThread.detach();
            }

            else{
                CLIENT[unit_ID] = newConnection;
                ++client_counter;
                //std::cout<<"Client connected successfully"<<std::endl;
                std::thread newThread(ClientHandler, unit_ID);
                newThread.detach();
            }
        }
    }
}
