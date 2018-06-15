#include "SQLConnection.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include "builder.cpp"

using namespace std;

int main() {
    string command = "";
    while(command != "exit") {

        // cout << ": ";
        cin >> command;
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
