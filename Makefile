prog: cubie_cube.o coord_cube.o optiqtm.o pruning.o symmetry.o
		g++ -g cubie_cube.o coord_cube.o optiqtm.o pruning.o symmetry.o -o optiqtm
	
cubie_cube.o : cubie_cube.cpp
		g++ -g -Wall -c cubie_cube.cpp
		
coord_cube.o : coord_cube.cpp
		g++ -g -Wall -c coord_cube.cpp

optiqtm.o : optiqtm.cpp
		g++ -g -Wall -c optiqtm.cpp

pruning.o : pruning.cpp
		g++ -g -Wall -c pruning.cpp

symmetry.o : symmetry.cpp
		g++ -g -Wall -c symmetry.cpp

