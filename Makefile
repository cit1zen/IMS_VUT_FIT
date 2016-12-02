all: experiment

depencencies=params.h fire_engine.h fire_type.h fire.h fire_engine.cpp fire.cpp main.cpp
experiment: $(depencencies)
	g++ -l simlib fire_engine.cpp fire.cpp main.cpp -o $@ 