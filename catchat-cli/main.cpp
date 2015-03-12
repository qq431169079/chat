#include <ncurses.h>
#include <catchat/catchat.hpp>
#include <catchat/identity.hpp>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

using std::cout;
using std::endl;


int main(int argc, char ** argv)
{
    initscr();
    printw(argv[0]);
    refresh();
    if (argc != 2) {
        endwin();
        cout << "Usage: " << argv[0] << " <port>" << endl;
        return 1;
    }

    catchat::catchat init;
    init.read_config();

    std::stringstream ss(argv[1]);

    uint16_t port;
    ss >> port;

    if (ss.fail()) {
        endwin();
        cout << "Unable to parse port" << endl;
        return 1;
    }

    init.dht_port(port);

    init.dht_start();

    // An example of how to add another node
    if (init.dht_port() != 12876) {             // If we don't have the default port,
        printw("We gon ping!");             // we're gonna do the pinging.

        struct sockaddr_in in;                  // Construct the remote address
        in.sin_family = AF_INET;
        in.sin_port = htons(12876);             // Port of other instance
        in.sin_addr.s_addr = htonl(0xc0a8017f); // 192.168.1.127 aka my ip address
        init.dht_ping_node((struct sockaddr*)&in, sizeof(in));
    }

    //Function to add some number of identities
    catchat::identity id(init);
    id.credentials("user", "pass");
    id.start();

    printw("COMPLETED THE FUN");
    refresh();
    sleep(10);

    init.dht_stop();
    endwin();
    return 0;
}
