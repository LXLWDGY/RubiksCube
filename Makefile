prog: cubie_cube.o coord_cube.o optiqtm.o pruning.o symmetry.o
		g++ -g cubie_cube.o coord_cube.o optiqtm.o pruning.o symmetry.o -o optiqtm
	
cubie_cube.o : cube_defs.h cubie_cube.cpp
		g++ -g -Wall -c cubie_cube.cpp
		
coord_cube.o : cube_defs.h coord_cube.cpp
		g++ -g -Wall -c coord_cube.cpp

optiqtm.o : cube_defs.h optiqtm.cpp
		g++ -g -Wall -c optiqtm.cpp

pruning.o : cube_defs.h pruning.cpp
		g++ -g -Wall -c pruning.cpp

symmetry.o : cube_defs.h symmetry.cpp
		g++ -g -Wall -c symmetry.cpp

