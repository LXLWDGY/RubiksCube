#include "cube_defs.h"
#include <cstring>
#include <cstdio>
#include <cassert>
namespace tk_gjz010_rubik_symmetry{
    const CubieCube CubieBasicSym[4]={
    {{{URF,1},{DFR,2},{DLF,1},{UFL,2},{UBR,2},{DRB,1},{DBL,2},{ULB,1}},    //S_URF3
    {{UF,1},{FR,0},{DF,1},{FL,0},{UB,1},{BR,0}, //S_URF3
    {DB,1},{BL,0},{UR,1},{DR,1},{DL,1},{UL,1}}},
    {{{DLF,0},{DFR,0},{DRB,0},{DBL,0},{UFL,0},{URF,0},{UBR,0},{ULB,0}},    //S_F2
    {{DL,0},{DF,0},{DR,0},{DB,0},{UL,0},{UF,0}, //S_F2
    {UR,0},{UB,0},{FL,0},{FR,0},{BR,0},{BL,0}}},
    {{{UBR,0},{URF,0},{UFL,0},{ULB,0},{DRB,0},{DFR,0},{DLF,0},{DBL,0}},    //S_U4
    {{UB,0},{UR,0},{UF,0},{UL,0},{DB,0},{DR,0}, //S_U4
    {DF,0},{DL,0},{BR,1},{FR,1},{FL,1},{BL,1}}},
    {{{UFL,3},{URF,3},{UBR,3},{ULB,3},{DLF,3},{DFR,3},{DRB,3},{DBL,3}},     //S_LR
    {{UL,0},{UF,0},{UR,0},{UB,0},{DL,0},{DF,0}, //S_LR2
    {DR,0},{DB,0},{FL,0},{FR,0},{BR,0},{BL,0}}}
    };
    const int kMaxSym[4]={3,2,4,2};

}
void InitSymCubes(){
    int id=0;
    CubieCube curr = tk_gjz010_rubik_cubiecube::kIdCube,next;
    for(int i=0;i<tk_gjz010_rubik_symmetry::kMaxSym[S_URF3];i++){
        for(int j=0;j<tk_gjz010_rubik_symmetry::kMaxSym[S_F2];j++){
            for(int k=0;k<tk_gjz010_rubik_symmetry::kMaxSym[S_U4];k++){
                for(int l=0;l<tk_gjz010_rubik_symmetry::kMaxSym[S_LR2];l++){
                    SymCube[id]=curr;
                    id++;
                    tk_gjz010_rubik_cubiecube::CubieCubeMultiply(&curr,&tk_gjz010_rubik_symmetry::CubieBasicSym[S_LR2],&next);
                    curr=next;
                }
                tk_gjz010_rubik_cubiecube::CubieCubeMultiply(&curr,&tk_gjz010_rubik_symmetry::CubieBasicSym[S_U4],&next);
                curr=next;
            }
            tk_gjz010_rubik_cubiecube::CubieCubeMultiply(&curr,&tk_gjz010_rubik_symmetry::CubieBasicSym[S_F2],&next);
            curr=next;
        }
        tk_gjz010_rubik_cubiecube::CubieCubeMultiply(&curr,&tk_gjz010_rubik_symmetry::CubieBasicSym[S_URF3],&next);
        curr=next;
    }
};
void InitInvSymIdx(){
    CubieCube cc;
    for(int i=0;i<kSym_Oh;i++)
        for(int j=0;j<kSym_Oh;j++){
            tk_gjz010_rubik_cubiecube::CubieCubeCornerMultiply(&SymCube[i],&SymCube[j],&cc);
            if (cc.co[URF].c == URF && cc.co[UFL].c == UFL && cc.co[ULB].c == ULB)
	        {
		        InvSymIdx[i] = j;
		        break;
	        }
        }
};
void InitSymIdxMultiply(){
    CubieCube cc;
    for(int i=0;i<kSym_Oh;i++)
        for(int j=0;j<kSym_Oh;j++){
            tk_gjz010_rubik_cubiecube::CubieCubeEdgeMultiply(&SymCube[i],&SymCube[j],&cc);
            for(int k=0;k<kSym_Oh;k++)
                if (cc.eo[UR].e == SymCube[k].eo[UR].e &&
                    cc.eo[UF].e == SymCube[k].eo[UF].e)
	            {
		            SymIdxMultiply[i][j]=k;
		            break;
	            }
        }
};
void InitRawFlipSliceRep()
/*{
    char flag[kSlice][kFlip];
    int id=0;
    CubieCube cc_slice,cc_flip,cc_conj;
    int max_size=kSlice*kFlip;
    memset(flag,0,sizeof(char)*max_size);
    for(int i=0;i<kSlice;i++){
        cc_slice=InvSlice(i);
        for(int j=0;j<kFlip;j++){
            if(flag[i][j]){continue;}
            cc_flip=InvFlip(j);
            for(Edge ed=UR;ed<=BR;ed=(Edge)(ed+1)){
                cc_flip.eo[ed].e=cc_slice.eo[ed].e;
            }
		    for (int k=0;k<kSym_D4h;k++)
		    {
			    cc_conj=EdgeConjugate(cc_flip,k);
			    flag[Slice(cc_conj)][Flip(cc_conj)]=1;//no representant
		    }
		    RawFlipSliceRep[id] = i*kFlip+j;
		    //std::cout<<"Solving id "<<id<<std::endl;
		    id++;
		    
            
        }
        
    }
    
};
*/
{
int i,j,k,n,min,idx=0;
CubieCube ccK,ccJ,ccI;
char flag[kSlice*kFlip];
for (j=0;j<kSlice*kFlip;j++) flag[j]=0;
for (j=0;j<kSlice;j++)
{
	ccJ = InvSlice(j);
	for (k=0;k<kFlip;k++)
	{
	    //printf("Solving %d\n",idx);
		min = kFlip*j + k;
		if (flag[min]==1) continue;//no representant
		ccK = InvFlip(k);
		for (n=0;n<12;n++){
		ccK.eo[n].e = ccJ.eo[n].e; 
		//printf("Moving edge %d\n",ccK.eo[n].e);
		}
		//merge ccK and ccJ
		
		for (i=0;i<kSym_D4h;i++)
		{
			ccI = EdgeConjugate(ccK,i); 
			n = kFlip*Slice(ccI) + Flip(ccI);
			//printf("Solving conflict %d\n",n);
			//std::cout<<"Solving id "<<idx<<std::endl;
			flag[n]=1;//no representant
		}
		RawFlipSliceRep[idx++] = min;
		assert(idx<=kFlipSlice);
	}
}
}
void InitSymFlipSliceClassMove(){
    CubieCube cc_flip,cc_slice,sfs[4];
    for(int index=0;index<kFlipSlice;index++){
        int rep=RawFlipSliceRep[index];
        cc_slice=InvSlice(rep/kFlip);
        cc_flip=InvFlip(rep%kFlip);
        for (int n=0;n<12;n++) cc_flip.eo[n].e = cc_slice.eo[n].e;
        sfs[0]=cc_flip;
        for(Move m=mU1;m<=mB3;m=(Move)(m+2)){
            Axis axis=(Axis)(m>>1);
	        tk_gjz010_rubik_cubiecube::CubieCubeEdgeMultiply(&sfs[0],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&sfs[1]);
	        SymFlipSliceClassMove[index][m] = SymFlipSlice(sfs[1]);
	        tk_gjz010_rubik_cubiecube::CubieCubeEdgeMultiply(&sfs[1],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&sfs[2]);
	        tk_gjz010_rubik_cubiecube::CubieCubeEdgeMultiply(&sfs[2],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&sfs[3]);
	        SymFlipSliceClassMove[index][m+1] = SymFlipSlice(sfs[3]);
	        tk_gjz010_rubik_cubiecube::CubieCubeEdgeMultiply(&sfs[3],&tk_gjz010_rubik_cubiecube::BasicCubeMove[axis],&sfs[0]);
        }
    }
}
int SymFlipSliceMove(int sym_flip_slice, int m){
    int aSym,aClass,bSym,bClass,mConj,b;
    aClass = sym_flip_slice>>4;aSym = sym_flip_slice&15;
    mConj = MoveConjugate[m][aSym];
    b = SymFlipSliceClassMove[aClass][mConj];
    bClass = b>>4; bSym = b&15;
    return (bClass<<4) + SymIdxMultiply[bSym][aSym];
};
void InitMoveConjugate(){
    CubieCube cc;
    int flag;
    for (int i=0;i<kMove;i++)
        for (int j=0;j<kSym_Oh;j++)
        {
	        cc = EdgeConjugate(MoveCube[i],j);
	        for (int k=0;k<kMove;k++)//find the conjugate cube cube
	        {
		        flag=1;
		        for (Edge ed=UR;ed<=BR;ed=(Edge)(ed+1))//test all edges
			        if (MoveCube[k].eo[ed].e!=cc.eo[ed].e){
			            flag=0;
			            break;
			        } 
		        if (flag==1){
		            MoveConjugate[i][j]=k;
		            break;
		        }
	        }	
        }	
};
void InitTwistConjugate(){
    CubieCube cc,cc_conj;
    for (int i=0;i<kTwist;i++){
        cc=InvTwist(i);
        for (int j=0;j<kSym_D4h;j++)
        {
	        cc_conj = CornerConjugate(cc,j);
		    TwistConjugate[i][j]=Twist(cc_conj);
        }	
    }
};
CubieCube EdgeConjugate(CubieCube cc, int sym_idx){
    CubieCube ap;
    tk_gjz010_rubik_cubiecube::CubieCubeEdgeMultiply(&SymCube[sym_idx],&cc,&ap);
    tk_gjz010_rubik_cubiecube::CubieCubeEdgeMultiply(&ap,&SymCube[InvSymIdx[sym_idx]],&cc);
    return cc;
};
CubieCube CornerConjugate(CubieCube cc, int sym_idx){
    CubieCube ap;
    tk_gjz010_rubik_cubiecube::CubieCubeCornerMultiply(&SymCube[sym_idx],&cc,&ap);
    tk_gjz010_rubik_cubiecube::CubieCubeCornerMultiply(&ap,&SymCube[InvSymIdx[sym_idx]],&cc);
    return cc;
};
unsigned long long GetSymmetries(CubieCube cc){
    unsigned long long result=0;
    char flag;
    CubieCube ap,pap;
    //The step is to calc P^(-1)*A*P
    //Nothing to do with Pen Pineapple Apple Pen.
    for(int i=0;i<kSym_Oh;i++){
        flag=0;
        tk_gjz010_rubik_cubiecube::CubieCubeMultiply(&SymCube[i],&cc,&ap);
        tk_gjz010_rubik_cubiecube::CubieCubeMultiply(&ap,&SymCube[InvSymIdx[i]],&pap);
	    for (Corner corn=URF;corn<=DRB;corn=(Corner)(corn+1)){
		    if (pap.co[corn].c!=cc.co[corn].c || pap.co[corn].o!=cc.co[corn].o){flag=1;break;};
	    }
	    if (flag) continue;
	    for (Edge ed=UR;ed<=BR;ed=(Edge)(ed+1)){
		    if (pap.eo[ed].e!=cc.eo[ed].e || pap.eo[ed].o!=cc.eo[ed].o){flag=1;break;};
	    }
	    if (!flag) result|=1ULL<<i;
    }
    return result;
}
int SymCount(unsigned long long sym){
    int count=0;
    while(sym!=0){
        count+=(sym&1);
        sym>>=1;
    }
    return count;
}
void InitGESymmetries(){
    for (Move m=mU1;m<=mB3;m=(Move)(m+1)){
        ESymmetries[m]=0;
        GESymmetries[m]=0;
    }
    for (int i=0;i<kSym_Oh;i++)
        for (Move m=mU1;m<=mB3;m=(Move)(m+1))
        {
	        if (MoveConjugate[m][i]==m) ESymmetries[m] |= 1ULL<<i;
	        if (MoveConjugate[m][i]>=m) GESymmetries[m] |= 1ULL<<i;
        }

};
