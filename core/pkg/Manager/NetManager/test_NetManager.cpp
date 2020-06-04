#include<NetManager/test_NetManager.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;
NetManager nn;
void* run_serv(void *args)
{
    nn.service_nmdns(args);
    return 0;
}
void* run_accept(void *args)
{
    nn.service_accept(args);
    return 0;
}
void* run_query(void *args)
{
    nn.discovery();
    return 0;
}
bool test_net_manager(){
    NetManager kk();
    pthread_t tids[3];
    //run_serv();
    // test accept and mdns
    //  pthread_create(&tids[0],NULL,run_serv,NULL);
    //  pthread_create(&tids[0],NULL,run_accept,NULL);
    //test query
    //pthread_create(&tids[0],NULL,run_query,NULL);
    //while(1);
    cout<<"test: NetManager test finished"<<endl;
    return true;
}