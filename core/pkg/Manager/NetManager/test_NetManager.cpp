#include <NetManager/test_NetManager.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;
NetManager *nn;
void *run_serv(void *args)
{
    nn->service_nmdns(args);
    return 0;
}
void *run_accept(void *args)
{
    nn->service_accept(args);
    return 0;
}
void *run_query(void *args)
{
    nn->discovery();
    return 0;
}
void *run_daemon(void *args)
{
    nn->deamon();
    return 0;
}

void *run_sender(void *args)
{
    nn->sender(args);
    return 0;
}

bool test_net_manager()
{
    nn = new NetManager();
    pthread_t tids[5];
    //run_serv();
    // test accept and mdns
    // pthread_create(&tids[0], NULL, run_serv, NULL);
    // pthread_create(&tids[1], NULL, run_accept, NULL);
    //test query
    // pthread_create(&tids[2], NULL, run_query, NULL);
    //test deamon

    int mode = 1;
    //deamon
    if (mode == 0)
    {
        pthread_create(&tids[3], NULL, run_daemon, NULL);
    }
    if (mode == 1)
    {
        pthread_create(&tids[4], NULL, run_sender, NULL);
    }

    while (1)
        sleep(100);
    cout << "test: NetManager test finished" << endl;
    return true;
}