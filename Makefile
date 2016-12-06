CXX := g++
COMPILE_FLAGS := -std=c++11 -I/. -I/usr/local/include/boost_1_62_0 -g -Wall -fexceptions
LIBS := ./libs/fhe/fhe.a -lntl -lgmp -lmysqlclient -lmd -lboost_system -lboost_thread
LINK_FLAGS := -L/. -L/usr/local/lib/boost_1_62_0

all : main.o common.o Base64.o DBUtility.o CryptoUtility.o DSAuth.o Node.o VTree.o PTree.o#Talk2Svr.o#auth_ds.o crypto_fhe_utility.o p_tree.o p_list.o v_tree.o v_list.o#MyDB.o
	$(CXX) -o a.out $^ $(LINK_FLAGS) $(LIBS)
	@echo
	@echo "========================================================="
	@echo "run a.out..."
	@echo
	@./a.out
	@echo "========================================================="

%.o : %.cpp
	$(CXX) -c -o $@ $< $(COMPILE_FLAGS)

clean :
	rm -rf *.o a.out

clear :
	rm -rf *.o *.key a.out

.PHONY : all clean clear
