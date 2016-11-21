#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csetjmp>
#include <cassert>
#include <csignal>
#include "cube_defs.h"

CubieCube SymCube[kSym_Oh];
CubieCube MoveCube[kMove];
int InvSymIdx[kSym_Oh];
int SymIdxMultiply[kSym_Oh][kSym_Oh];
int MoveConjugate[kMove][kSym_Oh];
int TwistConjugate[kTwist][kSym_D4h]; //Only 16
int RawFlipSliceRep[kFlipSlice];
char NextMove[kMBits][kMove+1];
unsigned short int TwistMove[kTwist][kMove];
unsigned short int Corn6PosMove[kCorn6Pos][kMove];
unsigned short int Edge4PosMove[kEdge4Pos][kMove];
int Edge6PosMove[kEdge6Pos][kMove];
int SymFlipSliceClassMove[kFlipSlice][kMove];
short *MovesCloserToTarget[kTwist];
short MoveBitsConjugate[kMBits][kSym_Oh];
unsigned long long ESymmetries[kMove],GESymmetries[kMove];

void pp() {
	printf(".");
	fflush(stdout);
	return;
}

int main() {
	printf("Initializing Tables..."); fflush(stdout);
	InitSymCubes();
	InitMoveCubes();
	InitSymIdxMultiply();
	InitMoveConjugate();
	InitMoveBitsConjugate();
	InitGESymmetries();
	InitTwistConjugate();
	InitRawFlipSliceRep();
	InitTwistMove();
	InitCorn6PosMove();
	InitEdge4PosMove();
	InitEdge6PosMove();
	InitSymFlipSliceClassMove();
	InitMovesCloserToTarget();
	InitNextMove();
	printf("\n");

	while (1) {
		printf("Enter cube (x to exit): "); fflush(stdout);
		char maneuver[256];
		if (fgets(maneuver, sizeof(maneuver), stdin) == NULL) break;
		if (maneuver[0] == 'x') return 0;
		int man_len = strlen(maneuver);
		if (man_len == 0) continue;
		if (maneuver[man_len - 1] == '\n') maneuver[man_len - 1] = 0;
		if (man_len > 1 && maneuver[man_len - 2] == '\r') maneuver[man_len - 2] = 0;
		printf("\nFinding optimal solution for: %s\n", maneuver);
		CubieCube CC = StringToCubieCube(maneuver);
		SolveOptimal(CC);
	}
	return 0;
}
