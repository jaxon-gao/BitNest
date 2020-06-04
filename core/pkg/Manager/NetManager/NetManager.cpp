#include <NetManager/NetManager.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h> /* socket类定义需要*/
#include <sys/epoll.h>  /* epoll头文件 */
#include <fcntl.h>      /* nonblocking需要 */
#include <sys/resource.h>
#include <config.h>
#include <sstream>
#include <map>
#include <ECC/uECC.h>
#include <string>
#include <hash/sha256.h>
#include <mdns/mdns.h>
#include <msg/msg.h>
#include <tools/net_tool.h>
#include <DHTManager/DHTManager.h>
#include <iomanip>
using namespace std;
#define MAXEPOLL 10000 /* 对于服务器来说，这个值可以很大的！ */
#define MAXLINE 1024
#define PORT 6000
#define MAXBACK 1000
#define FILE_PORT 6001
NetManager::NetManager(PeerInfo *seed)
{
    //创建epoll
    epoll_acc_fd = epoll_create(MAXEPOLL); //!> create
    epoll_out = epoll_create(MAXEPOLL);    //!> create
    epoll_in = epoll_create(MAXEPOLL);     //!> create
    //生成私钥
    keys = new KeyPair();
    uint8_t SK[32];
    uint8_t PK[64];
    uECC_make_key(PK, SK, uECC_secp256k1());
    stringstream ss;
    for (int i = 0; i < 32; i++)
    {
        ss << hex << SK[i];
    }
    string num;
    ss >> num;
    keys->sk = uint256(num);
    ss.clear();
    for (int i = 0; i < 32; i++)
    {
        ss << hex << PK[i];
    }
    ss >> num;
    keys->pk[0] = uint256(num);

    ss.clear();
    for (int i = 0; i < 32; i++)
    {
        ss << hex << PK[i + 32];
    }
    ss >> num;
    keys->pk[1] = uint256(num);
    uint8_t hash_output[32];
    uint8_t hash_input[96];
    memcpy(hash_input, SK, 32);
    memcpy(hash_input + 32, PK, 64);
    //计算本节点哈希主机名
    sha256_t hash_cnt;
    sha256_init(&hash_cnt);
    sha256_update(&hash_cnt, (unsigned char *)hash_input, 96);
    sha256_final(&hash_cnt, hash_output);
    ss.clear();
    for (int i = 0; i < 32; i++)
    {
        ss << hex << hash_output[i];
    }
    ss >> hostname;
    my = new uint256(hostname);
    root_path += "/" + hostname;
    string cmd = "mkdir -p " + root_path;
    system(cmd.c_str());
    DHT = new DHTManager(*my, epoll_in);
    Union = new UnionManager(epoll_in);
}
//种子功能未使用，两个函数函数体相同
NetManager::NetManager()
{
    keys = new KeyPair();
    uint8_t SK[32];
    uint8_t PK[64];
    uECC_make_key(PK, SK, uECC_secp256k1());
    string num;
    stringstream ss;
    for (int i = 0; i < 32; i++)
    {
        ss << setw(2) << setfill('0') << hex << (int)SK[i];
    }
    ss >> num;
    keys->sk = uint256(num);
    ss.clear();
    for (int i = 0; i < 32; i++)
    {
        ss << setw(2) << setfill('0') << hex << (int)PK[i];
    }
    ss >> num;
    keys->pk[0] = uint256(num);

    ss.clear();
    for (int i = 0; i < 32; i++)
    {
        ss << setw(2) << setfill('0') << hex << (int)PK[i + 32];
    }
    ss >> num;
    keys->pk[1] = uint256(num);

    uint8_t hash_output[32];

    uint8_t hash_input[96];
    memcpy(hash_input, SK, 32);
    memcpy(hash_input + 32, PK, 64);

    sha256_t hash_cnt;
    sha256_init(&hash_cnt);
    sha256_update(&hash_cnt, (unsigned char *)hash_input, 96);
    sha256_final(&hash_cnt, hash_output);

    stringstream sn;
    for (int i = 0; i < 32; i++)
    {
        sn << setw(2) << setfill('0') << hex << (unsigned int)hash_output[i];
    }
    sn >> hostname;
    int index = 0;
    my = new uint256(hostname);
    root_path += "/" + hostname;
    string cmd = "mkdir -p " + root_path;
    system(cmd.c_str());

    DHT = new DHTManager(*my);
}
//发送PBFT请求
bool NetManager::sendPBFT(string msg)
{
}
//收包线程
void *NetManager::reciver(void *args)
{
    msg_header m;
    int listener = listen_fd;
    while (1)
    {
        recv_header(listener, m);
        switch (m.kind)
        {
        case DHT_FIND_NODE:
            break;
        case NK_PBFT_PRE:
            break;
        case NK_PBFT_REQ:
            break;
        case NK_PBFT_RES:
            break;
        case NK_PBFT_COM:
            break;
        case NK_PBFT_PPR:
            break;

        default:
            break;
        }
    }
}
/*
    from hash.__BitNest._tcp.local 
    get hash host name
    */
string get_Host_Name(string domainname)
{
    for (int i = 0; i < domainname.size(); i++)
    {
        if (domainname[i] == '.')
        {
            return domainname.substr(0, i);
        }
    }
}
/*
    from "A xxx.xxx.xxx.xxx"
    get IPV4 address "xxx.xxx.xxx.xxx" 
    */
string get_IPV4(string ret)
{
    return ret.substr(2, ret.size() - 2);
}
/*
    from "AAAA xxxxxxx"
    get IPV6 address "xxxxxxx" 
    */
string get_IPV6(string ret)
{
    return ret.substr(5, ret.size() - 5);
}
//启动发现
void NetManager::discovery()
{
    int ret;
    int service_port = 42424;
    vector<string> ans;
    reciveSequence = &ans;
    ret = send_mdns_query(service.c_str());
#ifdef _WIN32
    WSACleanup();
#endif
    string hash_str;

    PeerInfo *aim;
    map<uint256, string> findings;
    int sockfd;
    for (int i = 0; i < ans.size(); i++)
    {
        if (ans[i][0] == 'A')
        {
            continue;
        }
        hash_str = get_Host_Name(ans[i]);

        if (hash_str == hostname)
        {
            continue;
        }
        uint256 hash_num(hash_str);
        aim = new PeerInfo(hash_num, 0);
        if (DHT->OldNode(aim))
        {
            continue;
        }
        else
        {
            i++;
            string IP4_str = get_IPV4(ans[i]);
            findings[hash_num] = IP4_str;
        }
    }
    map<uint256, string>::iterator iter = findings.begin();
    while (iter != findings.end())
    {
        uint256 hash_num(iter->first);
        aim = new PeerInfo(hash_num, 0);
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        setreuseaddr(sockfd);

        aim->fd(sockfd);
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(iter->second.c_str());
        addr.sin_port = htons(PORT);

        int ret = connect(sockfd, (sockaddr *)&addr, sizeof(sockaddr));
        cout << "connect:" << hex << iter->first << endl;
        msg_header h;
        h.pk[0] = keys->pk[0];
        h.pk[1] = keys->pk[1];
        h.kind = DHT_CONNECT;
        h.msg_size = 0;
        send_msg(sockfd, h, hostname);
        usleep(100);
        recv_header(sockfd, h);
        if (h.kind == DHT_CONNECT)
        {
            aim->pk[0] = h.pk[0];
            aim->pk[1] = h.pk[1];
            DHT->AddNode(aim);
        }
        iter++;
    }
}
//使用种子对节点初始化
void NetManager::discovery(PeerInfo *p)
{
}
void *NetManager::service_accept(void *arg)
{
    msg_header header;
    int conn_fd, nread;
    int wait_fds; //!> epoll_wait 的返回值
    int i;
    char kind;
    string buffer;
    PeerInfo *connectPeer;
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    struct epoll_event ev;
    struct epoll_event evs[MAXEPOLL];
    struct rlimit rlt; //!> 设置连接数所需
    char buf[MAXLINE];
    socklen_t len = sizeof(struct sockaddr_in);

    //!> 设置每个进程允许打开的最大文件数
    //!> 每个主机是不一样的哦，一般服务器应该很大吧！
    //!>
    rlt.rlim_max = rlt.rlim_cur = MAXEPOLL;
    if (setrlimit(RLIMIT_NOFILE, &rlt) == -1)
    {
        printf("Setrlimit Error : %d\n", errno);
        exit(EXIT_FAILURE);
    }

    //!> server 套接口
    //!>
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    //!> 建立套接字
    if ((NetManager::listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Socket Error...\n", errno);
        exit(EXIT_FAILURE);
    }

    //!> 设置非阻塞模式
    //!>
    if (setnonblocking(NetManager::listen_fd) == -1)
    {
        printf("Setnonblocking Error : %d\n", errno);
        exit(EXIT_FAILURE);
    }

    //!> 绑定
    //!>
    if (bind(NetManager::listen_fd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) == -1)
    {
        printf("Bind Error : %d\n", errno);
        exit(EXIT_FAILURE);
    }

    //!> 监听
    //!>
    if (listen(NetManager::listen_fd, MAXBACK) == -1)
    {
        printf("Listen Error : %d\n", errno);
        exit(EXIT_FAILURE);
    }

    //!> 创建epoll
    //!>

    ev.events = EPOLLIN | EPOLLET;      //!> accept Read!
    ev.data.fd = NetManager::listen_fd; //!> 将listen_fd 加入
    if (epoll_ctl(NetManager::epoll_acc_fd, EPOLL_CTL_ADD, listen_fd, &ev) < 0)
    {
        printf("Epoll Error : %d\n", errno);
        exit(EXIT_FAILURE);
    }
    NetManager::acc_cur_fds = 1;

    while (1)
    {
        if ((wait_fds = epoll_wait(NetManager::epoll_acc_fd, evs, acc_cur_fds, -1)) == -1)
        {
            printf("Epoll Wait Error : %d\n", errno);
            exit(EXIT_FAILURE);
        }
        for (i = 0; i < wait_fds; i++)
        {
            if (evs[i].data.fd == listen_fd && acc_cur_fds < MAXEPOLL)
            {
                if ((conn_fd = accept(listen_fd, (struct sockaddr *)&cliaddr, &len)) == -1)
                {
                    printf("Accept Error : %d\n", errno);
                    exit(EXIT_FAILURE);
                }
                ev.events = EPOLLIN | EPOLLET; //!> accept Read!
                ev.data.fd = conn_fd;          //!> 将conn_fd 加入
                if (epoll_ctl(NetManager::epoll_acc_fd, EPOLL_CTL_ADD, conn_fd, &ev) < 0)
                {
                    printf("Epoll Error : %d\n", errno);
                    exit(EXIT_FAILURE);
                }
                ++acc_cur_fds;
                continue;
            }
            //recv_header中没有处理错误
            kind = recv_header(evs[i].data.fd, header);

            recv_content(evs[i].data.fd, buffer);

            if (kind == DHT_CONNECT)
            {
                uint256 PeerHash = uint256(buffer);
                connectPeer = new PeerInfo(PeerHash, evs[i].data.fd);
                connectPeer->pk[0] = header.pk[0];
                connectPeer->pk[1] = header.pk[1];
                header.pk[0] = NetManager::keys->pk[0];
                header.pk[1] = NetManager::keys->pk[1];
                header.kind = DHT_CONNECT;
                header.msg_size = 0;
                send_msg(evs[i].data.fd, header, "");
                DHT->AddNode(connectPeer);
                cout << "new connection:" << hex << PeerHash << endl;
                //增加监听限制。
                //目前的监听限制在DHT和Union中保护
                epoll_ctl(NetManager::epoll_in, EPOLL_CTL_ADD, evs[i].data.fd, &ev); //!> 计入FD

                epoll_ctl(NetManager::epoll_acc_fd, EPOLL_CTL_DEL, evs[i].data.fd, &ev); //!> 删除计入的fd
                --acc_cur_fds;
            }
        }
    }
    close(listen_fd);
}

void *NetManager::service_nmdns(void *arg)
{
    int service_port = 42424;
    int ret;
    ret = service_mdns(hostname.c_str(), service.c_str(), service_port);
}

NetManager *callback_n;
void *union_callback(void *arg)
{
    callback_n->service_union(arg);
}
void *net_callback(void *arg)
{
    callback_n->service_net(arg);
}
void *mdns_callback(void *arg)
{
    callback_n->service_nmdns(arg);
}
void *accept_callback(void *arg)
{
    callback_n->service_accept(arg);
}
void *reciever_callback(void *arg)
{
    callback_n->service_accept(arg);
}

void NetManager::deamon()
{
    callback_n = this;
    pthread_t threads[5];
    pthread_create(&threads[0], 0, union_callback, NULL);
    pthread_create(&threads[1], 0, net_callback, NULL);
    pthread_create(&threads[2], 0, mdns_callback, NULL);
    pthread_create(&threads[3], 0, accept_callback, NULL);
    pthread_create(&threads[4], 0, reciever_callback, NULL);
    while (1)
        ;
}

void *NetManager::service_net(void *args)
{
    while (1)
    {
        sleep(1);
        if (Union->isUnionLeader())
        {
            publicBlock *b = Blocks->packUp_u();
            //发送联盟区块。
            Emit(b);
        }
        if (time(0) % 45 == 5)
        {
            //sync_contract();
            innerBlock *b = Blocks->packUp();
            Emit(b);
        }
    }
}

void *NetManager::AddEventEmit(void *arg)
{
    struct epoll_event ev;
    ev.events = EPOLLOUT || EPOLLET;
    int *args = (int *)arg;
    int epoll = args[0];

    //优先发送联盟全体
    vector<PeerInfo *> ran = Union->getAll();
    for (int i = 0; i < ran.size(); i++)
    {
        ev.data.fd = ran[i]->fd();
        lock_p(lock_out);
        if (epoll_ctl(epoll, EPOLL_CTL_ADD, listen_fd, &ev) < 0)
        {
            printf("Epoll Error : %d\n", errno);
            exit(EXIT_FAILURE);
        }
        ++out_cur_fds;
        lock_v(lock_out);
    }

    //选择随机节点发送验证
    ran = DHT->GetRandomNode();
    for (int i = 0; i < ran.size(); i++)
    {
        ev.data.fd = ran[i]->fd();
        lock_p(lock_out);
        if (epoll_ctl(epoll, EPOLL_CTL_ADD, listen_fd, &ev) < 0)
        {
            printf("Epoll Error : %d\n", errno);
            exit(EXIT_FAILURE);
        }
        ++out_cur_fds;
        lock_v(lock_out);
    }
}
void *callback_add(void *arg)
{
    callback_n->AddEventEmit(arg);
}
void NetManager::Emit(msg_header &h, string msg)
{
    struct epoll_event ev;
    struct epoll_event evs[MAXEPOLL];
    ev.events = EPOLLOUT || EPOLLET;
    out_cur_fds = 0;
    int epoll_emit = epoll_create(MAXEPOLL);
    int args[2];
    pthread_t add;
    args[0] = epoll_emit;
    pthread_create(&add, 0, callback_add, (void *)args);
    time_t _1 = time(0);
    time_t _2 = time(0);
    int wait_fds;
    while (1)
    {
        if ((wait_fds = epoll_wait(epoll_emit, evs, acc_cur_fds, -1)) == -1)
        {
            printf("Epoll Wait Error : %d\n", errno);
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < wait_fds; i++)
        {
            send_msg(evs[i].data.fd, h, msg);

            lock_p(lock_out);
            epoll_ctl(epoll_emit, EPOLL_CTL_DEL, evs[i].data.fd, &ev);
            out_cur_fds--;
            lock_v(lock_out);
        }
        _2 = time(0);
        //两秒后结束。
        if (_2 - _1 == 2)
        {
            break;
        }
    }
}

void NetManager::Emit(PoST *p)
{
    string data = p->data();
    msg_header h;
    h.kind = NK_SEND_POST;
    h.pk[0] = keys->pk[0];
    h.pk[1] = keys->pk[1];
    Emit(h, data);
}
void NetManager::Emit(publicBlock *b)
{
    string data = b->data();
    msg_header h;
    h.kind = NK_SEND_PB;
    h.pk[0] = keys->pk[0];
    h.pk[1] = keys->pk[1];
    Emit(h, data);
}
void NetManager::Emit(innerBlock *b)
{
    string data = b->data();
    msg_header h;
    h.kind = NK_SEND_IB;
    h.pk[0] = keys->pk[0];
    h.pk[1] = keys->pk[1];
    Emit(h, data);
}
void NetManager::Emit(BackUp *c)
{
    string data = c->data();
    msg_header h;
    h.kind = NK_SEND_BAK;
    h.pk[0] = keys->pk[0];
    h.pk[1] = keys->pk[1];
    Emit(h, data);
}
void NetManager::Emit(payment *c)
{
    string data = c->data();
    msg_header h;
    h.kind = NK_SEND_PAY;
    h.pk[0] = keys->pk[0];
    h.pk[1] = keys->pk[1];
    Emit(h, data);
}
void NetManager::Emit(Storage *c)
{
    string data = c->data();
    msg_header h;
    h.kind = NK_SEND_STO;
    h.pk[0] = keys->pk[0];
    h.pk[1] = keys->pk[1];
    Emit(h, data);
}
stirng get_ip_from_fd(int fd)
{
    struct sockaddr_in sa;
    int len = sizeof(sa);
    string ans = "";
    if (!getpeername(sockfd, (struct sockaddr *)&sa, &len))
    {
        ans = string(inet_ntoa(sa.sin_addr))
    }
    return ans;
}
void *NetManager::service_union(void *args)
{
    Union->service_union(args);
}

StorageData *NetManager::FileReciever(PeerInfo *p)
{
    string ip = get_ip_from_fd(p->fd());
    if (ip.size() == 0)
    {
        return NULL;
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    int cli_sock;
    sockaddr_in serv_addr;
    sockaddr_in cli_addr;
    socklen_t len = sizeof(cli_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(FILE_PORT);
    serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    bind(sock, (sockaddr *)&serv_addr, sizeof(sockaddr));
    listen(sock, 3);
    cli_sock = accept(sock, (sockaddr *)&cli_addr, &len);
    //接收文件
    return recv_file(cli_sock);
}
void NetManager::FileSender(PeerInfo *p, const StorageData &file)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    int ser_sock;
    sockaddr_in my_addr;
    sockaddr_in ser_addr;
    socklen_t len = sizeof(cli_addr);
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(FILE_PORT);
    my_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    ser_sock = connect(sock, (sockaddr *)&ser_addr, &len);
    //上传文件
    send_file(sock, file);
}