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
void err_fun(double *nom_x, double *delta_x, double *out_2282374485527159040) {
   out_2282374485527159040[0] = delta_x[0] + nom_x[0];
   out_2282374485527159040[1] = delta_x[1] + nom_x[1];
   out_2282374485527159040[2] = delta_x[2] + nom_x[2];
   out_2282374485527159040[3] = delta_x[3] + nom_x[3];
   out_2282374485527159040[4] = delta_x[4] + nom_x[4];
   out_2282374485527159040[5] = delta_x[5] + nom_x[5];
   out_2282374485527159040[6] = delta_x[6] + nom_x[6];
   out_2282374485527159040[7] = delta_x[7] + nom_x[7];
   out_2282374485527159040[8] = delta_x[8] + nom_x[8];
   out_2282374485527159040[9] = delta_x[9] + nom_x[9];
   out_2282374485527159040[10] = delta_x[10] + nom_x[10];
   out_2282374485527159040[11] = delta_x[11] + nom_x[11];
   out_2282374485527159040[12] = delta_x[12] + nom_x[12];
   out_2282374485527159040[13] = delta_x[13] + nom_x[13];
   out_2282374485527159040[14] = delta_x[14] + nom_x[14];
   out_2282374485527159040[15] = delta_x[15] + nom_x[15];
   out_2282374485527159040[16] = delta_x[16] + nom_x[16];
   out_2282374485527159040[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_4495318315073688785) {
   out_4495318315073688785[0] = -nom_x[0] + true_x[0];
   out_4495318315073688785[1] = -nom_x[1] + true_x[1];
   out_4495318315073688785[2] = -nom_x[2] + true_x[2];
   out_4495318315073688785[3] = -nom_x[3] + true_x[3];
   out_4495318315073688785[4] = -nom_x[4] + true_x[4];
   out_4495318315073688785[5] = -nom_x[5] + true_x[5];
   out_4495318315073688785[6] = -nom_x[6] + true_x[6];
   out_4495318315073688785[7] = -nom_x[7] + true_x[7];
   out_4495318315073688785[8] = -nom_x[8] + true_x[8];
   out_4495318315073688785[9] = -nom_x[9] + true_x[9];
   out_4495318315073688785[10] = -nom_x[10] + true_x[10];
   out_4495318315073688785[11] = -nom_x[11] + true_x[11];
   out_4495318315073688785[12] = -nom_x[12] + true_x[12];
   out_4495318315073688785[13] = -nom_x[13] + true_x[13];
   out_4495318315073688785[14] = -nom_x[14] + true_x[14];
   out_4495318315073688785[15] = -nom_x[15] + true_x[15];
   out_4495318315073688785[16] = -nom_x[16] + true_x[16];
   out_4495318315073688785[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_8988321217165806383) {
   out_8988321217165806383[0] = 1.0;
   out_8988321217165806383[1] = 0.0;
   out_8988321217165806383[2] = 0.0;
   out_8988321217165806383[3] = 0.0;
   out_8988321217165806383[4] = 0.0;
   out_8988321217165806383[5] = 0.0;
   out_8988321217165806383[6] = 0.0;
   out_8988321217165806383[7] = 0.0;
   out_8988321217165806383[8] = 0.0;
   out_8988321217165806383[9] = 0.0;
   out_8988321217165806383[10] = 0.0;
   out_8988321217165806383[11] = 0.0;
   out_8988321217165806383[12] = 0.0;
   out_8988321217165806383[13] = 0.0;
   out_8988321217165806383[14] = 0.0;
   out_8988321217165806383[15] = 0.0;
   out_8988321217165806383[16] = 0.0;
   out_8988321217165806383[17] = 0.0;
   out_8988321217165806383[18] = 0.0;
   out_8988321217165806383[19] = 1.0;
   out_8988321217165806383[20] = 0.0;
   out_8988321217165806383[21] = 0.0;
   out_8988321217165806383[22] = 0.0;
   out_8988321217165806383[23] = 0.0;
   out_8988321217165806383[24] = 0.0;
   out_8988321217165806383[25] = 0.0;
   out_8988321217165806383[26] = 0.0;
   out_8988321217165806383[27] = 0.0;
   out_8988321217165806383[28] = 0.0;
   out_8988321217165806383[29] = 0.0;
   out_8988321217165806383[30] = 0.0;
   out_8988321217165806383[31] = 0.0;
   out_8988321217165806383[32] = 0.0;
   out_8988321217165806383[33] = 0.0;
   out_8988321217165806383[34] = 0.0;
   out_8988321217165806383[35] = 0.0;
   out_8988321217165806383[36] = 0.0;
   out_8988321217165806383[37] = 0.0;
   out_8988321217165806383[38] = 1.0;
   out_8988321217165806383[39] = 0.0;
   out_8988321217165806383[40] = 0.0;
   out_8988321217165806383[41] = 0.0;
   out_8988321217165806383[42] = 0.0;
   out_8988321217165806383[43] = 0.0;
   out_8988321217165806383[44] = 0.0;
   out_8988321217165806383[45] = 0.0;
   out_8988321217165806383[46] = 0.0;
   out_8988321217165806383[47] = 0.0;
   out_8988321217165806383[48] = 0.0;
   out_8988321217165806383[49] = 0.0;
   out_8988321217165806383[50] = 0.0;
   out_8988321217165806383[51] = 0.0;
   out_8988321217165806383[52] = 0.0;
   out_8988321217165806383[53] = 0.0;
   out_8988321217165806383[54] = 0.0;
   out_8988321217165806383[55] = 0.0;
   out_8988321217165806383[56] = 0.0;
   out_8988321217165806383[57] = 1.0;
   out_8988321217165806383[58] = 0.0;
   out_8988321217165806383[59] = 0.0;
   out_8988321217165806383[60] = 0.0;
   out_8988321217165806383[61] = 0.0;
   out_8988321217165806383[62] = 0.0;
   out_8988321217165806383[63] = 0.0;
   out_8988321217165806383[64] = 0.0;
   out_8988321217165806383[65] = 0.0;
   out_8988321217165806383[66] = 0.0;
   out_8988321217165806383[67] = 0.0;
   out_8988321217165806383[68] = 0.0;
   out_8988321217165806383[69] = 0.0;
   out_8988321217165806383[70] = 0.0;
   out_8988321217165806383[71] = 0.0;
   out_8988321217165806383[72] = 0.0;
   out_8988321217165806383[73] = 0.0;
   out_8988321217165806383[74] = 0.0;
   out_8988321217165806383[75] = 0.0;
   out_8988321217165806383[76] = 1.0;
   out_8988321217165806383[77] = 0.0;
   out_8988321217165806383[78] = 0.0;
   out_8988321217165806383[79] = 0.0;
   out_8988321217165806383[80] = 0.0;
   out_8988321217165806383[81] = 0.0;
   out_8988321217165806383[82] = 0.0;
   out_8988321217165806383[83] = 0.0;
   out_8988321217165806383[84] = 0.0;
   out_8988321217165806383[85] = 0.0;
   out_8988321217165806383[86] = 0.0;
   out_8988321217165806383[87] = 0.0;
   out_8988321217165806383[88] = 0.0;
   out_8988321217165806383[89] = 0.0;
   out_8988321217165806383[90] = 0.0;
   out_8988321217165806383[91] = 0.0;
   out_8988321217165806383[92] = 0.0;
   out_8988321217165806383[93] = 0.0;
   out_8988321217165806383[94] = 0.0;
   out_8988321217165806383[95] = 1.0;
   out_8988321217165806383[96] = 0.0;
   out_8988321217165806383[97] = 0.0;
   out_8988321217165806383[98] = 0.0;
   out_8988321217165806383[99] = 0.0;
   out_8988321217165806383[100] = 0.0;
   out_8988321217165806383[101] = 0.0;
   out_8988321217165806383[102] = 0.0;
   out_8988321217165806383[103] = 0.0;
   out_8988321217165806383[104] = 0.0;
   out_8988321217165806383[105] = 0.0;
   out_8988321217165806383[106] = 0.0;
   out_8988321217165806383[107] = 0.0;
   out_8988321217165806383[108] = 0.0;
   out_8988321217165806383[109] = 0.0;
   out_8988321217165806383[110] = 0.0;
   out_8988321217165806383[111] = 0.0;
   out_8988321217165806383[112] = 0.0;
   out_8988321217165806383[113] = 0.0;
   out_8988321217165806383[114] = 1.0;
   out_8988321217165806383[115] = 0.0;
   out_8988321217165806383[116] = 0.0;
   out_8988321217165806383[117] = 0.0;
   out_8988321217165806383[118] = 0.0;
   out_8988321217165806383[119] = 0.0;
   out_8988321217165806383[120] = 0.0;
   out_8988321217165806383[121] = 0.0;
   out_8988321217165806383[122] = 0.0;
   out_8988321217165806383[123] = 0.0;
   out_8988321217165806383[124] = 0.0;
   out_8988321217165806383[125] = 0.0;
   out_8988321217165806383[126] = 0.0;
   out_8988321217165806383[127] = 0.0;
   out_8988321217165806383[128] = 0.0;
   out_8988321217165806383[129] = 0.0;
   out_8988321217165806383[130] = 0.0;
   out_8988321217165806383[131] = 0.0;
   out_8988321217165806383[132] = 0.0;
   out_8988321217165806383[133] = 1.0;
   out_8988321217165806383[134] = 0.0;
   out_8988321217165806383[135] = 0.0;
   out_8988321217165806383[136] = 0.0;
   out_8988321217165806383[137] = 0.0;
   out_8988321217165806383[138] = 0.0;
   out_8988321217165806383[139] = 0.0;
   out_8988321217165806383[140] = 0.0;
   out_8988321217165806383[141] = 0.0;
   out_8988321217165806383[142] = 0.0;
   out_8988321217165806383[143] = 0.0;
   out_8988321217165806383[144] = 0.0;
   out_8988321217165806383[145] = 0.0;
   out_8988321217165806383[146] = 0.0;
   out_8988321217165806383[147] = 0.0;
   out_8988321217165806383[148] = 0.0;
   out_8988321217165806383[149] = 0.0;
   out_8988321217165806383[150] = 0.0;
   out_8988321217165806383[151] = 0.0;
   out_8988321217165806383[152] = 1.0;
   out_8988321217165806383[153] = 0.0;
   out_8988321217165806383[154] = 0.0;
   out_8988321217165806383[155] = 0.0;
   out_8988321217165806383[156] = 0.0;
   out_8988321217165806383[157] = 0.0;
   out_8988321217165806383[158] = 0.0;
   out_8988321217165806383[159] = 0.0;
   out_8988321217165806383[160] = 0.0;
   out_8988321217165806383[161] = 0.0;
   out_8988321217165806383[162] = 0.0;
   out_8988321217165806383[163] = 0.0;
   out_8988321217165806383[164] = 0.0;
   out_8988321217165806383[165] = 0.0;
   out_8988321217165806383[166] = 0.0;
   out_8988321217165806383[167] = 0.0;
   out_8988321217165806383[168] = 0.0;
   out_8988321217165806383[169] = 0.0;
   out_8988321217165806383[170] = 0.0;
   out_8988321217165806383[171] = 1.0;
   out_8988321217165806383[172] = 0.0;
   out_8988321217165806383[173] = 0.0;
   out_8988321217165806383[174] = 0.0;
   out_8988321217165806383[175] = 0.0;
   out_8988321217165806383[176] = 0.0;
   out_8988321217165806383[177] = 0.0;
   out_8988321217165806383[178] = 0.0;
   out_8988321217165806383[179] = 0.0;
   out_8988321217165806383[180] = 0.0;
   out_8988321217165806383[181] = 0.0;
   out_8988321217165806383[182] = 0.0;
   out_8988321217165806383[183] = 0.0;
   out_8988321217165806383[184] = 0.0;
   out_8988321217165806383[185] = 0.0;
   out_8988321217165806383[186] = 0.0;
   out_8988321217165806383[187] = 0.0;
   out_8988321217165806383[188] = 0.0;
   out_8988321217165806383[189] = 0.0;
   out_8988321217165806383[190] = 1.0;
   out_8988321217165806383[191] = 0.0;
   out_8988321217165806383[192] = 0.0;
   out_8988321217165806383[193] = 0.0;
   out_8988321217165806383[194] = 0.0;
   out_8988321217165806383[195] = 0.0;
   out_8988321217165806383[196] = 0.0;
   out_8988321217165806383[197] = 0.0;
   out_8988321217165806383[198] = 0.0;
   out_8988321217165806383[199] = 0.0;
   out_8988321217165806383[200] = 0.0;
   out_8988321217165806383[201] = 0.0;
   out_8988321217165806383[202] = 0.0;
   out_8988321217165806383[203] = 0.0;
   out_8988321217165806383[204] = 0.0;
   out_8988321217165806383[205] = 0.0;
   out_8988321217165806383[206] = 0.0;
   out_8988321217165806383[207] = 0.0;
   out_8988321217165806383[208] = 0.0;
   out_8988321217165806383[209] = 1.0;
   out_8988321217165806383[210] = 0.0;
   out_8988321217165806383[211] = 0.0;
   out_8988321217165806383[212] = 0.0;
   out_8988321217165806383[213] = 0.0;
   out_8988321217165806383[214] = 0.0;
   out_8988321217165806383[215] = 0.0;
   out_8988321217165806383[216] = 0.0;
   out_8988321217165806383[217] = 0.0;
   out_8988321217165806383[218] = 0.0;
   out_8988321217165806383[219] = 0.0;
   out_8988321217165806383[220] = 0.0;
   out_8988321217165806383[221] = 0.0;
   out_8988321217165806383[222] = 0.0;
   out_8988321217165806383[223] = 0.0;
   out_8988321217165806383[224] = 0.0;
   out_8988321217165806383[225] = 0.0;
   out_8988321217165806383[226] = 0.0;
   out_8988321217165806383[227] = 0.0;
   out_8988321217165806383[228] = 1.0;
   out_8988321217165806383[229] = 0.0;
   out_8988321217165806383[230] = 0.0;
   out_8988321217165806383[231] = 0.0;
   out_8988321217165806383[232] = 0.0;
   out_8988321217165806383[233] = 0.0;
   out_8988321217165806383[234] = 0.0;
   out_8988321217165806383[235] = 0.0;
   out_8988321217165806383[236] = 0.0;
   out_8988321217165806383[237] = 0.0;
   out_8988321217165806383[238] = 0.0;
   out_8988321217165806383[239] = 0.0;
   out_8988321217165806383[240] = 0.0;
   out_8988321217165806383[241] = 0.0;
   out_8988321217165806383[242] = 0.0;
   out_8988321217165806383[243] = 0.0;
   out_8988321217165806383[244] = 0.0;
   out_8988321217165806383[245] = 0.0;
   out_8988321217165806383[246] = 0.0;
   out_8988321217165806383[247] = 1.0;
   out_8988321217165806383[248] = 0.0;
   out_8988321217165806383[249] = 0.0;
   out_8988321217165806383[250] = 0.0;
   out_8988321217165806383[251] = 0.0;
   out_8988321217165806383[252] = 0.0;
   out_8988321217165806383[253] = 0.0;
   out_8988321217165806383[254] = 0.0;
   out_8988321217165806383[255] = 0.0;
   out_8988321217165806383[256] = 0.0;
   out_8988321217165806383[257] = 0.0;
   out_8988321217165806383[258] = 0.0;
   out_8988321217165806383[259] = 0.0;
   out_8988321217165806383[260] = 0.0;
   out_8988321217165806383[261] = 0.0;
   out_8988321217165806383[262] = 0.0;
   out_8988321217165806383[263] = 0.0;
   out_8988321217165806383[264] = 0.0;
   out_8988321217165806383[265] = 0.0;
   out_8988321217165806383[266] = 1.0;
   out_8988321217165806383[267] = 0.0;
   out_8988321217165806383[268] = 0.0;
   out_8988321217165806383[269] = 0.0;
   out_8988321217165806383[270] = 0.0;
   out_8988321217165806383[271] = 0.0;
   out_8988321217165806383[272] = 0.0;
   out_8988321217165806383[273] = 0.0;
   out_8988321217165806383[274] = 0.0;
   out_8988321217165806383[275] = 0.0;
   out_8988321217165806383[276] = 0.0;
   out_8988321217165806383[277] = 0.0;
   out_8988321217165806383[278] = 0.0;
   out_8988321217165806383[279] = 0.0;
   out_8988321217165806383[280] = 0.0;
   out_8988321217165806383[281] = 0.0;
   out_8988321217165806383[282] = 0.0;
   out_8988321217165806383[283] = 0.0;
   out_8988321217165806383[284] = 0.0;
   out_8988321217165806383[285] = 1.0;
   out_8988321217165806383[286] = 0.0;
   out_8988321217165806383[287] = 0.0;
   out_8988321217165806383[288] = 0.0;
   out_8988321217165806383[289] = 0.0;
   out_8988321217165806383[290] = 0.0;
   out_8988321217165806383[291] = 0.0;
   out_8988321217165806383[292] = 0.0;
   out_8988321217165806383[293] = 0.0;
   out_8988321217165806383[294] = 0.0;
   out_8988321217165806383[295] = 0.0;
   out_8988321217165806383[296] = 0.0;
   out_8988321217165806383[297] = 0.0;
   out_8988321217165806383[298] = 0.0;
   out_8988321217165806383[299] = 0.0;
   out_8988321217165806383[300] = 0.0;
   out_8988321217165806383[301] = 0.0;
   out_8988321217165806383[302] = 0.0;
   out_8988321217165806383[303] = 0.0;
   out_8988321217165806383[304] = 1.0;
   out_8988321217165806383[305] = 0.0;
   out_8988321217165806383[306] = 0.0;
   out_8988321217165806383[307] = 0.0;
   out_8988321217165806383[308] = 0.0;
   out_8988321217165806383[309] = 0.0;
   out_8988321217165806383[310] = 0.0;
   out_8988321217165806383[311] = 0.0;
   out_8988321217165806383[312] = 0.0;
   out_8988321217165806383[313] = 0.0;
   out_8988321217165806383[314] = 0.0;
   out_8988321217165806383[315] = 0.0;
   out_8988321217165806383[316] = 0.0;
   out_8988321217165806383[317] = 0.0;
   out_8988321217165806383[318] = 0.0;
   out_8988321217165806383[319] = 0.0;
   out_8988321217165806383[320] = 0.0;
   out_8988321217165806383[321] = 0.0;
   out_8988321217165806383[322] = 0.0;
   out_8988321217165806383[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_7445325378273019703) {
   out_7445325378273019703[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_7445325378273019703[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_7445325378273019703[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_7445325378273019703[3] = dt*state[12] + state[3];
   out_7445325378273019703[4] = dt*state[13] + state[4];
   out_7445325378273019703[5] = dt*state[14] + state[5];
   out_7445325378273019703[6] = state[6];
   out_7445325378273019703[7] = state[7];
   out_7445325378273019703[8] = state[8];
   out_7445325378273019703[9] = state[9];
   out_7445325378273019703[10] = state[10];
   out_7445325378273019703[11] = state[11];
   out_7445325378273019703[12] = state[12];
   out_7445325378273019703[13] = state[13];
   out_7445325378273019703[14] = state[14];
   out_7445325378273019703[15] = state[15];
   out_7445325378273019703[16] = state[16];
   out_7445325378273019703[17] = state[17];
}
void F_fun(double *state, double dt, double *out_1682818178324058156) {
   out_1682818178324058156[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1682818178324058156[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1682818178324058156[2] = 0;
   out_1682818178324058156[3] = 0;
   out_1682818178324058156[4] = 0;
   out_1682818178324058156[5] = 0;
   out_1682818178324058156[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1682818178324058156[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1682818178324058156[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1682818178324058156[9] = 0;
   out_1682818178324058156[10] = 0;
   out_1682818178324058156[11] = 0;
   out_1682818178324058156[12] = 0;
   out_1682818178324058156[13] = 0;
   out_1682818178324058156[14] = 0;
   out_1682818178324058156[15] = 0;
   out_1682818178324058156[16] = 0;
   out_1682818178324058156[17] = 0;
   out_1682818178324058156[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1682818178324058156[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1682818178324058156[20] = 0;
   out_1682818178324058156[21] = 0;
   out_1682818178324058156[22] = 0;
   out_1682818178324058156[23] = 0;
   out_1682818178324058156[24] = 0;
   out_1682818178324058156[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1682818178324058156[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1682818178324058156[27] = 0;
   out_1682818178324058156[28] = 0;
   out_1682818178324058156[29] = 0;
   out_1682818178324058156[30] = 0;
   out_1682818178324058156[31] = 0;
   out_1682818178324058156[32] = 0;
   out_1682818178324058156[33] = 0;
   out_1682818178324058156[34] = 0;
   out_1682818178324058156[35] = 0;
   out_1682818178324058156[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1682818178324058156[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1682818178324058156[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1682818178324058156[39] = 0;
   out_1682818178324058156[40] = 0;
   out_1682818178324058156[41] = 0;
   out_1682818178324058156[42] = 0;
   out_1682818178324058156[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1682818178324058156[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1682818178324058156[45] = 0;
   out_1682818178324058156[46] = 0;
   out_1682818178324058156[47] = 0;
   out_1682818178324058156[48] = 0;
   out_1682818178324058156[49] = 0;
   out_1682818178324058156[50] = 0;
   out_1682818178324058156[51] = 0;
   out_1682818178324058156[52] = 0;
   out_1682818178324058156[53] = 0;
   out_1682818178324058156[54] = 0;
   out_1682818178324058156[55] = 0;
   out_1682818178324058156[56] = 0;
   out_1682818178324058156[57] = 1;
   out_1682818178324058156[58] = 0;
   out_1682818178324058156[59] = 0;
   out_1682818178324058156[60] = 0;
   out_1682818178324058156[61] = 0;
   out_1682818178324058156[62] = 0;
   out_1682818178324058156[63] = 0;
   out_1682818178324058156[64] = 0;
   out_1682818178324058156[65] = 0;
   out_1682818178324058156[66] = dt;
   out_1682818178324058156[67] = 0;
   out_1682818178324058156[68] = 0;
   out_1682818178324058156[69] = 0;
   out_1682818178324058156[70] = 0;
   out_1682818178324058156[71] = 0;
   out_1682818178324058156[72] = 0;
   out_1682818178324058156[73] = 0;
   out_1682818178324058156[74] = 0;
   out_1682818178324058156[75] = 0;
   out_1682818178324058156[76] = 1;
   out_1682818178324058156[77] = 0;
   out_1682818178324058156[78] = 0;
   out_1682818178324058156[79] = 0;
   out_1682818178324058156[80] = 0;
   out_1682818178324058156[81] = 0;
   out_1682818178324058156[82] = 0;
   out_1682818178324058156[83] = 0;
   out_1682818178324058156[84] = 0;
   out_1682818178324058156[85] = dt;
   out_1682818178324058156[86] = 0;
   out_1682818178324058156[87] = 0;
   out_1682818178324058156[88] = 0;
   out_1682818178324058156[89] = 0;
   out_1682818178324058156[90] = 0;
   out_1682818178324058156[91] = 0;
   out_1682818178324058156[92] = 0;
   out_1682818178324058156[93] = 0;
   out_1682818178324058156[94] = 0;
   out_1682818178324058156[95] = 1;
   out_1682818178324058156[96] = 0;
   out_1682818178324058156[97] = 0;
   out_1682818178324058156[98] = 0;
   out_1682818178324058156[99] = 0;
   out_1682818178324058156[100] = 0;
   out_1682818178324058156[101] = 0;
   out_1682818178324058156[102] = 0;
   out_1682818178324058156[103] = 0;
   out_1682818178324058156[104] = dt;
   out_1682818178324058156[105] = 0;
   out_1682818178324058156[106] = 0;
   out_1682818178324058156[107] = 0;
   out_1682818178324058156[108] = 0;
   out_1682818178324058156[109] = 0;
   out_1682818178324058156[110] = 0;
   out_1682818178324058156[111] = 0;
   out_1682818178324058156[112] = 0;
   out_1682818178324058156[113] = 0;
   out_1682818178324058156[114] = 1;
   out_1682818178324058156[115] = 0;
   out_1682818178324058156[116] = 0;
   out_1682818178324058156[117] = 0;
   out_1682818178324058156[118] = 0;
   out_1682818178324058156[119] = 0;
   out_1682818178324058156[120] = 0;
   out_1682818178324058156[121] = 0;
   out_1682818178324058156[122] = 0;
   out_1682818178324058156[123] = 0;
   out_1682818178324058156[124] = 0;
   out_1682818178324058156[125] = 0;
   out_1682818178324058156[126] = 0;
   out_1682818178324058156[127] = 0;
   out_1682818178324058156[128] = 0;
   out_1682818178324058156[129] = 0;
   out_1682818178324058156[130] = 0;
   out_1682818178324058156[131] = 0;
   out_1682818178324058156[132] = 0;
   out_1682818178324058156[133] = 1;
   out_1682818178324058156[134] = 0;
   out_1682818178324058156[135] = 0;
   out_1682818178324058156[136] = 0;
   out_1682818178324058156[137] = 0;
   out_1682818178324058156[138] = 0;
   out_1682818178324058156[139] = 0;
   out_1682818178324058156[140] = 0;
   out_1682818178324058156[141] = 0;
   out_1682818178324058156[142] = 0;
   out_1682818178324058156[143] = 0;
   out_1682818178324058156[144] = 0;
   out_1682818178324058156[145] = 0;
   out_1682818178324058156[146] = 0;
   out_1682818178324058156[147] = 0;
   out_1682818178324058156[148] = 0;
   out_1682818178324058156[149] = 0;
   out_1682818178324058156[150] = 0;
   out_1682818178324058156[151] = 0;
   out_1682818178324058156[152] = 1;
   out_1682818178324058156[153] = 0;
   out_1682818178324058156[154] = 0;
   out_1682818178324058156[155] = 0;
   out_1682818178324058156[156] = 0;
   out_1682818178324058156[157] = 0;
   out_1682818178324058156[158] = 0;
   out_1682818178324058156[159] = 0;
   out_1682818178324058156[160] = 0;
   out_1682818178324058156[161] = 0;
   out_1682818178324058156[162] = 0;
   out_1682818178324058156[163] = 0;
   out_1682818178324058156[164] = 0;
   out_1682818178324058156[165] = 0;
   out_1682818178324058156[166] = 0;
   out_1682818178324058156[167] = 0;
   out_1682818178324058156[168] = 0;
   out_1682818178324058156[169] = 0;
   out_1682818178324058156[170] = 0;
   out_1682818178324058156[171] = 1;
   out_1682818178324058156[172] = 0;
   out_1682818178324058156[173] = 0;
   out_1682818178324058156[174] = 0;
   out_1682818178324058156[175] = 0;
   out_1682818178324058156[176] = 0;
   out_1682818178324058156[177] = 0;
   out_1682818178324058156[178] = 0;
   out_1682818178324058156[179] = 0;
   out_1682818178324058156[180] = 0;
   out_1682818178324058156[181] = 0;
   out_1682818178324058156[182] = 0;
   out_1682818178324058156[183] = 0;
   out_1682818178324058156[184] = 0;
   out_1682818178324058156[185] = 0;
   out_1682818178324058156[186] = 0;
   out_1682818178324058156[187] = 0;
   out_1682818178324058156[188] = 0;
   out_1682818178324058156[189] = 0;
   out_1682818178324058156[190] = 1;
   out_1682818178324058156[191] = 0;
   out_1682818178324058156[192] = 0;
   out_1682818178324058156[193] = 0;
   out_1682818178324058156[194] = 0;
   out_1682818178324058156[195] = 0;
   out_1682818178324058156[196] = 0;
   out_1682818178324058156[197] = 0;
   out_1682818178324058156[198] = 0;
   out_1682818178324058156[199] = 0;
   out_1682818178324058156[200] = 0;
   out_1682818178324058156[201] = 0;
   out_1682818178324058156[202] = 0;
   out_1682818178324058156[203] = 0;
   out_1682818178324058156[204] = 0;
   out_1682818178324058156[205] = 0;
   out_1682818178324058156[206] = 0;
   out_1682818178324058156[207] = 0;
   out_1682818178324058156[208] = 0;
   out_1682818178324058156[209] = 1;
   out_1682818178324058156[210] = 0;
   out_1682818178324058156[211] = 0;
   out_1682818178324058156[212] = 0;
   out_1682818178324058156[213] = 0;
   out_1682818178324058156[214] = 0;
   out_1682818178324058156[215] = 0;
   out_1682818178324058156[216] = 0;
   out_1682818178324058156[217] = 0;
   out_1682818178324058156[218] = 0;
   out_1682818178324058156[219] = 0;
   out_1682818178324058156[220] = 0;
   out_1682818178324058156[221] = 0;
   out_1682818178324058156[222] = 0;
   out_1682818178324058156[223] = 0;
   out_1682818178324058156[224] = 0;
   out_1682818178324058156[225] = 0;
   out_1682818178324058156[226] = 0;
   out_1682818178324058156[227] = 0;
   out_1682818178324058156[228] = 1;
   out_1682818178324058156[229] = 0;
   out_1682818178324058156[230] = 0;
   out_1682818178324058156[231] = 0;
   out_1682818178324058156[232] = 0;
   out_1682818178324058156[233] = 0;
   out_1682818178324058156[234] = 0;
   out_1682818178324058156[235] = 0;
   out_1682818178324058156[236] = 0;
   out_1682818178324058156[237] = 0;
   out_1682818178324058156[238] = 0;
   out_1682818178324058156[239] = 0;
   out_1682818178324058156[240] = 0;
   out_1682818178324058156[241] = 0;
   out_1682818178324058156[242] = 0;
   out_1682818178324058156[243] = 0;
   out_1682818178324058156[244] = 0;
   out_1682818178324058156[245] = 0;
   out_1682818178324058156[246] = 0;
   out_1682818178324058156[247] = 1;
   out_1682818178324058156[248] = 0;
   out_1682818178324058156[249] = 0;
   out_1682818178324058156[250] = 0;
   out_1682818178324058156[251] = 0;
   out_1682818178324058156[252] = 0;
   out_1682818178324058156[253] = 0;
   out_1682818178324058156[254] = 0;
   out_1682818178324058156[255] = 0;
   out_1682818178324058156[256] = 0;
   out_1682818178324058156[257] = 0;
   out_1682818178324058156[258] = 0;
   out_1682818178324058156[259] = 0;
   out_1682818178324058156[260] = 0;
   out_1682818178324058156[261] = 0;
   out_1682818178324058156[262] = 0;
   out_1682818178324058156[263] = 0;
   out_1682818178324058156[264] = 0;
   out_1682818178324058156[265] = 0;
   out_1682818178324058156[266] = 1;
   out_1682818178324058156[267] = 0;
   out_1682818178324058156[268] = 0;
   out_1682818178324058156[269] = 0;
   out_1682818178324058156[270] = 0;
   out_1682818178324058156[271] = 0;
   out_1682818178324058156[272] = 0;
   out_1682818178324058156[273] = 0;
   out_1682818178324058156[274] = 0;
   out_1682818178324058156[275] = 0;
   out_1682818178324058156[276] = 0;
   out_1682818178324058156[277] = 0;
   out_1682818178324058156[278] = 0;
   out_1682818178324058156[279] = 0;
   out_1682818178324058156[280] = 0;
   out_1682818178324058156[281] = 0;
   out_1682818178324058156[282] = 0;
   out_1682818178324058156[283] = 0;
   out_1682818178324058156[284] = 0;
   out_1682818178324058156[285] = 1;
   out_1682818178324058156[286] = 0;
   out_1682818178324058156[287] = 0;
   out_1682818178324058156[288] = 0;
   out_1682818178324058156[289] = 0;
   out_1682818178324058156[290] = 0;
   out_1682818178324058156[291] = 0;
   out_1682818178324058156[292] = 0;
   out_1682818178324058156[293] = 0;
   out_1682818178324058156[294] = 0;
   out_1682818178324058156[295] = 0;
   out_1682818178324058156[296] = 0;
   out_1682818178324058156[297] = 0;
   out_1682818178324058156[298] = 0;
   out_1682818178324058156[299] = 0;
   out_1682818178324058156[300] = 0;
   out_1682818178324058156[301] = 0;
   out_1682818178324058156[302] = 0;
   out_1682818178324058156[303] = 0;
   out_1682818178324058156[304] = 1;
   out_1682818178324058156[305] = 0;
   out_1682818178324058156[306] = 0;
   out_1682818178324058156[307] = 0;
   out_1682818178324058156[308] = 0;
   out_1682818178324058156[309] = 0;
   out_1682818178324058156[310] = 0;
   out_1682818178324058156[311] = 0;
   out_1682818178324058156[312] = 0;
   out_1682818178324058156[313] = 0;
   out_1682818178324058156[314] = 0;
   out_1682818178324058156[315] = 0;
   out_1682818178324058156[316] = 0;
   out_1682818178324058156[317] = 0;
   out_1682818178324058156[318] = 0;
   out_1682818178324058156[319] = 0;
   out_1682818178324058156[320] = 0;
   out_1682818178324058156[321] = 0;
   out_1682818178324058156[322] = 0;
   out_1682818178324058156[323] = 1;
}
void h_4(double *state, double *unused, double *out_4679628837592052355) {
   out_4679628837592052355[0] = state[6] + state[9];
   out_4679628837592052355[1] = state[7] + state[10];
   out_4679628837592052355[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_43959551107985041) {
   out_43959551107985041[0] = 0;
   out_43959551107985041[1] = 0;
   out_43959551107985041[2] = 0;
   out_43959551107985041[3] = 0;
   out_43959551107985041[4] = 0;
   out_43959551107985041[5] = 0;
   out_43959551107985041[6] = 1;
   out_43959551107985041[7] = 0;
   out_43959551107985041[8] = 0;
   out_43959551107985041[9] = 1;
   out_43959551107985041[10] = 0;
   out_43959551107985041[11] = 0;
   out_43959551107985041[12] = 0;
   out_43959551107985041[13] = 0;
   out_43959551107985041[14] = 0;
   out_43959551107985041[15] = 0;
   out_43959551107985041[16] = 0;
   out_43959551107985041[17] = 0;
   out_43959551107985041[18] = 0;
   out_43959551107985041[19] = 0;
   out_43959551107985041[20] = 0;
   out_43959551107985041[21] = 0;
   out_43959551107985041[22] = 0;
   out_43959551107985041[23] = 0;
   out_43959551107985041[24] = 0;
   out_43959551107985041[25] = 1;
   out_43959551107985041[26] = 0;
   out_43959551107985041[27] = 0;
   out_43959551107985041[28] = 1;
   out_43959551107985041[29] = 0;
   out_43959551107985041[30] = 0;
   out_43959551107985041[31] = 0;
   out_43959551107985041[32] = 0;
   out_43959551107985041[33] = 0;
   out_43959551107985041[34] = 0;
   out_43959551107985041[35] = 0;
   out_43959551107985041[36] = 0;
   out_43959551107985041[37] = 0;
   out_43959551107985041[38] = 0;
   out_43959551107985041[39] = 0;
   out_43959551107985041[40] = 0;
   out_43959551107985041[41] = 0;
   out_43959551107985041[42] = 0;
   out_43959551107985041[43] = 0;
   out_43959551107985041[44] = 1;
   out_43959551107985041[45] = 0;
   out_43959551107985041[46] = 0;
   out_43959551107985041[47] = 1;
   out_43959551107985041[48] = 0;
   out_43959551107985041[49] = 0;
   out_43959551107985041[50] = 0;
   out_43959551107985041[51] = 0;
   out_43959551107985041[52] = 0;
   out_43959551107985041[53] = 0;
}
void h_10(double *state, double *unused, double *out_2324693601250545730) {
   out_2324693601250545730[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_2324693601250545730[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_2324693601250545730[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_2927403466411711434) {
   out_2927403466411711434[0] = 0;
   out_2927403466411711434[1] = 9.8100000000000005*cos(state[1]);
   out_2927403466411711434[2] = 0;
   out_2927403466411711434[3] = 0;
   out_2927403466411711434[4] = -state[8];
   out_2927403466411711434[5] = state[7];
   out_2927403466411711434[6] = 0;
   out_2927403466411711434[7] = state[5];
   out_2927403466411711434[8] = -state[4];
   out_2927403466411711434[9] = 0;
   out_2927403466411711434[10] = 0;
   out_2927403466411711434[11] = 0;
   out_2927403466411711434[12] = 1;
   out_2927403466411711434[13] = 0;
   out_2927403466411711434[14] = 0;
   out_2927403466411711434[15] = 1;
   out_2927403466411711434[16] = 0;
   out_2927403466411711434[17] = 0;
   out_2927403466411711434[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_2927403466411711434[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_2927403466411711434[20] = 0;
   out_2927403466411711434[21] = state[8];
   out_2927403466411711434[22] = 0;
   out_2927403466411711434[23] = -state[6];
   out_2927403466411711434[24] = -state[5];
   out_2927403466411711434[25] = 0;
   out_2927403466411711434[26] = state[3];
   out_2927403466411711434[27] = 0;
   out_2927403466411711434[28] = 0;
   out_2927403466411711434[29] = 0;
   out_2927403466411711434[30] = 0;
   out_2927403466411711434[31] = 1;
   out_2927403466411711434[32] = 0;
   out_2927403466411711434[33] = 0;
   out_2927403466411711434[34] = 1;
   out_2927403466411711434[35] = 0;
   out_2927403466411711434[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_2927403466411711434[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_2927403466411711434[38] = 0;
   out_2927403466411711434[39] = -state[7];
   out_2927403466411711434[40] = state[6];
   out_2927403466411711434[41] = 0;
   out_2927403466411711434[42] = state[4];
   out_2927403466411711434[43] = -state[3];
   out_2927403466411711434[44] = 0;
   out_2927403466411711434[45] = 0;
   out_2927403466411711434[46] = 0;
   out_2927403466411711434[47] = 0;
   out_2927403466411711434[48] = 0;
   out_2927403466411711434[49] = 0;
   out_2927403466411711434[50] = 1;
   out_2927403466411711434[51] = 0;
   out_2927403466411711434[52] = 0;
   out_2927403466411711434[53] = 1;
}
void h_13(double *state, double *unused, double *out_5301313385047798381) {
   out_5301313385047798381[0] = state[3];
   out_5301313385047798381[1] = state[4];
   out_5301313385047798381[2] = state[5];
}
void H_13(double *state, double *unused, double *out_3256233376440317842) {
   out_3256233376440317842[0] = 0;
   out_3256233376440317842[1] = 0;
   out_3256233376440317842[2] = 0;
   out_3256233376440317842[3] = 1;
   out_3256233376440317842[4] = 0;
   out_3256233376440317842[5] = 0;
   out_3256233376440317842[6] = 0;
   out_3256233376440317842[7] = 0;
   out_3256233376440317842[8] = 0;
   out_3256233376440317842[9] = 0;
   out_3256233376440317842[10] = 0;
   out_3256233376440317842[11] = 0;
   out_3256233376440317842[12] = 0;
   out_3256233376440317842[13] = 0;
   out_3256233376440317842[14] = 0;
   out_3256233376440317842[15] = 0;
   out_3256233376440317842[16] = 0;
   out_3256233376440317842[17] = 0;
   out_3256233376440317842[18] = 0;
   out_3256233376440317842[19] = 0;
   out_3256233376440317842[20] = 0;
   out_3256233376440317842[21] = 0;
   out_3256233376440317842[22] = 1;
   out_3256233376440317842[23] = 0;
   out_3256233376440317842[24] = 0;
   out_3256233376440317842[25] = 0;
   out_3256233376440317842[26] = 0;
   out_3256233376440317842[27] = 0;
   out_3256233376440317842[28] = 0;
   out_3256233376440317842[29] = 0;
   out_3256233376440317842[30] = 0;
   out_3256233376440317842[31] = 0;
   out_3256233376440317842[32] = 0;
   out_3256233376440317842[33] = 0;
   out_3256233376440317842[34] = 0;
   out_3256233376440317842[35] = 0;
   out_3256233376440317842[36] = 0;
   out_3256233376440317842[37] = 0;
   out_3256233376440317842[38] = 0;
   out_3256233376440317842[39] = 0;
   out_3256233376440317842[40] = 0;
   out_3256233376440317842[41] = 1;
   out_3256233376440317842[42] = 0;
   out_3256233376440317842[43] = 0;
   out_3256233376440317842[44] = 0;
   out_3256233376440317842[45] = 0;
   out_3256233376440317842[46] = 0;
   out_3256233376440317842[47] = 0;
   out_3256233376440317842[48] = 0;
   out_3256233376440317842[49] = 0;
   out_3256233376440317842[50] = 0;
   out_3256233376440317842[51] = 0;
   out_3256233376440317842[52] = 0;
   out_3256233376440317842[53] = 0;
}
void h_14(double *state, double *unused, double *out_5419473069984494684) {
   out_5419473069984494684[0] = state[6];
   out_5419473069984494684[1] = state[7];
   out_5419473069984494684[2] = state[8];
}
void H_14(double *state, double *unused, double *out_3038828881187387255) {
   out_3038828881187387255[0] = 0;
   out_3038828881187387255[1] = 0;
   out_3038828881187387255[2] = 0;
   out_3038828881187387255[3] = 0;
   out_3038828881187387255[4] = 0;
   out_3038828881187387255[5] = 0;
   out_3038828881187387255[6] = 1;
   out_3038828881187387255[7] = 0;
   out_3038828881187387255[8] = 0;
   out_3038828881187387255[9] = 0;
   out_3038828881187387255[10] = 0;
   out_3038828881187387255[11] = 0;
   out_3038828881187387255[12] = 0;
   out_3038828881187387255[13] = 0;
   out_3038828881187387255[14] = 0;
   out_3038828881187387255[15] = 0;
   out_3038828881187387255[16] = 0;
   out_3038828881187387255[17] = 0;
   out_3038828881187387255[18] = 0;
   out_3038828881187387255[19] = 0;
   out_3038828881187387255[20] = 0;
   out_3038828881187387255[21] = 0;
   out_3038828881187387255[22] = 0;
   out_3038828881187387255[23] = 0;
   out_3038828881187387255[24] = 0;
   out_3038828881187387255[25] = 1;
   out_3038828881187387255[26] = 0;
   out_3038828881187387255[27] = 0;
   out_3038828881187387255[28] = 0;
   out_3038828881187387255[29] = 0;
   out_3038828881187387255[30] = 0;
   out_3038828881187387255[31] = 0;
   out_3038828881187387255[32] = 0;
   out_3038828881187387255[33] = 0;
   out_3038828881187387255[34] = 0;
   out_3038828881187387255[35] = 0;
   out_3038828881187387255[36] = 0;
   out_3038828881187387255[37] = 0;
   out_3038828881187387255[38] = 0;
   out_3038828881187387255[39] = 0;
   out_3038828881187387255[40] = 0;
   out_3038828881187387255[41] = 0;
   out_3038828881187387255[42] = 0;
   out_3038828881187387255[43] = 0;
   out_3038828881187387255[44] = 1;
   out_3038828881187387255[45] = 0;
   out_3038828881187387255[46] = 0;
   out_3038828881187387255[47] = 0;
   out_3038828881187387255[48] = 0;
   out_3038828881187387255[49] = 0;
   out_3038828881187387255[50] = 0;
   out_3038828881187387255[51] = 0;
   out_3038828881187387255[52] = 0;
   out_3038828881187387255[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_2282374485527159040) {
  err_fun(nom_x, delta_x, out_2282374485527159040);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_4495318315073688785) {
  inv_err_fun(nom_x, true_x, out_4495318315073688785);
}
void pose_H_mod_fun(double *state, double *out_8988321217165806383) {
  H_mod_fun(state, out_8988321217165806383);
}
void pose_f_fun(double *state, double dt, double *out_7445325378273019703) {
  f_fun(state,  dt, out_7445325378273019703);
}
void pose_F_fun(double *state, double dt, double *out_1682818178324058156) {
  F_fun(state,  dt, out_1682818178324058156);
}
void pose_h_4(double *state, double *unused, double *out_4679628837592052355) {
  h_4(state, unused, out_4679628837592052355);
}
void pose_H_4(double *state, double *unused, double *out_43959551107985041) {
  H_4(state, unused, out_43959551107985041);
}
void pose_h_10(double *state, double *unused, double *out_2324693601250545730) {
  h_10(state, unused, out_2324693601250545730);
}
void pose_H_10(double *state, double *unused, double *out_2927403466411711434) {
  H_10(state, unused, out_2927403466411711434);
}
void pose_h_13(double *state, double *unused, double *out_5301313385047798381) {
  h_13(state, unused, out_5301313385047798381);
}
void pose_H_13(double *state, double *unused, double *out_3256233376440317842) {
  H_13(state, unused, out_3256233376440317842);
}
void pose_h_14(double *state, double *unused, double *out_5419473069984494684) {
  h_14(state, unused, out_5419473069984494684);
}
void pose_H_14(double *state, double *unused, double *out_3038828881187387255) {
  H_14(state, unused, out_3038828881187387255);
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
