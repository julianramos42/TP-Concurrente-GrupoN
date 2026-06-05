#include <iostream>
#include <thread>
#include <cstdlib>
#include <ctime>

#include "productor.h"

using namespace std;

int main()
{
    srand(time(NULL));

    thread t1(productor);
    thread t2(productor);
    thread t3(productor);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
