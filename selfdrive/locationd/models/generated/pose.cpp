#include "pose.h"

namespace {
#define DIM 18
#define EDIM 18
#define MEDIM 18
typedef void (*Hfun)(double *, double *, double *);
const static double MAHA_THRESH_4 = 7.814727903251177;
const static double MAHA_THRESH_10 = 7.814727903251177;
const static double MAHA_THRESH_13 = 7.814727903251177;
const static double MAHA_THRESH_14 = 7.814727903251177;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_3585092682791090136) {
   out_3585092682791090136[0] = delta_x[0] + nom_x[0];
   out_3585092682791090136[1] = delta_x[1] + nom_x[1];
   out_3585092682791090136[2] = delta_x[2] + nom_x[2];
   out_3585092682791090136[3] = delta_x[3] + nom_x[3];
   out_3585092682791090136[4] = delta_x[4] + nom_x[4];
   out_3585092682791090136[5] = delta_x[5] + nom_x[5];
   out_3585092682791090136[6] = delta_x[6] + nom_x[6];
   out_3585092682791090136[7] = delta_x[7] + nom_x[7];
   out_3585092682791090136[8] = delta_x[8] + nom_x[8];
   out_3585092682791090136[9] = delta_x[9] + nom_x[9];
   out_3585092682791090136[10] = delta_x[10] + nom_x[10];
   out_3585092682791090136[11] = delta_x[11] + nom_x[11];
   out_3585092682791090136[12] = delta_x[12] + nom_x[12];
   out_3585092682791090136[13] = delta_x[13] + nom_x[13];
   out_3585092682791090136[14] = delta_x[14] + nom_x[14];
   out_3585092682791090136[15] = delta_x[15] + nom_x[15];
   out_3585092682791090136[16] = delta_x[16] + nom_x[16];
   out_3585092682791090136[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_3602092163882833613) {
   out_3602092163882833613[0] = -nom_x[0] + true_x[0];
   out_3602092163882833613[1] = -nom_x[1] + true_x[1];
   out_3602092163882833613[2] = -nom_x[2] + true_x[2];
   out_3602092163882833613[3] = -nom_x[3] + true_x[3];
   out_3602092163882833613[4] = -nom_x[4] + true_x[4];
   out_3602092163882833613[5] = -nom_x[5] + true_x[5];
   out_3602092163882833613[6] = -nom_x[6] + true_x[6];
   out_3602092163882833613[7] = -nom_x[7] + true_x[7];
   out_3602092163882833613[8] = -nom_x[8] + true_x[8];
   out_3602092163882833613[9] = -nom_x[9] + true_x[9];
   out_3602092163882833613[10] = -nom_x[10] + true_x[10];
   out_3602092163882833613[11] = -nom_x[11] + true_x[11];
   out_3602092163882833613[12] = -nom_x[12] + true_x[12];
   out_3602092163882833613[13] = -nom_x[13] + true_x[13];
   out_3602092163882833613[14] = -nom_x[14] + true_x[14];
   out_3602092163882833613[15] = -nom_x[15] + true_x[15];
   out_3602092163882833613[16] = -nom_x[16] + true_x[16];
   out_3602092163882833613[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_9072779523166268742) {
   out_9072779523166268742[0] = 1.0;
   out_9072779523166268742[1] = 0.0;
   out_9072779523166268742[2] = 0.0;
   out_9072779523166268742[3] = 0.0;
   out_9072779523166268742[4] = 0.0;
   out_9072779523166268742[5] = 0.0;
   out_9072779523166268742[6] = 0.0;
   out_9072779523166268742[7] = 0.0;
   out_9072779523166268742[8] = 0.0;
   out_9072779523166268742[9] = 0.0;
   out_9072779523166268742[10] = 0.0;
   out_9072779523166268742[11] = 0.0;
   out_9072779523166268742[12] = 0.0;
   out_9072779523166268742[13] = 0.0;
   out_9072779523166268742[14] = 0.0;
   out_9072779523166268742[15] = 0.0;
   out_9072779523166268742[16] = 0.0;
   out_9072779523166268742[17] = 0.0;
   out_9072779523166268742[18] = 0.0;
   out_9072779523166268742[19] = 1.0;
   out_9072779523166268742[20] = 0.0;
   out_9072779523166268742[21] = 0.0;
   out_9072779523166268742[22] = 0.0;
   out_9072779523166268742[23] = 0.0;
   out_9072779523166268742[24] = 0.0;
   out_9072779523166268742[25] = 0.0;
   out_9072779523166268742[26] = 0.0;
   out_9072779523166268742[27] = 0.0;
   out_9072779523166268742[28] = 0.0;
   out_9072779523166268742[29] = 0.0;
   out_9072779523166268742[30] = 0.0;
   out_9072779523166268742[31] = 0.0;
   out_9072779523166268742[32] = 0.0;
   out_9072779523166268742[33] = 0.0;
   out_9072779523166268742[34] = 0.0;
   out_9072779523166268742[35] = 0.0;
   out_9072779523166268742[36] = 0.0;
   out_9072779523166268742[37] = 0.0;
   out_9072779523166268742[38] = 1.0;
   out_9072779523166268742[39] = 0.0;
   out_9072779523166268742[40] = 0.0;
   out_9072779523166268742[41] = 0.0;
   out_9072779523166268742[42] = 0.0;
   out_9072779523166268742[43] = 0.0;
   out_9072779523166268742[44] = 0.0;
   out_9072779523166268742[45] = 0.0;
   out_9072779523166268742[46] = 0.0;
   out_9072779523166268742[47] = 0.0;
   out_9072779523166268742[48] = 0.0;
   out_9072779523166268742[49] = 0.0;
   out_9072779523166268742[50] = 0.0;
   out_9072779523166268742[51] = 0.0;
   out_9072779523166268742[52] = 0.0;
   out_9072779523166268742[53] = 0.0;
   out_9072779523166268742[54] = 0.0;
   out_9072779523166268742[55] = 0.0;
   out_9072779523166268742[56] = 0.0;
   out_9072779523166268742[57] = 1.0;
   out_9072779523166268742[58] = 0.0;
   out_9072779523166268742[59] = 0.0;
   out_9072779523166268742[60] = 0.0;
   out_9072779523166268742[61] = 0.0;
   out_9072779523166268742[62] = 0.0;
   out_9072779523166268742[63] = 0.0;
   out_9072779523166268742[64] = 0.0;
   out_9072779523166268742[65] = 0.0;
   out_9072779523166268742[66] = 0.0;
   out_9072779523166268742[67] = 0.0;
   out_9072779523166268742[68] = 0.0;
   out_9072779523166268742[69] = 0.0;
   out_9072779523166268742[70] = 0.0;
   out_9072779523166268742[71] = 0.0;
   out_9072779523166268742[72] = 0.0;
   out_9072779523166268742[73] = 0.0;
   out_9072779523166268742[74] = 0.0;
   out_9072779523166268742[75] = 0.0;
   out_9072779523166268742[76] = 1.0;
   out_9072779523166268742[77] = 0.0;
   out_9072779523166268742[78] = 0.0;
   out_9072779523166268742[79] = 0.0;
   out_9072779523166268742[80] = 0.0;
   out_9072779523166268742[81] = 0.0;
   out_9072779523166268742[82] = 0.0;
   out_9072779523166268742[83] = 0.0;
   out_9072779523166268742[84] = 0.0;
   out_9072779523166268742[85] = 0.0;
   out_9072779523166268742[86] = 0.0;
   out_9072779523166268742[87] = 0.0;
   out_9072779523166268742[88] = 0.0;
   out_9072779523166268742[89] = 0.0;
   out_9072779523166268742[90] = 0.0;
   out_9072779523166268742[91] = 0.0;
   out_9072779523166268742[92] = 0.0;
   out_9072779523166268742[93] = 0.0;
   out_9072779523166268742[94] = 0.0;
   out_9072779523166268742[95] = 1.0;
   out_9072779523166268742[96] = 0.0;
   out_9072779523166268742[97] = 0.0;
   out_9072779523166268742[98] = 0.0;
   out_9072779523166268742[99] = 0.0;
   out_9072779523166268742[100] = 0.0;
   out_9072779523166268742[101] = 0.0;
   out_9072779523166268742[102] = 0.0;
   out_9072779523166268742[103] = 0.0;
   out_9072779523166268742[104] = 0.0;
   out_9072779523166268742[105] = 0.0;
   out_9072779523166268742[106] = 0.0;
   out_9072779523166268742[107] = 0.0;
   out_9072779523166268742[108] = 0.0;
   out_9072779523166268742[109] = 0.0;
   out_9072779523166268742[110] = 0.0;
   out_9072779523166268742[111] = 0.0;
   out_9072779523166268742[112] = 0.0;
   out_9072779523166268742[113] = 0.0;
   out_9072779523166268742[114] = 1.0;
   out_9072779523166268742[115] = 0.0;
   out_9072779523166268742[116] = 0.0;
   out_9072779523166268742[117] = 0.0;
   out_9072779523166268742[118] = 0.0;
   out_9072779523166268742[119] = 0.0;
   out_9072779523166268742[120] = 0.0;
   out_9072779523166268742[121] = 0.0;
   out_9072779523166268742[122] = 0.0;
   out_9072779523166268742[123] = 0.0;
   out_9072779523166268742[124] = 0.0;
   out_9072779523166268742[125] = 0.0;
   out_9072779523166268742[126] = 0.0;
   out_9072779523166268742[127] = 0.0;
   out_9072779523166268742[128] = 0.0;
   out_9072779523166268742[129] = 0.0;
   out_9072779523166268742[130] = 0.0;
   out_9072779523166268742[131] = 0.0;
   out_9072779523166268742[132] = 0.0;
   out_9072779523166268742[133] = 1.0;
   out_9072779523166268742[134] = 0.0;
   out_9072779523166268742[135] = 0.0;
   out_9072779523166268742[136] = 0.0;
   out_9072779523166268742[137] = 0.0;
   out_9072779523166268742[138] = 0.0;
   out_9072779523166268742[139] = 0.0;
   out_9072779523166268742[140] = 0.0;
   out_9072779523166268742[141] = 0.0;
   out_9072779523166268742[142] = 0.0;
   out_9072779523166268742[143] = 0.0;
   out_9072779523166268742[144] = 0.0;
   out_9072779523166268742[145] = 0.0;
   out_9072779523166268742[146] = 0.0;
   out_9072779523166268742[147] = 0.0;
   out_9072779523166268742[148] = 0.0;
   out_9072779523166268742[149] = 0.0;
   out_9072779523166268742[150] = 0.0;
   out_9072779523166268742[151] = 0.0;
   out_9072779523166268742[152] = 1.0;
   out_9072779523166268742[153] = 0.0;
   out_9072779523166268742[154] = 0.0;
   out_9072779523166268742[155] = 0.0;
   out_9072779523166268742[156] = 0.0;
   out_9072779523166268742[157] = 0.0;
   out_9072779523166268742[158] = 0.0;
   out_9072779523166268742[159] = 0.0;
   out_9072779523166268742[160] = 0.0;
   out_9072779523166268742[161] = 0.0;
   out_9072779523166268742[162] = 0.0;
   out_9072779523166268742[163] = 0.0;
   out_9072779523166268742[164] = 0.0;
   out_9072779523166268742[165] = 0.0;
   out_9072779523166268742[166] = 0.0;
   out_9072779523166268742[167] = 0.0;
   out_9072779523166268742[168] = 0.0;
   out_9072779523166268742[169] = 0.0;
   out_9072779523166268742[170] = 0.0;
   out_9072779523166268742[171] = 1.0;
   out_9072779523166268742[172] = 0.0;
   out_9072779523166268742[173] = 0.0;
   out_9072779523166268742[174] = 0.0;
   out_9072779523166268742[175] = 0.0;
   out_9072779523166268742[176] = 0.0;
   out_9072779523166268742[177] = 0.0;
   out_9072779523166268742[178] = 0.0;
   out_9072779523166268742[179] = 0.0;
   out_9072779523166268742[180] = 0.0;
   out_9072779523166268742[181] = 0.0;
   out_9072779523166268742[182] = 0.0;
   out_9072779523166268742[183] = 0.0;
   out_9072779523166268742[184] = 0.0;
   out_9072779523166268742[185] = 0.0;
   out_9072779523166268742[186] = 0.0;
   out_9072779523166268742[187] = 0.0;
   out_9072779523166268742[188] = 0.0;
   out_9072779523166268742[189] = 0.0;
   out_9072779523166268742[190] = 1.0;
   out_9072779523166268742[191] = 0.0;
   out_9072779523166268742[192] = 0.0;
   out_9072779523166268742[193] = 0.0;
   out_9072779523166268742[194] = 0.0;
   out_9072779523166268742[195] = 0.0;
   out_9072779523166268742[196] = 0.0;
   out_9072779523166268742[197] = 0.0;
   out_9072779523166268742[198] = 0.0;
   out_9072779523166268742[199] = 0.0;
   out_9072779523166268742[200] = 0.0;
   out_9072779523166268742[201] = 0.0;
   out_9072779523166268742[202] = 0.0;
   out_9072779523166268742[203] = 0.0;
   out_9072779523166268742[204] = 0.0;
   out_9072779523166268742[205] = 0.0;
   out_9072779523166268742[206] = 0.0;
   out_9072779523166268742[207] = 0.0;
   out_9072779523166268742[208] = 0.0;
   out_9072779523166268742[209] = 1.0;
   out_9072779523166268742[210] = 0.0;
   out_9072779523166268742[211] = 0.0;
   out_9072779523166268742[212] = 0.0;
   out_9072779523166268742[213] = 0.0;
   out_9072779523166268742[214] = 0.0;
   out_9072779523166268742[215] = 0.0;
   out_9072779523166268742[216] = 0.0;
   out_9072779523166268742[217] = 0.0;
   out_9072779523166268742[218] = 0.0;
   out_9072779523166268742[219] = 0.0;
   out_9072779523166268742[220] = 0.0;
   out_9072779523166268742[221] = 0.0;
   out_9072779523166268742[222] = 0.0;
   out_9072779523166268742[223] = 0.0;
   out_9072779523166268742[224] = 0.0;
   out_9072779523166268742[225] = 0.0;
   out_9072779523166268742[226] = 0.0;
   out_9072779523166268742[227] = 0.0;
   out_9072779523166268742[228] = 1.0;
   out_9072779523166268742[229] = 0.0;
   out_9072779523166268742[230] = 0.0;
   out_9072779523166268742[231] = 0.0;
   out_9072779523166268742[232] = 0.0;
   out_9072779523166268742[233] = 0.0;
   out_9072779523166268742[234] = 0.0;
   out_9072779523166268742[235] = 0.0;
   out_9072779523166268742[236] = 0.0;
   out_9072779523166268742[237] = 0.0;
   out_9072779523166268742[238] = 0.0;
   out_9072779523166268742[239] = 0.0;
   out_9072779523166268742[240] = 0.0;
   out_9072779523166268742[241] = 0.0;
   out_9072779523166268742[242] = 0.0;
   out_9072779523166268742[243] = 0.0;
   out_9072779523166268742[244] = 0.0;
   out_9072779523166268742[245] = 0.0;
   out_9072779523166268742[246] = 0.0;
   out_9072779523166268742[247] = 1.0;
   out_9072779523166268742[248] = 0.0;
   out_9072779523166268742[249] = 0.0;
   out_9072779523166268742[250] = 0.0;
   out_9072779523166268742[251] = 0.0;
   out_9072779523166268742[252] = 0.0;
   out_9072779523166268742[253] = 0.0;
   out_9072779523166268742[254] = 0.0;
   out_9072779523166268742[255] = 0.0;
   out_9072779523166268742[256] = 0.0;
   out_9072779523166268742[257] = 0.0;
   out_9072779523166268742[258] = 0.0;
   out_9072779523166268742[259] = 0.0;
   out_9072779523166268742[260] = 0.0;
   out_9072779523166268742[261] = 0.0;
   out_9072779523166268742[262] = 0.0;
   out_9072779523166268742[263] = 0.0;
   out_9072779523166268742[264] = 0.0;
   out_9072779523166268742[265] = 0.0;
   out_9072779523166268742[266] = 1.0;
   out_9072779523166268742[267] = 0.0;
   out_9072779523166268742[268] = 0.0;
   out_9072779523166268742[269] = 0.0;
   out_9072779523166268742[270] = 0.0;
   out_9072779523166268742[271] = 0.0;
   out_9072779523166268742[272] = 0.0;
   out_9072779523166268742[273] = 0.0;
   out_9072779523166268742[274] = 0.0;
   out_9072779523166268742[275] = 0.0;
   out_9072779523166268742[276] = 0.0;
   out_9072779523166268742[277] = 0.0;
   out_9072779523166268742[278] = 0.0;
   out_9072779523166268742[279] = 0.0;
   out_9072779523166268742[280] = 0.0;
   out_9072779523166268742[281] = 0.0;
   out_9072779523166268742[282] = 0.0;
   out_9072779523166268742[283] = 0.0;
   out_9072779523166268742[284] = 0.0;
   out_9072779523166268742[285] = 1.0;
   out_9072779523166268742[286] = 0.0;
   out_9072779523166268742[287] = 0.0;
   out_9072779523166268742[288] = 0.0;
   out_9072779523166268742[289] = 0.0;
   out_9072779523166268742[290] = 0.0;
   out_9072779523166268742[291] = 0.0;
   out_9072779523166268742[292] = 0.0;
   out_9072779523166268742[293] = 0.0;
   out_9072779523166268742[294] = 0.0;
   out_9072779523166268742[295] = 0.0;
   out_9072779523166268742[296] = 0.0;
   out_9072779523166268742[297] = 0.0;
   out_9072779523166268742[298] = 0.0;
   out_9072779523166268742[299] = 0.0;
   out_9072779523166268742[300] = 0.0;
   out_9072779523166268742[301] = 0.0;
   out_9072779523166268742[302] = 0.0;
   out_9072779523166268742[303] = 0.0;
   out_9072779523166268742[304] = 1.0;
   out_9072779523166268742[305] = 0.0;
   out_9072779523166268742[306] = 0.0;
   out_9072779523166268742[307] = 0.0;
   out_9072779523166268742[308] = 0.0;
   out_9072779523166268742[309] = 0.0;
   out_9072779523166268742[310] = 0.0;
   out_9072779523166268742[311] = 0.0;
   out_9072779523166268742[312] = 0.0;
   out_9072779523166268742[313] = 0.0;
   out_9072779523166268742[314] = 0.0;
   out_9072779523166268742[315] = 0.0;
   out_9072779523166268742[316] = 0.0;
   out_9072779523166268742[317] = 0.0;
   out_9072779523166268742[318] = 0.0;
   out_9072779523166268742[319] = 0.0;
   out_9072779523166268742[320] = 0.0;
   out_9072779523166268742[321] = 0.0;
   out_9072779523166268742[322] = 0.0;
   out_9072779523166268742[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_8675559756142309217) {
   out_8675559756142309217[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_8675559756142309217[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_8675559756142309217[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_8675559756142309217[3] = dt*state[12] + state[3];
   out_8675559756142309217[4] = dt*state[13] + state[4];
   out_8675559756142309217[5] = dt*state[14] + state[5];
   out_8675559756142309217[6] = state[6];
   out_8675559756142309217[7] = state[7];
   out_8675559756142309217[8] = state[8];
   out_8675559756142309217[9] = state[9];
   out_8675559756142309217[10] = state[10];
   out_8675559756142309217[11] = state[11];
   out_8675559756142309217[12] = state[12];
   out_8675559756142309217[13] = state[13];
   out_8675559756142309217[14] = state[14];
   out_8675559756142309217[15] = state[15];
   out_8675559756142309217[16] = state[16];
   out_8675559756142309217[17] = state[17];
}
void F_fun(double *state, double dt, double *out_4846751114242430980) {
   out_4846751114242430980[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4846751114242430980[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4846751114242430980[2] = 0;
   out_4846751114242430980[3] = 0;
   out_4846751114242430980[4] = 0;
   out_4846751114242430980[5] = 0;
   out_4846751114242430980[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4846751114242430980[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4846751114242430980[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4846751114242430980[9] = 0;
   out_4846751114242430980[10] = 0;
   out_4846751114242430980[11] = 0;
   out_4846751114242430980[12] = 0;
   out_4846751114242430980[13] = 0;
   out_4846751114242430980[14] = 0;
   out_4846751114242430980[15] = 0;
   out_4846751114242430980[16] = 0;
   out_4846751114242430980[17] = 0;
   out_4846751114242430980[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4846751114242430980[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4846751114242430980[20] = 0;
   out_4846751114242430980[21] = 0;
   out_4846751114242430980[22] = 0;
   out_4846751114242430980[23] = 0;
   out_4846751114242430980[24] = 0;
   out_4846751114242430980[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4846751114242430980[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4846751114242430980[27] = 0;
   out_4846751114242430980[28] = 0;
   out_4846751114242430980[29] = 0;
   out_4846751114242430980[30] = 0;
   out_4846751114242430980[31] = 0;
   out_4846751114242430980[32] = 0;
   out_4846751114242430980[33] = 0;
   out_4846751114242430980[34] = 0;
   out_4846751114242430980[35] = 0;
   out_4846751114242430980[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4846751114242430980[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4846751114242430980[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4846751114242430980[39] = 0;
   out_4846751114242430980[40] = 0;
   out_4846751114242430980[41] = 0;
   out_4846751114242430980[42] = 0;
   out_4846751114242430980[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4846751114242430980[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4846751114242430980[45] = 0;
   out_4846751114242430980[46] = 0;
   out_4846751114242430980[47] = 0;
   out_4846751114242430980[48] = 0;
   out_4846751114242430980[49] = 0;
   out_4846751114242430980[50] = 0;
   out_4846751114242430980[51] = 0;
   out_4846751114242430980[52] = 0;
   out_4846751114242430980[53] = 0;
   out_4846751114242430980[54] = 0;
   out_4846751114242430980[55] = 0;
   out_4846751114242430980[56] = 0;
   out_4846751114242430980[57] = 1;
   out_4846751114242430980[58] = 0;
   out_4846751114242430980[59] = 0;
   out_4846751114242430980[60] = 0;
   out_4846751114242430980[61] = 0;
   out_4846751114242430980[62] = 0;
   out_4846751114242430980[63] = 0;
   out_4846751114242430980[64] = 0;
   out_4846751114242430980[65] = 0;
   out_4846751114242430980[66] = dt;
   out_4846751114242430980[67] = 0;
   out_4846751114242430980[68] = 0;
   out_4846751114242430980[69] = 0;
   out_4846751114242430980[70] = 0;
   out_4846751114242430980[71] = 0;
   out_4846751114242430980[72] = 0;
   out_4846751114242430980[73] = 0;
   out_4846751114242430980[74] = 0;
   out_4846751114242430980[75] = 0;
   out_4846751114242430980[76] = 1;
   out_4846751114242430980[77] = 0;
   out_4846751114242430980[78] = 0;
   out_4846751114242430980[79] = 0;
   out_4846751114242430980[80] = 0;
   out_4846751114242430980[81] = 0;
   out_4846751114242430980[82] = 0;
   out_4846751114242430980[83] = 0;
   out_4846751114242430980[84] = 0;
   out_4846751114242430980[85] = dt;
   out_4846751114242430980[86] = 0;
   out_4846751114242430980[87] = 0;
   out_4846751114242430980[88] = 0;
   out_4846751114242430980[89] = 0;
   out_4846751114242430980[90] = 0;
   out_4846751114242430980[91] = 0;
   out_4846751114242430980[92] = 0;
   out_4846751114242430980[93] = 0;
   out_4846751114242430980[94] = 0;
   out_4846751114242430980[95] = 1;
   out_4846751114242430980[96] = 0;
   out_4846751114242430980[97] = 0;
   out_4846751114242430980[98] = 0;
   out_4846751114242430980[99] = 0;
   out_4846751114242430980[100] = 0;
   out_4846751114242430980[101] = 0;
   out_4846751114242430980[102] = 0;
   out_4846751114242430980[103] = 0;
   out_4846751114242430980[104] = dt;
   out_4846751114242430980[105] = 0;
   out_4846751114242430980[106] = 0;
   out_4846751114242430980[107] = 0;
   out_4846751114242430980[108] = 0;
   out_4846751114242430980[109] = 0;
   out_4846751114242430980[110] = 0;
   out_4846751114242430980[111] = 0;
   out_4846751114242430980[112] = 0;
   out_4846751114242430980[113] = 0;
   out_4846751114242430980[114] = 1;
   out_4846751114242430980[115] = 0;
   out_4846751114242430980[116] = 0;
   out_4846751114242430980[117] = 0;
   out_4846751114242430980[118] = 0;
   out_4846751114242430980[119] = 0;
   out_4846751114242430980[120] = 0;
   out_4846751114242430980[121] = 0;
   out_4846751114242430980[122] = 0;
   out_4846751114242430980[123] = 0;
   out_4846751114242430980[124] = 0;
   out_4846751114242430980[125] = 0;
   out_4846751114242430980[126] = 0;
   out_4846751114242430980[127] = 0;
   out_4846751114242430980[128] = 0;
   out_4846751114242430980[129] = 0;
   out_4846751114242430980[130] = 0;
   out_4846751114242430980[131] = 0;
   out_4846751114242430980[132] = 0;
   out_4846751114242430980[133] = 1;
   out_4846751114242430980[134] = 0;
   out_4846751114242430980[135] = 0;
   out_4846751114242430980[136] = 0;
   out_4846751114242430980[137] = 0;
   out_4846751114242430980[138] = 0;
   out_4846751114242430980[139] = 0;
   out_4846751114242430980[140] = 0;
   out_4846751114242430980[141] = 0;
   out_4846751114242430980[142] = 0;
   out_4846751114242430980[143] = 0;
   out_4846751114242430980[144] = 0;
   out_4846751114242430980[145] = 0;
   out_4846751114242430980[146] = 0;
   out_4846751114242430980[147] = 0;
   out_4846751114242430980[148] = 0;
   out_4846751114242430980[149] = 0;
   out_4846751114242430980[150] = 0;
   out_4846751114242430980[151] = 0;
   out_4846751114242430980[152] = 1;
   out_4846751114242430980[153] = 0;
   out_4846751114242430980[154] = 0;
   out_4846751114242430980[155] = 0;
   out_4846751114242430980[156] = 0;
   out_4846751114242430980[157] = 0;
   out_4846751114242430980[158] = 0;
   out_4846751114242430980[159] = 0;
   out_4846751114242430980[160] = 0;
   out_4846751114242430980[161] = 0;
   out_4846751114242430980[162] = 0;
   out_4846751114242430980[163] = 0;
   out_4846751114242430980[164] = 0;
   out_4846751114242430980[165] = 0;
   out_4846751114242430980[166] = 0;
   out_4846751114242430980[167] = 0;
   out_4846751114242430980[168] = 0;
   out_4846751114242430980[169] = 0;
   out_4846751114242430980[170] = 0;
   out_4846751114242430980[171] = 1;
   out_4846751114242430980[172] = 0;
   out_4846751114242430980[173] = 0;
   out_4846751114242430980[174] = 0;
   out_4846751114242430980[175] = 0;
   out_4846751114242430980[176] = 0;
   out_4846751114242430980[177] = 0;
   out_4846751114242430980[178] = 0;
   out_4846751114242430980[179] = 0;
   out_4846751114242430980[180] = 0;
   out_4846751114242430980[181] = 0;
   out_4846751114242430980[182] = 0;
   out_4846751114242430980[183] = 0;
   out_4846751114242430980[184] = 0;
   out_4846751114242430980[185] = 0;
   out_4846751114242430980[186] = 0;
   out_4846751114242430980[187] = 0;
   out_4846751114242430980[188] = 0;
   out_4846751114242430980[189] = 0;
   out_4846751114242430980[190] = 1;
   out_4846751114242430980[191] = 0;
   out_4846751114242430980[192] = 0;
   out_4846751114242430980[193] = 0;
   out_4846751114242430980[194] = 0;
   out_4846751114242430980[195] = 0;
   out_4846751114242430980[196] = 0;
   out_4846751114242430980[197] = 0;
   out_4846751114242430980[198] = 0;
   out_4846751114242430980[199] = 0;
   out_4846751114242430980[200] = 0;
   out_4846751114242430980[201] = 0;
   out_4846751114242430980[202] = 0;
   out_4846751114242430980[203] = 0;
   out_4846751114242430980[204] = 0;
   out_4846751114242430980[205] = 0;
   out_4846751114242430980[206] = 0;
   out_4846751114242430980[207] = 0;
   out_4846751114242430980[208] = 0;
   out_4846751114242430980[209] = 1;
   out_4846751114242430980[210] = 0;
   out_4846751114242430980[211] = 0;
   out_4846751114242430980[212] = 0;
   out_4846751114242430980[213] = 0;
   out_4846751114242430980[214] = 0;
   out_4846751114242430980[215] = 0;
   out_4846751114242430980[216] = 0;
   out_4846751114242430980[217] = 0;
   out_4846751114242430980[218] = 0;
   out_4846751114242430980[219] = 0;
   out_4846751114242430980[220] = 0;
   out_4846751114242430980[221] = 0;
   out_4846751114242430980[222] = 0;
   out_4846751114242430980[223] = 0;
   out_4846751114242430980[224] = 0;
   out_4846751114242430980[225] = 0;
   out_4846751114242430980[226] = 0;
   out_4846751114242430980[227] = 0;
   out_4846751114242430980[228] = 1;
   out_4846751114242430980[229] = 0;
   out_4846751114242430980[230] = 0;
   out_4846751114242430980[231] = 0;
   out_4846751114242430980[232] = 0;
   out_4846751114242430980[233] = 0;
   out_4846751114242430980[234] = 0;
   out_4846751114242430980[235] = 0;
   out_4846751114242430980[236] = 0;
   out_4846751114242430980[237] = 0;
   out_4846751114242430980[238] = 0;
   out_4846751114242430980[239] = 0;
   out_4846751114242430980[240] = 0;
   out_4846751114242430980[241] = 0;
   out_4846751114242430980[242] = 0;
   out_4846751114242430980[243] = 0;
   out_4846751114242430980[244] = 0;
   out_4846751114242430980[245] = 0;
   out_4846751114242430980[246] = 0;
   out_4846751114242430980[247] = 1;
   out_4846751114242430980[248] = 0;
   out_4846751114242430980[249] = 0;
   out_4846751114242430980[250] = 0;
   out_4846751114242430980[251] = 0;
   out_4846751114242430980[252] = 0;
   out_4846751114242430980[253] = 0;
   out_4846751114242430980[254] = 0;
   out_4846751114242430980[255] = 0;
   out_4846751114242430980[256] = 0;
   out_4846751114242430980[257] = 0;
   out_4846751114242430980[258] = 0;
   out_4846751114242430980[259] = 0;
   out_4846751114242430980[260] = 0;
   out_4846751114242430980[261] = 0;
   out_4846751114242430980[262] = 0;
   out_4846751114242430980[263] = 0;
   out_4846751114242430980[264] = 0;
   out_4846751114242430980[265] = 0;
   out_4846751114242430980[266] = 1;
   out_4846751114242430980[267] = 0;
   out_4846751114242430980[268] = 0;
   out_4846751114242430980[269] = 0;
   out_4846751114242430980[270] = 0;
   out_4846751114242430980[271] = 0;
   out_4846751114242430980[272] = 0;
   out_4846751114242430980[273] = 0;
   out_4846751114242430980[274] = 0;
   out_4846751114242430980[275] = 0;
   out_4846751114242430980[276] = 0;
   out_4846751114242430980[277] = 0;
   out_4846751114242430980[278] = 0;
   out_4846751114242430980[279] = 0;
   out_4846751114242430980[280] = 0;
   out_4846751114242430980[281] = 0;
   out_4846751114242430980[282] = 0;
   out_4846751114242430980[283] = 0;
   out_4846751114242430980[284] = 0;
   out_4846751114242430980[285] = 1;
   out_4846751114242430980[286] = 0;
   out_4846751114242430980[287] = 0;
   out_4846751114242430980[288] = 0;
   out_4846751114242430980[289] = 0;
   out_4846751114242430980[290] = 0;
   out_4846751114242430980[291] = 0;
   out_4846751114242430980[292] = 0;
   out_4846751114242430980[293] = 0;
   out_4846751114242430980[294] = 0;
   out_4846751114242430980[295] = 0;
   out_4846751114242430980[296] = 0;
   out_4846751114242430980[297] = 0;
   out_4846751114242430980[298] = 0;
   out_4846751114242430980[299] = 0;
   out_4846751114242430980[300] = 0;
   out_4846751114242430980[301] = 0;
   out_4846751114242430980[302] = 0;
   out_4846751114242430980[303] = 0;
   out_4846751114242430980[304] = 1;
   out_4846751114242430980[305] = 0;
   out_4846751114242430980[306] = 0;
   out_4846751114242430980[307] = 0;
   out_4846751114242430980[308] = 0;
   out_4846751114242430980[309] = 0;
   out_4846751114242430980[310] = 0;
   out_4846751114242430980[311] = 0;
   out_4846751114242430980[312] = 0;
   out_4846751114242430980[313] = 0;
   out_4846751114242430980[314] = 0;
   out_4846751114242430980[315] = 0;
   out_4846751114242430980[316] = 0;
   out_4846751114242430980[317] = 0;
   out_4846751114242430980[318] = 0;
   out_4846751114242430980[319] = 0;
   out_4846751114242430980[320] = 0;
   out_4846751114242430980[321] = 0;
   out_4846751114242430980[322] = 0;
   out_4846751114242430980[323] = 1;
}
void h_4(double *state, double *unused, double *out_5930999592870125556) {
   out_5930999592870125556[0] = state[6] + state[9];
   out_5930999592870125556[1] = state[7] + state[10];
   out_5930999592870125556[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_5602157340699375302) {
   out_5602157340699375302[0] = 0;
   out_5602157340699375302[1] = 0;
   out_5602157340699375302[2] = 0;
   out_5602157340699375302[3] = 0;
   out_5602157340699375302[4] = 0;
   out_5602157340699375302[5] = 0;
   out_5602157340699375302[6] = 1;
   out_5602157340699375302[7] = 0;
   out_5602157340699375302[8] = 0;
   out_5602157340699375302[9] = 1;
   out_5602157340699375302[10] = 0;
   out_5602157340699375302[11] = 0;
   out_5602157340699375302[12] = 0;
   out_5602157340699375302[13] = 0;
   out_5602157340699375302[14] = 0;
   out_5602157340699375302[15] = 0;
   out_5602157340699375302[16] = 0;
   out_5602157340699375302[17] = 0;
   out_5602157340699375302[18] = 0;
   out_5602157340699375302[19] = 0;
   out_5602157340699375302[20] = 0;
   out_5602157340699375302[21] = 0;
   out_5602157340699375302[22] = 0;
   out_5602157340699375302[23] = 0;
   out_5602157340699375302[24] = 0;
   out_5602157340699375302[25] = 1;
   out_5602157340699375302[26] = 0;
   out_5602157340699375302[27] = 0;
   out_5602157340699375302[28] = 1;
   out_5602157340699375302[29] = 0;
   out_5602157340699375302[30] = 0;
   out_5602157340699375302[31] = 0;
   out_5602157340699375302[32] = 0;
   out_5602157340699375302[33] = 0;
   out_5602157340699375302[34] = 0;
   out_5602157340699375302[35] = 0;
   out_5602157340699375302[36] = 0;
   out_5602157340699375302[37] = 0;
   out_5602157340699375302[38] = 0;
   out_5602157340699375302[39] = 0;
   out_5602157340699375302[40] = 0;
   out_5602157340699375302[41] = 0;
   out_5602157340699375302[42] = 0;
   out_5602157340699375302[43] = 0;
   out_5602157340699375302[44] = 1;
   out_5602157340699375302[45] = 0;
   out_5602157340699375302[46] = 0;
   out_5602157340699375302[47] = 1;
   out_5602157340699375302[48] = 0;
   out_5602157340699375302[49] = 0;
   out_5602157340699375302[50] = 0;
   out_5602157340699375302[51] = 0;
   out_5602157340699375302[52] = 0;
   out_5602157340699375302[53] = 0;
}
void h_10(double *state, double *unused, double *out_5557120776990141355) {
   out_5557120776990141355[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_5557120776990141355[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_5557120776990141355[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_1023238183635137507) {
   out_1023238183635137507[0] = 0;
   out_1023238183635137507[1] = 9.8100000000000005*cos(state[1]);
   out_1023238183635137507[2] = 0;
   out_1023238183635137507[3] = 0;
   out_1023238183635137507[4] = -state[8];
   out_1023238183635137507[5] = state[7];
   out_1023238183635137507[6] = 0;
   out_1023238183635137507[7] = state[5];
   out_1023238183635137507[8] = -state[4];
   out_1023238183635137507[9] = 0;
   out_1023238183635137507[10] = 0;
   out_1023238183635137507[11] = 0;
   out_1023238183635137507[12] = 1;
   out_1023238183635137507[13] = 0;
   out_1023238183635137507[14] = 0;
   out_1023238183635137507[15] = 1;
   out_1023238183635137507[16] = 0;
   out_1023238183635137507[17] = 0;
   out_1023238183635137507[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_1023238183635137507[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_1023238183635137507[20] = 0;
   out_1023238183635137507[21] = state[8];
   out_1023238183635137507[22] = 0;
   out_1023238183635137507[23] = -state[6];
   out_1023238183635137507[24] = -state[5];
   out_1023238183635137507[25] = 0;
   out_1023238183635137507[26] = state[3];
   out_1023238183635137507[27] = 0;
   out_1023238183635137507[28] = 0;
   out_1023238183635137507[29] = 0;
   out_1023238183635137507[30] = 0;
   out_1023238183635137507[31] = 1;
   out_1023238183635137507[32] = 0;
   out_1023238183635137507[33] = 0;
   out_1023238183635137507[34] = 1;
   out_1023238183635137507[35] = 0;
   out_1023238183635137507[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_1023238183635137507[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_1023238183635137507[38] = 0;
   out_1023238183635137507[39] = -state[7];
   out_1023238183635137507[40] = state[6];
   out_1023238183635137507[41] = 0;
   out_1023238183635137507[42] = state[4];
   out_1023238183635137507[43] = -state[3];
   out_1023238183635137507[44] = 0;
   out_1023238183635137507[45] = 0;
   out_1023238183635137507[46] = 0;
   out_1023238183635137507[47] = 0;
   out_1023238183635137507[48] = 0;
   out_1023238183635137507[49] = 0;
   out_1023238183635137507[50] = 1;
   out_1023238183635137507[51] = 0;
   out_1023238183635137507[52] = 0;
   out_1023238183635137507[53] = 1;
}
void h_13(double *state, double *unused, double *out_6353105533166408740) {
   out_6353105533166408740[0] = state[3];
   out_6353105533166408740[1] = state[4];
   out_6353105533166408740[2] = state[5];
}
void H_13(double *state, double *unused, double *out_2008473867617325627) {
   out_2008473867617325627[0] = 0;
   out_2008473867617325627[1] = 0;
   out_2008473867617325627[2] = 0;
   out_2008473867617325627[3] = 1;
   out_2008473867617325627[4] = 0;
   out_2008473867617325627[5] = 0;
   out_2008473867617325627[6] = 0;
   out_2008473867617325627[7] = 0;
   out_2008473867617325627[8] = 0;
   out_2008473867617325627[9] = 0;
   out_2008473867617325627[10] = 0;
   out_2008473867617325627[11] = 0;
   out_2008473867617325627[12] = 0;
   out_2008473867617325627[13] = 0;
   out_2008473867617325627[14] = 0;
   out_2008473867617325627[15] = 0;
   out_2008473867617325627[16] = 0;
   out_2008473867617325627[17] = 0;
   out_2008473867617325627[18] = 0;
   out_2008473867617325627[19] = 0;
   out_2008473867617325627[20] = 0;
   out_2008473867617325627[21] = 0;
   out_2008473867617325627[22] = 1;
   out_2008473867617325627[23] = 0;
   out_2008473867617325627[24] = 0;
   out_2008473867617325627[25] = 0;
   out_2008473867617325627[26] = 0;
   out_2008473867617325627[27] = 0;
   out_2008473867617325627[28] = 0;
   out_2008473867617325627[29] = 0;
   out_2008473867617325627[30] = 0;
   out_2008473867617325627[31] = 0;
   out_2008473867617325627[32] = 0;
   out_2008473867617325627[33] = 0;
   out_2008473867617325627[34] = 0;
   out_2008473867617325627[35] = 0;
   out_2008473867617325627[36] = 0;
   out_2008473867617325627[37] = 0;
   out_2008473867617325627[38] = 0;
   out_2008473867617325627[39] = 0;
   out_2008473867617325627[40] = 0;
   out_2008473867617325627[41] = 1;
   out_2008473867617325627[42] = 0;
   out_2008473867617325627[43] = 0;
   out_2008473867617325627[44] = 0;
   out_2008473867617325627[45] = 0;
   out_2008473867617325627[46] = 0;
   out_2008473867617325627[47] = 0;
   out_2008473867617325627[48] = 0;
   out_2008473867617325627[49] = 0;
   out_2008473867617325627[50] = 0;
   out_2008473867617325627[51] = 0;
   out_2008473867617325627[52] = 0;
   out_2008473867617325627[53] = 0;
}
void h_14(double *state, double *unused, double *out_3322199193100514196) {
   out_3322199193100514196[0] = state[6];
   out_3322199193100514196[1] = state[7];
   out_3322199193100514196[2] = state[8];
}
void H_14(double *state, double *unused, double *out_8684945772994747598) {
   out_8684945772994747598[0] = 0;
   out_8684945772994747598[1] = 0;
   out_8684945772994747598[2] = 0;
   out_8684945772994747598[3] = 0;
   out_8684945772994747598[4] = 0;
   out_8684945772994747598[5] = 0;
   out_8684945772994747598[6] = 1;
   out_8684945772994747598[7] = 0;
   out_8684945772994747598[8] = 0;
   out_8684945772994747598[9] = 0;
   out_8684945772994747598[10] = 0;
   out_8684945772994747598[11] = 0;
   out_8684945772994747598[12] = 0;
   out_8684945772994747598[13] = 0;
   out_8684945772994747598[14] = 0;
   out_8684945772994747598[15] = 0;
   out_8684945772994747598[16] = 0;
   out_8684945772994747598[17] = 0;
   out_8684945772994747598[18] = 0;
   out_8684945772994747598[19] = 0;
   out_8684945772994747598[20] = 0;
   out_8684945772994747598[21] = 0;
   out_8684945772994747598[22] = 0;
   out_8684945772994747598[23] = 0;
   out_8684945772994747598[24] = 0;
   out_8684945772994747598[25] = 1;
   out_8684945772994747598[26] = 0;
   out_8684945772994747598[27] = 0;
   out_8684945772994747598[28] = 0;
   out_8684945772994747598[29] = 0;
   out_8684945772994747598[30] = 0;
   out_8684945772994747598[31] = 0;
   out_8684945772994747598[32] = 0;
   out_8684945772994747598[33] = 0;
   out_8684945772994747598[34] = 0;
   out_8684945772994747598[35] = 0;
   out_8684945772994747598[36] = 0;
   out_8684945772994747598[37] = 0;
   out_8684945772994747598[38] = 0;
   out_8684945772994747598[39] = 0;
   out_8684945772994747598[40] = 0;
   out_8684945772994747598[41] = 0;
   out_8684945772994747598[42] = 0;
   out_8684945772994747598[43] = 0;
   out_8684945772994747598[44] = 1;
   out_8684945772994747598[45] = 0;
   out_8684945772994747598[46] = 0;
   out_8684945772994747598[47] = 0;
   out_8684945772994747598[48] = 0;
   out_8684945772994747598[49] = 0;
   out_8684945772994747598[50] = 0;
   out_8684945772994747598[51] = 0;
   out_8684945772994747598[52] = 0;
   out_8684945772994747598[53] = 0;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_4, H_4, NULL, in_z, in_R, in_ea, MAHA_THRESH_4);
}
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_10, H_10, NULL, in_z, in_R, in_ea, MAHA_THRESH_10);
}
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_13, H_13, NULL, in_z, in_R, in_ea, MAHA_THRESH_13);
}
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_14, H_14, NULL, in_z, in_R, in_ea, MAHA_THRESH_14);
}
void pose_err_fun(double *nom_x, double *delta_x, double *out_3585092682791090136) {
  err_fun(nom_x, delta_x, out_3585092682791090136);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_3602092163882833613) {
  inv_err_fun(nom_x, true_x, out_3602092163882833613);
}
void pose_H_mod_fun(double *state, double *out_9072779523166268742) {
  H_mod_fun(state, out_9072779523166268742);
}
void pose_f_fun(double *state, double dt, double *out_8675559756142309217) {
  f_fun(state,  dt, out_8675559756142309217);
}
void pose_F_fun(double *state, double dt, double *out_4846751114242430980) {
  F_fun(state,  dt, out_4846751114242430980);
}
void pose_h_4(double *state, double *unused, double *out_5930999592870125556) {
  h_4(state, unused, out_5930999592870125556);
}
void pose_H_4(double *state, double *unused, double *out_5602157340699375302) {
  H_4(state, unused, out_5602157340699375302);
}
void pose_h_10(double *state, double *unused, double *out_5557120776990141355) {
  h_10(state, unused, out_5557120776990141355);
}
void pose_H_10(double *state, double *unused, double *out_1023238183635137507) {
  H_10(state, unused, out_1023238183635137507);
}
void pose_h_13(double *state, double *unused, double *out_6353105533166408740) {
  h_13(state, unused, out_6353105533166408740);
}
void pose_H_13(double *state, double *unused, double *out_2008473867617325627) {
  H_13(state, unused, out_2008473867617325627);
}
void pose_h_14(double *state, double *unused, double *out_3322199193100514196) {
  h_14(state, unused, out_3322199193100514196);
}
void pose_H_14(double *state, double *unused, double *out_8684945772994747598) {
  H_14(state, unused, out_8684945772994747598);
}
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
}

const EKF pose = {
  .name = "pose",
  .kinds = { 4, 10, 13, 14 },
  .feature_kinds = {  },
  .f_fun = pose_f_fun,
  .F_fun = pose_F_fun,
  .err_fun = pose_err_fun,
  .inv_err_fun = pose_inv_err_fun,
  .H_mod_fun = pose_H_mod_fun,
  .predict = pose_predict,
  .hs = {
    { 4, pose_h_4 },
    { 10, pose_h_10 },
    { 13, pose_h_13 },
    { 14, pose_h_14 },
  },
  .Hs = {
    { 4, pose_H_4 },
    { 10, pose_H_10 },
    { 13, pose_H_13 },
    { 14, pose_H_14 },
  },
  .updates = {
    { 4, pose_update_4 },
    { 10, pose_update_10 },
    { 13, pose_update_13 },
    { 14, pose_update_14 },
  },
  .Hes = {
  },
  .sets = {
  },
  .extra_routines = {
  },
};

ekf_lib_init(pose)
