#include <catchat/catchat.hpp>
#include <iostream>
using std::cout;


int main(int argc, char *argv[])
{
    if (argc != 0) {
        cout<<"usage: "<<argv[0]<<"\n";
        return 1;
    }
    catchat::catchat init;
    catchat::node node;
    node.run_once();
    return 0;
}
