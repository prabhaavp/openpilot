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
void err_fun(double *nom_x, double *delta_x, double *out_3350640148905151952) {
   out_3350640148905151952[0] = delta_x[0] + nom_x[0];
   out_3350640148905151952[1] = delta_x[1] + nom_x[1];
   out_3350640148905151952[2] = delta_x[2] + nom_x[2];
   out_3350640148905151952[3] = delta_x[3] + nom_x[3];
   out_3350640148905151952[4] = delta_x[4] + nom_x[4];
   out_3350640148905151952[5] = delta_x[5] + nom_x[5];
   out_3350640148905151952[6] = delta_x[6] + nom_x[6];
   out_3350640148905151952[7] = delta_x[7] + nom_x[7];
   out_3350640148905151952[8] = delta_x[8] + nom_x[8];
   out_3350640148905151952[9] = delta_x[9] + nom_x[9];
   out_3350640148905151952[10] = delta_x[10] + nom_x[10];
   out_3350640148905151952[11] = delta_x[11] + nom_x[11];
   out_3350640148905151952[12] = delta_x[12] + nom_x[12];
   out_3350640148905151952[13] = delta_x[13] + nom_x[13];
   out_3350640148905151952[14] = delta_x[14] + nom_x[14];
   out_3350640148905151952[15] = delta_x[15] + nom_x[15];
   out_3350640148905151952[16] = delta_x[16] + nom_x[16];
   out_3350640148905151952[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_5937141942059359877) {
   out_5937141942059359877[0] = -nom_x[0] + true_x[0];
   out_5937141942059359877[1] = -nom_x[1] + true_x[1];
   out_5937141942059359877[2] = -nom_x[2] + true_x[2];
   out_5937141942059359877[3] = -nom_x[3] + true_x[3];
   out_5937141942059359877[4] = -nom_x[4] + true_x[4];
   out_5937141942059359877[5] = -nom_x[5] + true_x[5];
   out_5937141942059359877[6] = -nom_x[6] + true_x[6];
   out_5937141942059359877[7] = -nom_x[7] + true_x[7];
   out_5937141942059359877[8] = -nom_x[8] + true_x[8];
   out_5937141942059359877[9] = -nom_x[9] + true_x[9];
   out_5937141942059359877[10] = -nom_x[10] + true_x[10];
   out_5937141942059359877[11] = -nom_x[11] + true_x[11];
   out_5937141942059359877[12] = -nom_x[12] + true_x[12];
   out_5937141942059359877[13] = -nom_x[13] + true_x[13];
   out_5937141942059359877[14] = -nom_x[14] + true_x[14];
   out_5937141942059359877[15] = -nom_x[15] + true_x[15];
   out_5937141942059359877[16] = -nom_x[16] + true_x[16];
   out_5937141942059359877[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_4176406737755320299) {
   out_4176406737755320299[0] = 1.0;
   out_4176406737755320299[1] = 0.0;
   out_4176406737755320299[2] = 0.0;
   out_4176406737755320299[3] = 0.0;
   out_4176406737755320299[4] = 0.0;
   out_4176406737755320299[5] = 0.0;
   out_4176406737755320299[6] = 0.0;
   out_4176406737755320299[7] = 0.0;
   out_4176406737755320299[8] = 0.0;
   out_4176406737755320299[9] = 0.0;
   out_4176406737755320299[10] = 0.0;
   out_4176406737755320299[11] = 0.0;
   out_4176406737755320299[12] = 0.0;
   out_4176406737755320299[13] = 0.0;
   out_4176406737755320299[14] = 0.0;
   out_4176406737755320299[15] = 0.0;
   out_4176406737755320299[16] = 0.0;
   out_4176406737755320299[17] = 0.0;
   out_4176406737755320299[18] = 0.0;
   out_4176406737755320299[19] = 1.0;
   out_4176406737755320299[20] = 0.0;
   out_4176406737755320299[21] = 0.0;
   out_4176406737755320299[22] = 0.0;
   out_4176406737755320299[23] = 0.0;
   out_4176406737755320299[24] = 0.0;
   out_4176406737755320299[25] = 0.0;
   out_4176406737755320299[26] = 0.0;
   out_4176406737755320299[27] = 0.0;
   out_4176406737755320299[28] = 0.0;
   out_4176406737755320299[29] = 0.0;
   out_4176406737755320299[30] = 0.0;
   out_4176406737755320299[31] = 0.0;
   out_4176406737755320299[32] = 0.0;
   out_4176406737755320299[33] = 0.0;
   out_4176406737755320299[34] = 0.0;
   out_4176406737755320299[35] = 0.0;
   out_4176406737755320299[36] = 0.0;
   out_4176406737755320299[37] = 0.0;
   out_4176406737755320299[38] = 1.0;
   out_4176406737755320299[39] = 0.0;
   out_4176406737755320299[40] = 0.0;
   out_4176406737755320299[41] = 0.0;
   out_4176406737755320299[42] = 0.0;
   out_4176406737755320299[43] = 0.0;
   out_4176406737755320299[44] = 0.0;
   out_4176406737755320299[45] = 0.0;
   out_4176406737755320299[46] = 0.0;
   out_4176406737755320299[47] = 0.0;
   out_4176406737755320299[48] = 0.0;
   out_4176406737755320299[49] = 0.0;
   out_4176406737755320299[50] = 0.0;
   out_4176406737755320299[51] = 0.0;
   out_4176406737755320299[52] = 0.0;
   out_4176406737755320299[53] = 0.0;
   out_4176406737755320299[54] = 0.0;
   out_4176406737755320299[55] = 0.0;
   out_4176406737755320299[56] = 0.0;
   out_4176406737755320299[57] = 1.0;
   out_4176406737755320299[58] = 0.0;
   out_4176406737755320299[59] = 0.0;
   out_4176406737755320299[60] = 0.0;
   out_4176406737755320299[61] = 0.0;
   out_4176406737755320299[62] = 0.0;
   out_4176406737755320299[63] = 0.0;
   out_4176406737755320299[64] = 0.0;
   out_4176406737755320299[65] = 0.0;
   out_4176406737755320299[66] = 0.0;
   out_4176406737755320299[67] = 0.0;
   out_4176406737755320299[68] = 0.0;
   out_4176406737755320299[69] = 0.0;
   out_4176406737755320299[70] = 0.0;
   out_4176406737755320299[71] = 0.0;
   out_4176406737755320299[72] = 0.0;
   out_4176406737755320299[73] = 0.0;
   out_4176406737755320299[74] = 0.0;
   out_4176406737755320299[75] = 0.0;
   out_4176406737755320299[76] = 1.0;
   out_4176406737755320299[77] = 0.0;
   out_4176406737755320299[78] = 0.0;
   out_4176406737755320299[79] = 0.0;
   out_4176406737755320299[80] = 0.0;
   out_4176406737755320299[81] = 0.0;
   out_4176406737755320299[82] = 0.0;
   out_4176406737755320299[83] = 0.0;
   out_4176406737755320299[84] = 0.0;
   out_4176406737755320299[85] = 0.0;
   out_4176406737755320299[86] = 0.0;
   out_4176406737755320299[87] = 0.0;
   out_4176406737755320299[88] = 0.0;
   out_4176406737755320299[89] = 0.0;
   out_4176406737755320299[90] = 0.0;
   out_4176406737755320299[91] = 0.0;
   out_4176406737755320299[92] = 0.0;
   out_4176406737755320299[93] = 0.0;
   out_4176406737755320299[94] = 0.0;
   out_4176406737755320299[95] = 1.0;
   out_4176406737755320299[96] = 0.0;
   out_4176406737755320299[97] = 0.0;
   out_4176406737755320299[98] = 0.0;
   out_4176406737755320299[99] = 0.0;
   out_4176406737755320299[100] = 0.0;
   out_4176406737755320299[101] = 0.0;
   out_4176406737755320299[102] = 0.0;
   out_4176406737755320299[103] = 0.0;
   out_4176406737755320299[104] = 0.0;
   out_4176406737755320299[105] = 0.0;
   out_4176406737755320299[106] = 0.0;
   out_4176406737755320299[107] = 0.0;
   out_4176406737755320299[108] = 0.0;
   out_4176406737755320299[109] = 0.0;
   out_4176406737755320299[110] = 0.0;
   out_4176406737755320299[111] = 0.0;
   out_4176406737755320299[112] = 0.0;
   out_4176406737755320299[113] = 0.0;
   out_4176406737755320299[114] = 1.0;
   out_4176406737755320299[115] = 0.0;
   out_4176406737755320299[116] = 0.0;
   out_4176406737755320299[117] = 0.0;
   out_4176406737755320299[118] = 0.0;
   out_4176406737755320299[119] = 0.0;
   out_4176406737755320299[120] = 0.0;
   out_4176406737755320299[121] = 0.0;
   out_4176406737755320299[122] = 0.0;
   out_4176406737755320299[123] = 0.0;
   out_4176406737755320299[124] = 0.0;
   out_4176406737755320299[125] = 0.0;
   out_4176406737755320299[126] = 0.0;
   out_4176406737755320299[127] = 0.0;
   out_4176406737755320299[128] = 0.0;
   out_4176406737755320299[129] = 0.0;
   out_4176406737755320299[130] = 0.0;
   out_4176406737755320299[131] = 0.0;
   out_4176406737755320299[132] = 0.0;
   out_4176406737755320299[133] = 1.0;
   out_4176406737755320299[134] = 0.0;
   out_4176406737755320299[135] = 0.0;
   out_4176406737755320299[136] = 0.0;
   out_4176406737755320299[137] = 0.0;
   out_4176406737755320299[138] = 0.0;
   out_4176406737755320299[139] = 0.0;
   out_4176406737755320299[140] = 0.0;
   out_4176406737755320299[141] = 0.0;
   out_4176406737755320299[142] = 0.0;
   out_4176406737755320299[143] = 0.0;
   out_4176406737755320299[144] = 0.0;
   out_4176406737755320299[145] = 0.0;
   out_4176406737755320299[146] = 0.0;
   out_4176406737755320299[147] = 0.0;
   out_4176406737755320299[148] = 0.0;
   out_4176406737755320299[149] = 0.0;
   out_4176406737755320299[150] = 0.0;
   out_4176406737755320299[151] = 0.0;
   out_4176406737755320299[152] = 1.0;
   out_4176406737755320299[153] = 0.0;
   out_4176406737755320299[154] = 0.0;
   out_4176406737755320299[155] = 0.0;
   out_4176406737755320299[156] = 0.0;
   out_4176406737755320299[157] = 0.0;
   out_4176406737755320299[158] = 0.0;
   out_4176406737755320299[159] = 0.0;
   out_4176406737755320299[160] = 0.0;
   out_4176406737755320299[161] = 0.0;
   out_4176406737755320299[162] = 0.0;
   out_4176406737755320299[163] = 0.0;
   out_4176406737755320299[164] = 0.0;
   out_4176406737755320299[165] = 0.0;
   out_4176406737755320299[166] = 0.0;
   out_4176406737755320299[167] = 0.0;
   out_4176406737755320299[168] = 0.0;
   out_4176406737755320299[169] = 0.0;
   out_4176406737755320299[170] = 0.0;
   out_4176406737755320299[171] = 1.0;
   out_4176406737755320299[172] = 0.0;
   out_4176406737755320299[173] = 0.0;
   out_4176406737755320299[174] = 0.0;
   out_4176406737755320299[175] = 0.0;
   out_4176406737755320299[176] = 0.0;
   out_4176406737755320299[177] = 0.0;
   out_4176406737755320299[178] = 0.0;
   out_4176406737755320299[179] = 0.0;
   out_4176406737755320299[180] = 0.0;
   out_4176406737755320299[181] = 0.0;
   out_4176406737755320299[182] = 0.0;
   out_4176406737755320299[183] = 0.0;
   out_4176406737755320299[184] = 0.0;
   out_4176406737755320299[185] = 0.0;
   out_4176406737755320299[186] = 0.0;
   out_4176406737755320299[187] = 0.0;
   out_4176406737755320299[188] = 0.0;
   out_4176406737755320299[189] = 0.0;
   out_4176406737755320299[190] = 1.0;
   out_4176406737755320299[191] = 0.0;
   out_4176406737755320299[192] = 0.0;
   out_4176406737755320299[193] = 0.0;
   out_4176406737755320299[194] = 0.0;
   out_4176406737755320299[195] = 0.0;
   out_4176406737755320299[196] = 0.0;
   out_4176406737755320299[197] = 0.0;
   out_4176406737755320299[198] = 0.0;
   out_4176406737755320299[199] = 0.0;
   out_4176406737755320299[200] = 0.0;
   out_4176406737755320299[201] = 0.0;
   out_4176406737755320299[202] = 0.0;
   out_4176406737755320299[203] = 0.0;
   out_4176406737755320299[204] = 0.0;
   out_4176406737755320299[205] = 0.0;
   out_4176406737755320299[206] = 0.0;
   out_4176406737755320299[207] = 0.0;
   out_4176406737755320299[208] = 0.0;
   out_4176406737755320299[209] = 1.0;
   out_4176406737755320299[210] = 0.0;
   out_4176406737755320299[211] = 0.0;
   out_4176406737755320299[212] = 0.0;
   out_4176406737755320299[213] = 0.0;
   out_4176406737755320299[214] = 0.0;
   out_4176406737755320299[215] = 0.0;
   out_4176406737755320299[216] = 0.0;
   out_4176406737755320299[217] = 0.0;
   out_4176406737755320299[218] = 0.0;
   out_4176406737755320299[219] = 0.0;
   out_4176406737755320299[220] = 0.0;
   out_4176406737755320299[221] = 0.0;
   out_4176406737755320299[222] = 0.0;
   out_4176406737755320299[223] = 0.0;
   out_4176406737755320299[224] = 0.0;
   out_4176406737755320299[225] = 0.0;
   out_4176406737755320299[226] = 0.0;
   out_4176406737755320299[227] = 0.0;
   out_4176406737755320299[228] = 1.0;
   out_4176406737755320299[229] = 0.0;
   out_4176406737755320299[230] = 0.0;
   out_4176406737755320299[231] = 0.0;
   out_4176406737755320299[232] = 0.0;
   out_4176406737755320299[233] = 0.0;
   out_4176406737755320299[234] = 0.0;
   out_4176406737755320299[235] = 0.0;
   out_4176406737755320299[236] = 0.0;
   out_4176406737755320299[237] = 0.0;
   out_4176406737755320299[238] = 0.0;
   out_4176406737755320299[239] = 0.0;
   out_4176406737755320299[240] = 0.0;
   out_4176406737755320299[241] = 0.0;
   out_4176406737755320299[242] = 0.0;
   out_4176406737755320299[243] = 0.0;
   out_4176406737755320299[244] = 0.0;
   out_4176406737755320299[245] = 0.0;
   out_4176406737755320299[246] = 0.0;
   out_4176406737755320299[247] = 1.0;
   out_4176406737755320299[248] = 0.0;
   out_4176406737755320299[249] = 0.0;
   out_4176406737755320299[250] = 0.0;
   out_4176406737755320299[251] = 0.0;
   out_4176406737755320299[252] = 0.0;
   out_4176406737755320299[253] = 0.0;
   out_4176406737755320299[254] = 0.0;
   out_4176406737755320299[255] = 0.0;
   out_4176406737755320299[256] = 0.0;
   out_4176406737755320299[257] = 0.0;
   out_4176406737755320299[258] = 0.0;
   out_4176406737755320299[259] = 0.0;
   out_4176406737755320299[260] = 0.0;
   out_4176406737755320299[261] = 0.0;
   out_4176406737755320299[262] = 0.0;
   out_4176406737755320299[263] = 0.0;
   out_4176406737755320299[264] = 0.0;
   out_4176406737755320299[265] = 0.0;
   out_4176406737755320299[266] = 1.0;
   out_4176406737755320299[267] = 0.0;
   out_4176406737755320299[268] = 0.0;
   out_4176406737755320299[269] = 0.0;
   out_4176406737755320299[270] = 0.0;
   out_4176406737755320299[271] = 0.0;
   out_4176406737755320299[272] = 0.0;
   out_4176406737755320299[273] = 0.0;
   out_4176406737755320299[274] = 0.0;
   out_4176406737755320299[275] = 0.0;
   out_4176406737755320299[276] = 0.0;
   out_4176406737755320299[277] = 0.0;
   out_4176406737755320299[278] = 0.0;
   out_4176406737755320299[279] = 0.0;
   out_4176406737755320299[280] = 0.0;
   out_4176406737755320299[281] = 0.0;
   out_4176406737755320299[282] = 0.0;
   out_4176406737755320299[283] = 0.0;
   out_4176406737755320299[284] = 0.0;
   out_4176406737755320299[285] = 1.0;
   out_4176406737755320299[286] = 0.0;
   out_4176406737755320299[287] = 0.0;
   out_4176406737755320299[288] = 0.0;
   out_4176406737755320299[289] = 0.0;
   out_4176406737755320299[290] = 0.0;
   out_4176406737755320299[291] = 0.0;
   out_4176406737755320299[292] = 0.0;
   out_4176406737755320299[293] = 0.0;
   out_4176406737755320299[294] = 0.0;
   out_4176406737755320299[295] = 0.0;
   out_4176406737755320299[296] = 0.0;
   out_4176406737755320299[297] = 0.0;
   out_4176406737755320299[298] = 0.0;
   out_4176406737755320299[299] = 0.0;
   out_4176406737755320299[300] = 0.0;
   out_4176406737755320299[301] = 0.0;
   out_4176406737755320299[302] = 0.0;
   out_4176406737755320299[303] = 0.0;
   out_4176406737755320299[304] = 1.0;
   out_4176406737755320299[305] = 0.0;
   out_4176406737755320299[306] = 0.0;
   out_4176406737755320299[307] = 0.0;
   out_4176406737755320299[308] = 0.0;
   out_4176406737755320299[309] = 0.0;
   out_4176406737755320299[310] = 0.0;
   out_4176406737755320299[311] = 0.0;
   out_4176406737755320299[312] = 0.0;
   out_4176406737755320299[313] = 0.0;
   out_4176406737755320299[314] = 0.0;
   out_4176406737755320299[315] = 0.0;
   out_4176406737755320299[316] = 0.0;
   out_4176406737755320299[317] = 0.0;
   out_4176406737755320299[318] = 0.0;
   out_4176406737755320299[319] = 0.0;
   out_4176406737755320299[320] = 0.0;
   out_4176406737755320299[321] = 0.0;
   out_4176406737755320299[322] = 0.0;
   out_4176406737755320299[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_3911862024342109205) {
   out_3911862024342109205[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_3911862024342109205[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_3911862024342109205[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_3911862024342109205[3] = dt*state[12] + state[3];
   out_3911862024342109205[4] = dt*state[13] + state[4];
   out_3911862024342109205[5] = dt*state[14] + state[5];
   out_3911862024342109205[6] = state[6];
   out_3911862024342109205[7] = state[7];
   out_3911862024342109205[8] = state[8];
   out_3911862024342109205[9] = state[9];
   out_3911862024342109205[10] = state[10];
   out_3911862024342109205[11] = state[11];
   out_3911862024342109205[12] = state[12];
   out_3911862024342109205[13] = state[13];
   out_3911862024342109205[14] = state[14];
   out_3911862024342109205[15] = state[15];
   out_3911862024342109205[16] = state[16];
   out_3911862024342109205[17] = state[17];
}
void F_fun(double *state, double dt, double *out_7558296450784675551) {
   out_7558296450784675551[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7558296450784675551[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7558296450784675551[2] = 0;
   out_7558296450784675551[3] = 0;
   out_7558296450784675551[4] = 0;
   out_7558296450784675551[5] = 0;
   out_7558296450784675551[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7558296450784675551[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7558296450784675551[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7558296450784675551[9] = 0;
   out_7558296450784675551[10] = 0;
   out_7558296450784675551[11] = 0;
   out_7558296450784675551[12] = 0;
   out_7558296450784675551[13] = 0;
   out_7558296450784675551[14] = 0;
   out_7558296450784675551[15] = 0;
   out_7558296450784675551[16] = 0;
   out_7558296450784675551[17] = 0;
   out_7558296450784675551[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7558296450784675551[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7558296450784675551[20] = 0;
   out_7558296450784675551[21] = 0;
   out_7558296450784675551[22] = 0;
   out_7558296450784675551[23] = 0;
   out_7558296450784675551[24] = 0;
   out_7558296450784675551[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7558296450784675551[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7558296450784675551[27] = 0;
   out_7558296450784675551[28] = 0;
   out_7558296450784675551[29] = 0;
   out_7558296450784675551[30] = 0;
   out_7558296450784675551[31] = 0;
   out_7558296450784675551[32] = 0;
   out_7558296450784675551[33] = 0;
   out_7558296450784675551[34] = 0;
   out_7558296450784675551[35] = 0;
   out_7558296450784675551[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7558296450784675551[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7558296450784675551[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7558296450784675551[39] = 0;
   out_7558296450784675551[40] = 0;
   out_7558296450784675551[41] = 0;
   out_7558296450784675551[42] = 0;
   out_7558296450784675551[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7558296450784675551[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7558296450784675551[45] = 0;
   out_7558296450784675551[46] = 0;
   out_7558296450784675551[47] = 0;
   out_7558296450784675551[48] = 0;
   out_7558296450784675551[49] = 0;
   out_7558296450784675551[50] = 0;
   out_7558296450784675551[51] = 0;
   out_7558296450784675551[52] = 0;
   out_7558296450784675551[53] = 0;
   out_7558296450784675551[54] = 0;
   out_7558296450784675551[55] = 0;
   out_7558296450784675551[56] = 0;
   out_7558296450784675551[57] = 1;
   out_7558296450784675551[58] = 0;
   out_7558296450784675551[59] = 0;
   out_7558296450784675551[60] = 0;
   out_7558296450784675551[61] = 0;
   out_7558296450784675551[62] = 0;
   out_7558296450784675551[63] = 0;
   out_7558296450784675551[64] = 0;
   out_7558296450784675551[65] = 0;
   out_7558296450784675551[66] = dt;
   out_7558296450784675551[67] = 0;
   out_7558296450784675551[68] = 0;
   out_7558296450784675551[69] = 0;
   out_7558296450784675551[70] = 0;
   out_7558296450784675551[71] = 0;
   out_7558296450784675551[72] = 0;
   out_7558296450784675551[73] = 0;
   out_7558296450784675551[74] = 0;
   out_7558296450784675551[75] = 0;
   out_7558296450784675551[76] = 1;
   out_7558296450784675551[77] = 0;
   out_7558296450784675551[78] = 0;
   out_7558296450784675551[79] = 0;
   out_7558296450784675551[80] = 0;
   out_7558296450784675551[81] = 0;
   out_7558296450784675551[82] = 0;
   out_7558296450784675551[83] = 0;
   out_7558296450784675551[84] = 0;
   out_7558296450784675551[85] = dt;
   out_7558296450784675551[86] = 0;
   out_7558296450784675551[87] = 0;
   out_7558296450784675551[88] = 0;
   out_7558296450784675551[89] = 0;
   out_7558296450784675551[90] = 0;
   out_7558296450784675551[91] = 0;
   out_7558296450784675551[92] = 0;
   out_7558296450784675551[93] = 0;
   out_7558296450784675551[94] = 0;
   out_7558296450784675551[95] = 1;
   out_7558296450784675551[96] = 0;
   out_7558296450784675551[97] = 0;
   out_7558296450784675551[98] = 0;
   out_7558296450784675551[99] = 0;
   out_7558296450784675551[100] = 0;
   out_7558296450784675551[101] = 0;
   out_7558296450784675551[102] = 0;
   out_7558296450784675551[103] = 0;
   out_7558296450784675551[104] = dt;
   out_7558296450784675551[105] = 0;
   out_7558296450784675551[106] = 0;
   out_7558296450784675551[107] = 0;
   out_7558296450784675551[108] = 0;
   out_7558296450784675551[109] = 0;
   out_7558296450784675551[110] = 0;
   out_7558296450784675551[111] = 0;
   out_7558296450784675551[112] = 0;
   out_7558296450784675551[113] = 0;
   out_7558296450784675551[114] = 1;
   out_7558296450784675551[115] = 0;
   out_7558296450784675551[116] = 0;
   out_7558296450784675551[117] = 0;
   out_7558296450784675551[118] = 0;
   out_7558296450784675551[119] = 0;
   out_7558296450784675551[120] = 0;
   out_7558296450784675551[121] = 0;
   out_7558296450784675551[122] = 0;
   out_7558296450784675551[123] = 0;
   out_7558296450784675551[124] = 0;
   out_7558296450784675551[125] = 0;
   out_7558296450784675551[126] = 0;
   out_7558296450784675551[127] = 0;
   out_7558296450784675551[128] = 0;
   out_7558296450784675551[129] = 0;
   out_7558296450784675551[130] = 0;
   out_7558296450784675551[131] = 0;
   out_7558296450784675551[132] = 0;
   out_7558296450784675551[133] = 1;
   out_7558296450784675551[134] = 0;
   out_7558296450784675551[135] = 0;
   out_7558296450784675551[136] = 0;
   out_7558296450784675551[137] = 0;
   out_7558296450784675551[138] = 0;
   out_7558296450784675551[139] = 0;
   out_7558296450784675551[140] = 0;
   out_7558296450784675551[141] = 0;
   out_7558296450784675551[142] = 0;
   out_7558296450784675551[143] = 0;
   out_7558296450784675551[144] = 0;
   out_7558296450784675551[145] = 0;
   out_7558296450784675551[146] = 0;
   out_7558296450784675551[147] = 0;
   out_7558296450784675551[148] = 0;
   out_7558296450784675551[149] = 0;
   out_7558296450784675551[150] = 0;
   out_7558296450784675551[151] = 0;
   out_7558296450784675551[152] = 1;
   out_7558296450784675551[153] = 0;
   out_7558296450784675551[154] = 0;
   out_7558296450784675551[155] = 0;
   out_7558296450784675551[156] = 0;
   out_7558296450784675551[157] = 0;
   out_7558296450784675551[158] = 0;
   out_7558296450784675551[159] = 0;
   out_7558296450784675551[160] = 0;
   out_7558296450784675551[161] = 0;
   out_7558296450784675551[162] = 0;
   out_7558296450784675551[163] = 0;
   out_7558296450784675551[164] = 0;
   out_7558296450784675551[165] = 0;
   out_7558296450784675551[166] = 0;
   out_7558296450784675551[167] = 0;
   out_7558296450784675551[168] = 0;
   out_7558296450784675551[169] = 0;
   out_7558296450784675551[170] = 0;
   out_7558296450784675551[171] = 1;
   out_7558296450784675551[172] = 0;
   out_7558296450784675551[173] = 0;
   out_7558296450784675551[174] = 0;
   out_7558296450784675551[175] = 0;
   out_7558296450784675551[176] = 0;
   out_7558296450784675551[177] = 0;
   out_7558296450784675551[178] = 0;
   out_7558296450784675551[179] = 0;
   out_7558296450784675551[180] = 0;
   out_7558296450784675551[181] = 0;
   out_7558296450784675551[182] = 0;
   out_7558296450784675551[183] = 0;
   out_7558296450784675551[184] = 0;
   out_7558296450784675551[185] = 0;
   out_7558296450784675551[186] = 0;
   out_7558296450784675551[187] = 0;
   out_7558296450784675551[188] = 0;
   out_7558296450784675551[189] = 0;
   out_7558296450784675551[190] = 1;
   out_7558296450784675551[191] = 0;
   out_7558296450784675551[192] = 0;
   out_7558296450784675551[193] = 0;
   out_7558296450784675551[194] = 0;
   out_7558296450784675551[195] = 0;
   out_7558296450784675551[196] = 0;
   out_7558296450784675551[197] = 0;
   out_7558296450784675551[198] = 0;
   out_7558296450784675551[199] = 0;
   out_7558296450784675551[200] = 0;
   out_7558296450784675551[201] = 0;
   out_7558296450784675551[202] = 0;
   out_7558296450784675551[203] = 0;
   out_7558296450784675551[204] = 0;
   out_7558296450784675551[205] = 0;
   out_7558296450784675551[206] = 0;
   out_7558296450784675551[207] = 0;
   out_7558296450784675551[208] = 0;
   out_7558296450784675551[209] = 1;
   out_7558296450784675551[210] = 0;
   out_7558296450784675551[211] = 0;
   out_7558296450784675551[212] = 0;
   out_7558296450784675551[213] = 0;
   out_7558296450784675551[214] = 0;
   out_7558296450784675551[215] = 0;
   out_7558296450784675551[216] = 0;
   out_7558296450784675551[217] = 0;
   out_7558296450784675551[218] = 0;
   out_7558296450784675551[219] = 0;
   out_7558296450784675551[220] = 0;
   out_7558296450784675551[221] = 0;
   out_7558296450784675551[222] = 0;
   out_7558296450784675551[223] = 0;
   out_7558296450784675551[224] = 0;
   out_7558296450784675551[225] = 0;
   out_7558296450784675551[226] = 0;
   out_7558296450784675551[227] = 0;
   out_7558296450784675551[228] = 1;
   out_7558296450784675551[229] = 0;
   out_7558296450784675551[230] = 0;
   out_7558296450784675551[231] = 0;
   out_7558296450784675551[232] = 0;
   out_7558296450784675551[233] = 0;
   out_7558296450784675551[234] = 0;
   out_7558296450784675551[235] = 0;
   out_7558296450784675551[236] = 0;
   out_7558296450784675551[237] = 0;
   out_7558296450784675551[238] = 0;
   out_7558296450784675551[239] = 0;
   out_7558296450784675551[240] = 0;
   out_7558296450784675551[241] = 0;
   out_7558296450784675551[242] = 0;
   out_7558296450784675551[243] = 0;
   out_7558296450784675551[244] = 0;
   out_7558296450784675551[245] = 0;
   out_7558296450784675551[246] = 0;
   out_7558296450784675551[247] = 1;
   out_7558296450784675551[248] = 0;
   out_7558296450784675551[249] = 0;
   out_7558296450784675551[250] = 0;
   out_7558296450784675551[251] = 0;
   out_7558296450784675551[252] = 0;
   out_7558296450784675551[253] = 0;
   out_7558296450784675551[254] = 0;
   out_7558296450784675551[255] = 0;
   out_7558296450784675551[256] = 0;
   out_7558296450784675551[257] = 0;
   out_7558296450784675551[258] = 0;
   out_7558296450784675551[259] = 0;
   out_7558296450784675551[260] = 0;
   out_7558296450784675551[261] = 0;
   out_7558296450784675551[262] = 0;
   out_7558296450784675551[263] = 0;
   out_7558296450784675551[264] = 0;
   out_7558296450784675551[265] = 0;
   out_7558296450784675551[266] = 1;
   out_7558296450784675551[267] = 0;
   out_7558296450784675551[268] = 0;
   out_7558296450784675551[269] = 0;
   out_7558296450784675551[270] = 0;
   out_7558296450784675551[271] = 0;
   out_7558296450784675551[272] = 0;
   out_7558296450784675551[273] = 0;
   out_7558296450784675551[274] = 0;
   out_7558296450784675551[275] = 0;
   out_7558296450784675551[276] = 0;
   out_7558296450784675551[277] = 0;
   out_7558296450784675551[278] = 0;
   out_7558296450784675551[279] = 0;
   out_7558296450784675551[280] = 0;
   out_7558296450784675551[281] = 0;
   out_7558296450784675551[282] = 0;
   out_7558296450784675551[283] = 0;
   out_7558296450784675551[284] = 0;
   out_7558296450784675551[285] = 1;
   out_7558296450784675551[286] = 0;
   out_7558296450784675551[287] = 0;
   out_7558296450784675551[288] = 0;
   out_7558296450784675551[289] = 0;
   out_7558296450784675551[290] = 0;
   out_7558296450784675551[291] = 0;
   out_7558296450784675551[292] = 0;
   out_7558296450784675551[293] = 0;
   out_7558296450784675551[294] = 0;
   out_7558296450784675551[295] = 0;
   out_7558296450784675551[296] = 0;
   out_7558296450784675551[297] = 0;
   out_7558296450784675551[298] = 0;
   out_7558296450784675551[299] = 0;
   out_7558296450784675551[300] = 0;
   out_7558296450784675551[301] = 0;
   out_7558296450784675551[302] = 0;
   out_7558296450784675551[303] = 0;
   out_7558296450784675551[304] = 1;
   out_7558296450784675551[305] = 0;
   out_7558296450784675551[306] = 0;
   out_7558296450784675551[307] = 0;
   out_7558296450784675551[308] = 0;
   out_7558296450784675551[309] = 0;
   out_7558296450784675551[310] = 0;
   out_7558296450784675551[311] = 0;
   out_7558296450784675551[312] = 0;
   out_7558296450784675551[313] = 0;
   out_7558296450784675551[314] = 0;
   out_7558296450784675551[315] = 0;
   out_7558296450784675551[316] = 0;
   out_7558296450784675551[317] = 0;
   out_7558296450784675551[318] = 0;
   out_7558296450784675551[319] = 0;
   out_7558296450784675551[320] = 0;
   out_7558296450784675551[321] = 0;
   out_7558296450784675551[322] = 0;
   out_7558296450784675551[323] = 1;
}
void h_4(double *state, double *unused, double *out_7569011205382429115) {
   out_7569011205382429115[0] = state[6] + state[9];
   out_7569011205382429115[1] = state[7] + state[10];
   out_7569011205382429115[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_833561905108507896) {
   out_833561905108507896[0] = 0;
   out_833561905108507896[1] = 0;
   out_833561905108507896[2] = 0;
   out_833561905108507896[3] = 0;
   out_833561905108507896[4] = 0;
   out_833561905108507896[5] = 0;
   out_833561905108507896[6] = 1;
   out_833561905108507896[7] = 0;
   out_833561905108507896[8] = 0;
   out_833561905108507896[9] = 1;
   out_833561905108507896[10] = 0;
   out_833561905108507896[11] = 0;
   out_833561905108507896[12] = 0;
   out_833561905108507896[13] = 0;
   out_833561905108507896[14] = 0;
   out_833561905108507896[15] = 0;
   out_833561905108507896[16] = 0;
   out_833561905108507896[17] = 0;
   out_833561905108507896[18] = 0;
   out_833561905108507896[19] = 0;
   out_833561905108507896[20] = 0;
   out_833561905108507896[21] = 0;
   out_833561905108507896[22] = 0;
   out_833561905108507896[23] = 0;
   out_833561905108507896[24] = 0;
   out_833561905108507896[25] = 1;
   out_833561905108507896[26] = 0;
   out_833561905108507896[27] = 0;
   out_833561905108507896[28] = 1;
   out_833561905108507896[29] = 0;
   out_833561905108507896[30] = 0;
   out_833561905108507896[31] = 0;
   out_833561905108507896[32] = 0;
   out_833561905108507896[33] = 0;
   out_833561905108507896[34] = 0;
   out_833561905108507896[35] = 0;
   out_833561905108507896[36] = 0;
   out_833561905108507896[37] = 0;
   out_833561905108507896[38] = 0;
   out_833561905108507896[39] = 0;
   out_833561905108507896[40] = 0;
   out_833561905108507896[41] = 0;
   out_833561905108507896[42] = 0;
   out_833561905108507896[43] = 0;
   out_833561905108507896[44] = 1;
   out_833561905108507896[45] = 0;
   out_833561905108507896[46] = 0;
   out_833561905108507896[47] = 1;
   out_833561905108507896[48] = 0;
   out_833561905108507896[49] = 0;
   out_833561905108507896[50] = 0;
   out_833561905108507896[51] = 0;
   out_833561905108507896[52] = 0;
   out_833561905108507896[53] = 0;
}
void h_10(double *state, double *unused, double *out_1713401149757965889) {
   out_1713401149757965889[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_1713401149757965889[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_1713401149757965889[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_4851606762710325905) {
   out_4851606762710325905[0] = 0;
   out_4851606762710325905[1] = 9.8100000000000005*cos(state[1]);
   out_4851606762710325905[2] = 0;
   out_4851606762710325905[3] = 0;
   out_4851606762710325905[4] = -state[8];
   out_4851606762710325905[5] = state[7];
   out_4851606762710325905[6] = 0;
   out_4851606762710325905[7] = state[5];
   out_4851606762710325905[8] = -state[4];
   out_4851606762710325905[9] = 0;
   out_4851606762710325905[10] = 0;
   out_4851606762710325905[11] = 0;
   out_4851606762710325905[12] = 1;
   out_4851606762710325905[13] = 0;
   out_4851606762710325905[14] = 0;
   out_4851606762710325905[15] = 1;
   out_4851606762710325905[16] = 0;
   out_4851606762710325905[17] = 0;
   out_4851606762710325905[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_4851606762710325905[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_4851606762710325905[20] = 0;
   out_4851606762710325905[21] = state[8];
   out_4851606762710325905[22] = 0;
   out_4851606762710325905[23] = -state[6];
   out_4851606762710325905[24] = -state[5];
   out_4851606762710325905[25] = 0;
   out_4851606762710325905[26] = state[3];
   out_4851606762710325905[27] = 0;
   out_4851606762710325905[28] = 0;
   out_4851606762710325905[29] = 0;
   out_4851606762710325905[30] = 0;
   out_4851606762710325905[31] = 1;
   out_4851606762710325905[32] = 0;
   out_4851606762710325905[33] = 0;
   out_4851606762710325905[34] = 1;
   out_4851606762710325905[35] = 0;
   out_4851606762710325905[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_4851606762710325905[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_4851606762710325905[38] = 0;
   out_4851606762710325905[39] = -state[7];
   out_4851606762710325905[40] = state[6];
   out_4851606762710325905[41] = 0;
   out_4851606762710325905[42] = state[4];
   out_4851606762710325905[43] = -state[3];
   out_4851606762710325905[44] = 0;
   out_4851606762710325905[45] = 0;
   out_4851606762710325905[46] = 0;
   out_4851606762710325905[47] = 0;
   out_4851606762710325905[48] = 0;
   out_4851606762710325905[49] = 0;
   out_4851606762710325905[50] = 1;
   out_4851606762710325905[51] = 0;
   out_4851606762710325905[52] = 0;
   out_4851606762710325905[53] = 1;
}
void h_13(double *state, double *unused, double *out_4066253850474937797) {
   out_4066253850474937797[0] = state[3];
   out_4066253850474937797[1] = state[4];
   out_4066253850474937797[2] = state[5];
}
void H_13(double *state, double *unused, double *out_4667317368411031920) {
   out_4667317368411031920[0] = 0;
   out_4667317368411031920[1] = 0;
   out_4667317368411031920[2] = 0;
   out_4667317368411031920[3] = 1;
   out_4667317368411031920[4] = 0;
   out_4667317368411031920[5] = 0;
   out_4667317368411031920[6] = 0;
   out_4667317368411031920[7] = 0;
   out_4667317368411031920[8] = 0;
   out_4667317368411031920[9] = 0;
   out_4667317368411031920[10] = 0;
   out_4667317368411031920[11] = 0;
   out_4667317368411031920[12] = 0;
   out_4667317368411031920[13] = 0;
   out_4667317368411031920[14] = 0;
   out_4667317368411031920[15] = 0;
   out_4667317368411031920[16] = 0;
   out_4667317368411031920[17] = 0;
   out_4667317368411031920[18] = 0;
   out_4667317368411031920[19] = 0;
   out_4667317368411031920[20] = 0;
   out_4667317368411031920[21] = 0;
   out_4667317368411031920[22] = 1;
   out_4667317368411031920[23] = 0;
   out_4667317368411031920[24] = 0;
   out_4667317368411031920[25] = 0;
   out_4667317368411031920[26] = 0;
   out_4667317368411031920[27] = 0;
   out_4667317368411031920[28] = 0;
   out_4667317368411031920[29] = 0;
   out_4667317368411031920[30] = 0;
   out_4667317368411031920[31] = 0;
   out_4667317368411031920[32] = 0;
   out_4667317368411031920[33] = 0;
   out_4667317368411031920[34] = 0;
   out_4667317368411031920[35] = 0;
   out_4667317368411031920[36] = 0;
   out_4667317368411031920[37] = 0;
   out_4667317368411031920[38] = 0;
   out_4667317368411031920[39] = 0;
   out_4667317368411031920[40] = 0;
   out_4667317368411031920[41] = 1;
   out_4667317368411031920[42] = 0;
   out_4667317368411031920[43] = 0;
   out_4667317368411031920[44] = 0;
   out_4667317368411031920[45] = 0;
   out_4667317368411031920[46] = 0;
   out_4667317368411031920[47] = 0;
   out_4667317368411031920[48] = 0;
   out_4667317368411031920[49] = 0;
   out_4667317368411031920[50] = 0;
   out_4667317368411031920[51] = 0;
   out_4667317368411031920[52] = 0;
   out_4667317368411031920[53] = 0;
}
void h_14(double *state, double *unused, double *out_1299243001364970001) {
   out_1299243001364970001[0] = state[6];
   out_1299243001364970001[1] = state[7];
   out_1299243001364970001[2] = state[8];
}
void H_14(double *state, double *unused, double *out_8314707720388248320) {
   out_8314707720388248320[0] = 0;
   out_8314707720388248320[1] = 0;
   out_8314707720388248320[2] = 0;
   out_8314707720388248320[3] = 0;
   out_8314707720388248320[4] = 0;
   out_8314707720388248320[5] = 0;
   out_8314707720388248320[6] = 1;
   out_8314707720388248320[7] = 0;
   out_8314707720388248320[8] = 0;
   out_8314707720388248320[9] = 0;
   out_8314707720388248320[10] = 0;
   out_8314707720388248320[11] = 0;
   out_8314707720388248320[12] = 0;
   out_8314707720388248320[13] = 0;
   out_8314707720388248320[14] = 0;
   out_8314707720388248320[15] = 0;
   out_8314707720388248320[16] = 0;
   out_8314707720388248320[17] = 0;
   out_8314707720388248320[18] = 0;
   out_8314707720388248320[19] = 0;
   out_8314707720388248320[20] = 0;
   out_8314707720388248320[21] = 0;
   out_8314707720388248320[22] = 0;
   out_8314707720388248320[23] = 0;
   out_8314707720388248320[24] = 0;
   out_8314707720388248320[25] = 1;
   out_8314707720388248320[26] = 0;
   out_8314707720388248320[27] = 0;
   out_8314707720388248320[28] = 0;
   out_8314707720388248320[29] = 0;
   out_8314707720388248320[30] = 0;
   out_8314707720388248320[31] = 0;
   out_8314707720388248320[32] = 0;
   out_8314707720388248320[33] = 0;
   out_8314707720388248320[34] = 0;
   out_8314707720388248320[35] = 0;
   out_8314707720388248320[36] = 0;
   out_8314707720388248320[37] = 0;
   out_8314707720388248320[38] = 0;
   out_8314707720388248320[39] = 0;
   out_8314707720388248320[40] = 0;
   out_8314707720388248320[41] = 0;
   out_8314707720388248320[42] = 0;
   out_8314707720388248320[43] = 0;
   out_8314707720388248320[44] = 1;
   out_8314707720388248320[45] = 0;
   out_8314707720388248320[46] = 0;
   out_8314707720388248320[47] = 0;
   out_8314707720388248320[48] = 0;
   out_8314707720388248320[49] = 0;
   out_8314707720388248320[50] = 0;
   out_8314707720388248320[51] = 0;
   out_8314707720388248320[52] = 0;
   out_8314707720388248320[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_3350640148905151952) {
  err_fun(nom_x, delta_x, out_3350640148905151952);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_5937141942059359877) {
  inv_err_fun(nom_x, true_x, out_5937141942059359877);
}
void pose_H_mod_fun(double *state, double *out_4176406737755320299) {
  H_mod_fun(state, out_4176406737755320299);
}
void pose_f_fun(double *state, double dt, double *out_3911862024342109205) {
  f_fun(state,  dt, out_3911862024342109205);
}
void pose_F_fun(double *state, double dt, double *out_7558296450784675551) {
  F_fun(state,  dt, out_7558296450784675551);
}
void pose_h_4(double *state, double *unused, double *out_7569011205382429115) {
  h_4(state, unused, out_7569011205382429115);
}
void pose_H_4(double *state, double *unused, double *out_833561905108507896) {
  H_4(state, unused, out_833561905108507896);
}
void pose_h_10(double *state, double *unused, double *out_1713401149757965889) {
  h_10(state, unused, out_1713401149757965889);
}
void pose_H_10(double *state, double *unused, double *out_4851606762710325905) {
  H_10(state, unused, out_4851606762710325905);
}
void pose_h_13(double *state, double *unused, double *out_4066253850474937797) {
  h_13(state, unused, out_4066253850474937797);
}
void pose_H_13(double *state, double *unused, double *out_4667317368411031920) {
  H_13(state, unused, out_4667317368411031920);
}
void pose_h_14(double *state, double *unused, double *out_1299243001364970001) {
  h_14(state, unused, out_1299243001364970001);
}
void pose_H_14(double *state, double *unused, double *out_8314707720388248320) {
  H_14(state, unused, out_8314707720388248320);
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
