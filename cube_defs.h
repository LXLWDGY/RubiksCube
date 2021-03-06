#ifndef _CUBEDEFS_H_
#define _CUBEDEFS_H_
/*Consts*/
const int kSym_Oh = 48;
const int kSym_D4h = 16;
const int kMove = 12;
const int kFlip = 2048; //Types of flip of edges
const int kSlice = 495; //Types of positions of UDSlices
const int kTwist = 2187; //Types of rotations(twists) of corners
const int kFlipSlice = 64430; //Types of equivalence classes,combining both NFLIP and NSLICE but much smaller than their product
const int kCorn6Pos = 20160; //Positions of 6 corners
const int kEdge6Pos = 665280; //Positions of 6 edges 
const int kEdge4Pos = 11880; //12!
const int kGoal = 281816820;
const unsigned long long int kCosetBuf = 1219276800;
const unsigned long long int kCoset = 9754214400ULL;
const int kNode = 40;
const int kMBits = 4096;

/*Basic Types*/
enum Axis {
	U, R, F, D, L, B,aEnd
};
typedef Axis Color;
enum Move {
	mU1, mU3, mR1, mR3, mF1, mF3, mD1, mD3, mL1, mL3, mB1, mB3,mEnd
};
enum Corner{
	URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB,cEnd
};
enum Edge{
	UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR,eEnd
};
enum Facelet{
	U1, U2, U3, U4, U5, U6, U7, U8, U9,
	R1, R2, R3, R4, R5, R6, R7, R8, R9,
	F1, F2, F3, F4, F5, F6, F7, F8, F9,
	D1, D2, D3, D4, D5, D6, D7, D8, D9,
	L1, L2, L3, L4, L5, L6, L7, L8, L9,
	B1, B2, B3, B4, B5, B6, B7, B8, B9,fEnd
};
enum BasicSym{
	S_URF3, S_F2, S_U4, S_LR2,symEnd
};

/*Advanced Types*/
struct CornerO {
    Corner c;
    char o;
};
struct EdgeO{
    Edge e;
    char o;
};
struct FaceletCube{
    Color f[54];
};
struct CubieCube{
	CornerO co[8];
	EdgeO eo[12];
};
struct CoordCube{	
	int sym_flip_slice;
	int edge_6_pos;
	short int edge_4_pos;						
	short int twist;
	short int corn_6_pos;
	short int parity;
};
struct SearchNode{
	unsigned short int flip_slice_U, flip_slice_R, flip_slice_F;
	unsigned short int sym_U, sym_R, sym_F;
	unsigned short int parity;
	unsigned short int twist_U, twist_R, twist_F;
	unsigned short int corn_6_pos;
	unsigned short int edge_4_pos;
	int edge_6_pos;
	short int moves_closer_target_U, moves_closer_target_R, moves_closer_target_F;
	short int moves_allowed;
	short int move;
	short int dist_U, dist_R, dist_F;
	unsigned long long m_sym;	
};

//Global Variables & Tables
extern CubieCube SymCube[kSym_Oh];
extern CubieCube MoveCube[kMove];
extern int InvSymIdx[kSym_Oh];
extern int SymIdxMultiply[kSym_Oh][kSym_Oh];
extern int MoveConjugate[kMove][kSym_Oh];
extern int TwistConjugate[kTwist][kSym_D4h]; //Only 16
extern int RawFlipSliceRep[kFlipSlice];
extern char NextMove[kMBits][kMove+1];
extern unsigned short int TwistMove[kTwist][kMove];
extern unsigned short int Corn6PosMove[kCorn6Pos][kMove];
extern unsigned short int Edge4PosMove[kEdge4Pos][kMove];
extern int Edge6PosMove[kEdge6Pos][kMove];
extern int SymFlipSliceClassMove[kFlipSlice][kMove];
extern short *MovesCloserToTarget[kTwist];
extern short MoveBitsConjugate[kMBits][kSym_Oh];
extern unsigned long long ESymmetries[kMove],GESymmetries[kMove];

namespace tk_gjz010_rubik_cubiecube{
    extern const CubieCube kIdCube;
    extern const CubieCube BasicCubeMove[6];
    extern void CubieCubeMultiply(const CubieCube* a,const CubieCube* b,CubieCube* ab);
    extern void CubieCubeCornerMultiply(const CubieCube* a,const CubieCube* b,CubieCube* ab);
    extern void CubieCubeEdgeMultiply(const CubieCube* a,const CubieCube* b,CubieCube* ab);
}


//char *gCoset;

//char *gVisitedA;
//char *gVisitedB;

/* TODO(guojz16): List of methods to implement.*/

/*cubie_cube.c*/
CubieCube CubeAxMove(CubieCube cc,Axis ax);
void InitMoveCubes();
CubieCube FaceletCubeToCubieCube(FaceletCube fc);
FaceletCube CubieCubeToFaceletCube(CubieCube fc);
CubieCube StringToCubieCube(char* defString);
void CubieCubeToString(CubieCube cc, char* def_string);
CubieCube InvCubieCube(CubieCube cc);

/*coord_cube.c*/
int Cnk(unsigned char n,unsigned char k);
unsigned short int Twist(CubieCube cc);
CubieCube InvTwist(unsigned short int twist);
unsigned short int Flip(CubieCube cc);
CubieCube InvFlip(unsigned short int flip);
unsigned short int Corn6Pos(CubieCube cc);
CubieCube InvCorn6Pos(unsigned short int idx);
int Edge6Pos(CubieCube cc);
CubieCube InvEdge6Pos(int idx);
int Edge4Pos(CubieCube cc);
CubieCube InvEdge4Pos(int idx);
unsigned short int Slice(CubieCube cc);
CubieCube InvSlice(unsigned short int slice);
int SymFlipSlice(CubieCube cc);
int CornerParity(CubieCube cc);
int EdgeParity(CubieCube cc);
void InitTwistMove();
void InitCorn6PosMove();
void InitEdge6PosMove();
void InitEdge4PosMove();
CoordCube CubieCubeToCoordCube(CubieCube cc);

/* TODO(lxlwdgy): List of methods to implement.*/

/*symmetry.c*/
void InitSymCubes();
void InitInvSymIdx();
void InitSymIdxMultiply();
void InitRawFlipSliceRep();
void InitSymFlipSliceClassMove();
int SymFlipSliceMove(int symFlipSlice, int m);
void InitMoveConjugate();
void InitTwistConjugate();
CubieCube EdgeConjugate(CubieCube cc, int symIdx);
CubieCube CornerConjugate(CubieCube cc, int symIdx);
unsigned long long GetSymmetries(CubieCube cc);
int SymCount(unsigned long long sym);
void InitGESymmetries();

/*TODO(guojz16):End of the list.*/

/*pruning.c*/
void InitMovesCloserToTarget();
void InitMoveBitsConjugate();
int DistanceToTarget(CoordCube co);
void SolveOptimal(CubieCube cu);
void InitNextMove();

/*TODO(lxlwdgy):End of the list.*/

/*complex_algo.c*/
void pp();
#endif
