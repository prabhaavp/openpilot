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
void err_fun(double *nom_x, double *delta_x, double *out_2520069339484736335) {
   out_2520069339484736335[0] = delta_x[0] + nom_x[0];
   out_2520069339484736335[1] = delta_x[1] + nom_x[1];
   out_2520069339484736335[2] = delta_x[2] + nom_x[2];
   out_2520069339484736335[3] = delta_x[3] + nom_x[3];
   out_2520069339484736335[4] = delta_x[4] + nom_x[4];
   out_2520069339484736335[5] = delta_x[5] + nom_x[5];
   out_2520069339484736335[6] = delta_x[6] + nom_x[6];
   out_2520069339484736335[7] = delta_x[7] + nom_x[7];
   out_2520069339484736335[8] = delta_x[8] + nom_x[8];
   out_2520069339484736335[9] = delta_x[9] + nom_x[9];
   out_2520069339484736335[10] = delta_x[10] + nom_x[10];
   out_2520069339484736335[11] = delta_x[11] + nom_x[11];
   out_2520069339484736335[12] = delta_x[12] + nom_x[12];
   out_2520069339484736335[13] = delta_x[13] + nom_x[13];
   out_2520069339484736335[14] = delta_x[14] + nom_x[14];
   out_2520069339484736335[15] = delta_x[15] + nom_x[15];
   out_2520069339484736335[16] = delta_x[16] + nom_x[16];
   out_2520069339484736335[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_3053044731700442871) {
   out_3053044731700442871[0] = -nom_x[0] + true_x[0];
   out_3053044731700442871[1] = -nom_x[1] + true_x[1];
   out_3053044731700442871[2] = -nom_x[2] + true_x[2];
   out_3053044731700442871[3] = -nom_x[3] + true_x[3];
   out_3053044731700442871[4] = -nom_x[4] + true_x[4];
   out_3053044731700442871[5] = -nom_x[5] + true_x[5];
   out_3053044731700442871[6] = -nom_x[6] + true_x[6];
   out_3053044731700442871[7] = -nom_x[7] + true_x[7];
   out_3053044731700442871[8] = -nom_x[8] + true_x[8];
   out_3053044731700442871[9] = -nom_x[9] + true_x[9];
   out_3053044731700442871[10] = -nom_x[10] + true_x[10];
   out_3053044731700442871[11] = -nom_x[11] + true_x[11];
   out_3053044731700442871[12] = -nom_x[12] + true_x[12];
   out_3053044731700442871[13] = -nom_x[13] + true_x[13];
   out_3053044731700442871[14] = -nom_x[14] + true_x[14];
   out_3053044731700442871[15] = -nom_x[15] + true_x[15];
   out_3053044731700442871[16] = -nom_x[16] + true_x[16];
   out_3053044731700442871[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_6611456164701546289) {
   out_6611456164701546289[0] = 1.0;
   out_6611456164701546289[1] = 0.0;
   out_6611456164701546289[2] = 0.0;
   out_6611456164701546289[3] = 0.0;
   out_6611456164701546289[4] = 0.0;
   out_6611456164701546289[5] = 0.0;
   out_6611456164701546289[6] = 0.0;
   out_6611456164701546289[7] = 0.0;
   out_6611456164701546289[8] = 0.0;
   out_6611456164701546289[9] = 0.0;
   out_6611456164701546289[10] = 0.0;
   out_6611456164701546289[11] = 0.0;
   out_6611456164701546289[12] = 0.0;
   out_6611456164701546289[13] = 0.0;
   out_6611456164701546289[14] = 0.0;
   out_6611456164701546289[15] = 0.0;
   out_6611456164701546289[16] = 0.0;
   out_6611456164701546289[17] = 0.0;
   out_6611456164701546289[18] = 0.0;
   out_6611456164701546289[19] = 1.0;
   out_6611456164701546289[20] = 0.0;
   out_6611456164701546289[21] = 0.0;
   out_6611456164701546289[22] = 0.0;
   out_6611456164701546289[23] = 0.0;
   out_6611456164701546289[24] = 0.0;
   out_6611456164701546289[25] = 0.0;
   out_6611456164701546289[26] = 0.0;
   out_6611456164701546289[27] = 0.0;
   out_6611456164701546289[28] = 0.0;
   out_6611456164701546289[29] = 0.0;
   out_6611456164701546289[30] = 0.0;
   out_6611456164701546289[31] = 0.0;
   out_6611456164701546289[32] = 0.0;
   out_6611456164701546289[33] = 0.0;
   out_6611456164701546289[34] = 0.0;
   out_6611456164701546289[35] = 0.0;
   out_6611456164701546289[36] = 0.0;
   out_6611456164701546289[37] = 0.0;
   out_6611456164701546289[38] = 1.0;
   out_6611456164701546289[39] = 0.0;
   out_6611456164701546289[40] = 0.0;
   out_6611456164701546289[41] = 0.0;
   out_6611456164701546289[42] = 0.0;
   out_6611456164701546289[43] = 0.0;
   out_6611456164701546289[44] = 0.0;
   out_6611456164701546289[45] = 0.0;
   out_6611456164701546289[46] = 0.0;
   out_6611456164701546289[47] = 0.0;
   out_6611456164701546289[48] = 0.0;
   out_6611456164701546289[49] = 0.0;
   out_6611456164701546289[50] = 0.0;
   out_6611456164701546289[51] = 0.0;
   out_6611456164701546289[52] = 0.0;
   out_6611456164701546289[53] = 0.0;
   out_6611456164701546289[54] = 0.0;
   out_6611456164701546289[55] = 0.0;
   out_6611456164701546289[56] = 0.0;
   out_6611456164701546289[57] = 1.0;
   out_6611456164701546289[58] = 0.0;
   out_6611456164701546289[59] = 0.0;
   out_6611456164701546289[60] = 0.0;
   out_6611456164701546289[61] = 0.0;
   out_6611456164701546289[62] = 0.0;
   out_6611456164701546289[63] = 0.0;
   out_6611456164701546289[64] = 0.0;
   out_6611456164701546289[65] = 0.0;
   out_6611456164701546289[66] = 0.0;
   out_6611456164701546289[67] = 0.0;
   out_6611456164701546289[68] = 0.0;
   out_6611456164701546289[69] = 0.0;
   out_6611456164701546289[70] = 0.0;
   out_6611456164701546289[71] = 0.0;
   out_6611456164701546289[72] = 0.0;
   out_6611456164701546289[73] = 0.0;
   out_6611456164701546289[74] = 0.0;
   out_6611456164701546289[75] = 0.0;
   out_6611456164701546289[76] = 1.0;
   out_6611456164701546289[77] = 0.0;
   out_6611456164701546289[78] = 0.0;
   out_6611456164701546289[79] = 0.0;
   out_6611456164701546289[80] = 0.0;
   out_6611456164701546289[81] = 0.0;
   out_6611456164701546289[82] = 0.0;
   out_6611456164701546289[83] = 0.0;
   out_6611456164701546289[84] = 0.0;
   out_6611456164701546289[85] = 0.0;
   out_6611456164701546289[86] = 0.0;
   out_6611456164701546289[87] = 0.0;
   out_6611456164701546289[88] = 0.0;
   out_6611456164701546289[89] = 0.0;
   out_6611456164701546289[90] = 0.0;
   out_6611456164701546289[91] = 0.0;
   out_6611456164701546289[92] = 0.0;
   out_6611456164701546289[93] = 0.0;
   out_6611456164701546289[94] = 0.0;
   out_6611456164701546289[95] = 1.0;
   out_6611456164701546289[96] = 0.0;
   out_6611456164701546289[97] = 0.0;
   out_6611456164701546289[98] = 0.0;
   out_6611456164701546289[99] = 0.0;
   out_6611456164701546289[100] = 0.0;
   out_6611456164701546289[101] = 0.0;
   out_6611456164701546289[102] = 0.0;
   out_6611456164701546289[103] = 0.0;
   out_6611456164701546289[104] = 0.0;
   out_6611456164701546289[105] = 0.0;
   out_6611456164701546289[106] = 0.0;
   out_6611456164701546289[107] = 0.0;
   out_6611456164701546289[108] = 0.0;
   out_6611456164701546289[109] = 0.0;
   out_6611456164701546289[110] = 0.0;
   out_6611456164701546289[111] = 0.0;
   out_6611456164701546289[112] = 0.0;
   out_6611456164701546289[113] = 0.0;
   out_6611456164701546289[114] = 1.0;
   out_6611456164701546289[115] = 0.0;
   out_6611456164701546289[116] = 0.0;
   out_6611456164701546289[117] = 0.0;
   out_6611456164701546289[118] = 0.0;
   out_6611456164701546289[119] = 0.0;
   out_6611456164701546289[120] = 0.0;
   out_6611456164701546289[121] = 0.0;
   out_6611456164701546289[122] = 0.0;
   out_6611456164701546289[123] = 0.0;
   out_6611456164701546289[124] = 0.0;
   out_6611456164701546289[125] = 0.0;
   out_6611456164701546289[126] = 0.0;
   out_6611456164701546289[127] = 0.0;
   out_6611456164701546289[128] = 0.0;
   out_6611456164701546289[129] = 0.0;
   out_6611456164701546289[130] = 0.0;
   out_6611456164701546289[131] = 0.0;
   out_6611456164701546289[132] = 0.0;
   out_6611456164701546289[133] = 1.0;
   out_6611456164701546289[134] = 0.0;
   out_6611456164701546289[135] = 0.0;
   out_6611456164701546289[136] = 0.0;
   out_6611456164701546289[137] = 0.0;
   out_6611456164701546289[138] = 0.0;
   out_6611456164701546289[139] = 0.0;
   out_6611456164701546289[140] = 0.0;
   out_6611456164701546289[141] = 0.0;
   out_6611456164701546289[142] = 0.0;
   out_6611456164701546289[143] = 0.0;
   out_6611456164701546289[144] = 0.0;
   out_6611456164701546289[145] = 0.0;
   out_6611456164701546289[146] = 0.0;
   out_6611456164701546289[147] = 0.0;
   out_6611456164701546289[148] = 0.0;
   out_6611456164701546289[149] = 0.0;
   out_6611456164701546289[150] = 0.0;
   out_6611456164701546289[151] = 0.0;
   out_6611456164701546289[152] = 1.0;
   out_6611456164701546289[153] = 0.0;
   out_6611456164701546289[154] = 0.0;
   out_6611456164701546289[155] = 0.0;
   out_6611456164701546289[156] = 0.0;
   out_6611456164701546289[157] = 0.0;
   out_6611456164701546289[158] = 0.0;
   out_6611456164701546289[159] = 0.0;
   out_6611456164701546289[160] = 0.0;
   out_6611456164701546289[161] = 0.0;
   out_6611456164701546289[162] = 0.0;
   out_6611456164701546289[163] = 0.0;
   out_6611456164701546289[164] = 0.0;
   out_6611456164701546289[165] = 0.0;
   out_6611456164701546289[166] = 0.0;
   out_6611456164701546289[167] = 0.0;
   out_6611456164701546289[168] = 0.0;
   out_6611456164701546289[169] = 0.0;
   out_6611456164701546289[170] = 0.0;
   out_6611456164701546289[171] = 1.0;
   out_6611456164701546289[172] = 0.0;
   out_6611456164701546289[173] = 0.0;
   out_6611456164701546289[174] = 0.0;
   out_6611456164701546289[175] = 0.0;
   out_6611456164701546289[176] = 0.0;
   out_6611456164701546289[177] = 0.0;
   out_6611456164701546289[178] = 0.0;
   out_6611456164701546289[179] = 0.0;
   out_6611456164701546289[180] = 0.0;
   out_6611456164701546289[181] = 0.0;
   out_6611456164701546289[182] = 0.0;
   out_6611456164701546289[183] = 0.0;
   out_6611456164701546289[184] = 0.0;
   out_6611456164701546289[185] = 0.0;
   out_6611456164701546289[186] = 0.0;
   out_6611456164701546289[187] = 0.0;
   out_6611456164701546289[188] = 0.0;
   out_6611456164701546289[189] = 0.0;
   out_6611456164701546289[190] = 1.0;
   out_6611456164701546289[191] = 0.0;
   out_6611456164701546289[192] = 0.0;
   out_6611456164701546289[193] = 0.0;
   out_6611456164701546289[194] = 0.0;
   out_6611456164701546289[195] = 0.0;
   out_6611456164701546289[196] = 0.0;
   out_6611456164701546289[197] = 0.0;
   out_6611456164701546289[198] = 0.0;
   out_6611456164701546289[199] = 0.0;
   out_6611456164701546289[200] = 0.0;
   out_6611456164701546289[201] = 0.0;
   out_6611456164701546289[202] = 0.0;
   out_6611456164701546289[203] = 0.0;
   out_6611456164701546289[204] = 0.0;
   out_6611456164701546289[205] = 0.0;
   out_6611456164701546289[206] = 0.0;
   out_6611456164701546289[207] = 0.0;
   out_6611456164701546289[208] = 0.0;
   out_6611456164701546289[209] = 1.0;
   out_6611456164701546289[210] = 0.0;
   out_6611456164701546289[211] = 0.0;
   out_6611456164701546289[212] = 0.0;
   out_6611456164701546289[213] = 0.0;
   out_6611456164701546289[214] = 0.0;
   out_6611456164701546289[215] = 0.0;
   out_6611456164701546289[216] = 0.0;
   out_6611456164701546289[217] = 0.0;
   out_6611456164701546289[218] = 0.0;
   out_6611456164701546289[219] = 0.0;
   out_6611456164701546289[220] = 0.0;
   out_6611456164701546289[221] = 0.0;
   out_6611456164701546289[222] = 0.0;
   out_6611456164701546289[223] = 0.0;
   out_6611456164701546289[224] = 0.0;
   out_6611456164701546289[225] = 0.0;
   out_6611456164701546289[226] = 0.0;
   out_6611456164701546289[227] = 0.0;
   out_6611456164701546289[228] = 1.0;
   out_6611456164701546289[229] = 0.0;
   out_6611456164701546289[230] = 0.0;
   out_6611456164701546289[231] = 0.0;
   out_6611456164701546289[232] = 0.0;
   out_6611456164701546289[233] = 0.0;
   out_6611456164701546289[234] = 0.0;
   out_6611456164701546289[235] = 0.0;
   out_6611456164701546289[236] = 0.0;
   out_6611456164701546289[237] = 0.0;
   out_6611456164701546289[238] = 0.0;
   out_6611456164701546289[239] = 0.0;
   out_6611456164701546289[240] = 0.0;
   out_6611456164701546289[241] = 0.0;
   out_6611456164701546289[242] = 0.0;
   out_6611456164701546289[243] = 0.0;
   out_6611456164701546289[244] = 0.0;
   out_6611456164701546289[245] = 0.0;
   out_6611456164701546289[246] = 0.0;
   out_6611456164701546289[247] = 1.0;
   out_6611456164701546289[248] = 0.0;
   out_6611456164701546289[249] = 0.0;
   out_6611456164701546289[250] = 0.0;
   out_6611456164701546289[251] = 0.0;
   out_6611456164701546289[252] = 0.0;
   out_6611456164701546289[253] = 0.0;
   out_6611456164701546289[254] = 0.0;
   out_6611456164701546289[255] = 0.0;
   out_6611456164701546289[256] = 0.0;
   out_6611456164701546289[257] = 0.0;
   out_6611456164701546289[258] = 0.0;
   out_6611456164701546289[259] = 0.0;
   out_6611456164701546289[260] = 0.0;
   out_6611456164701546289[261] = 0.0;
   out_6611456164701546289[262] = 0.0;
   out_6611456164701546289[263] = 0.0;
   out_6611456164701546289[264] = 0.0;
   out_6611456164701546289[265] = 0.0;
   out_6611456164701546289[266] = 1.0;
   out_6611456164701546289[267] = 0.0;
   out_6611456164701546289[268] = 0.0;
   out_6611456164701546289[269] = 0.0;
   out_6611456164701546289[270] = 0.0;
   out_6611456164701546289[271] = 0.0;
   out_6611456164701546289[272] = 0.0;
   out_6611456164701546289[273] = 0.0;
   out_6611456164701546289[274] = 0.0;
   out_6611456164701546289[275] = 0.0;
   out_6611456164701546289[276] = 0.0;
   out_6611456164701546289[277] = 0.0;
   out_6611456164701546289[278] = 0.0;
   out_6611456164701546289[279] = 0.0;
   out_6611456164701546289[280] = 0.0;
   out_6611456164701546289[281] = 0.0;
   out_6611456164701546289[282] = 0.0;
   out_6611456164701546289[283] = 0.0;
   out_6611456164701546289[284] = 0.0;
   out_6611456164701546289[285] = 1.0;
   out_6611456164701546289[286] = 0.0;
   out_6611456164701546289[287] = 0.0;
   out_6611456164701546289[288] = 0.0;
   out_6611456164701546289[289] = 0.0;
   out_6611456164701546289[290] = 0.0;
   out_6611456164701546289[291] = 0.0;
   out_6611456164701546289[292] = 0.0;
   out_6611456164701546289[293] = 0.0;
   out_6611456164701546289[294] = 0.0;
   out_6611456164701546289[295] = 0.0;
   out_6611456164701546289[296] = 0.0;
   out_6611456164701546289[297] = 0.0;
   out_6611456164701546289[298] = 0.0;
   out_6611456164701546289[299] = 0.0;
   out_6611456164701546289[300] = 0.0;
   out_6611456164701546289[301] = 0.0;
   out_6611456164701546289[302] = 0.0;
   out_6611456164701546289[303] = 0.0;
   out_6611456164701546289[304] = 1.0;
   out_6611456164701546289[305] = 0.0;
   out_6611456164701546289[306] = 0.0;
   out_6611456164701546289[307] = 0.0;
   out_6611456164701546289[308] = 0.0;
   out_6611456164701546289[309] = 0.0;
   out_6611456164701546289[310] = 0.0;
   out_6611456164701546289[311] = 0.0;
   out_6611456164701546289[312] = 0.0;
   out_6611456164701546289[313] = 0.0;
   out_6611456164701546289[314] = 0.0;
   out_6611456164701546289[315] = 0.0;
   out_6611456164701546289[316] = 0.0;
   out_6611456164701546289[317] = 0.0;
   out_6611456164701546289[318] = 0.0;
   out_6611456164701546289[319] = 0.0;
   out_6611456164701546289[320] = 0.0;
   out_6611456164701546289[321] = 0.0;
   out_6611456164701546289[322] = 0.0;
   out_6611456164701546289[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_7258279731957877583) {
   out_7258279731957877583[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_7258279731957877583[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_7258279731957877583[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_7258279731957877583[3] = dt*state[12] + state[3];
   out_7258279731957877583[4] = dt*state[13] + state[4];
   out_7258279731957877583[5] = dt*state[14] + state[5];
   out_7258279731957877583[6] = state[6];
   out_7258279731957877583[7] = state[7];
   out_7258279731957877583[8] = state[8];
   out_7258279731957877583[9] = state[9];
   out_7258279731957877583[10] = state[10];
   out_7258279731957877583[11] = state[11];
   out_7258279731957877583[12] = state[12];
   out_7258279731957877583[13] = state[13];
   out_7258279731957877583[14] = state[14];
   out_7258279731957877583[15] = state[15];
   out_7258279731957877583[16] = state[16];
   out_7258279731957877583[17] = state[17];
}
void F_fun(double *state, double dt, double *out_3804310599737068228) {
   out_3804310599737068228[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3804310599737068228[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3804310599737068228[2] = 0;
   out_3804310599737068228[3] = 0;
   out_3804310599737068228[4] = 0;
   out_3804310599737068228[5] = 0;
   out_3804310599737068228[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3804310599737068228[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3804310599737068228[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3804310599737068228[9] = 0;
   out_3804310599737068228[10] = 0;
   out_3804310599737068228[11] = 0;
   out_3804310599737068228[12] = 0;
   out_3804310599737068228[13] = 0;
   out_3804310599737068228[14] = 0;
   out_3804310599737068228[15] = 0;
   out_3804310599737068228[16] = 0;
   out_3804310599737068228[17] = 0;
   out_3804310599737068228[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3804310599737068228[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3804310599737068228[20] = 0;
   out_3804310599737068228[21] = 0;
   out_3804310599737068228[22] = 0;
   out_3804310599737068228[23] = 0;
   out_3804310599737068228[24] = 0;
   out_3804310599737068228[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3804310599737068228[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3804310599737068228[27] = 0;
   out_3804310599737068228[28] = 0;
   out_3804310599737068228[29] = 0;
   out_3804310599737068228[30] = 0;
   out_3804310599737068228[31] = 0;
   out_3804310599737068228[32] = 0;
   out_3804310599737068228[33] = 0;
   out_3804310599737068228[34] = 0;
   out_3804310599737068228[35] = 0;
   out_3804310599737068228[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3804310599737068228[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3804310599737068228[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3804310599737068228[39] = 0;
   out_3804310599737068228[40] = 0;
   out_3804310599737068228[41] = 0;
   out_3804310599737068228[42] = 0;
   out_3804310599737068228[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3804310599737068228[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3804310599737068228[45] = 0;
   out_3804310599737068228[46] = 0;
   out_3804310599737068228[47] = 0;
   out_3804310599737068228[48] = 0;
   out_3804310599737068228[49] = 0;
   out_3804310599737068228[50] = 0;
   out_3804310599737068228[51] = 0;
   out_3804310599737068228[52] = 0;
   out_3804310599737068228[53] = 0;
   out_3804310599737068228[54] = 0;
   out_3804310599737068228[55] = 0;
   out_3804310599737068228[56] = 0;
   out_3804310599737068228[57] = 1;
   out_3804310599737068228[58] = 0;
   out_3804310599737068228[59] = 0;
   out_3804310599737068228[60] = 0;
   out_3804310599737068228[61] = 0;
   out_3804310599737068228[62] = 0;
   out_3804310599737068228[63] = 0;
   out_3804310599737068228[64] = 0;
   out_3804310599737068228[65] = 0;
   out_3804310599737068228[66] = dt;
   out_3804310599737068228[67] = 0;
   out_3804310599737068228[68] = 0;
   out_3804310599737068228[69] = 0;
   out_3804310599737068228[70] = 0;
   out_3804310599737068228[71] = 0;
   out_3804310599737068228[72] = 0;
   out_3804310599737068228[73] = 0;
   out_3804310599737068228[74] = 0;
   out_3804310599737068228[75] = 0;
   out_3804310599737068228[76] = 1;
   out_3804310599737068228[77] = 0;
   out_3804310599737068228[78] = 0;
   out_3804310599737068228[79] = 0;
   out_3804310599737068228[80] = 0;
   out_3804310599737068228[81] = 0;
   out_3804310599737068228[82] = 0;
   out_3804310599737068228[83] = 0;
   out_3804310599737068228[84] = 0;
   out_3804310599737068228[85] = dt;
   out_3804310599737068228[86] = 0;
   out_3804310599737068228[87] = 0;
   out_3804310599737068228[88] = 0;
   out_3804310599737068228[89] = 0;
   out_3804310599737068228[90] = 0;
   out_3804310599737068228[91] = 0;
   out_3804310599737068228[92] = 0;
   out_3804310599737068228[93] = 0;
   out_3804310599737068228[94] = 0;
   out_3804310599737068228[95] = 1;
   out_3804310599737068228[96] = 0;
   out_3804310599737068228[97] = 0;
   out_3804310599737068228[98] = 0;
   out_3804310599737068228[99] = 0;
   out_3804310599737068228[100] = 0;
   out_3804310599737068228[101] = 0;
   out_3804310599737068228[102] = 0;
   out_3804310599737068228[103] = 0;
   out_3804310599737068228[104] = dt;
   out_3804310599737068228[105] = 0;
   out_3804310599737068228[106] = 0;
   out_3804310599737068228[107] = 0;
   out_3804310599737068228[108] = 0;
   out_3804310599737068228[109] = 0;
   out_3804310599737068228[110] = 0;
   out_3804310599737068228[111] = 0;
   out_3804310599737068228[112] = 0;
   out_3804310599737068228[113] = 0;
   out_3804310599737068228[114] = 1;
   out_3804310599737068228[115] = 0;
   out_3804310599737068228[116] = 0;
   out_3804310599737068228[117] = 0;
   out_3804310599737068228[118] = 0;
   out_3804310599737068228[119] = 0;
   out_3804310599737068228[120] = 0;
   out_3804310599737068228[121] = 0;
   out_3804310599737068228[122] = 0;
   out_3804310599737068228[123] = 0;
   out_3804310599737068228[124] = 0;
   out_3804310599737068228[125] = 0;
   out_3804310599737068228[126] = 0;
   out_3804310599737068228[127] = 0;
   out_3804310599737068228[128] = 0;
   out_3804310599737068228[129] = 0;
   out_3804310599737068228[130] = 0;
   out_3804310599737068228[131] = 0;
   out_3804310599737068228[132] = 0;
   out_3804310599737068228[133] = 1;
   out_3804310599737068228[134] = 0;
   out_3804310599737068228[135] = 0;
   out_3804310599737068228[136] = 0;
   out_3804310599737068228[137] = 0;
   out_3804310599737068228[138] = 0;
   out_3804310599737068228[139] = 0;
   out_3804310599737068228[140] = 0;
   out_3804310599737068228[141] = 0;
   out_3804310599737068228[142] = 0;
   out_3804310599737068228[143] = 0;
   out_3804310599737068228[144] = 0;
   out_3804310599737068228[145] = 0;
   out_3804310599737068228[146] = 0;
   out_3804310599737068228[147] = 0;
   out_3804310599737068228[148] = 0;
   out_3804310599737068228[149] = 0;
   out_3804310599737068228[150] = 0;
   out_3804310599737068228[151] = 0;
   out_3804310599737068228[152] = 1;
   out_3804310599737068228[153] = 0;
   out_3804310599737068228[154] = 0;
   out_3804310599737068228[155] = 0;
   out_3804310599737068228[156] = 0;
   out_3804310599737068228[157] = 0;
   out_3804310599737068228[158] = 0;
   out_3804310599737068228[159] = 0;
   out_3804310599737068228[160] = 0;
   out_3804310599737068228[161] = 0;
   out_3804310599737068228[162] = 0;
   out_3804310599737068228[163] = 0;
   out_3804310599737068228[164] = 0;
   out_3804310599737068228[165] = 0;
   out_3804310599737068228[166] = 0;
   out_3804310599737068228[167] = 0;
   out_3804310599737068228[168] = 0;
   out_3804310599737068228[169] = 0;
   out_3804310599737068228[170] = 0;
   out_3804310599737068228[171] = 1;
   out_3804310599737068228[172] = 0;
   out_3804310599737068228[173] = 0;
   out_3804310599737068228[174] = 0;
   out_3804310599737068228[175] = 0;
   out_3804310599737068228[176] = 0;
   out_3804310599737068228[177] = 0;
   out_3804310599737068228[178] = 0;
   out_3804310599737068228[179] = 0;
   out_3804310599737068228[180] = 0;
   out_3804310599737068228[181] = 0;
   out_3804310599737068228[182] = 0;
   out_3804310599737068228[183] = 0;
   out_3804310599737068228[184] = 0;
   out_3804310599737068228[185] = 0;
   out_3804310599737068228[186] = 0;
   out_3804310599737068228[187] = 0;
   out_3804310599737068228[188] = 0;
   out_3804310599737068228[189] = 0;
   out_3804310599737068228[190] = 1;
   out_3804310599737068228[191] = 0;
   out_3804310599737068228[192] = 0;
   out_3804310599737068228[193] = 0;
   out_3804310599737068228[194] = 0;
   out_3804310599737068228[195] = 0;
   out_3804310599737068228[196] = 0;
   out_3804310599737068228[197] = 0;
   out_3804310599737068228[198] = 0;
   out_3804310599737068228[199] = 0;
   out_3804310599737068228[200] = 0;
   out_3804310599737068228[201] = 0;
   out_3804310599737068228[202] = 0;
   out_3804310599737068228[203] = 0;
   out_3804310599737068228[204] = 0;
   out_3804310599737068228[205] = 0;
   out_3804310599737068228[206] = 0;
   out_3804310599737068228[207] = 0;
   out_3804310599737068228[208] = 0;
   out_3804310599737068228[209] = 1;
   out_3804310599737068228[210] = 0;
   out_3804310599737068228[211] = 0;
   out_3804310599737068228[212] = 0;
   out_3804310599737068228[213] = 0;
   out_3804310599737068228[214] = 0;
   out_3804310599737068228[215] = 0;
   out_3804310599737068228[216] = 0;
   out_3804310599737068228[217] = 0;
   out_3804310599737068228[218] = 0;
   out_3804310599737068228[219] = 0;
   out_3804310599737068228[220] = 0;
   out_3804310599737068228[221] = 0;
   out_3804310599737068228[222] = 0;
   out_3804310599737068228[223] = 0;
   out_3804310599737068228[224] = 0;
   out_3804310599737068228[225] = 0;
   out_3804310599737068228[226] = 0;
   out_3804310599737068228[227] = 0;
   out_3804310599737068228[228] = 1;
   out_3804310599737068228[229] = 0;
   out_3804310599737068228[230] = 0;
   out_3804310599737068228[231] = 0;
   out_3804310599737068228[232] = 0;
   out_3804310599737068228[233] = 0;
   out_3804310599737068228[234] = 0;
   out_3804310599737068228[235] = 0;
   out_3804310599737068228[236] = 0;
   out_3804310599737068228[237] = 0;
   out_3804310599737068228[238] = 0;
   out_3804310599737068228[239] = 0;
   out_3804310599737068228[240] = 0;
   out_3804310599737068228[241] = 0;
   out_3804310599737068228[242] = 0;
   out_3804310599737068228[243] = 0;
   out_3804310599737068228[244] = 0;
   out_3804310599737068228[245] = 0;
   out_3804310599737068228[246] = 0;
   out_3804310599737068228[247] = 1;
   out_3804310599737068228[248] = 0;
   out_3804310599737068228[249] = 0;
   out_3804310599737068228[250] = 0;
   out_3804310599737068228[251] = 0;
   out_3804310599737068228[252] = 0;
   out_3804310599737068228[253] = 0;
   out_3804310599737068228[254] = 0;
   out_3804310599737068228[255] = 0;
   out_3804310599737068228[256] = 0;
   out_3804310599737068228[257] = 0;
   out_3804310599737068228[258] = 0;
   out_3804310599737068228[259] = 0;
   out_3804310599737068228[260] = 0;
   out_3804310599737068228[261] = 0;
   out_3804310599737068228[262] = 0;
   out_3804310599737068228[263] = 0;
   out_3804310599737068228[264] = 0;
   out_3804310599737068228[265] = 0;
   out_3804310599737068228[266] = 1;
   out_3804310599737068228[267] = 0;
   out_3804310599737068228[268] = 0;
   out_3804310599737068228[269] = 0;
   out_3804310599737068228[270] = 0;
   out_3804310599737068228[271] = 0;
   out_3804310599737068228[272] = 0;
   out_3804310599737068228[273] = 0;
   out_3804310599737068228[274] = 0;
   out_3804310599737068228[275] = 0;
   out_3804310599737068228[276] = 0;
   out_3804310599737068228[277] = 0;
   out_3804310599737068228[278] = 0;
   out_3804310599737068228[279] = 0;
   out_3804310599737068228[280] = 0;
   out_3804310599737068228[281] = 0;
   out_3804310599737068228[282] = 0;
   out_3804310599737068228[283] = 0;
   out_3804310599737068228[284] = 0;
   out_3804310599737068228[285] = 1;
   out_3804310599737068228[286] = 0;
   out_3804310599737068228[287] = 0;
   out_3804310599737068228[288] = 0;
   out_3804310599737068228[289] = 0;
   out_3804310599737068228[290] = 0;
   out_3804310599737068228[291] = 0;
   out_3804310599737068228[292] = 0;
   out_3804310599737068228[293] = 0;
   out_3804310599737068228[294] = 0;
   out_3804310599737068228[295] = 0;
   out_3804310599737068228[296] = 0;
   out_3804310599737068228[297] = 0;
   out_3804310599737068228[298] = 0;
   out_3804310599737068228[299] = 0;
   out_3804310599737068228[300] = 0;
   out_3804310599737068228[301] = 0;
   out_3804310599737068228[302] = 0;
   out_3804310599737068228[303] = 0;
   out_3804310599737068228[304] = 1;
   out_3804310599737068228[305] = 0;
   out_3804310599737068228[306] = 0;
   out_3804310599737068228[307] = 0;
   out_3804310599737068228[308] = 0;
   out_3804310599737068228[309] = 0;
   out_3804310599737068228[310] = 0;
   out_3804310599737068228[311] = 0;
   out_3804310599737068228[312] = 0;
   out_3804310599737068228[313] = 0;
   out_3804310599737068228[314] = 0;
   out_3804310599737068228[315] = 0;
   out_3804310599737068228[316] = 0;
   out_3804310599737068228[317] = 0;
   out_3804310599737068228[318] = 0;
   out_3804310599737068228[319] = 0;
   out_3804310599737068228[320] = 0;
   out_3804310599737068228[321] = 0;
   out_3804310599737068228[322] = 0;
   out_3804310599737068228[323] = 1;
}
void h_4(double *state, double *unused, double *out_1042392600665946689) {
   out_1042392600665946689[0] = state[6] + state[9];
   out_1042392600665946689[1] = state[7] + state[10];
   out_1042392600665946689[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_2890926242950183985) {
   out_2890926242950183985[0] = 0;
   out_2890926242950183985[1] = 0;
   out_2890926242950183985[2] = 0;
   out_2890926242950183985[3] = 0;
   out_2890926242950183985[4] = 0;
   out_2890926242950183985[5] = 0;
   out_2890926242950183985[6] = 1;
   out_2890926242950183985[7] = 0;
   out_2890926242950183985[8] = 0;
   out_2890926242950183985[9] = 1;
   out_2890926242950183985[10] = 0;
   out_2890926242950183985[11] = 0;
   out_2890926242950183985[12] = 0;
   out_2890926242950183985[13] = 0;
   out_2890926242950183985[14] = 0;
   out_2890926242950183985[15] = 0;
   out_2890926242950183985[16] = 0;
   out_2890926242950183985[17] = 0;
   out_2890926242950183985[18] = 0;
   out_2890926242950183985[19] = 0;
   out_2890926242950183985[20] = 0;
   out_2890926242950183985[21] = 0;
   out_2890926242950183985[22] = 0;
   out_2890926242950183985[23] = 0;
   out_2890926242950183985[24] = 0;
   out_2890926242950183985[25] = 1;
   out_2890926242950183985[26] = 0;
   out_2890926242950183985[27] = 0;
   out_2890926242950183985[28] = 1;
   out_2890926242950183985[29] = 0;
   out_2890926242950183985[30] = 0;
   out_2890926242950183985[31] = 0;
   out_2890926242950183985[32] = 0;
   out_2890926242950183985[33] = 0;
   out_2890926242950183985[34] = 0;
   out_2890926242950183985[35] = 0;
   out_2890926242950183985[36] = 0;
   out_2890926242950183985[37] = 0;
   out_2890926242950183985[38] = 0;
   out_2890926242950183985[39] = 0;
   out_2890926242950183985[40] = 0;
   out_2890926242950183985[41] = 0;
   out_2890926242950183985[42] = 0;
   out_2890926242950183985[43] = 0;
   out_2890926242950183985[44] = 1;
   out_2890926242950183985[45] = 0;
   out_2890926242950183985[46] = 0;
   out_2890926242950183985[47] = 1;
   out_2890926242950183985[48] = 0;
   out_2890926242950183985[49] = 0;
   out_2890926242950183985[50] = 0;
   out_2890926242950183985[51] = 0;
   out_2890926242950183985[52] = 0;
   out_2890926242950183985[53] = 0;
}
void h_10(double *state, double *unused, double *out_8809755638952996463) {
   out_8809755638952996463[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_8809755638952996463[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_8809755638952996463[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_8718760487604320532) {
   out_8718760487604320532[0] = 0;
   out_8718760487604320532[1] = 9.8100000000000005*cos(state[1]);
   out_8718760487604320532[2] = 0;
   out_8718760487604320532[3] = 0;
   out_8718760487604320532[4] = -state[8];
   out_8718760487604320532[5] = state[7];
   out_8718760487604320532[6] = 0;
   out_8718760487604320532[7] = state[5];
   out_8718760487604320532[8] = -state[4];
   out_8718760487604320532[9] = 0;
   out_8718760487604320532[10] = 0;
   out_8718760487604320532[11] = 0;
   out_8718760487604320532[12] = 1;
   out_8718760487604320532[13] = 0;
   out_8718760487604320532[14] = 0;
   out_8718760487604320532[15] = 1;
   out_8718760487604320532[16] = 0;
   out_8718760487604320532[17] = 0;
   out_8718760487604320532[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_8718760487604320532[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_8718760487604320532[20] = 0;
   out_8718760487604320532[21] = state[8];
   out_8718760487604320532[22] = 0;
   out_8718760487604320532[23] = -state[6];
   out_8718760487604320532[24] = -state[5];
   out_8718760487604320532[25] = 0;
   out_8718760487604320532[26] = state[3];
   out_8718760487604320532[27] = 0;
   out_8718760487604320532[28] = 0;
   out_8718760487604320532[29] = 0;
   out_8718760487604320532[30] = 0;
   out_8718760487604320532[31] = 1;
   out_8718760487604320532[32] = 0;
   out_8718760487604320532[33] = 0;
   out_8718760487604320532[34] = 1;
   out_8718760487604320532[35] = 0;
   out_8718760487604320532[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_8718760487604320532[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_8718760487604320532[38] = 0;
   out_8718760487604320532[39] = -state[7];
   out_8718760487604320532[40] = state[6];
   out_8718760487604320532[41] = 0;
   out_8718760487604320532[42] = state[4];
   out_8718760487604320532[43] = -state[3];
   out_8718760487604320532[44] = 0;
   out_8718760487604320532[45] = 0;
   out_8718760487604320532[46] = 0;
   out_8718760487604320532[47] = 0;
   out_8718760487604320532[48] = 0;
   out_8718760487604320532[49] = 0;
   out_8718760487604320532[50] = 1;
   out_8718760487604320532[51] = 0;
   out_8718760487604320532[52] = 0;
   out_8718760487604320532[53] = 1;
}
void h_13(double *state, double *unused, double *out_8270645238899386460) {
   out_8270645238899386460[0] = state[3];
   out_8270645238899386460[1] = state[4];
   out_8270645238899386460[2] = state[5];
}
void H_13(double *state, double *unused, double *out_942829220352340039) {
   out_942829220352340039[0] = 0;
   out_942829220352340039[1] = 0;
   out_942829220352340039[2] = 0;
   out_942829220352340039[3] = 1;
   out_942829220352340039[4] = 0;
   out_942829220352340039[5] = 0;
   out_942829220352340039[6] = 0;
   out_942829220352340039[7] = 0;
   out_942829220352340039[8] = 0;
   out_942829220352340039[9] = 0;
   out_942829220352340039[10] = 0;
   out_942829220352340039[11] = 0;
   out_942829220352340039[12] = 0;
   out_942829220352340039[13] = 0;
   out_942829220352340039[14] = 0;
   out_942829220352340039[15] = 0;
   out_942829220352340039[16] = 0;
   out_942829220352340039[17] = 0;
   out_942829220352340039[18] = 0;
   out_942829220352340039[19] = 0;
   out_942829220352340039[20] = 0;
   out_942829220352340039[21] = 0;
   out_942829220352340039[22] = 1;
   out_942829220352340039[23] = 0;
   out_942829220352340039[24] = 0;
   out_942829220352340039[25] = 0;
   out_942829220352340039[26] = 0;
   out_942829220352340039[27] = 0;
   out_942829220352340039[28] = 0;
   out_942829220352340039[29] = 0;
   out_942829220352340039[30] = 0;
   out_942829220352340039[31] = 0;
   out_942829220352340039[32] = 0;
   out_942829220352340039[33] = 0;
   out_942829220352340039[34] = 0;
   out_942829220352340039[35] = 0;
   out_942829220352340039[36] = 0;
   out_942829220352340039[37] = 0;
   out_942829220352340039[38] = 0;
   out_942829220352340039[39] = 0;
   out_942829220352340039[40] = 0;
   out_942829220352340039[41] = 1;
   out_942829220352340039[42] = 0;
   out_942829220352340039[43] = 0;
   out_942829220352340039[44] = 0;
   out_942829220352340039[45] = 0;
   out_942829220352340039[46] = 0;
   out_942829220352340039[47] = 0;
   out_942829220352340039[48] = 0;
   out_942829220352340039[49] = 0;
   out_942829220352340039[50] = 0;
   out_942829220352340039[51] = 0;
   out_942829220352340039[52] = 0;
   out_942829220352340039[53] = 0;
}
void h_14(double *state, double *unused, double *out_891035561933187989) {
   out_891035561933187989[0] = state[6];
   out_891035561933187989[1] = state[7];
   out_891035561933187989[2] = state[8];
}
void H_14(double *state, double *unused, double *out_191862189345188311) {
   out_191862189345188311[0] = 0;
   out_191862189345188311[1] = 0;
   out_191862189345188311[2] = 0;
   out_191862189345188311[3] = 0;
   out_191862189345188311[4] = 0;
   out_191862189345188311[5] = 0;
   out_191862189345188311[6] = 1;
   out_191862189345188311[7] = 0;
   out_191862189345188311[8] = 0;
   out_191862189345188311[9] = 0;
   out_191862189345188311[10] = 0;
   out_191862189345188311[11] = 0;
   out_191862189345188311[12] = 0;
   out_191862189345188311[13] = 0;
   out_191862189345188311[14] = 0;
   out_191862189345188311[15] = 0;
   out_191862189345188311[16] = 0;
   out_191862189345188311[17] = 0;
   out_191862189345188311[18] = 0;
   out_191862189345188311[19] = 0;
   out_191862189345188311[20] = 0;
   out_191862189345188311[21] = 0;
   out_191862189345188311[22] = 0;
   out_191862189345188311[23] = 0;
   out_191862189345188311[24] = 0;
   out_191862189345188311[25] = 1;
   out_191862189345188311[26] = 0;
   out_191862189345188311[27] = 0;
   out_191862189345188311[28] = 0;
   out_191862189345188311[29] = 0;
   out_191862189345188311[30] = 0;
   out_191862189345188311[31] = 0;
   out_191862189345188311[32] = 0;
   out_191862189345188311[33] = 0;
   out_191862189345188311[34] = 0;
   out_191862189345188311[35] = 0;
   out_191862189345188311[36] = 0;
   out_191862189345188311[37] = 0;
   out_191862189345188311[38] = 0;
   out_191862189345188311[39] = 0;
   out_191862189345188311[40] = 0;
   out_191862189345188311[41] = 0;
   out_191862189345188311[42] = 0;
   out_191862189345188311[43] = 0;
   out_191862189345188311[44] = 1;
   out_191862189345188311[45] = 0;
   out_191862189345188311[46] = 0;
   out_191862189345188311[47] = 0;
   out_191862189345188311[48] = 0;
   out_191862189345188311[49] = 0;
   out_191862189345188311[50] = 0;
   out_191862189345188311[51] = 0;
   out_191862189345188311[52] = 0;
   out_191862189345188311[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_2520069339484736335) {
  err_fun(nom_x, delta_x, out_2520069339484736335);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_3053044731700442871) {
  inv_err_fun(nom_x, true_x, out_3053044731700442871);
}
void pose_H_mod_fun(double *state, double *out_6611456164701546289) {
  H_mod_fun(state, out_6611456164701546289);
}
void pose_f_fun(double *state, double dt, double *out_7258279731957877583) {
  f_fun(state,  dt, out_7258279731957877583);
}
void pose_F_fun(double *state, double dt, double *out_3804310599737068228) {
  F_fun(state,  dt, out_3804310599737068228);
}
void pose_h_4(double *state, double *unused, double *out_1042392600665946689) {
  h_4(state, unused, out_1042392600665946689);
}
void pose_H_4(double *state, double *unused, double *out_2890926242950183985) {
  H_4(state, unused, out_2890926242950183985);
}
void pose_h_10(double *state, double *unused, double *out_8809755638952996463) {
  h_10(state, unused, out_8809755638952996463);
}
void pose_H_10(double *state, double *unused, double *out_8718760487604320532) {
  H_10(state, unused, out_8718760487604320532);
}
void pose_h_13(double *state, double *unused, double *out_8270645238899386460) {
  h_13(state, unused, out_8270645238899386460);
}
void pose_H_13(double *state, double *unused, double *out_942829220352340039) {
  H_13(state, unused, out_942829220352340039);
}
void pose_h_14(double *state, double *unused, double *out_891035561933187989) {
  h_14(state, unused, out_891035561933187989);
}
void pose_H_14(double *state, double *unused, double *out_191862189345188311) {
  H_14(state, unused, out_191862189345188311);
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
