#ifndef NEST_MSG
#define NEST_MSG
#include <uint256.h>
//PBFT可以实现的事：
/*
1、启动合同:data携带合同哈希
2、时空证明:两方协商->同时公开->PoST:证明者：pk证明者公钥 data 挑战者ID。挑战者：pk挑战者公钥 data 证明者ID
3、完成合同:data携带合同哈希
4、加入联盟:data携带联盟号：固定长度的联盟字符串
5、选举代表:pk选举人 data被选中者
6、联盟分裂:pk联盟代表 跟一段选中的个体：原先联盟代表随机选择哈希最大的一半。组成新的联盟，选取随机字符串代表联盟。
7、分裂证据:达成联盟分裂共识。节点共同维持旧联盟链和新联盟链。
8、区块共识:联盟内区块由联盟内PBFT，外加联盟外随机选取的个体进行验证。公共区块由全体PBFT
共同完成。由所有人验证。TODO：使用零知识证明加速验证
*/

//提升：增加包复用功能（参考TCP数据包设计）

//PBFT请求
#define NK_PBFT_REQ 'r'
//PBFT预准备
#define NK_PBFT_PPR 'p'
//PBFT准备
#define NK_PBFT_PRE 'P'
//PBFT提交
#define NK_PBFT_COM 'c'
//PBFT回复
#define NK_PBFT_RES 'R'

//可用空间请求
#define NK_SPACE 's'

//DHT 发现节点请求
#define DHT_CONNECT 'd'
#define DHT_FIND_NODE 'D'

//查询：data为目标
#define NK_QUERY_PAY '1'
#define NK_QUERY_STO '2'
#define NK_QUERY_BAK '3'
#define NK_QUERY_POST '4'
#define NK_QUERY_IB '5'
#define NK_QUERY_PB '6'

#define NK_SEND_PAY '7'
#define NK_SEND_STO '8'
#define NK_SEND_BAK '9'
#define NK_SEND_POST '0'
#define NK_SEND_IB 'a'
#define NK_SEND_PB 'b'

//回复和错误
#define NK_QUERY_ERR '-'

//nest message kind normal正常消息
#define NK_NORMAL 'n'
struct msg_header
{
  char kind;
  uint256 pk[2];
  uint256 data[2];
  int msg_size;
};

#endif