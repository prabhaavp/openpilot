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
void err_fun(double *nom_x, double *delta_x, double *out_2522441676304835175) {
   out_2522441676304835175[0] = delta_x[0] + nom_x[0];
   out_2522441676304835175[1] = delta_x[1] + nom_x[1];
   out_2522441676304835175[2] = delta_x[2] + nom_x[2];
   out_2522441676304835175[3] = delta_x[3] + nom_x[3];
   out_2522441676304835175[4] = delta_x[4] + nom_x[4];
   out_2522441676304835175[5] = delta_x[5] + nom_x[5];
   out_2522441676304835175[6] = delta_x[6] + nom_x[6];
   out_2522441676304835175[7] = delta_x[7] + nom_x[7];
   out_2522441676304835175[8] = delta_x[8] + nom_x[8];
   out_2522441676304835175[9] = delta_x[9] + nom_x[9];
   out_2522441676304835175[10] = delta_x[10] + nom_x[10];
   out_2522441676304835175[11] = delta_x[11] + nom_x[11];
   out_2522441676304835175[12] = delta_x[12] + nom_x[12];
   out_2522441676304835175[13] = delta_x[13] + nom_x[13];
   out_2522441676304835175[14] = delta_x[14] + nom_x[14];
   out_2522441676304835175[15] = delta_x[15] + nom_x[15];
   out_2522441676304835175[16] = delta_x[16] + nom_x[16];
   out_2522441676304835175[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_497817490349892178) {
   out_497817490349892178[0] = -nom_x[0] + true_x[0];
   out_497817490349892178[1] = -nom_x[1] + true_x[1];
   out_497817490349892178[2] = -nom_x[2] + true_x[2];
   out_497817490349892178[3] = -nom_x[3] + true_x[3];
   out_497817490349892178[4] = -nom_x[4] + true_x[4];
   out_497817490349892178[5] = -nom_x[5] + true_x[5];
   out_497817490349892178[6] = -nom_x[6] + true_x[6];
   out_497817490349892178[7] = -nom_x[7] + true_x[7];
   out_497817490349892178[8] = -nom_x[8] + true_x[8];
   out_497817490349892178[9] = -nom_x[9] + true_x[9];
   out_497817490349892178[10] = -nom_x[10] + true_x[10];
   out_497817490349892178[11] = -nom_x[11] + true_x[11];
   out_497817490349892178[12] = -nom_x[12] + true_x[12];
   out_497817490349892178[13] = -nom_x[13] + true_x[13];
   out_497817490349892178[14] = -nom_x[14] + true_x[14];
   out_497817490349892178[15] = -nom_x[15] + true_x[15];
   out_497817490349892178[16] = -nom_x[16] + true_x[16];
   out_497817490349892178[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_8320576250642144753) {
   out_8320576250642144753[0] = 1.0;
   out_8320576250642144753[1] = 0.0;
   out_8320576250642144753[2] = 0.0;
   out_8320576250642144753[3] = 0.0;
   out_8320576250642144753[4] = 0.0;
   out_8320576250642144753[5] = 0.0;
   out_8320576250642144753[6] = 0.0;
   out_8320576250642144753[7] = 0.0;
   out_8320576250642144753[8] = 0.0;
   out_8320576250642144753[9] = 0.0;
   out_8320576250642144753[10] = 0.0;
   out_8320576250642144753[11] = 0.0;
   out_8320576250642144753[12] = 0.0;
   out_8320576250642144753[13] = 0.0;
   out_8320576250642144753[14] = 0.0;
   out_8320576250642144753[15] = 0.0;
   out_8320576250642144753[16] = 0.0;
   out_8320576250642144753[17] = 0.0;
   out_8320576250642144753[18] = 0.0;
   out_8320576250642144753[19] = 1.0;
   out_8320576250642144753[20] = 0.0;
   out_8320576250642144753[21] = 0.0;
   out_8320576250642144753[22] = 0.0;
   out_8320576250642144753[23] = 0.0;
   out_8320576250642144753[24] = 0.0;
   out_8320576250642144753[25] = 0.0;
   out_8320576250642144753[26] = 0.0;
   out_8320576250642144753[27] = 0.0;
   out_8320576250642144753[28] = 0.0;
   out_8320576250642144753[29] = 0.0;
   out_8320576250642144753[30] = 0.0;
   out_8320576250642144753[31] = 0.0;
   out_8320576250642144753[32] = 0.0;
   out_8320576250642144753[33] = 0.0;
   out_8320576250642144753[34] = 0.0;
   out_8320576250642144753[35] = 0.0;
   out_8320576250642144753[36] = 0.0;
   out_8320576250642144753[37] = 0.0;
   out_8320576250642144753[38] = 1.0;
   out_8320576250642144753[39] = 0.0;
   out_8320576250642144753[40] = 0.0;
   out_8320576250642144753[41] = 0.0;
   out_8320576250642144753[42] = 0.0;
   out_8320576250642144753[43] = 0.0;
   out_8320576250642144753[44] = 0.0;
   out_8320576250642144753[45] = 0.0;
   out_8320576250642144753[46] = 0.0;
   out_8320576250642144753[47] = 0.0;
   out_8320576250642144753[48] = 0.0;
   out_8320576250642144753[49] = 0.0;
   out_8320576250642144753[50] = 0.0;
   out_8320576250642144753[51] = 0.0;
   out_8320576250642144753[52] = 0.0;
   out_8320576250642144753[53] = 0.0;
   out_8320576250642144753[54] = 0.0;
   out_8320576250642144753[55] = 0.0;
   out_8320576250642144753[56] = 0.0;
   out_8320576250642144753[57] = 1.0;
   out_8320576250642144753[58] = 0.0;
   out_8320576250642144753[59] = 0.0;
   out_8320576250642144753[60] = 0.0;
   out_8320576250642144753[61] = 0.0;
   out_8320576250642144753[62] = 0.0;
   out_8320576250642144753[63] = 0.0;
   out_8320576250642144753[64] = 0.0;
   out_8320576250642144753[65] = 0.0;
   out_8320576250642144753[66] = 0.0;
   out_8320576250642144753[67] = 0.0;
   out_8320576250642144753[68] = 0.0;
   out_8320576250642144753[69] = 0.0;
   out_8320576250642144753[70] = 0.0;
   out_8320576250642144753[71] = 0.0;
   out_8320576250642144753[72] = 0.0;
   out_8320576250642144753[73] = 0.0;
   out_8320576250642144753[74] = 0.0;
   out_8320576250642144753[75] = 0.0;
   out_8320576250642144753[76] = 1.0;
   out_8320576250642144753[77] = 0.0;
   out_8320576250642144753[78] = 0.0;
   out_8320576250642144753[79] = 0.0;
   out_8320576250642144753[80] = 0.0;
   out_8320576250642144753[81] = 0.0;
   out_8320576250642144753[82] = 0.0;
   out_8320576250642144753[83] = 0.0;
   out_8320576250642144753[84] = 0.0;
   out_8320576250642144753[85] = 0.0;
   out_8320576250642144753[86] = 0.0;
   out_8320576250642144753[87] = 0.0;
   out_8320576250642144753[88] = 0.0;
   out_8320576250642144753[89] = 0.0;
   out_8320576250642144753[90] = 0.0;
   out_8320576250642144753[91] = 0.0;
   out_8320576250642144753[92] = 0.0;
   out_8320576250642144753[93] = 0.0;
   out_8320576250642144753[94] = 0.0;
   out_8320576250642144753[95] = 1.0;
   out_8320576250642144753[96] = 0.0;
   out_8320576250642144753[97] = 0.0;
   out_8320576250642144753[98] = 0.0;
   out_8320576250642144753[99] = 0.0;
   out_8320576250642144753[100] = 0.0;
   out_8320576250642144753[101] = 0.0;
   out_8320576250642144753[102] = 0.0;
   out_8320576250642144753[103] = 0.0;
   out_8320576250642144753[104] = 0.0;
   out_8320576250642144753[105] = 0.0;
   out_8320576250642144753[106] = 0.0;
   out_8320576250642144753[107] = 0.0;
   out_8320576250642144753[108] = 0.0;
   out_8320576250642144753[109] = 0.0;
   out_8320576250642144753[110] = 0.0;
   out_8320576250642144753[111] = 0.0;
   out_8320576250642144753[112] = 0.0;
   out_8320576250642144753[113] = 0.0;
   out_8320576250642144753[114] = 1.0;
   out_8320576250642144753[115] = 0.0;
   out_8320576250642144753[116] = 0.0;
   out_8320576250642144753[117] = 0.0;
   out_8320576250642144753[118] = 0.0;
   out_8320576250642144753[119] = 0.0;
   out_8320576250642144753[120] = 0.0;
   out_8320576250642144753[121] = 0.0;
   out_8320576250642144753[122] = 0.0;
   out_8320576250642144753[123] = 0.0;
   out_8320576250642144753[124] = 0.0;
   out_8320576250642144753[125] = 0.0;
   out_8320576250642144753[126] = 0.0;
   out_8320576250642144753[127] = 0.0;
   out_8320576250642144753[128] = 0.0;
   out_8320576250642144753[129] = 0.0;
   out_8320576250642144753[130] = 0.0;
   out_8320576250642144753[131] = 0.0;
   out_8320576250642144753[132] = 0.0;
   out_8320576250642144753[133] = 1.0;
   out_8320576250642144753[134] = 0.0;
   out_8320576250642144753[135] = 0.0;
   out_8320576250642144753[136] = 0.0;
   out_8320576250642144753[137] = 0.0;
   out_8320576250642144753[138] = 0.0;
   out_8320576250642144753[139] = 0.0;
   out_8320576250642144753[140] = 0.0;
   out_8320576250642144753[141] = 0.0;
   out_8320576250642144753[142] = 0.0;
   out_8320576250642144753[143] = 0.0;
   out_8320576250642144753[144] = 0.0;
   out_8320576250642144753[145] = 0.0;
   out_8320576250642144753[146] = 0.0;
   out_8320576250642144753[147] = 0.0;
   out_8320576250642144753[148] = 0.0;
   out_8320576250642144753[149] = 0.0;
   out_8320576250642144753[150] = 0.0;
   out_8320576250642144753[151] = 0.0;
   out_8320576250642144753[152] = 1.0;
   out_8320576250642144753[153] = 0.0;
   out_8320576250642144753[154] = 0.0;
   out_8320576250642144753[155] = 0.0;
   out_8320576250642144753[156] = 0.0;
   out_8320576250642144753[157] = 0.0;
   out_8320576250642144753[158] = 0.0;
   out_8320576250642144753[159] = 0.0;
   out_8320576250642144753[160] = 0.0;
   out_8320576250642144753[161] = 0.0;
   out_8320576250642144753[162] = 0.0;
   out_8320576250642144753[163] = 0.0;
   out_8320576250642144753[164] = 0.0;
   out_8320576250642144753[165] = 0.0;
   out_8320576250642144753[166] = 0.0;
   out_8320576250642144753[167] = 0.0;
   out_8320576250642144753[168] = 0.0;
   out_8320576250642144753[169] = 0.0;
   out_8320576250642144753[170] = 0.0;
   out_8320576250642144753[171] = 1.0;
   out_8320576250642144753[172] = 0.0;
   out_8320576250642144753[173] = 0.0;
   out_8320576250642144753[174] = 0.0;
   out_8320576250642144753[175] = 0.0;
   out_8320576250642144753[176] = 0.0;
   out_8320576250642144753[177] = 0.0;
   out_8320576250642144753[178] = 0.0;
   out_8320576250642144753[179] = 0.0;
   out_8320576250642144753[180] = 0.0;
   out_8320576250642144753[181] = 0.0;
   out_8320576250642144753[182] = 0.0;
   out_8320576250642144753[183] = 0.0;
   out_8320576250642144753[184] = 0.0;
   out_8320576250642144753[185] = 0.0;
   out_8320576250642144753[186] = 0.0;
   out_8320576250642144753[187] = 0.0;
   out_8320576250642144753[188] = 0.0;
   out_8320576250642144753[189] = 0.0;
   out_8320576250642144753[190] = 1.0;
   out_8320576250642144753[191] = 0.0;
   out_8320576250642144753[192] = 0.0;
   out_8320576250642144753[193] = 0.0;
   out_8320576250642144753[194] = 0.0;
   out_8320576250642144753[195] = 0.0;
   out_8320576250642144753[196] = 0.0;
   out_8320576250642144753[197] = 0.0;
   out_8320576250642144753[198] = 0.0;
   out_8320576250642144753[199] = 0.0;
   out_8320576250642144753[200] = 0.0;
   out_8320576250642144753[201] = 0.0;
   out_8320576250642144753[202] = 0.0;
   out_8320576250642144753[203] = 0.0;
   out_8320576250642144753[204] = 0.0;
   out_8320576250642144753[205] = 0.0;
   out_8320576250642144753[206] = 0.0;
   out_8320576250642144753[207] = 0.0;
   out_8320576250642144753[208] = 0.0;
   out_8320576250642144753[209] = 1.0;
   out_8320576250642144753[210] = 0.0;
   out_8320576250642144753[211] = 0.0;
   out_8320576250642144753[212] = 0.0;
   out_8320576250642144753[213] = 0.0;
   out_8320576250642144753[214] = 0.0;
   out_8320576250642144753[215] = 0.0;
   out_8320576250642144753[216] = 0.0;
   out_8320576250642144753[217] = 0.0;
   out_8320576250642144753[218] = 0.0;
   out_8320576250642144753[219] = 0.0;
   out_8320576250642144753[220] = 0.0;
   out_8320576250642144753[221] = 0.0;
   out_8320576250642144753[222] = 0.0;
   out_8320576250642144753[223] = 0.0;
   out_8320576250642144753[224] = 0.0;
   out_8320576250642144753[225] = 0.0;
   out_8320576250642144753[226] = 0.0;
   out_8320576250642144753[227] = 0.0;
   out_8320576250642144753[228] = 1.0;
   out_8320576250642144753[229] = 0.0;
   out_8320576250642144753[230] = 0.0;
   out_8320576250642144753[231] = 0.0;
   out_8320576250642144753[232] = 0.0;
   out_8320576250642144753[233] = 0.0;
   out_8320576250642144753[234] = 0.0;
   out_8320576250642144753[235] = 0.0;
   out_8320576250642144753[236] = 0.0;
   out_8320576250642144753[237] = 0.0;
   out_8320576250642144753[238] = 0.0;
   out_8320576250642144753[239] = 0.0;
   out_8320576250642144753[240] = 0.0;
   out_8320576250642144753[241] = 0.0;
   out_8320576250642144753[242] = 0.0;
   out_8320576250642144753[243] = 0.0;
   out_8320576250642144753[244] = 0.0;
   out_8320576250642144753[245] = 0.0;
   out_8320576250642144753[246] = 0.0;
   out_8320576250642144753[247] = 1.0;
   out_8320576250642144753[248] = 0.0;
   out_8320576250642144753[249] = 0.0;
   out_8320576250642144753[250] = 0.0;
   out_8320576250642144753[251] = 0.0;
   out_8320576250642144753[252] = 0.0;
   out_8320576250642144753[253] = 0.0;
   out_8320576250642144753[254] = 0.0;
   out_8320576250642144753[255] = 0.0;
   out_8320576250642144753[256] = 0.0;
   out_8320576250642144753[257] = 0.0;
   out_8320576250642144753[258] = 0.0;
   out_8320576250642144753[259] = 0.0;
   out_8320576250642144753[260] = 0.0;
   out_8320576250642144753[261] = 0.0;
   out_8320576250642144753[262] = 0.0;
   out_8320576250642144753[263] = 0.0;
   out_8320576250642144753[264] = 0.0;
   out_8320576250642144753[265] = 0.0;
   out_8320576250642144753[266] = 1.0;
   out_8320576250642144753[267] = 0.0;
   out_8320576250642144753[268] = 0.0;
   out_8320576250642144753[269] = 0.0;
   out_8320576250642144753[270] = 0.0;
   out_8320576250642144753[271] = 0.0;
   out_8320576250642144753[272] = 0.0;
   out_8320576250642144753[273] = 0.0;
   out_8320576250642144753[274] = 0.0;
   out_8320576250642144753[275] = 0.0;
   out_8320576250642144753[276] = 0.0;
   out_8320576250642144753[277] = 0.0;
   out_8320576250642144753[278] = 0.0;
   out_8320576250642144753[279] = 0.0;
   out_8320576250642144753[280] = 0.0;
   out_8320576250642144753[281] = 0.0;
   out_8320576250642144753[282] = 0.0;
   out_8320576250642144753[283] = 0.0;
   out_8320576250642144753[284] = 0.0;
   out_8320576250642144753[285] = 1.0;
   out_8320576250642144753[286] = 0.0;
   out_8320576250642144753[287] = 0.0;
   out_8320576250642144753[288] = 0.0;
   out_8320576250642144753[289] = 0.0;
   out_8320576250642144753[290] = 0.0;
   out_8320576250642144753[291] = 0.0;
   out_8320576250642144753[292] = 0.0;
   out_8320576250642144753[293] = 0.0;
   out_8320576250642144753[294] = 0.0;
   out_8320576250642144753[295] = 0.0;
   out_8320576250642144753[296] = 0.0;
   out_8320576250642144753[297] = 0.0;
   out_8320576250642144753[298] = 0.0;
   out_8320576250642144753[299] = 0.0;
   out_8320576250642144753[300] = 0.0;
   out_8320576250642144753[301] = 0.0;
   out_8320576250642144753[302] = 0.0;
   out_8320576250642144753[303] = 0.0;
   out_8320576250642144753[304] = 1.0;
   out_8320576250642144753[305] = 0.0;
   out_8320576250642144753[306] = 0.0;
   out_8320576250642144753[307] = 0.0;
   out_8320576250642144753[308] = 0.0;
   out_8320576250642144753[309] = 0.0;
   out_8320576250642144753[310] = 0.0;
   out_8320576250642144753[311] = 0.0;
   out_8320576250642144753[312] = 0.0;
   out_8320576250642144753[313] = 0.0;
   out_8320576250642144753[314] = 0.0;
   out_8320576250642144753[315] = 0.0;
   out_8320576250642144753[316] = 0.0;
   out_8320576250642144753[317] = 0.0;
   out_8320576250642144753[318] = 0.0;
   out_8320576250642144753[319] = 0.0;
   out_8320576250642144753[320] = 0.0;
   out_8320576250642144753[321] = 0.0;
   out_8320576250642144753[322] = 0.0;
   out_8320576250642144753[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_368313394866714597) {
   out_368313394866714597[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_368313394866714597[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_368313394866714597[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_368313394866714597[3] = dt*state[12] + state[3];
   out_368313394866714597[4] = dt*state[13] + state[4];
   out_368313394866714597[5] = dt*state[14] + state[5];
   out_368313394866714597[6] = state[6];
   out_368313394866714597[7] = state[7];
   out_368313394866714597[8] = state[8];
   out_368313394866714597[9] = state[9];
   out_368313394866714597[10] = state[10];
   out_368313394866714597[11] = state[11];
   out_368313394866714597[12] = state[12];
   out_368313394866714597[13] = state[13];
   out_368313394866714597[14] = state[14];
   out_368313394866714597[15] = state[15];
   out_368313394866714597[16] = state[16];
   out_368313394866714597[17] = state[17];
}
void F_fun(double *state, double dt, double *out_6193746690085684006) {
   out_6193746690085684006[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6193746690085684006[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6193746690085684006[2] = 0;
   out_6193746690085684006[3] = 0;
   out_6193746690085684006[4] = 0;
   out_6193746690085684006[5] = 0;
   out_6193746690085684006[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6193746690085684006[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6193746690085684006[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6193746690085684006[9] = 0;
   out_6193746690085684006[10] = 0;
   out_6193746690085684006[11] = 0;
   out_6193746690085684006[12] = 0;
   out_6193746690085684006[13] = 0;
   out_6193746690085684006[14] = 0;
   out_6193746690085684006[15] = 0;
   out_6193746690085684006[16] = 0;
   out_6193746690085684006[17] = 0;
   out_6193746690085684006[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6193746690085684006[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6193746690085684006[20] = 0;
   out_6193746690085684006[21] = 0;
   out_6193746690085684006[22] = 0;
   out_6193746690085684006[23] = 0;
   out_6193746690085684006[24] = 0;
   out_6193746690085684006[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6193746690085684006[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6193746690085684006[27] = 0;
   out_6193746690085684006[28] = 0;
   out_6193746690085684006[29] = 0;
   out_6193746690085684006[30] = 0;
   out_6193746690085684006[31] = 0;
   out_6193746690085684006[32] = 0;
   out_6193746690085684006[33] = 0;
   out_6193746690085684006[34] = 0;
   out_6193746690085684006[35] = 0;
   out_6193746690085684006[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6193746690085684006[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6193746690085684006[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6193746690085684006[39] = 0;
   out_6193746690085684006[40] = 0;
   out_6193746690085684006[41] = 0;
   out_6193746690085684006[42] = 0;
   out_6193746690085684006[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6193746690085684006[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6193746690085684006[45] = 0;
   out_6193746690085684006[46] = 0;
   out_6193746690085684006[47] = 0;
   out_6193746690085684006[48] = 0;
   out_6193746690085684006[49] = 0;
   out_6193746690085684006[50] = 0;
   out_6193746690085684006[51] = 0;
   out_6193746690085684006[52] = 0;
   out_6193746690085684006[53] = 0;
   out_6193746690085684006[54] = 0;
   out_6193746690085684006[55] = 0;
   out_6193746690085684006[56] = 0;
   out_6193746690085684006[57] = 1;
   out_6193746690085684006[58] = 0;
   out_6193746690085684006[59] = 0;
   out_6193746690085684006[60] = 0;
   out_6193746690085684006[61] = 0;
   out_6193746690085684006[62] = 0;
   out_6193746690085684006[63] = 0;
   out_6193746690085684006[64] = 0;
   out_6193746690085684006[65] = 0;
   out_6193746690085684006[66] = dt;
   out_6193746690085684006[67] = 0;
   out_6193746690085684006[68] = 0;
   out_6193746690085684006[69] = 0;
   out_6193746690085684006[70] = 0;
   out_6193746690085684006[71] = 0;
   out_6193746690085684006[72] = 0;
   out_6193746690085684006[73] = 0;
   out_6193746690085684006[74] = 0;
   out_6193746690085684006[75] = 0;
   out_6193746690085684006[76] = 1;
   out_6193746690085684006[77] = 0;
   out_6193746690085684006[78] = 0;
   out_6193746690085684006[79] = 0;
   out_6193746690085684006[80] = 0;
   out_6193746690085684006[81] = 0;
   out_6193746690085684006[82] = 0;
   out_6193746690085684006[83] = 0;
   out_6193746690085684006[84] = 0;
   out_6193746690085684006[85] = dt;
   out_6193746690085684006[86] = 0;
   out_6193746690085684006[87] = 0;
   out_6193746690085684006[88] = 0;
   out_6193746690085684006[89] = 0;
   out_6193746690085684006[90] = 0;
   out_6193746690085684006[91] = 0;
   out_6193746690085684006[92] = 0;
   out_6193746690085684006[93] = 0;
   out_6193746690085684006[94] = 0;
   out_6193746690085684006[95] = 1;
   out_6193746690085684006[96] = 0;
   out_6193746690085684006[97] = 0;
   out_6193746690085684006[98] = 0;
   out_6193746690085684006[99] = 0;
   out_6193746690085684006[100] = 0;
   out_6193746690085684006[101] = 0;
   out_6193746690085684006[102] = 0;
   out_6193746690085684006[103] = 0;
   out_6193746690085684006[104] = dt;
   out_6193746690085684006[105] = 0;
   out_6193746690085684006[106] = 0;
   out_6193746690085684006[107] = 0;
   out_6193746690085684006[108] = 0;
   out_6193746690085684006[109] = 0;
   out_6193746690085684006[110] = 0;
   out_6193746690085684006[111] = 0;
   out_6193746690085684006[112] = 0;
   out_6193746690085684006[113] = 0;
   out_6193746690085684006[114] = 1;
   out_6193746690085684006[115] = 0;
   out_6193746690085684006[116] = 0;
   out_6193746690085684006[117] = 0;
   out_6193746690085684006[118] = 0;
   out_6193746690085684006[119] = 0;
   out_6193746690085684006[120] = 0;
   out_6193746690085684006[121] = 0;
   out_6193746690085684006[122] = 0;
   out_6193746690085684006[123] = 0;
   out_6193746690085684006[124] = 0;
   out_6193746690085684006[125] = 0;
   out_6193746690085684006[126] = 0;
   out_6193746690085684006[127] = 0;
   out_6193746690085684006[128] = 0;
   out_6193746690085684006[129] = 0;
   out_6193746690085684006[130] = 0;
   out_6193746690085684006[131] = 0;
   out_6193746690085684006[132] = 0;
   out_6193746690085684006[133] = 1;
   out_6193746690085684006[134] = 0;
   out_6193746690085684006[135] = 0;
   out_6193746690085684006[136] = 0;
   out_6193746690085684006[137] = 0;
   out_6193746690085684006[138] = 0;
   out_6193746690085684006[139] = 0;
   out_6193746690085684006[140] = 0;
   out_6193746690085684006[141] = 0;
   out_6193746690085684006[142] = 0;
   out_6193746690085684006[143] = 0;
   out_6193746690085684006[144] = 0;
   out_6193746690085684006[145] = 0;
   out_6193746690085684006[146] = 0;
   out_6193746690085684006[147] = 0;
   out_6193746690085684006[148] = 0;
   out_6193746690085684006[149] = 0;
   out_6193746690085684006[150] = 0;
   out_6193746690085684006[151] = 0;
   out_6193746690085684006[152] = 1;
   out_6193746690085684006[153] = 0;
   out_6193746690085684006[154] = 0;
   out_6193746690085684006[155] = 0;
   out_6193746690085684006[156] = 0;
   out_6193746690085684006[157] = 0;
   out_6193746690085684006[158] = 0;
   out_6193746690085684006[159] = 0;
   out_6193746690085684006[160] = 0;
   out_6193746690085684006[161] = 0;
   out_6193746690085684006[162] = 0;
   out_6193746690085684006[163] = 0;
   out_6193746690085684006[164] = 0;
   out_6193746690085684006[165] = 0;
   out_6193746690085684006[166] = 0;
   out_6193746690085684006[167] = 0;
   out_6193746690085684006[168] = 0;
   out_6193746690085684006[169] = 0;
   out_6193746690085684006[170] = 0;
   out_6193746690085684006[171] = 1;
   out_6193746690085684006[172] = 0;
   out_6193746690085684006[173] = 0;
   out_6193746690085684006[174] = 0;
   out_6193746690085684006[175] = 0;
   out_6193746690085684006[176] = 0;
   out_6193746690085684006[177] = 0;
   out_6193746690085684006[178] = 0;
   out_6193746690085684006[179] = 0;
   out_6193746690085684006[180] = 0;
   out_6193746690085684006[181] = 0;
   out_6193746690085684006[182] = 0;
   out_6193746690085684006[183] = 0;
   out_6193746690085684006[184] = 0;
   out_6193746690085684006[185] = 0;
   out_6193746690085684006[186] = 0;
   out_6193746690085684006[187] = 0;
   out_6193746690085684006[188] = 0;
   out_6193746690085684006[189] = 0;
   out_6193746690085684006[190] = 1;
   out_6193746690085684006[191] = 0;
   out_6193746690085684006[192] = 0;
   out_6193746690085684006[193] = 0;
   out_6193746690085684006[194] = 0;
   out_6193746690085684006[195] = 0;
   out_6193746690085684006[196] = 0;
   out_6193746690085684006[197] = 0;
   out_6193746690085684006[198] = 0;
   out_6193746690085684006[199] = 0;
   out_6193746690085684006[200] = 0;
   out_6193746690085684006[201] = 0;
   out_6193746690085684006[202] = 0;
   out_6193746690085684006[203] = 0;
   out_6193746690085684006[204] = 0;
   out_6193746690085684006[205] = 0;
   out_6193746690085684006[206] = 0;
   out_6193746690085684006[207] = 0;
   out_6193746690085684006[208] = 0;
   out_6193746690085684006[209] = 1;
   out_6193746690085684006[210] = 0;
   out_6193746690085684006[211] = 0;
   out_6193746690085684006[212] = 0;
   out_6193746690085684006[213] = 0;
   out_6193746690085684006[214] = 0;
   out_6193746690085684006[215] = 0;
   out_6193746690085684006[216] = 0;
   out_6193746690085684006[217] = 0;
   out_6193746690085684006[218] = 0;
   out_6193746690085684006[219] = 0;
   out_6193746690085684006[220] = 0;
   out_6193746690085684006[221] = 0;
   out_6193746690085684006[222] = 0;
   out_6193746690085684006[223] = 0;
   out_6193746690085684006[224] = 0;
   out_6193746690085684006[225] = 0;
   out_6193746690085684006[226] = 0;
   out_6193746690085684006[227] = 0;
   out_6193746690085684006[228] = 1;
   out_6193746690085684006[229] = 0;
   out_6193746690085684006[230] = 0;
   out_6193746690085684006[231] = 0;
   out_6193746690085684006[232] = 0;
   out_6193746690085684006[233] = 0;
   out_6193746690085684006[234] = 0;
   out_6193746690085684006[235] = 0;
   out_6193746690085684006[236] = 0;
   out_6193746690085684006[237] = 0;
   out_6193746690085684006[238] = 0;
   out_6193746690085684006[239] = 0;
   out_6193746690085684006[240] = 0;
   out_6193746690085684006[241] = 0;
   out_6193746690085684006[242] = 0;
   out_6193746690085684006[243] = 0;
   out_6193746690085684006[244] = 0;
   out_6193746690085684006[245] = 0;
   out_6193746690085684006[246] = 0;
   out_6193746690085684006[247] = 1;
   out_6193746690085684006[248] = 0;
   out_6193746690085684006[249] = 0;
   out_6193746690085684006[250] = 0;
   out_6193746690085684006[251] = 0;
   out_6193746690085684006[252] = 0;
   out_6193746690085684006[253] = 0;
   out_6193746690085684006[254] = 0;
   out_6193746690085684006[255] = 0;
   out_6193746690085684006[256] = 0;
   out_6193746690085684006[257] = 0;
   out_6193746690085684006[258] = 0;
   out_6193746690085684006[259] = 0;
   out_6193746690085684006[260] = 0;
   out_6193746690085684006[261] = 0;
   out_6193746690085684006[262] = 0;
   out_6193746690085684006[263] = 0;
   out_6193746690085684006[264] = 0;
   out_6193746690085684006[265] = 0;
   out_6193746690085684006[266] = 1;
   out_6193746690085684006[267] = 0;
   out_6193746690085684006[268] = 0;
   out_6193746690085684006[269] = 0;
   out_6193746690085684006[270] = 0;
   out_6193746690085684006[271] = 0;
   out_6193746690085684006[272] = 0;
   out_6193746690085684006[273] = 0;
   out_6193746690085684006[274] = 0;
   out_6193746690085684006[275] = 0;
   out_6193746690085684006[276] = 0;
   out_6193746690085684006[277] = 0;
   out_6193746690085684006[278] = 0;
   out_6193746690085684006[279] = 0;
   out_6193746690085684006[280] = 0;
   out_6193746690085684006[281] = 0;
   out_6193746690085684006[282] = 0;
   out_6193746690085684006[283] = 0;
   out_6193746690085684006[284] = 0;
   out_6193746690085684006[285] = 1;
   out_6193746690085684006[286] = 0;
   out_6193746690085684006[287] = 0;
   out_6193746690085684006[288] = 0;
   out_6193746690085684006[289] = 0;
   out_6193746690085684006[290] = 0;
   out_6193746690085684006[291] = 0;
   out_6193746690085684006[292] = 0;
   out_6193746690085684006[293] = 0;
   out_6193746690085684006[294] = 0;
   out_6193746690085684006[295] = 0;
   out_6193746690085684006[296] = 0;
   out_6193746690085684006[297] = 0;
   out_6193746690085684006[298] = 0;
   out_6193746690085684006[299] = 0;
   out_6193746690085684006[300] = 0;
   out_6193746690085684006[301] = 0;
   out_6193746690085684006[302] = 0;
   out_6193746690085684006[303] = 0;
   out_6193746690085684006[304] = 1;
   out_6193746690085684006[305] = 0;
   out_6193746690085684006[306] = 0;
   out_6193746690085684006[307] = 0;
   out_6193746690085684006[308] = 0;
   out_6193746690085684006[309] = 0;
   out_6193746690085684006[310] = 0;
   out_6193746690085684006[311] = 0;
   out_6193746690085684006[312] = 0;
   out_6193746690085684006[313] = 0;
   out_6193746690085684006[314] = 0;
   out_6193746690085684006[315] = 0;
   out_6193746690085684006[316] = 0;
   out_6193746690085684006[317] = 0;
   out_6193746690085684006[318] = 0;
   out_6193746690085684006[319] = 0;
   out_6193746690085684006[320] = 0;
   out_6193746690085684006[321] = 0;
   out_6193746690085684006[322] = 0;
   out_6193746690085684006[323] = 1;
}
void h_4(double *state, double *unused, double *out_5598758435398906091) {
   out_5598758435398906091[0] = state[6] + state[9];
   out_5598758435398906091[1] = state[7] + state[10];
   out_5598758435398906091[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_120271366668919986) {
   out_120271366668919986[0] = 0;
   out_120271366668919986[1] = 0;
   out_120271366668919986[2] = 0;
   out_120271366668919986[3] = 0;
   out_120271366668919986[4] = 0;
   out_120271366668919986[5] = 0;
   out_120271366668919986[6] = 1;
   out_120271366668919986[7] = 0;
   out_120271366668919986[8] = 0;
   out_120271366668919986[9] = 1;
   out_120271366668919986[10] = 0;
   out_120271366668919986[11] = 0;
   out_120271366668919986[12] = 0;
   out_120271366668919986[13] = 0;
   out_120271366668919986[14] = 0;
   out_120271366668919986[15] = 0;
   out_120271366668919986[16] = 0;
   out_120271366668919986[17] = 0;
   out_120271366668919986[18] = 0;
   out_120271366668919986[19] = 0;
   out_120271366668919986[20] = 0;
   out_120271366668919986[21] = 0;
   out_120271366668919986[22] = 0;
   out_120271366668919986[23] = 0;
   out_120271366668919986[24] = 0;
   out_120271366668919986[25] = 1;
   out_120271366668919986[26] = 0;
   out_120271366668919986[27] = 0;
   out_120271366668919986[28] = 1;
   out_120271366668919986[29] = 0;
   out_120271366668919986[30] = 0;
   out_120271366668919986[31] = 0;
   out_120271366668919986[32] = 0;
   out_120271366668919986[33] = 0;
   out_120271366668919986[34] = 0;
   out_120271366668919986[35] = 0;
   out_120271366668919986[36] = 0;
   out_120271366668919986[37] = 0;
   out_120271366668919986[38] = 0;
   out_120271366668919986[39] = 0;
   out_120271366668919986[40] = 0;
   out_120271366668919986[41] = 0;
   out_120271366668919986[42] = 0;
   out_120271366668919986[43] = 0;
   out_120271366668919986[44] = 1;
   out_120271366668919986[45] = 0;
   out_120271366668919986[46] = 0;
   out_120271366668919986[47] = 1;
   out_120271366668919986[48] = 0;
   out_120271366668919986[49] = 0;
   out_120271366668919986[50] = 0;
   out_120271366668919986[51] = 0;
   out_120271366668919986[52] = 0;
   out_120271366668919986[53] = 0;
}
void h_10(double *state, double *unused, double *out_2397420084850967746) {
   out_2397420084850967746[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_2397420084850967746[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_2397420084850967746[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_6919231927501633514) {
   out_6919231927501633514[0] = 0;
   out_6919231927501633514[1] = 9.8100000000000005*cos(state[1]);
   out_6919231927501633514[2] = 0;
   out_6919231927501633514[3] = 0;
   out_6919231927501633514[4] = -state[8];
   out_6919231927501633514[5] = state[7];
   out_6919231927501633514[6] = 0;
   out_6919231927501633514[7] = state[5];
   out_6919231927501633514[8] = -state[4];
   out_6919231927501633514[9] = 0;
   out_6919231927501633514[10] = 0;
   out_6919231927501633514[11] = 0;
   out_6919231927501633514[12] = 1;
   out_6919231927501633514[13] = 0;
   out_6919231927501633514[14] = 0;
   out_6919231927501633514[15] = 1;
   out_6919231927501633514[16] = 0;
   out_6919231927501633514[17] = 0;
   out_6919231927501633514[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_6919231927501633514[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_6919231927501633514[20] = 0;
   out_6919231927501633514[21] = state[8];
   out_6919231927501633514[22] = 0;
   out_6919231927501633514[23] = -state[6];
   out_6919231927501633514[24] = -state[5];
   out_6919231927501633514[25] = 0;
   out_6919231927501633514[26] = state[3];
   out_6919231927501633514[27] = 0;
   out_6919231927501633514[28] = 0;
   out_6919231927501633514[29] = 0;
   out_6919231927501633514[30] = 0;
   out_6919231927501633514[31] = 1;
   out_6919231927501633514[32] = 0;
   out_6919231927501633514[33] = 0;
   out_6919231927501633514[34] = 1;
   out_6919231927501633514[35] = 0;
   out_6919231927501633514[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_6919231927501633514[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_6919231927501633514[38] = 0;
   out_6919231927501633514[39] = -state[7];
   out_6919231927501633514[40] = state[6];
   out_6919231927501633514[41] = 0;
   out_6919231927501633514[42] = state[4];
   out_6919231927501633514[43] = -state[3];
   out_6919231927501633514[44] = 0;
   out_6919231927501633514[45] = 0;
   out_6919231927501633514[46] = 0;
   out_6919231927501633514[47] = 0;
   out_6919231927501633514[48] = 0;
   out_6919231927501633514[49] = 0;
   out_6919231927501633514[50] = 1;
   out_6919231927501633514[51] = 0;
   out_6919231927501633514[52] = 0;
   out_6919231927501633514[53] = 1;
}
void h_13(double *state, double *unused, double *out_3860918846742744675) {
   out_3860918846742744675[0] = state[3];
   out_3860918846742744675[1] = state[4];
   out_3860918846742744675[2] = state[5];
}
void H_13(double *state, double *unused, double *out_7490359841647780943) {
   out_7490359841647780943[0] = 0;
   out_7490359841647780943[1] = 0;
   out_7490359841647780943[2] = 0;
   out_7490359841647780943[3] = 1;
   out_7490359841647780943[4] = 0;
   out_7490359841647780943[5] = 0;
   out_7490359841647780943[6] = 0;
   out_7490359841647780943[7] = 0;
   out_7490359841647780943[8] = 0;
   out_7490359841647780943[9] = 0;
   out_7490359841647780943[10] = 0;
   out_7490359841647780943[11] = 0;
   out_7490359841647780943[12] = 0;
   out_7490359841647780943[13] = 0;
   out_7490359841647780943[14] = 0;
   out_7490359841647780943[15] = 0;
   out_7490359841647780943[16] = 0;
   out_7490359841647780943[17] = 0;
   out_7490359841647780943[18] = 0;
   out_7490359841647780943[19] = 0;
   out_7490359841647780943[20] = 0;
   out_7490359841647780943[21] = 0;
   out_7490359841647780943[22] = 1;
   out_7490359841647780943[23] = 0;
   out_7490359841647780943[24] = 0;
   out_7490359841647780943[25] = 0;
   out_7490359841647780943[26] = 0;
   out_7490359841647780943[27] = 0;
   out_7490359841647780943[28] = 0;
   out_7490359841647780943[29] = 0;
   out_7490359841647780943[30] = 0;
   out_7490359841647780943[31] = 0;
   out_7490359841647780943[32] = 0;
   out_7490359841647780943[33] = 0;
   out_7490359841647780943[34] = 0;
   out_7490359841647780943[35] = 0;
   out_7490359841647780943[36] = 0;
   out_7490359841647780943[37] = 0;
   out_7490359841647780943[38] = 0;
   out_7490359841647780943[39] = 0;
   out_7490359841647780943[40] = 0;
   out_7490359841647780943[41] = 1;
   out_7490359841647780943[42] = 0;
   out_7490359841647780943[43] = 0;
   out_7490359841647780943[44] = 0;
   out_7490359841647780943[45] = 0;
   out_7490359841647780943[46] = 0;
   out_7490359841647780943[47] = 0;
   out_7490359841647780943[48] = 0;
   out_7490359841647780943[49] = 0;
   out_7490359841647780943[50] = 0;
   out_7490359841647780943[51] = 0;
   out_7490359841647780943[52] = 0;
   out_7490359841647780943[53] = 0;
}
void h_14(double *state, double *unused, double *out_148712011300453905) {
   out_148712011300453905[0] = state[6];
   out_148712011300453905[1] = state[7];
   out_148712011300453905[2] = state[8];
}
void H_14(double *state, double *unused, double *out_3842969489670564543) {
   out_3842969489670564543[0] = 0;
   out_3842969489670564543[1] = 0;
   out_3842969489670564543[2] = 0;
   out_3842969489670564543[3] = 0;
   out_3842969489670564543[4] = 0;
   out_3842969489670564543[5] = 0;
   out_3842969489670564543[6] = 1;
   out_3842969489670564543[7] = 0;
   out_3842969489670564543[8] = 0;
   out_3842969489670564543[9] = 0;
   out_3842969489670564543[10] = 0;
   out_3842969489670564543[11] = 0;
   out_3842969489670564543[12] = 0;
   out_3842969489670564543[13] = 0;
   out_3842969489670564543[14] = 0;
   out_3842969489670564543[15] = 0;
   out_3842969489670564543[16] = 0;
   out_3842969489670564543[17] = 0;
   out_3842969489670564543[18] = 0;
   out_3842969489670564543[19] = 0;
   out_3842969489670564543[20] = 0;
   out_3842969489670564543[21] = 0;
   out_3842969489670564543[22] = 0;
   out_3842969489670564543[23] = 0;
   out_3842969489670564543[24] = 0;
   out_3842969489670564543[25] = 1;
   out_3842969489670564543[26] = 0;
   out_3842969489670564543[27] = 0;
   out_3842969489670564543[28] = 0;
   out_3842969489670564543[29] = 0;
   out_3842969489670564543[30] = 0;
   out_3842969489670564543[31] = 0;
   out_3842969489670564543[32] = 0;
   out_3842969489670564543[33] = 0;
   out_3842969489670564543[34] = 0;
   out_3842969489670564543[35] = 0;
   out_3842969489670564543[36] = 0;
   out_3842969489670564543[37] = 0;
   out_3842969489670564543[38] = 0;
   out_3842969489670564543[39] = 0;
   out_3842969489670564543[40] = 0;
   out_3842969489670564543[41] = 0;
   out_3842969489670564543[42] = 0;
   out_3842969489670564543[43] = 0;
   out_3842969489670564543[44] = 1;
   out_3842969489670564543[45] = 0;
   out_3842969489670564543[46] = 0;
   out_3842969489670564543[47] = 0;
   out_3842969489670564543[48] = 0;
   out_3842969489670564543[49] = 0;
   out_3842969489670564543[50] = 0;
   out_3842969489670564543[51] = 0;
   out_3842969489670564543[52] = 0;
   out_3842969489670564543[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_2522441676304835175) {
  err_fun(nom_x, delta_x, out_2522441676304835175);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_497817490349892178) {
  inv_err_fun(nom_x, true_x, out_497817490349892178);
}
void pose_H_mod_fun(double *state, double *out_8320576250642144753) {
  H_mod_fun(state, out_8320576250642144753);
}
void pose_f_fun(double *state, double dt, double *out_368313394866714597) {
  f_fun(state,  dt, out_368313394866714597);
}
void pose_F_fun(double *state, double dt, double *out_6193746690085684006) {
  F_fun(state,  dt, out_6193746690085684006);
}
void pose_h_4(double *state, double *unused, double *out_5598758435398906091) {
  h_4(state, unused, out_5598758435398906091);
}
void pose_H_4(double *state, double *unused, double *out_120271366668919986) {
  H_4(state, unused, out_120271366668919986);
}
void pose_h_10(double *state, double *unused, double *out_2397420084850967746) {
  h_10(state, unused, out_2397420084850967746);
}
void pose_H_10(double *state, double *unused, double *out_6919231927501633514) {
  H_10(state, unused, out_6919231927501633514);
}
void pose_h_13(double *state, double *unused, double *out_3860918846742744675) {
  h_13(state, unused, out_3860918846742744675);
}
void pose_H_13(double *state, double *unused, double *out_7490359841647780943) {
  H_13(state, unused, out_7490359841647780943);
}
void pose_h_14(double *state, double *unused, double *out_148712011300453905) {
  h_14(state, unused, out_148712011300453905);
}
void pose_H_14(double *state, double *unused, double *out_3842969489670564543) {
  H_14(state, unused, out_3842969489670564543);
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
