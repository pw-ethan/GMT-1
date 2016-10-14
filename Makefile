CXX := g++
COMPILE_FLAGS := -I/. -g -Wall -fexceptions
LIBS := ./libs/fhe/fhe.a -lntl -lgmp -lmysqlclient -lmd
LINK_FLAGS := -L/.

all : main.o Node.o VTree.o common.o DBUtility.o Base64.o#auth_ds.o crypto_fhe_utility.o p_tree.o p_list.o v_tree.o v_list.o#MyDB.o
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

.PHONY : all clean
