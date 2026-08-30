#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_5924875962979878419) {
   out_5924875962979878419[0] = delta_x[0] + nom_x[0];
   out_5924875962979878419[1] = delta_x[1] + nom_x[1];
   out_5924875962979878419[2] = delta_x[2] + nom_x[2];
   out_5924875962979878419[3] = delta_x[3] + nom_x[3];
   out_5924875962979878419[4] = delta_x[4] + nom_x[4];
   out_5924875962979878419[5] = delta_x[5] + nom_x[5];
   out_5924875962979878419[6] = delta_x[6] + nom_x[6];
   out_5924875962979878419[7] = delta_x[7] + nom_x[7];
   out_5924875962979878419[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_3730797253670217675) {
   out_3730797253670217675[0] = -nom_x[0] + true_x[0];
   out_3730797253670217675[1] = -nom_x[1] + true_x[1];
   out_3730797253670217675[2] = -nom_x[2] + true_x[2];
   out_3730797253670217675[3] = -nom_x[3] + true_x[3];
   out_3730797253670217675[4] = -nom_x[4] + true_x[4];
   out_3730797253670217675[5] = -nom_x[5] + true_x[5];
   out_3730797253670217675[6] = -nom_x[6] + true_x[6];
   out_3730797253670217675[7] = -nom_x[7] + true_x[7];
   out_3730797253670217675[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_8998384333727686430) {
   out_8998384333727686430[0] = 1.0;
   out_8998384333727686430[1] = 0.0;
   out_8998384333727686430[2] = 0.0;
   out_8998384333727686430[3] = 0.0;
   out_8998384333727686430[4] = 0.0;
   out_8998384333727686430[5] = 0.0;
   out_8998384333727686430[6] = 0.0;
   out_8998384333727686430[7] = 0.0;
   out_8998384333727686430[8] = 0.0;
   out_8998384333727686430[9] = 0.0;
   out_8998384333727686430[10] = 1.0;
   out_8998384333727686430[11] = 0.0;
   out_8998384333727686430[12] = 0.0;
   out_8998384333727686430[13] = 0.0;
   out_8998384333727686430[14] = 0.0;
   out_8998384333727686430[15] = 0.0;
   out_8998384333727686430[16] = 0.0;
   out_8998384333727686430[17] = 0.0;
   out_8998384333727686430[18] = 0.0;
   out_8998384333727686430[19] = 0.0;
   out_8998384333727686430[20] = 1.0;
   out_8998384333727686430[21] = 0.0;
   out_8998384333727686430[22] = 0.0;
   out_8998384333727686430[23] = 0.0;
   out_8998384333727686430[24] = 0.0;
   out_8998384333727686430[25] = 0.0;
   out_8998384333727686430[26] = 0.0;
   out_8998384333727686430[27] = 0.0;
   out_8998384333727686430[28] = 0.0;
   out_8998384333727686430[29] = 0.0;
   out_8998384333727686430[30] = 1.0;
   out_8998384333727686430[31] = 0.0;
   out_8998384333727686430[32] = 0.0;
   out_8998384333727686430[33] = 0.0;
   out_8998384333727686430[34] = 0.0;
   out_8998384333727686430[35] = 0.0;
   out_8998384333727686430[36] = 0.0;
   out_8998384333727686430[37] = 0.0;
   out_8998384333727686430[38] = 0.0;
   out_8998384333727686430[39] = 0.0;
   out_8998384333727686430[40] = 1.0;
   out_8998384333727686430[41] = 0.0;
   out_8998384333727686430[42] = 0.0;
   out_8998384333727686430[43] = 0.0;
   out_8998384333727686430[44] = 0.0;
   out_8998384333727686430[45] = 0.0;
   out_8998384333727686430[46] = 0.0;
   out_8998384333727686430[47] = 0.0;
   out_8998384333727686430[48] = 0.0;
   out_8998384333727686430[49] = 0.0;
   out_8998384333727686430[50] = 1.0;
   out_8998384333727686430[51] = 0.0;
   out_8998384333727686430[52] = 0.0;
   out_8998384333727686430[53] = 0.0;
   out_8998384333727686430[54] = 0.0;
   out_8998384333727686430[55] = 0.0;
   out_8998384333727686430[56] = 0.0;
   out_8998384333727686430[57] = 0.0;
   out_8998384333727686430[58] = 0.0;
   out_8998384333727686430[59] = 0.0;
   out_8998384333727686430[60] = 1.0;
   out_8998384333727686430[61] = 0.0;
   out_8998384333727686430[62] = 0.0;
   out_8998384333727686430[63] = 0.0;
   out_8998384333727686430[64] = 0.0;
   out_8998384333727686430[65] = 0.0;
   out_8998384333727686430[66] = 0.0;
   out_8998384333727686430[67] = 0.0;
   out_8998384333727686430[68] = 0.0;
   out_8998384333727686430[69] = 0.0;
   out_8998384333727686430[70] = 1.0;
   out_8998384333727686430[71] = 0.0;
   out_8998384333727686430[72] = 0.0;
   out_8998384333727686430[73] = 0.0;
   out_8998384333727686430[74] = 0.0;
   out_8998384333727686430[75] = 0.0;
   out_8998384333727686430[76] = 0.0;
   out_8998384333727686430[77] = 0.0;
   out_8998384333727686430[78] = 0.0;
   out_8998384333727686430[79] = 0.0;
   out_8998384333727686430[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_5904937932592495420) {
   out_5904937932592495420[0] = state[0];
   out_5904937932592495420[1] = state[1];
   out_5904937932592495420[2] = state[2];
   out_5904937932592495420[3] = state[3];
   out_5904937932592495420[4] = state[4];
   out_5904937932592495420[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_5904937932592495420[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_5904937932592495420[7] = state[7];
   out_5904937932592495420[8] = state[8];
}
void F_fun(double *state, double dt, double *out_874526194948226047) {
   out_874526194948226047[0] = 1;
   out_874526194948226047[1] = 0;
   out_874526194948226047[2] = 0;
   out_874526194948226047[3] = 0;
   out_874526194948226047[4] = 0;
   out_874526194948226047[5] = 0;
   out_874526194948226047[6] = 0;
   out_874526194948226047[7] = 0;
   out_874526194948226047[8] = 0;
   out_874526194948226047[9] = 0;
   out_874526194948226047[10] = 1;
   out_874526194948226047[11] = 0;
   out_874526194948226047[12] = 0;
   out_874526194948226047[13] = 0;
   out_874526194948226047[14] = 0;
   out_874526194948226047[15] = 0;
   out_874526194948226047[16] = 0;
   out_874526194948226047[17] = 0;
   out_874526194948226047[18] = 0;
   out_874526194948226047[19] = 0;
   out_874526194948226047[20] = 1;
   out_874526194948226047[21] = 0;
   out_874526194948226047[22] = 0;
   out_874526194948226047[23] = 0;
   out_874526194948226047[24] = 0;
   out_874526194948226047[25] = 0;
   out_874526194948226047[26] = 0;
   out_874526194948226047[27] = 0;
   out_874526194948226047[28] = 0;
   out_874526194948226047[29] = 0;
   out_874526194948226047[30] = 1;
   out_874526194948226047[31] = 0;
   out_874526194948226047[32] = 0;
   out_874526194948226047[33] = 0;
   out_874526194948226047[34] = 0;
   out_874526194948226047[35] = 0;
   out_874526194948226047[36] = 0;
   out_874526194948226047[37] = 0;
   out_874526194948226047[38] = 0;
   out_874526194948226047[39] = 0;
   out_874526194948226047[40] = 1;
   out_874526194948226047[41] = 0;
   out_874526194948226047[42] = 0;
   out_874526194948226047[43] = 0;
   out_874526194948226047[44] = 0;
   out_874526194948226047[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_874526194948226047[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_874526194948226047[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_874526194948226047[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_874526194948226047[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_874526194948226047[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_874526194948226047[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_874526194948226047[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_874526194948226047[53] = -9.8100000000000005*dt;
   out_874526194948226047[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_874526194948226047[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_874526194948226047[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_874526194948226047[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_874526194948226047[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_874526194948226047[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_874526194948226047[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_874526194948226047[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_874526194948226047[62] = 0;
   out_874526194948226047[63] = 0;
   out_874526194948226047[64] = 0;
   out_874526194948226047[65] = 0;
   out_874526194948226047[66] = 0;
   out_874526194948226047[67] = 0;
   out_874526194948226047[68] = 0;
   out_874526194948226047[69] = 0;
   out_874526194948226047[70] = 1;
   out_874526194948226047[71] = 0;
   out_874526194948226047[72] = 0;
   out_874526194948226047[73] = 0;
   out_874526194948226047[74] = 0;
   out_874526194948226047[75] = 0;
   out_874526194948226047[76] = 0;
   out_874526194948226047[77] = 0;
   out_874526194948226047[78] = 0;
   out_874526194948226047[79] = 0;
   out_874526194948226047[80] = 1;
}
void h_25(double *state, double *unused, double *out_5000351285338572771) {
   out_5000351285338572771[0] = state[6];
}
void H_25(double *state, double *unused, double *out_194803892184550807) {
   out_194803892184550807[0] = 0;
   out_194803892184550807[1] = 0;
   out_194803892184550807[2] = 0;
   out_194803892184550807[3] = 0;
   out_194803892184550807[4] = 0;
   out_194803892184550807[5] = 0;
   out_194803892184550807[6] = 1;
   out_194803892184550807[7] = 0;
   out_194803892184550807[8] = 0;
}
void h_24(double *state, double *unused, double *out_651987176130886021) {
   out_651987176130886021[0] = state[4];
   out_651987176130886021[1] = state[5];
}
void H_24(double *state, double *unused, double *out_5015125396840539070) {
   out_5015125396840539070[0] = 0;
   out_5015125396840539070[1] = 0;
   out_5015125396840539070[2] = 0;
   out_5015125396840539070[3] = 0;
   out_5015125396840539070[4] = 1;
   out_5015125396840539070[5] = 0;
   out_5015125396840539070[6] = 0;
   out_5015125396840539070[7] = 0;
   out_5015125396840539070[8] = 0;
   out_5015125396840539070[9] = 0;
   out_5015125396840539070[10] = 0;
   out_5015125396840539070[11] = 0;
   out_5015125396840539070[12] = 0;
   out_5015125396840539070[13] = 0;
   out_5015125396840539070[14] = 1;
   out_5015125396840539070[15] = 0;
   out_5015125396840539070[16] = 0;
   out_5015125396840539070[17] = 0;
}
void h_30(double *state, double *unused, double *out_8755349036136976630) {
   out_8755349036136976630[0] = state[4];
}
void H_30(double *state, double *unused, double *out_324142839327790877) {
   out_324142839327790877[0] = 0;
   out_324142839327790877[1] = 0;
   out_324142839327790877[2] = 0;
   out_324142839327790877[3] = 0;
   out_324142839327790877[4] = 1;
   out_324142839327790877[5] = 0;
   out_324142839327790877[6] = 0;
   out_324142839327790877[7] = 0;
   out_324142839327790877[8] = 0;
}
void h_26(double *state, double *unused, double *out_4689721769911729341) {
   out_4689721769911729341[0] = state[7];
}
void H_26(double *state, double *unused, double *out_3936307211058607031) {
   out_3936307211058607031[0] = 0;
   out_3936307211058607031[1] = 0;
   out_3936307211058607031[2] = 0;
   out_3936307211058607031[3] = 0;
   out_3936307211058607031[4] = 0;
   out_3936307211058607031[5] = 0;
   out_3936307211058607031[6] = 0;
   out_3936307211058607031[7] = 1;
   out_3936307211058607031[8] = 0;
}
void h_27(double *state, double *unused, double *out_7961308757030146390) {
   out_7961308757030146390[0] = state[3];
}
void H_27(double *state, double *unused, double *out_2498906151128215788) {
   out_2498906151128215788[0] = 0;
   out_2498906151128215788[1] = 0;
   out_2498906151128215788[2] = 0;
   out_2498906151128215788[3] = 1;
   out_2498906151128215788[4] = 0;
   out_2498906151128215788[5] = 0;
   out_2498906151128215788[6] = 0;
   out_2498906151128215788[7] = 0;
   out_2498906151128215788[8] = 0;
}
void h_29(double *state, double *unused, double *out_6510677364401263223) {
   out_6510677364401263223[0] = state[1];
}
void H_29(double *state, double *unused, double *out_186088504986601307) {
   out_186088504986601307[0] = 0;
   out_186088504986601307[1] = 1;
   out_186088504986601307[2] = 0;
   out_186088504986601307[3] = 0;
   out_186088504986601307[4] = 0;
   out_186088504986601307[5] = 0;
   out_186088504986601307[6] = 0;
   out_186088504986601307[7] = 0;
   out_186088504986601307[8] = 0;
}
void h_28(double *state, double *unused, double *out_4842705445598448391) {
   out_4842705445598448391[0] = state[0];
}
void H_28(double *state, double *unused, double *out_2248638606432440570) {
   out_2248638606432440570[0] = 1;
   out_2248638606432440570[1] = 0;
   out_2248638606432440570[2] = 0;
   out_2248638606432440570[3] = 0;
   out_2248638606432440570[4] = 0;
   out_2248638606432440570[5] = 0;
   out_2248638606432440570[6] = 0;
   out_2248638606432440570[7] = 0;
   out_2248638606432440570[8] = 0;
}
void h_31(double *state, double *unused, double *out_902047987059200907) {
   out_902047987059200907[0] = state[8];
}
void H_31(double *state, double *unused, double *out_164157930307590379) {
   out_164157930307590379[0] = 0;
   out_164157930307590379[1] = 0;
   out_164157930307590379[2] = 0;
   out_164157930307590379[3] = 0;
   out_164157930307590379[4] = 0;
   out_164157930307590379[5] = 0;
   out_164157930307590379[6] = 0;
   out_164157930307590379[7] = 0;
   out_164157930307590379[8] = 1;
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

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_5924875962979878419) {
  err_fun(nom_x, delta_x, out_5924875962979878419);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_3730797253670217675) {
  inv_err_fun(nom_x, true_x, out_3730797253670217675);
}
void car_H_mod_fun(double *state, double *out_8998384333727686430) {
  H_mod_fun(state, out_8998384333727686430);
}
void car_f_fun(double *state, double dt, double *out_5904937932592495420) {
  f_fun(state,  dt, out_5904937932592495420);
}
void car_F_fun(double *state, double dt, double *out_874526194948226047) {
  F_fun(state,  dt, out_874526194948226047);
}
void car_h_25(double *state, double *unused, double *out_5000351285338572771) {
  h_25(state, unused, out_5000351285338572771);
}
void car_H_25(double *state, double *unused, double *out_194803892184550807) {
  H_25(state, unused, out_194803892184550807);
}
void car_h_24(double *state, double *unused, double *out_651987176130886021) {
  h_24(state, unused, out_651987176130886021);
}
void car_H_24(double *state, double *unused, double *out_5015125396840539070) {
  H_24(state, unused, out_5015125396840539070);
}
void car_h_30(double *state, double *unused, double *out_8755349036136976630) {
  h_30(state, unused, out_8755349036136976630);
}
void car_H_30(double *state, double *unused, double *out_324142839327790877) {
  H_30(state, unused, out_324142839327790877);
}
void car_h_26(double *state, double *unused, double *out_4689721769911729341) {
  h_26(state, unused, out_4689721769911729341);
}
void car_H_26(double *state, double *unused, double *out_3936307211058607031) {
  H_26(state, unused, out_3936307211058607031);
}
void car_h_27(double *state, double *unused, double *out_7961308757030146390) {
  h_27(state, unused, out_7961308757030146390);
}
void car_H_27(double *state, double *unused, double *out_2498906151128215788) {
  H_27(state, unused, out_2498906151128215788);
}
void car_h_29(double *state, double *unused, double *out_6510677364401263223) {
  h_29(state, unused, out_6510677364401263223);
}
void car_H_29(double *state, double *unused, double *out_186088504986601307) {
  H_29(state, unused, out_186088504986601307);
}
void car_h_28(double *state, double *unused, double *out_4842705445598448391) {
  h_28(state, unused, out_4842705445598448391);
}
void car_H_28(double *state, double *unused, double *out_2248638606432440570) {
  H_28(state, unused, out_2248638606432440570);
}
void car_h_31(double *state, double *unused, double *out_902047987059200907) {
  h_31(state, unused, out_902047987059200907);
}
void car_H_31(double *state, double *unused, double *out_164157930307590379) {
  H_31(state, unused, out_164157930307590379);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_lib_init(car)
