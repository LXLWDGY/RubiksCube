#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <inttypes.h>
#include "cube_defs.h"

namespace Pruning {
	char *visitedA, *visitedB;
	char save_file[] = "prune_table";
	const short int MovesDefault[12] = {
		0xffd, 0xffc, 0xff7, 0xff3, 0xfdf, 0xfcf, 0xf7c, 0xf3c, 0xdf3, 0xcf3, 0x7cf, 0x3cf
	};
	
	void GenTable(FILE *PruneTable) {
		visitedA = (char *)calloc(kGoal / 8 + 1, 1);
		visitedB = (char *)calloc(kGoal / 8 + 1, 1);

		printf("\nGenerating pruning table for the first time.\n");
		fflush(stdout);

		unsigned long int entry_count = 1;
		int flip_slice, flip_slice_new, twist, twist_new, parity, parity_new, index, index_new, sym_flip_slice_new, sym_new;
		Move next_move;
		visitedA[0] = 1; visitedB[0] = 1;
		while (entry_count < kGoal) {
			for (flip_slice = 0; flip_slice < kFlipSlice; ++flip_slice)
				for (twist = 0; twist < kTwist; ++twist)
					for (parity = 0; parity < 2; ++parity) {
						index = ((kTwist * flip_slice + twist) << 1) + parity;
						if (visitedA[index >> 3] & (1 << (index & 7))) continue;
						else {
							for (next_move = mU1; next_move <= mB3; next_move = (Move)(next_move + 1)) {
								sym_flip_slice_new = SymFlipSliceClassMove[flip_slice][next_move];
								sym_new = sym_flip_slice_new & 15;
								flip_slice_new = sym_flip_slice_new >> 4;
								parity_new = parity ^ 1;
								twist_new = TwistConjugate[TwistMove[twist][next_move]][sym_new];
								index_new = ((kTwist * flip_slice_new + twist_new) << 1) + parity_new;
								if (visitedA[index_new >> 3] & 1 << (index_new & 7)) {
									MovesCloserToTarget[twist][(flip_slice << 1) + parity] |= (1 << next_move);
									printf("MovesCloserToTarget[%d][%d] |= (%d);\n",twist,(flip_slice << 1) + parity,1 << next_move);
									if (!(visitedB[index >> 3] & 1 << (index & 7))) {
										visitedB[index >> 3] |= (1 << index & 7);
										++entry_count;
									}
								}
							}
						}
					}
			for (int it = 0; it < kGoal / 8 + 1; ++it) visitedA[it] |= visitedB[it];
			printf("%9lu of %u entries done.\n", entry_count, kGoal);
			fflush(stdout);
		}

		printf("\nSaving pruning table to disk...");
		fflush(stdout);
		PruneTable = fopen(save_file, "w+b");
		for (int it = 0; it < kTwist; ++it) fwrite(MovesCloserToTarget[it], kFlipSlice * 4, 1, PruneTable);
		fclose(PruneTable);

		free(visitedA); free(visitedB);
		printf("Pruning table generation complete.\n");
		fflush(stdout);
	}
	void LoadTable(FILE *PruneTable) {
		printf("\nLoading pruning table from disk...");
		for (int it = 0; it < kTwist; it++) {
			if (it % 200 == 0) pp();
			fread(MovesCloserToTarget[it], kFlipSlice * 4, 1, PruneTable);
		}
		fclose(PruneTable);
	}
};

using namespace Pruning;

void InitMovesCloserToTarget() {
	FILE *PruneTable;
	for (int it = 0; it < kTwist; ++it) MovesCloserToTarget[it] = (short *)calloc(kFlipSlice * 2, 2);

	PruneTable = fopen(save_file, "r+b");
	if (PruneTable == NULL) GenTable(PruneTable);
	else {
		fseek(PruneTable, 0, SEEK_END);
		if (ftell(PruneTable) == kFlipSlice * kTwist * 4)
			LoadTable(PruneTable);
		else {
			fclose(PruneTable);
			GenTable(PruneTable);
		}
	}
}

void InitMoveBitsConjugate() {
	for (int bits = 0; bits < kMBits; ++bits)
		for (int sym = 0; sym < kSym_Oh; ++sym) {
			MoveBitsConjugate[bits][sym] = 0;
			for (Move move = mU1; move <= mB3; move = (Move)(move + 1))
				if (bits & 1 << move)
					MoveBitsConjugate[bits][sym] |= (1 << MoveConjugate[move][InvSymIdx[sym]]);
		}
}

int DistanceToTarget(CoordCube co) {
	int sym_flip_slice = co.sym_flip_slice;
	int flip_slice = sym_flip_slice >> 4;
	int sym = sym_flip_slice & 15;
	int twist = co.twist;
	int twist_conj = TwistConjugate[twist][sym];
	int parity = co.parity;
	int dist = 0;

	while (1) {
		short int move_bits = MovesCloserToTarget[twist_conj][(flip_slice << 1) + parity];
		if (move_bits == 0) break;
		move_bits = MoveBitsConjugate[move_bits][sym];
		for (short int move = 0; move < kMove; ++move)
			if (move_bits & 1 << move) {
				++dist;
				sym_flip_slice = SymFlipSliceMove(sym_flip_slice, move);
				sym = sym_flip_slice & 15;
				flip_slice = sym_flip_slice >> 4;
				twist = TwistMove[twist][move];
				twist_conj = TwistConjugate[twist][sym];
				parity ^= 1;
				break;
			}
	}

	return dist;
}

void InitNextMove() {
	for (int move_bits = 0; move_bits < kMBits; ++move_bits)
		for (int move = 0; move <= kMove; ++move)
			NextMove[move_bits][move] = -1;
	for (int move_bits = 0; move_bits < kMBits; ++move_bits)
		for (int move = 0; move < kMove; ++move)
			for (int next_move = move; next_move < kMove; ++next_move)
				if (move_bits & (1 << next_move)) {
					NextMove[move_bits][move] = next_move;
					break;
				}
}

void SolveOptimal(CubieCube C) {
	int move_conj_U, move_conj_R, move_conj_F, twist_conj_U, twist_conj_R, twist_conj_F, bits;
	CoordCube CU, CR, CF;
	int r_depth, OptimalDist = 30;
	short int move;
	SearchNode stack[kNode];
	SearchNode *P, *P_new;
	int man_len;
	unsigned long long int sym, sym_new, node_count = 0, test_count = 0;

	sym = GetSymmetries(C);
	if (sym == 1)
		printf("Cube has no symmetry.\n");
	else
		printf("Cube has %u symmetries.\n", SymCount(sym));

	fflush(stdout);

	CU = CubieCubeToCoordCube(C);
	C = EdgeConjugate(C, 16);
	C = CornerConjugate(C, 16);
	CR = CubieCubeToCoordCube(C);
	C = EdgeConjugate(C, 16);
	C = CornerConjugate(C, 16);
	CF = CubieCubeToCoordCube(C);

	P = stack;
	P->dist_U = DistanceToTarget(CU);
	P->dist_R = DistanceToTarget(CR);
	P->dist_F = DistanceToTarget(CF);
	P->flip_slice_U = CU.sym_flip_slice >> 4;
	P->flip_slice_R = CR.sym_flip_slice >> 4;
	P->flip_slice_F = CF.sym_flip_slice >> 4;
	P->parity = CU.parity;
	P->twist_U = CU.twist;
	P->twist_R = CR.twist;
	P->twist_F = CF.twist;
	P->corn_6_pos = CU.corn_6_pos;
	P->edge_6_pos = CU.edge_6_pos;
	P->edge_4_pos = CU.edge_4_pos;

	for (int it = 0; it < kNode; ++it) stack[it].move = mU1 - 1;
	twist_conj_U = TwistConjugate[P->twist_U][P->sym_U];
	twist_conj_R = TwistConjugate[P->twist_R][P->sym_R];
	twist_conj_F = TwistConjugate[P->twist_F][P->sym_F];

	P->moves_closer_target_U = MoveBitsConjugate[MovesCloserToTarget[twist_conj_U][(P->flip_slice_U << 1) + P->parity]][P->sym_U];
	P->moves_closer_target_R = MoveBitsConjugate[MovesCloserToTarget[twist_conj_R][(P->flip_slice_R << 1) + P->parity]][SymIdxMultiply[P->sym_R][16]];
	P->moves_closer_target_F = MoveBitsConjugate[MovesCloserToTarget[twist_conj_F][(P->flip_slice_F << 1) + P->parity]][SymIdxMultiply[P->sym_F][32]];

	P->moves_allowed = 0xfff;
	P->m_sym = sym;

	if (P->dist_U == 0 && P->dist_R == 0 && P->dist_F == 0)
		r_depth = man_len = 2;
	else {
		man_len = P->dist_U;
		if (P->dist_R > man_len) man_len = P->dist_R;
		if (P->dist_F > man_len) man_len = P->dist_F;
		r_depth = man_len;

		if (P->dist_U == man_len) P->moves_allowed &= P->moves_closer_target_U;
		if (P->dist_R == man_len) P->moves_allowed &= P->moves_closer_target_R;
		if (P->dist_F == man_len) P->moves_allowed &= P->moves_closer_target_F;
	}

	while (1) {
		P->move = NextMove[P->moves_allowed][++(P->move)];
		if (P->move == -1) {
			if (r_depth == man_len) {
				printf("depth %2u completed, %14I64u nodes, %14I64u tests\n", man_len, node_count, test_count);
			}
			else  {
				++r_depth;
				--P;
			}
			continue;
		}

		move = P->move;
		sym_new = P->m_sym;
		if ((sym_new & GESymmetries[move]) != sym_new) continue;
		if (man_len - r_depth >= 2)
			if (move == (P - 1)->move && move == (P - 2) -> move) continue;
		P_new = P + 1;

		P_new->edge_6_pos = Edge6PosMove[P->edge_6_pos][move];
		P_new->edge_4_pos = Edge4PosMove[P->edge_4_pos][move];
		P_new->corn_6_pos = Corn6PosMove[P->corn_6_pos][move];

		if (r_depth == 1) {
			++test_count;
			if (P_new->corn_6_pos == 0 && P_new->edge_6_pos == 0 && P_new->edge_4_pos == 0) {
				if (OptimalDist > man_len) OptimalDist = man_len;
				char *mv[] = {"U", "U'", "R", "R'", "F", "F'", "D", "D'", "L", "L'", "B", "B'"};
				for (int it = 0; it < man_len; ++it) printf("%s ", mv[stack[it].move]);
				printf(" (%uq*)\n", man_len);
				printf("\n");
				return;
			}
			continue;
		}

		P_new->dist_U = P->dist_U + ((P->moves_closer_target_U & (1 << move))? -1 : 1);
		P_new->dist_R = P->dist_R + ((P->moves_closer_target_R & (1 << move))? -1 : 1);
		P_new->dist_F = P->dist_F + ((P->moves_closer_target_F & (1 << move))? -1 : 1);
		P_new->parity = P->parity ^ 1;
		P_new->m_sym = P->m_sym & ESymmetries[move];
		
		move_conj_U = MoveConjugate[move][P->sym_U];
		bits = SymFlipSliceClassMove[P->flip_slice_U][move_conj_U];
		P_new->flip_slice_U = bits >> 4;
		P_new->sym_U = SymIdxMultiply[bits & 15][P->sym_U];
		P_new->twist_U = TwistMove[P->twist_U][move];

		move = MoveConjugate[move][16];
		move_conj_R = MoveConjugate[move][P->sym_R];
		bits = SymFlipSliceClassMove[P->flip_slice_R][move_conj_R];
		P_new->flip_slice_R = bits >> 4;
		P_new->sym_R = SymIdxMultiply[bits & 15][P->sym_R];
		P_new->twist_R = TwistMove[P->twist_R][move];
		
		move = MoveConjugate[move][16];
		move_conj_F = MoveConjugate[move][P->sym_F];
		bits = SymFlipSliceClassMove[P->flip_slice_F][move_conj_F];
		P_new->flip_slice_F = bits >> 4;
		P_new->sym_F = SymIdxMultiply[bits & 15][P->sym_F];
		P_new->twist_F = TwistMove[P->twist_F][move];

		twist_conj_U = TwistConjugate[P_new->twist_U][P_new->sym_U];
		twist_conj_R = TwistConjugate[P_new->twist_R][P_new->sym_R];
		twist_conj_F = TwistConjugate[P_new->twist_F][P_new->sym_F];

		P_new->moves_closer_target_U = MoveBitsConjugate[MovesCloserToTarget[twist_conj_U][(P_new->flip_slice_U << 1) + P_new->parity]][P_new->sym_U];
		P_new->moves_closer_target_R = MoveBitsConjugate[MovesCloserToTarget[twist_conj_R][(P_new->flip_slice_R << 1) + P_new->parity]][P_new->sym_R];
		P_new->moves_closer_target_F = MoveBitsConjugate[MovesCloserToTarget[twist_conj_F][(P_new->flip_slice_F << 1) + P_new->parity]][P_new->sym_F];

		P_new->moves_allowed = MovesDefault[P->move];
		--r_depth;
		if (P_new->dist_U == r_depth) P_new->moves_allowed &= P_new->moves_closer_target_U;
		if (P_new->dist_R == r_depth) P_new->moves_allowed &= P_new->moves_closer_target_R;
		if (P_new->dist_F == r_depth) P_new->moves_allowed &= P_new->moves_closer_target_F;

		++node_count; ++P;
	}
}
