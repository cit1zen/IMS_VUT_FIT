all: experiment

depencencies=params.h fire_engine.h fire_type.h fire.h fire_engine.cpp fire.cpp main.cpp
experiment: $(depencencies)
	g++ --std=c++11 -l simlib fire_engine.cpp fire.cpp main.cpp -o $@ 

.PHONY: run, clean
run: experiment
	./experiment

clean:
	rm experiment