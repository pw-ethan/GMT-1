CC := g++
COMPILE_FLAGS := -I/. -I/usr/local/include/boost_1_62_0 -g -Wall -fexceptions -DDEBUG_ERROR #-DCOMPLETE_INFO
LIBS := ./libs/fhe/fhe.a -lntl -lgmp -lmysqlclient -lmd -lboost_system -lboost_thread
LINK_FLAGS := -L/. -L/usr/local/lib/boost_1_62_0

all : main.o CryptoUtility.o common.o CtxtNode.o ZZNode.o CtxtWeightsList.o CtxtSiblingPathNode.o Auth.o DSAuth.o VTree.o PTree.o Base64.o DBUtility.o Node.o #Talk2Svr.o#auth_ds.o crypto_fhe_utility.o p_tree.o p_list.o v_tree.o v_list.o#MyDB.o
	$(CC) -o a.out $^ $(LINK_FLAGS) $(LIBS)
	@echo
	@echo "========================================================="
	@echo "run a.out..."
	@echo
	@./a.out
	@echo "========================================================="

PTree.o : PTree.cpp
	$(CC) -c -o $@ $< -std=c++11 $(COMPILE_FLAGS)

VTree.o : VTree.cpp
	$(CC) -c -o $@ $< -std=c++11 $(COMPILE_FLAGS)

%.o : %.cpp
	$(CC) -c -o $@ $< $(COMPILE_FLAGS)

clean :
	rm -rf *.o a.out

clear :
	rm -rf *.o *.key a.out

.PHONY : all clean clear
