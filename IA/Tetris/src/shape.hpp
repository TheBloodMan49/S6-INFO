#pragma once

static const int NB_KINDS = 7;
static const int NB_ROTATIONS = 4;
static const int SIZE = 4;

enum kind { O_KIND, I_KIND, S_KIND, Z_KIND, L_KIND, J_KIND, T_KIND };

extern int PIECES[NB_KINDS][NB_ROTATIONS][SIZE][SIZE];
