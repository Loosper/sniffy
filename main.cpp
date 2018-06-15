#include "SQLConnection.h"

#include <stdio.h>
#include <unistd.h>
#include <iomanip>
#include <iostream>
#include <sys/types.h>
#include <signal.h>

#include "builder.cpp"

using namespace std;

int main() {
    string command = "";
    cout << "\e[1;32m\
                       _  __  __       \n \
                     (_)/ _|/ _|      \n \
            ___ _ __  _| |_| |_ _   _ \n \
           / __| '_ \\| |  _|  _| | | |\n \
           \\_  \\ | | | | | | | | |_| |\n \
           |___/_| |_|_|_| |_|  \\__, |\n \
                                 __/ |\n \
                                |___/  \n";

    cout << "COMMANDS: \n";
    cout << setw(15) << std::left << "sniff:" << "start sniffing network packets\n";
    cout << setw(15) << std::left << "mac_address:" << "view recorded MAC addresses\n";
    cout << setw(15) << std::left << "ipv4_address:" << "view recorded IP addresses\n";
    cout << setw(15) << std::left << "frame: view" << "recorded Ethernet frames\n";
    cout << setw(15) << std::left << "ipv4_packet:" << "view recorded IP packets\n";
    cout << setw(15) << std::left << "arp_cache:" << "view recorded ARP caches\n";
    cout <<endl;

    while(command != "exit") {

        // cout << ": ";
        cin >> command;
        cout << endl;
        if (cin.eof())
            break;
        
        pid_t pid = fork();

        if(pid == 0){
            if(command == "build"){
                Builder builder = Builder();
                builder.run();
            } else if(command == "sniff"){
                Sniffer snif = Sniffer();
                snif.run();
            } else if( command == "mac_address" || command == "ipv4_address" || command == "frame" || command == "ipv4_packet" || command == "arp_cache"){
                Selector selector = Selector(command);
                selector.run();
            } else if( command != "exit"){
                cout << "E: Unknown command" << endl;
            }
            exit(0);
        } else if (pid > 0){
            if(command == "snif exit"){
                kill(pid, SIGKILL);
            } else if (command == "exit"){
                exit(0);
            }
        } else {
            perror("fork");
            exit(0);
        }

    }
    return 0;
}
