#ifndef NEST_NET_MANAGER
#define NEST_NET_MANAGER
#include <PeerInfo/PeerInfo.h>
#include <DHTManager/DHTManager.h>
#include <BlockManager/BlockManager.h>
#include <UnionManager/UnionManager.h>
#include <StoreManager/StorageManager.h>
#include <WalletManager/WalletManager.h>
#include <contract/backup.h>
#include <contract/storage.h>
#include <map>
#include <KeyPair/KeyPair.h>
#include <tools/net_tool.h>
#include <FileSystem/StorageData.h>
#include <string>
#include <sendlist/sendlist.h>
using namespace std;

string get_ip_from_fd(int fd);
class NetManager
{
private:
    DHTManager *DHT;
    BlockManager *Blocks;
    UnionManager *Union;
    StorageManager *Files;
    WalletManager *wallet;
    //来自客户端的连接
    vector<int> cli_fd;

    //服务方自己执行的存储合约
    map<uint256, Storage *> stores;
    map<uint256, BackUp *> backups;

    //从这个队列中接收文件才保存
    vector<PeerInfo *> files;

    //当前运行阶段 0: 运行 1：结算 2：选举 3：打包 4：签名
    int CurrEra;

    //哈希地址、密钥对
    uint256 *my;
    KeyPair *keys;

    //接收连接epoll
    int listen_fd, epoll_acc_fd, acc_cur_fds;

    //接收epoll
    int epoll_in;

    //发送队列
    sendlist SendList;

public:
    //网络管理器入口
    void deamon();
    NetManager(PeerInfo *seed);
    NetManager();
    //发送PBFT请求
    bool sendPBFT(string msg);
    //启动发现
    void discovery();
    void discovery(PeerInfo *p);
    //P2P服务
    void *AddEventIn(void *arg);
    void *sender(void *args);
    void *reciver(void *args);
    void *service_accept(void *args);
    void *service_nmdns(void *args);
    void *service_net(void *args);
    void *service_union(void *args);
    //工具函数发送同步信息
    void *AddEventEmit(void *arg);
    void Emit(PoST *p);
    void Emit(publicBlock *b);
    void Emit(innerBlock *b);
    void Emit(BackUp *c);
    void Emit(payment *c);
    void Emit(Storage *c);
    void Emit(msg_header &h, string msg);
    //文件传输
    StorageData *FileReciever(PeerInfo *p);
    void FileSender(PeerInfo *p, StorageData &file);

    //合同同步
    void contract_sync();
};
#endif