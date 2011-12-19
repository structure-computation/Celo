#include <Celo/StringHelp.h>
#include <Celo/EventLoop.h>
#include <Celo/Signal.h>
#include <Celo/Timer.h>
#include <signal.h>
#include <iostream>

struct MyObserver {
    bool timeout() {
        std::cout << "pouet" << std::endl;
        return false;
    }
    bool signal( int sig ) {
        std::cout << "sig " << sig << std::endl;
        return true;
    }
};


int main() {
    EventLoop el;
    MyObserver mo;

    int sigs[] = { SIGINT, SIGQUIT, SIGKILL, -1 };
    el << new Signal<MyObserver>( &mo, sigs );
    
    el << new Timer<MyObserver>( &mo, 1 );

    return el.run();
}
