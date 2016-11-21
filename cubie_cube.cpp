#include "cube_defs.h"
#include <cctype>
#include <cstring>
/*++++++++++++++++The layout of the facelets on the cube+++++++++++++++++++++++

             |************|
             |*U1**U2**U3*|
             |************|
             |*U4**U5**U6*|
             |************|
             |*U7**U8**U9*|
             |************|
|************|************|************|************|
|*L1**L2**L3*|*F1**F2**F3*|*R1**R2**F3*|*B1**B2**B3*|
|************|************|************|************|
|*L4**L5**L6*|*F4**F5**F6*|*R4**R5**R6*|*B4**B5**B6*|
|************|************|************|************|
|*L7**L8**L9*|*F7**F8**F9*|*R7**R8**R9*|*B7**B8**B9*|
|************|************|************|************|
             |************|
             |*D1**D2**D3*|
             |************|
             |*D4**D5**D6*|
             |************|
             |*D7**D8**D9*|
             |************|
*/
namespace tk_gjz010_rubik_cubiecube{
//TODO(gjz010):encapsulate those methods in class CubieCube.
//Note that this is a b*a multiply.
void CubieCubeCornerMultiply(const CubieCube* a,const CubieCube* b,CubieCube* ab){
    char ori,oria,orib;
    ori=0;
    for(Corner corn=URF;corn<=DRB;corn=(Corner)(corn+1)){
        ab->co[corn].c=a->co[b->co[corn].c].c;
        oria=a->co[b->co[corn].c].o;
        orib=b->co[corn].o;
        if(oria<3 && orib<3){
            ori=oria+orib;
            if(ori>=3) ori-=3;
        }else if(oria<3 && orib>=3){
            ori=oria+orib;
            if(ori>=6) ori-=3;
        }else if(oria>=3 && orib<3){
            ori=oria-orib;
            if(ori<3) ori+=3;
        }else if(oria>=3 && orib>=3){
            ori=oria-orib;
            if(ori<0) ori+=3;
        }
    }
}
void CubieCubeEdgeMultiply(const CubieCube* a,const CubieCube* b,CubieCube* ab){
    for(Edge ed=UR;ed<=BR;ed=(Edge)(ed+1)){
        ab->eo[ed].e=a->eo[b->eo[ed].e].e;
        ab->eo[ed].o = (b->eo[ed].o+a->eo[b->eo[ed].e].o)%2;
    }
}
void CubieCubeMultiply(const CubieCube* a,const CubieCube* b,CubieCube* ab){
    CubieCubeCornerMultiply(a,b,ab);
    CubieCubeEdgeMultiply(a,b,ab);
}
//Note that as cube move is represented in a replaced-by way,the move should be the right-hand operand.
const CubieCube BasicCubeMove[6]={
 {{{UBR,0},{URF,0},{UFL,0},{ULB,0},{DFR,0},{DLF,0},{DBL,0},{DRB,0}},   //U
 {{UB,0},{UR,0},{UF,0},{UL,0},{DR,0},{DF,0},{DL,0},{DB,0},{FR,0},{FL,0},{BL,0},{BR,0}}},
 {{{DFR,2},{UFL,0},{ULB,0},{URF,1},{DRB,1},{DLF,0},{DBL,0},{UBR,2}},   //R
 {{FR,0},{UF,0},{UL,0},{UB,0},{BR,0},{DF,0},{DL,0},{DB,0},{DR,0},{FL,0},{BL,0},{UR,0}}},   
 {{{UFL,1},{DLF,2},{ULB,0},{UBR,0},{URF,2},{DFR,1},{DBL,0},{DRB,0}},   //F
 {{UR,0},{FL,1},{UL,0},{UB,0},{DR,0},{FR,1},{DL,0},{DB,0},{UF,1},{DF,1},{BL,0},{BR,0}}}, 
 {{{URF,0},{UFL,0},{ULB,0},{UBR,0},{DLF,0},{DBL,0},{DRB,0},{DFR,0}},   //D
 {{UR,0},{UF,0},{UL,0},{UB,0},{DF,0},{DL,0},{DB,0},{DR,0},{FR,0},{FL,0},{BL,0},{BR,0}}},
 {{{URF,0},{ULB,1},{DBL,2},{UBR,0},{DFR,0},{UFL,2},{DLF,1},{DRB,0}},   //L
 {{UR,0},{UF,0},{BL,0},{UB,0},{DR,0},{DF,0},{FL,0},{DB,0},{FR,0},{UL,0},{DL,0},{BR,0}}},
 {{{URF,0},{UFL,0},{UBR,1},{DRB,2},{DFR,0},{DLF,0},{ULB,2},{DBL,1}},   //B
 {{UR,0},{UF,0},{UL,0},{BR,1},{DR,0},{DF,0},{DL,0},{BL,1},{FR,0},{FL,0},{UB,1},{DB,1}}}
};  
const Facelet CornerFacelet[8][3]={
	{U9,R1,F3},{U7,F1,L3},{U1,L1,B3},{U3,B1,R3},
	{D3,F9,R7},{D1,L9,F7},{D7,B9,L7},{D9,R9,B7}
};
	
const Facelet EdgeFacelet[12][2]={
	{U6,R2},{U8,F2},{U4,L2},{U2,B2},{D6,R8},{D2,F8},{D4,L8},
	{D8,B8},{F6,R4},{F4,L6},{B6,L4},{B4,R6}
};
const CubieCube kIdCube={
	{{URF,0},{UFL,0},{ULB,0},{UBR,0},{DFR,0},{DLF,0},{DBL,0},{DRB,0}},
	{{UR,0},{UF,0},{UL,0},{UB,0},{DR,0},{DF,0},
	{DL,0},{DB,0},{FR,0},{FL,0},{BL,0},{BR,0}}
};
const Facelet SingmasterToFacelet[48]={
	U8,F2,U6,R2,U2,B2,U4,L2,D2,F8,D6,R8,D8,B8,D4,L8,F6,R4,F4,L6,B4,R6,B6,L4,
	U9,F3,R1,U3,R3,B1,U1,B3,L1,U7,L3,F1,D3,R7,F9,D1,F7,L9,D7,L7,B9,D9,B7,R9};	
	
const char ColorTable[]="URFDLB"; 
} //Namespace
CubieCube CubeAxMove(CubieCube cc,Axis ax){
    CubieCube result;
    tk_gjz010_rubik_cubiecube::CubieCubeMultiply(&cc,&tk_gjz010_rubik_cubiecube::BasicCubeMove[ax],&result);
    return result;
};
void InitMoveCubes(){
    CubieCube cc;
    for(Axis ax=U;ax<=B;ax=(Axis)(ax+1)){
        cc=tk_gjz010_rubik_cubiecube::BasicCubeMove[ax];
        MoveCube[ax*2]=cc;
        CubeAxMove(cc,ax);
        MoveCube[ax*2+1]=CubeAxMove(cc,ax);
    }
};
CubieCube FaceletCubeToCubieCube(FaceletCube fc){
    CubieCube result;
    for(Corner corn=URF;corn<=DRB;corn=(Corner)(corn+1)){
        int ori;
	    for (ori=0;ori<3;ori++)
	    if (fc.f[tk_gjz010_rubik_cubiecube::CornerFacelet[corn][ori]]==U || fc.f[tk_gjz010_rubik_cubiecube::CornerFacelet[corn][ori]]==D) break;
	    Color c1=fc.f[tk_gjz010_rubik_cubiecube::CornerFacelet[corn][(ori+1)%3]];
	    Color c2=fc.f[tk_gjz010_rubik_cubiecube::CornerFacelet[corn][(ori+2)%3]];
	
	    for(Corner corn2=URF;corn2<=DRB;corn2=(Corner)(corn2+1)){
		    if (c1==(Color)(tk_gjz010_rubik_cubiecube::CornerFacelet[corn2][1]/9)&&
		        c2==(Color)(tk_gjz010_rubik_cubiecube::CornerFacelet[corn2][2]/9))
		    {
			    result.co[corn].c = corn2;
			    result.co[corn].o = ori;
			    break;
		    }
	    }
	}
    for(Edge ed=UR;ed<=BR;ed=(Edge)(ed+1))
        for(Edge ed2=UR;ed2<=BR;ed2=(Edge)(ed2+1)){
	        if (fc.f[tk_gjz010_rubik_cubiecube::EdgeFacelet[ed][0]]==(Color)(tk_gjz010_rubik_cubiecube::EdgeFacelet[ed2][0]/9) &&
		    fc.f[tk_gjz010_rubik_cubiecube::EdgeFacelet[ed][1]]==(Color)(tk_gjz010_rubik_cubiecube::EdgeFacelet[ed2][1]/9)){
		        result.eo[ed].e = ed2;
		        result.eo[ed].o = 0;
		        break;	
	        }
	        if (fc.f[tk_gjz010_rubik_cubiecube::EdgeFacelet[ed][0]]==(Color)(tk_gjz010_rubik_cubiecube::EdgeFacelet[ed2][1]/9) &&
		    fc.f[tk_gjz010_rubik_cubiecube::EdgeFacelet[ed][1]]==(Color)(tk_gjz010_rubik_cubiecube::EdgeFacelet[ed2][0]/9)){
		        result.eo[ed].e = ed2;
		        result.eo[ed].o = 1;
		        break;	
	        }
        }
    return result;
};
FaceletCube CubieCubeToFaceletCube(CubieCube cc){
    FaceletCube result;
    for(Corner corn=URF;corn<=DRB;corn=(Corner)(corn+1)){
        Corner orig=cc.co[corn].c;
	    char ori=cc.co[corn].o;
	    for (int n=0;n<3;n++) result.f[tk_gjz010_rubik_cubiecube::CornerFacelet[corn][(n+ori)%3]]=(Color)(tk_gjz010_rubik_cubiecube::CornerFacelet[orig][n]/9);
    }
    for(Edge ed=UR;ed<=BR;ed=(Edge)(ed+1)){
        Edge orig = cc.eo[ed].e;
	    char ori = cc.eo[ed].o;
	    for (int n=0;n<2;n++) result.f[tk_gjz010_rubik_cubiecube::EdgeFacelet[ed][(n+ori)%2]] = (Color)(tk_gjz010_rubik_cubiecube::EdgeFacelet[orig][n]/9);
    }
    return result;
};
//TODO(gjz010):understand the mysterious representation and rewrite it.
CubieCube StringToCubieCube(char* def_string){
	char *sp;
	int i, j, n,pow, fCount[7] = {0,0,0,0,0,0,0};
	CubieCube ccRet=tk_gjz010_rubik_cubiecube::kIdCube;
	FaceletCube fc;
	Axis ax;
	
	sp= strstr(def_string,"(");
	if (sp!=NULL) *sp=0;//remove maneuver length if present	
	sp= strstr(def_string,"//");
	if (sp!=NULL) *sp=0;//remove comment if present
	n = strlen(def_string);
	for (i=0;i<n;i++)
	{
		switch (toupper(def_string[i]))//check if definition string has Singmaster notation
		{
			case 'U': fCount[0]++; break;
			case 'R': fCount[1]++; break;
			case 'F': fCount[2]++; break;
			case 'D': fCount[3]++; break;
			case 'L': fCount[4]++; break;
			case 'B': fCount[5]++; break;
			case '\'':case '2': fCount[6]++; break;
		}
	}
	if (fCount[0]==8 && fCount[1]==8 && fCount[2]==8 && fCount[3]==8 && 
		fCount[4]==8 && fCount[5]==8 && fCount[6]==0)
	{//Singmaster notation
		j = 0;
		for (i=0;i<n;i++)	
		switch (toupper(def_string[i]))
		{
			case 'U': fc.f[tk_gjz010_rubik_cubiecube::SingmasterToFacelet[j++]] = U; break;
			case 'R': fc.f[tk_gjz010_rubik_cubiecube::SingmasterToFacelet[j++]] = R; break;
			case 'F': fc.f[tk_gjz010_rubik_cubiecube::SingmasterToFacelet[j++]] = F; break;
			case 'D': fc.f[tk_gjz010_rubik_cubiecube::SingmasterToFacelet[j++]] = D; break;
			case 'L': fc.f[tk_gjz010_rubik_cubiecube::SingmasterToFacelet[j++]] = L; break;
			case 'B': fc.f[tk_gjz010_rubik_cubiecube::SingmasterToFacelet[j++]] = B; break;
		}
		ccRet = FaceletCubeToCubieCube(fc);
	}
	else
	{//maneuver notation
		ax = U;
		for (i=0;i<n;i++)
		{
			switch (toupper(def_string[i]))
			{
				case 'U': ax = U; break;
				case 'R': ax = R; break;
				case 'F': ax = F; break;
				case 'D': ax = D; break;
				case 'L': ax = L; break;
				case 'B': ax = B; break;
				default: continue;	
			}
			if (i==n) {ccRet = CubeAxMove(ccRet,ax); break;}
			switch (def_string[i+1])
			{
				case '3':case'\'':case'-': pow = 3; break;
				case '2': pow = 2; break;
				default: pow = 1;
			}	
			for (j=0;j<pow;j++)
			ccRet = CubeAxMove(ccRet,ax);		
		}		
	}
	return ccRet;
};
void CubieCubeToString(CubieCube cc, char* def_string){
    FaceletCube fc = CubieCubeToFaceletCube(cc);	
    int i,j=0;
    for (i=0;i<48;i++){
	    def_string[i+j] = tk_gjz010_rubik_cubiecube::ColorTable[fc.f[tk_gjz010_rubik_cubiecube::SingmasterToFacelet[i]]];
	    if ((i<24 && i&1) || (i>24 && (i+1)%3==0)){
	        j++;
	        def_string[i+j]=' ';
	    }	
    }
    def_string[i+j-1]=0;	
};
CubieCube InvCubieCube(CubieCube cc){
    CubieCube result;
    for(Edge ed=UR;ed<=BR;ed=(Edge)(ed+1)) result.eo[cc.eo[ed].e].e=ed;
    for(Edge ed=UR;ed<=BR;ed=(Edge)(ed+1)) result.eo[ed].o=cc.eo[result.eo[ed].e].o;
    for(Corner corn=URF;corn<=DRB;corn=(Corner)(corn+1)) result.co[cc.co[corn].c].c=corn;
    int ori;
    for(Corner corn=URF;corn<=DRB;corn=(Corner)(corn+1)){
        ori=cc.co[result.co[corn].c].o;
        if (ori>=3) result.co[corn].o = ori;
	    else{
		    result.co[corn].o = -ori;
		    if (result.co[corn].o<0) result.co[corn].o+=3;
	    }
    }
    return result;
}

