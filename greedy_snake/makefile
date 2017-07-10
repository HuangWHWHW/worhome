cc=g++ -g -O0 -lpthread
executable=Greedy

src=$(shell find ./ -name *.cpp)

obj:$(src)
	$(cc) $(src) -o $(executable)

target:$(obj)

all:$(target)
