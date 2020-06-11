#include <UnionManager/test_union.h>
#include <pthread.h>
#include <iostream>
using namespace std;
UnionManager un(1);
void *test_union_callback(void *args)
{
    un.service_union(args);
}
bool test_union()
{
    pthread_t thread;
    // pthread_create(&thread, NULL, test_union_callback, NULL);
    // while (1)
    //     ;
    cout << "test: union test finished" << endl;
    return true;
}