CC=gcc
CFLAGS= -I ~/Desktop/project/BitNest/core/pkg \
-I ~/Desktop/project/BitNest/core/pkg/Manager \
-I ~/Desktop/project/BitNest/core/pkg/Structure \
-lpthread\
-lstdc++

objects = Structure/uint256/uint128_t.o \
Structure/uint256/uint256_t.o \
Structure/uint256/test_uint256.o \
Structure/PeerInfo/PeerInfo.o \
Structure/PeerInfo/test_Peerinfo.o \
config.o \
Manager/DHTManager/DHTManager.o \
Manager/DHTManager/test_bucket.o \
Manager/DHTManager/Bucket.o \
nlock.o \
Structure/FileSystem/NFile.o \
Structure/FileSystem/test_nfile.o  \
Structure/contract/contract.o \
Structure/contract/payment.o \
Structure/contract/storage.o \
Structure/contract/backup.o \
Structure/contract/test_contract.o \
Structure/contract/brief.o\
Structure/proof/signature.o\
Structure/proof/test_signature.o\
Structure/post/post.o\
Structure/post/test_post.o\
Structure/UserOvershot/UserOvershot.o\
Structure/UserOvershot/test_UserOvershot.o\
Structure/block/block.o\
Structure/block/innerBlock.o\
Structure/block/publicBlock.o\
Structure/tools/net_tool.o\
Structure/mdns/mdns.o\
Structure/hash/tap.o\
Structure/hash/sha256.o\
Structure/ECC/uECC.o\
Structure/tools/test_net_tool.o\
Manager/BlockManager/BlockManager.o\
Manager/UnionManager/UnionManager.o\
Manager/NetManager/NetManager.o\
Manager/NetManager/test_NetManager.o\
test_main.o 

test: $(objects)
	gcc -o debug/test_main $(objects) $(CFLAGS)
	./debug/test_main

$(objects): %.o: %.cpp
	$(CC) -c  $< -o $@ $(CFLAGS)

clean:
	rm $(objects)

