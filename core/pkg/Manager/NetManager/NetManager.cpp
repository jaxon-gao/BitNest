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
  DHT = new DHTManager(*my);
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
  int listener = listen_serve();
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
void NetManager::discovery(PeerInfo *p)
{
}
//P2P服务
void NetManager::service_accept()
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
  if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    printf("Socket Error...\n", errno);
    exit(EXIT_FAILURE);
  }

  //!> 设置非阻塞模式
  //!>
  if (setnonblocking(listen_fd) == -1)
  {
    printf("Setnonblocking Error : %d\n", errno);
    exit(EXIT_FAILURE);
  }

  //!> 绑定
  //!>
  if (bind(listen_fd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) == -1)
  {
    printf("Bind Error : %d\n", errno);
    exit(EXIT_FAILURE);
  }

  //!> 监听
  //!>
  if (listen(listen_fd, MAXBACK) == -1)
  {
    printf("Listen Error : %d\n", errno);
    exit(EXIT_FAILURE);
  }

  //!> 创建epoll
  //!>

  ev.events = EPOLLIN | EPOLLET; //!> accept Read!
  ev.data.fd = listen_fd;        //!> 将listen_fd 加入
  if (epoll_ctl(epoll_acc_fd, EPOLL_CTL_ADD, listen_fd, &ev) < 0)
  {
    printf("Epoll Error : %d\n", errno);
    exit(EXIT_FAILURE);
  }
  acc_cur_fds = 1;

  while (1)
  {
    if ((wait_fds = epoll_wait(epoll_acc_fd, evs, acc_cur_fds, -1)) == -1)
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
        if (epoll_ctl(epoll_acc_fd, EPOLL_CTL_ADD, conn_fd, &ev) < 0)
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
        header.pk[0] = keys->pk[0];
        header.pk[1] = keys->pk[1];
        header.kind = DHT_CONNECT;
        header.msg_size = 0;
        send_msg(evs[i].data.fd, header, "");
        DHT->AddNode(connectPeer);
        cout << "new connection:" << hex << PeerHash << endl;
        epoll_ctl(epoll_acc_fd, EPOLL_CTL_DEL, evs[i].data.fd, &ev); //!> 删除计入的fd
        --acc_cur_fds;
      }
    }
  }
  close(listen_fd);
}

void NetManager::service_nmdns()
{
  int service_port = 42424;
  int ret;
#ifdef _WIN32
  WORD versionWanted = MAKEWORD(1, 1);
  WSADATA wsaData;
  if (WSAStartup(versionWanted, &wsaData))
  {
    printf("Failed to initialize WinSock\n");
    return -1;
  }
#endif
  ret = service_mdns(hostname.c_str(), service.c_str(), service_port);
#ifdef _WIN32
  WSACleanup();
#endif
  return;
}

void NetManager::deamon()
{
  pthread_t UServe;
  //开启联盟选举线程
  pthread_create(&UServe, 0, Union->service_union, NULL);
  //网络管理器线程
  pthread_create(&UServe, 0, this->service_net, NULL);
  //接受连接线程
  pthread_create(&UServe, 0, this->service_accept, NULL);
  //多播DNS线程
  pthread_create(&UServe, 0, this->service_nmdns, NULL);
  //接收处理消息
  pthread_create(&UServe, 0, this->reciever, NULL);
}

void *NetManager::service_net(void *args)
{
  while (1)
  {
    sleep(1);
    if (Union->isUnionLeader())
    {
      publicBlock *b = Union->packUp_u();
      //发送联盟区块。
      Emit(b);
    }
    if (time(0) % 45 == 5)
    {
      sync_contract();
      innerBlock *b = Union->packUp();
      Emit(b);
    }
  }
}

void NetManager::Emit(msg_header &h, string msg)
{
  struct epoll_event ev;
  struct epoll_event evs[MAXEPOLL];
  ev.events = EPOLLOUT;
  vector<PeerInfo *> ran = DNT->GetRandomNode();
  for (int i = 0; i < ran.size(); i++)
  {
    ev.data.fd = ran[i]->fd();
    if (epoll_ctl(epoll_acc_fd, EPOLL_CTL_ADD, listen_fd, &ev) < 0)
    {
      printf("Epoll Error : %d\n", errno);
      exit(EXIT_FAILURE);
    }
  }
}