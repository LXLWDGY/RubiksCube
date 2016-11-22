#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csetjmp>
#include <cassert>
#include <csignal>
#include "cube_defs.h"

CubieCube SymCube[kSym_Oh];//Correct
CubieCube MoveCube[kMove];//Correct
int InvSymIdx[kSym_Oh];//Correct
int SymIdxMultiply[kSym_Oh][kSym_Oh];//Correct
int MoveConjugate[kMove][kSym_Oh];//Correct
int TwistConjugate[kTwist][kSym_D4h]; //Correct
int RawFlipSliceRep[kFlipSlice];//Correct
char NextMove[kMBits][kMove+1];//?
unsigned short int TwistMove[kTwist][kMove];//Correct
unsigned short int Corn6PosMove[kCorn6Pos][kMove];//Correct
unsigned short int Edge4PosMove[kEdge4Pos][kMove];//Correct
int Edge6PosMove[kEdge6Pos][kMove];//Correct
int SymFlipSliceClassMove[kFlipSlice][kMove];//Correct
short *MovesCloserToTarget[kTwist];
short MoveBitsConjugate[kMBits][kSym_Oh];//Correct
unsigned long long ESymmetries[kMove],GESymmetries[kMove];//Correct

void pp() {
	printf(".");
	fflush(stdout);
	return;
}

int main() {
	printf("Initializing Tables..."); fflush(stdout);
	printf("InitSymCubes();");fflush(stdout);
	InitSymCubes();
	printf("InitMoveCubes();");fflush(stdout);
	InitMoveCubes();
	printf("InitInvSymIdx();");fflush(stdout);
	InitInvSymIdx();
	printf("InitSymIdxMultiply();");fflush(stdout);
	InitSymIdxMultiply();
	printf("InitMoveConjugate();");fflush(stdout);
	InitMoveConjugate();
	printf("InitMoveBitsConjugate();");fflush(stdout);
	InitMoveBitsConjugate();
	printf("InitGESymmetries();");fflush(stdout);
	InitGESymmetries();
	printf("InitTwistConjugate();");fflush(stdout);
	InitTwistConjugate();
	printf("InitRawFlipSliceRep();");fflush(stdout);
	InitRawFlipSliceRep();
	printf("InitTwistMove();");fflush(stdout);
	InitTwistMove();
	printf("InitCorn6PosMove();");fflush(stdout);
	InitCorn6PosMove();
	printf("InitEdge4PosMove();");fflush(stdout);
	InitEdge4PosMove();
	printf("InitEdge6PosMove();");fflush(stdout);
	InitEdge6PosMove();
	printf("InitSymFlipSliceClassMove();");fflush(stdout);
	InitSymFlipSliceClassMove();
	printf("InitMovesCloserToTarget();");fflush(stdout);
	InitMovesCloserToTarget();
	printf("InitNextMove();");fflush(stdout);
	InitNextMove();
	printf("Done");fflush(stdout);
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
