LEXER_TOOL := flex
CXX ?= g++ # Set the C++ compiler to g++ iff it hasn't already been set
CPP_SRCS := $(wildcard *.cpp) 
OBJ_SRCS := parser.o lexer.o $(CPP_SRCS:.cpp=.o)
DEPS := $(OBJ_SRCS:.o=.d)
FLAGS=-g -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Wuninitialized -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo -Wstrict-overflow=5 -Wundef -Werror -Wno-unused -Wno-unused-parameter -Wno-deprecated-register
#add these FLAGS for profiling 
#CXX = clang++
#FLAGS+=-fprofile-instr-generate -fcoverage-mapping


.PHONY: all clean test cleantest


all: dmc stddrewno_mars.o

.PRECIOUS: %.prog

clean:
	rm -rf *.output *.o *.cc *.hh $(DEPS) dmc parser.dot parser.png

-include $(DEPS)

dmc: $(OBJ_SRCS)
	$(CXX) $(FLAGS) -g -std=c++14 -o $@ $(OBJ_SRCS)
	chmod a+x dmc

stddrewno_mars.o: stddrewno_mars.c
	gcc -c stddrewno_mars.c

%.o: %.cpp 
	$(CXX) $(FLAGS) -g -std=c++14 -MMD -MP -c -o $@ $<

parser.o: parser.cc
	$(CXX) $(FLAGS) -Wno-sign-compare -Wno-sign-conversion -Wno-switch-default -g -std=c++14 -MMD -MP -c -o $@ $<

parser.cc: drewno_mars.yy
	bison -Werror --graph=parser.dot --defines=frontend.hh -v $<
	# Use the below version if you have an old version of bison
	#bison -Wnone --graph=parser.dot --defines=frontend.hh -v $< 

lexer.yy.cc: drewno_mars.l
	$(LEXER_TOOL) --outfile=lexer.yy.cc $<

lexer.o: lexer.yy.cc
	$(CXX) $(FLAGS) -Wno-sign-compare -Wno-sign-conversion -Wno-old-style-cast -Wno-switch-default -g -std=c++14 -c lexer.yy.cc -o lexer.o

test: p7

p7: all
	$(MAKE) -C p7_tests/

cleantest:
	$(MAKE) -C *_tests/ clean

compile:
	./dmc p7_tests/test.dm -o prog.s

assemble:
	as -g prog.s -o prog.o

link:
	ld -dynamic-linker /lib64/ld-linux-x86-64.so.2 /usr/lib/x86_64-linux-gnu/crt1.o /usr/lib/x86_64-linux-gnu/crti.o -lc prog.o stddrewno_mars.o /usr/lib/x86_64-linux-gnu/crtn.o -o prog.exe

fulltoolchain: compile assemble link
