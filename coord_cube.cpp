#include "cube_defs.h"
#include <cstdio>
namespace tk_gjz010_rubik_coordcube{
    template<typename T,typename R> T min(T a,R b){
        return (a<b?a:b);
    }
    //helper function to push a list of numbers "forward":
    //eg. {1,2,3,4} -> {4,1,2,3}
    void PushForward(int* arr,int len){
        int last=(arr[len]);
        for(int i=len;i>0;i--){
            (arr[i])=(arr[i-1]);
        }
        (arr[0])=last;
    }
    //helper function to push a list of numbers "backward":
    //eg. {1,2,3,4} -> {2,3,4,1}
    void PushBackward(int* arr,int len){
        int first=(arr[0]);
        for(int i=0;i<len;i++){
            (arr[i])=(arr[i+1]);
        }
        (arr[len])=first;
    }
}
int Cnk(unsigned char n,unsigned char k){
    int a=1,b=1;
    if (n<k) return 0;
    k=tk_gjz010_rubik_coordcube::min(k,n-k);
    for(int p=0;p<k;p++){
        a=a*(n-p);
        b=b*(p+1);
    }
    return a/b;
}
unsigned short int Twist(CubieCube cc){
    short int result=0;
    for (Corner corn=URF;corn<DRB;corn=(Corner)(corn+1)) result=3*result+cc.co[corn].o; //Horner's method.
    return result;
}
CubieCube InvTwist(unsigned short int twist){
    CubieCube result=tk_gjz010_rubik_cubiecube::kIdCube;
    int twist_parity=0,tmod=0;
    for (Corner corn=DBL;corn>=URF;corn=(Corner)(corn-1)){
        tmod=twist%3;
        result.co[corn].o=tmod;
        twist_parity+=tmod;
        twist/=3;
    }
    result.co[DRB].o=(3-twist_parity%3)%3;
    return result;
}
unsigned short int Flip(CubieCube cc){
    short int result=0;
    for(Edge ed=UR;ed<BR;ed=(Edge)(ed+1)){
        result = 2*result + cc.eo[ed].o;
    }
    return result;
};
CubieCube InvFlip(unsigned short int flip){
/*
    CubieCube result=tk_gjz010_rubik_cubiecube::kIdCube;
    int flip_parity=0;
    int fmod;
    for (Edge ed=(Edge)(BR-1);ed>=UR;ed=(Edge)(ed-1)){
        fmod=flip&1;
        result.eo[ed].o=fmod;
        flip_parity+=fmod;
        flip>>=1;
    }
    result.eo[BR].o=(2-flip_parity%2)%2;
    return result;
*/
int e;
CubieCube ccRet=tk_gjz010_rubik_cubiecube::kIdCube;;
int flipParity=0;
for (e= BR-1;e>=UR;e--){flipParity += ccRet.eo[e].o = flip%2; flip /=2;}
ccRet.eo[BR].o = (2 - flipParity%2)%2;
return ccRet;
};
unsigned short int Corn6Pos(CubieCube cc){
    int a=0,b=0,x=0,k,perm[6];
    //compute the index a < (8 choose 6) and the permutation array perm.
    for(Corner corn=URF;corn<=DRB;corn=(Corner)(corn+1)){
        if (cc.co[corn].c<=DLF){
            a += Cnk(corn,x+1);
            perm[x] = cc.co[corn].c;
            x++;
        }
    }

    for(int j=5;j>0;j--){ //compute the index b < 6! for the permutation in perm
	    k=0;
	    while (perm[j]!=j){
	        tk_gjz010_rubik_coordcube::PushBackward(perm,j);
	        k++;
	    }
	    b=(j+1)*b+k; 
    }
    return 720*a+b;

};
CubieCube InvCorn6Pos(unsigned short int idx){
    int k, perm[6]={0,1,2,3,4,5};
    int b = idx%720;	//Permutation 
    int a = idx/720;	//Combination
    CubieCube result=tk_gjz010_rubik_cubiecube::kIdCube;
    for (Corner corn=URF;corn<=DRB;corn=(Corner)(corn+1))
        result.co[corn].c =(Corner)100;//Taint
    for (int j=1;j<6;j++){//generate permutation from index b
	    k=b%(j+1);
	    b/=j+1;
	    while (k>0){
	        k--;
	        tk_gjz010_rubik_coordcube::PushForward(perm,j);
	    }
    }
    int x=5;//generate combination and set edges
    for (Corner corn=DRB;corn>=URF;corn=(Corner)(corn-1))
    if (a-Cnk(corn,x+1)>=0) {
        result.co[corn].c=(Corner)perm[x];
        a-=Cnk(corn,x+1);
        x--;
    }
    return result;
};
int Edge6Pos(CubieCube cc){
    int a=0,b=0,x=0,k,perm[6];
    //compute the index a < (8 choose 6) and the permutation array perm.
    for(Edge ed=UR;ed<=BR;ed=(Edge)(ed+1)){
        if (cc.eo[ed].e<=DF){
            a += Cnk(ed,x+1);
            perm[x] = cc.eo[ed].e;
            x++;
        }
    }

    for(int j=5;j>0;j--){ //compute the index b < 6! for the permutation in perm
	    k=0;
	    while (perm[j]!=j){
	        tk_gjz010_rubik_coordcube::PushBackward(perm,j);
	        k++;
	    }
	    b=(j+1)*b+k; 
    }
    return 720*a+b;
};
CubieCube InvEdge6Pos(int idx){
    int k, perm[6]={0,1,2,3,4,5};
    int b = idx%720;	//Permutation 
    int a = idx/720;	//Combination
    CubieCube result=tk_gjz010_rubik_cubiecube::kIdCube;
    for (Edge ed=UR;ed<=BR;ed=(Edge)(ed+1))
        result.eo[ed].e =(Edge)100;//Taint
    for (int j=1;j<6;j++){//generate permutation from index b
	    k=b%(j+1);
	    b/=j+1;
	    while (k>0){
	        k--;
	        tk_gjz010_rubik_coordcube::PushForward(perm,j);
	    }
    }
    int x=5;//generate combination and set edges
    for (Edge ed=BR;ed>=UR;ed=(Edge)(ed-1))
    if (a-Cnk(ed,x+1)>=0) {
        result.eo[ed].e=(Edge)perm[x];
        a-=Cnk(ed,x+1);
        x--;
    }
    return result;
};
int Edge4Pos(CubieCube cc){
    int a=0,b=0,x=0,k,perm[4];
    //compute the index a < (8 choose 6) and the permutation array perm.
    for(Edge ed=BR;ed>=UR;ed=(Edge)(ed-1)){
        if (cc.eo[ed].e<=BR && cc.eo[ed].e>=FR){
            a += Cnk(11-ed,x+1);
            perm[3-x++] = cc.eo[ed].e;
        }
    }

    for(int j=3;j>0;j--){ //compute the index b < 6! for the permutation in perm
	    k=0;
	    while (perm[j]!=j+8){
	        tk_gjz010_rubik_coordcube::PushBackward(perm,j);
	        k++;
	    }
	    b=(j+1)*b+k; 
    }
    return 24*a+b;
};
CubieCube InvEdge4Pos(int idx){
    int k, perm[4]={8,9,10,11};
    int b = idx%24;	//Permutation 
    int a = idx/24;	//Combination
    CubieCube result=tk_gjz010_rubik_cubiecube::kIdCube;
    for (Edge ed=UR;ed<=BR;ed=(Edge)(ed+1))
        result.eo[ed].e =(Edge)100;//Taint
    for (int j=1;j<4;j++){//generate permutation from index b
	    k=b%(j+1);
	    b/=j+1;
	    while (k-->0){
	        tk_gjz010_rubik_coordcube::PushForward(perm,j);
	    }
    }
    int x=3;//generate combination and set edges
    for (Edge ed=UR;ed<=BR;ed=(Edge)(ed+1))
    if (a-Cnk(11-ed,x+1)>=0) {
        result.eo[ed].e=(Edge)perm[3-x];
        a-=Cnk(11-ed,x+1);
        x--;
    }
    x=0;
    for (Edge ed=UR;ed<=BR;ed=(Edge)(ed+1))//set the remaining edges 0..7
    if (result.eo[ed].e == 100){
        result.eo[ed].e = (Edge)x;
        x++;
    }
    return result;
};
unsigned short int Slice(CubieCube cc){
    int sum=0,k=0;
    for (Edge ed=BR;ed>=UR;ed=(Edge)(ed-1)){
        if (FR<=cc.eo[ed].e && cc.eo[ed].e<=BR ){
            sum+=Cnk(11-ed,k+1);
            k++;
        }
    }
    return sum;
};
CubieCube InvSlice(unsigned short int slice){
/*
    int a=slice;
    int perm[4]={8,9,10,11};
    CubieCube result=tk_gjz010_rubik_cubiecube::kIdCube;
    for(Edge ed=UR;ed<=BR;ed=(Edge)(ed+1)) result.eo[ed].e=(Edge)100; //Taint all edges

    int x=3; //Find the biggest positions one by one.
    for(Edge ed=UR;ed<=BR;ed=(Edge)(ed+1))
        if (a-Cnk(11-ed,x+1)>=0){
            result.eo[ed].e = (Edge)perm[x];
            a-=Cnk(11-ed,x+1);
            x--;
        }
    x=0;
    for (Edge ed=UR;ed<=BR;ed=(Edge)(ed+1)) //Fillback the non-UDslice edges.
        if (result.eo[ed].e==(Edge)100){
            result.eo[ed].e = (Edge)x;
            x++;
        };
    return result;
    */
int a=slice,j,x,perm[4]={8,9,10,11};;
CubieCube ccRet=tk_gjz010_rubik_cubiecube::kIdCube;
for (j=UR;j<=BR;j++) ccRet.eo[j].e =(Edge)255;//Invalidate all edges

x = 3;//generate combination and set edges
for (j=UR;j<=BR;j++)
if (a - Cnk(11-j,x+1)>=0) {ccRet.eo[j].e = Edge(perm[x]); a -=Cnk(11-j,x-- +1);}
for (j=UR,x=0;j<=BR;j++)//set the remaining edges 0..7
if (ccRet.eo[j].e == (Edge)255) ccRet.eo[j].e = (Edge)(x++);
return ccRet;
};
int SymFlipSlice(CubieCube cc){
    int rep=kFlip*kSlice;
    int raw_flip_slice;
    CubieCube cc_rep;
    int rep_index; //the symmetry representing the class
    for (int i=0;i<kSym_D4h;i++)
    {
	    cc_rep = EdgeConjugate(cc,i);
	    raw_flip_slice = kFlip*Slice(cc_rep) + Flip(cc_rep);
	    if (raw_flip_slice<rep) {
	        rep = raw_flip_slice;
	        rep_index=i;
	    }
    }	
    //rep not holds the rawFlipSlice coordinate of the representant ccRep
    //in the equivalence class of cube cc.
    //We have cc = symCube[idx]^-1*ccRep*symCube[idx]
    //Get the index of the ccRep-equivalence class from table rawFlipSliceRep now:
    //Binary search as RawFlipSliceRep is sorted.
    int rBound = kFlipSlice;
    int lBound = 0;
    int class_id = -1;
    do
    {
	    class_id = (lBound + rBound)/2;
	    if (rep<RawFlipSliceRep[class_id]) rBound = class_id;
	    else if (rep>RawFlipSliceRep[class_id]) lBound = class_id;
    }while (rep!=RawFlipSliceRep[class_id]);
    return (class_id<<4) + rep_index;	
}
int EdgeParity(CubieCube cc){
    int s=0;
    for (Edge e1=BR;e1>=UR+1;e1=(Edge)(e1-1))
    for (Edge e2=(Edge)(e1-1);e2>=UR;e2=(Edge)(e2-1))
	    if (cc.eo[e2].e>cc.eo[e1].e) s++;
    return s%2;
}
void InitTwistMove(){
    CubieCube twist[4];
    for(int i=0;i<kTwist;i++){
        twist[0]=InvTwist(i);
        for(Move m=mU1;m<=mB3;m=(Move)(m+2)){
            Axis axis=(Axis)(m>>1);
		    tk_gjz010_rubik_cubiecube::CubieCubeCornerMultiply(&twist[0],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&twist[1]);
		    TwistMove[i][m] = Twist(twist[1]);
		    tk_gjz010_rubik_cubiecube::CubieCubeCornerMultiply(&twist[1],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&twist[2]);
		    tk_gjz010_rubik_cubiecube::CubieCubeCornerMultiply(&twist[2],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&twist[3]);
		    TwistMove[i][m+1] = Twist(twist[3]);
		    tk_gjz010_rubik_cubiecube::CubieCubeCornerMultiply(&twist[3],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&twist[0]);
        }
    }
};
void InitCorn6PosMove(){
    CubieCube c6pos[4];
    for(int i=0;i<kCorn6Pos;i++){
        c6pos[0]=InvCorn6Pos(i);
        for(Move m=mU1;m<=mB3;m=(Move)(m+2)){
            Axis axis=(Axis)(m>>1);
		    tk_gjz010_rubik_cubiecube::CubieCubeCornerMultiply(&c6pos[0],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&c6pos[1]);
		    Corn6PosMove[i][m] = Corn6Pos(c6pos[1]);
		    tk_gjz010_rubik_cubiecube::CubieCubeCornerMultiply(&c6pos[1],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&c6pos[2]);
		    tk_gjz010_rubik_cubiecube::CubieCubeCornerMultiply(&c6pos[2],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&c6pos[3]);
		    Corn6PosMove[i][m+1] = Corn6Pos(c6pos[3]);
		    tk_gjz010_rubik_cubiecube::CubieCubeCornerMultiply(&c6pos[3],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&c6pos[0]);
        }
    }
};
void InitEdge6PosMove(){
    CubieCube e6pos[4];
    for(int i=0;i<kEdge6Pos;i++){
        //if (!(i%40000)) pp();
        e6pos[0]=InvEdge6Pos(i);
        for(Move m=mU1;m<=mB3;m=(Move)(m+2)){
            Axis axis=(Axis)(m>>1);
		    tk_gjz010_rubik_cubiecube::CubieCubeEdgeMultiply(&e6pos[0],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&e6pos[1]);
		    Edge6PosMove[i][m] = Edge6Pos(e6pos[1]);
		    tk_gjz010_rubik_cubiecube::CubieCubeEdgeMultiply(&e6pos[1],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&e6pos[2]);
		    tk_gjz010_rubik_cubiecube::CubieCubeEdgeMultiply(&e6pos[2],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&e6pos[3]);
		    Edge6PosMove[i][m+1] = Edge6Pos(e6pos[3]);
		    tk_gjz010_rubik_cubiecube::CubieCubeEdgeMultiply(&e6pos[3],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&e6pos[0]);
        }
    }
};
void InitEdge4PosMove(){
    CubieCube e4pos[4];
    for(int i=0;i<kEdge4Pos;i++){
        e4pos[0]=InvEdge4Pos(i);
        for(Move m=mU1;m<=mB3;m=(Move)(m+2)){
            Axis axis=(Axis)(m>>1);
		    tk_gjz010_rubik_cubiecube::CubieCubeEdgeMultiply(&e4pos[0],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&e4pos[1]);
		    Edge4PosMove[i][m] = Edge4Pos(e4pos[1]);
		    tk_gjz010_rubik_cubiecube::CubieCubeEdgeMultiply(&e4pos[1],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&e4pos[2]);
		    tk_gjz010_rubik_cubiecube::CubieCubeEdgeMultiply(&e4pos[2],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&e4pos[3]);
		    Edge4PosMove[i][m+1] = Edge4Pos(e4pos[3]);
		    tk_gjz010_rubik_cubiecube::CubieCubeEdgeMultiply(&e4pos[3],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&e4pos[0]);
        }
    }
};
int CornerParity(CubieCube cc)
{
	int i,j,s=0;	
	for (i=DRB;i>=URF+1;i--)
	for (j=i-1;j>=URF;j--)
		if (cc.co[j].c>cc.co[i].c) s++;
	return s%2;
}
CoordCube CubieCubeToCoordCube(CubieCube cc){
    CoordCube co;
    co.corn_6_pos = Corn6Pos(cc);
    co.edge_6_pos = Edge6Pos(cc);
    co.edge_4_pos = Edge4Pos(cc);
    co.parity = CornerParity(cc);
    co.sym_flip_slice = SymFlipSlice(cc);
    co.twist = Twist(cc);
    return co;
};
