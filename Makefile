CXX := g++
COMPILE_FLAGS := -I/. -g -Wall -fexceptions #-std=c++11
LIBS := ./libs/fhe/fhe.a -lntl -lgmp -lmysqlclient -lmd
LINK_FLAGS := -L/.

all : main.o auth_ds.o common.o crypto_fhe_utility.o p_tree.o p_list.o v_tree.o v_list.o#MyDB.o
	$(CXX) -o out $^ $(LINK_FLAGS) $(LIBS)

%.o : %.cpp
	$(CXX) -c -o $@ $< $(COMPILE_FLAGS)

clean :
	rm -rf *.o out

.PHONY : all clean
