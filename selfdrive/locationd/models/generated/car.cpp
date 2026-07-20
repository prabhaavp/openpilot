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
void err_fun(double *nom_x, double *delta_x, double *out_8452016858633359142) {
   out_8452016858633359142[0] = delta_x[0] + nom_x[0];
   out_8452016858633359142[1] = delta_x[1] + nom_x[1];
   out_8452016858633359142[2] = delta_x[2] + nom_x[2];
   out_8452016858633359142[3] = delta_x[3] + nom_x[3];
   out_8452016858633359142[4] = delta_x[4] + nom_x[4];
   out_8452016858633359142[5] = delta_x[5] + nom_x[5];
   out_8452016858633359142[6] = delta_x[6] + nom_x[6];
   out_8452016858633359142[7] = delta_x[7] + nom_x[7];
   out_8452016858633359142[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_6091261500618765150) {
   out_6091261500618765150[0] = -nom_x[0] + true_x[0];
   out_6091261500618765150[1] = -nom_x[1] + true_x[1];
   out_6091261500618765150[2] = -nom_x[2] + true_x[2];
   out_6091261500618765150[3] = -nom_x[3] + true_x[3];
   out_6091261500618765150[4] = -nom_x[4] + true_x[4];
   out_6091261500618765150[5] = -nom_x[5] + true_x[5];
   out_6091261500618765150[6] = -nom_x[6] + true_x[6];
   out_6091261500618765150[7] = -nom_x[7] + true_x[7];
   out_6091261500618765150[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_38657891020581010) {
   out_38657891020581010[0] = 1.0;
   out_38657891020581010[1] = 0.0;
   out_38657891020581010[2] = 0.0;
   out_38657891020581010[3] = 0.0;
   out_38657891020581010[4] = 0.0;
   out_38657891020581010[5] = 0.0;
   out_38657891020581010[6] = 0.0;
   out_38657891020581010[7] = 0.0;
   out_38657891020581010[8] = 0.0;
   out_38657891020581010[9] = 0.0;
   out_38657891020581010[10] = 1.0;
   out_38657891020581010[11] = 0.0;
   out_38657891020581010[12] = 0.0;
   out_38657891020581010[13] = 0.0;
   out_38657891020581010[14] = 0.0;
   out_38657891020581010[15] = 0.0;
   out_38657891020581010[16] = 0.0;
   out_38657891020581010[17] = 0.0;
   out_38657891020581010[18] = 0.0;
   out_38657891020581010[19] = 0.0;
   out_38657891020581010[20] = 1.0;
   out_38657891020581010[21] = 0.0;
   out_38657891020581010[22] = 0.0;
   out_38657891020581010[23] = 0.0;
   out_38657891020581010[24] = 0.0;
   out_38657891020581010[25] = 0.0;
   out_38657891020581010[26] = 0.0;
   out_38657891020581010[27] = 0.0;
   out_38657891020581010[28] = 0.0;
   out_38657891020581010[29] = 0.0;
   out_38657891020581010[30] = 1.0;
   out_38657891020581010[31] = 0.0;
   out_38657891020581010[32] = 0.0;
   out_38657891020581010[33] = 0.0;
   out_38657891020581010[34] = 0.0;
   out_38657891020581010[35] = 0.0;
   out_38657891020581010[36] = 0.0;
   out_38657891020581010[37] = 0.0;
   out_38657891020581010[38] = 0.0;
   out_38657891020581010[39] = 0.0;
   out_38657891020581010[40] = 1.0;
   out_38657891020581010[41] = 0.0;
   out_38657891020581010[42] = 0.0;
   out_38657891020581010[43] = 0.0;
   out_38657891020581010[44] = 0.0;
   out_38657891020581010[45] = 0.0;
   out_38657891020581010[46] = 0.0;
   out_38657891020581010[47] = 0.0;
   out_38657891020581010[48] = 0.0;
   out_38657891020581010[49] = 0.0;
   out_38657891020581010[50] = 1.0;
   out_38657891020581010[51] = 0.0;
   out_38657891020581010[52] = 0.0;
   out_38657891020581010[53] = 0.0;
   out_38657891020581010[54] = 0.0;
   out_38657891020581010[55] = 0.0;
   out_38657891020581010[56] = 0.0;
   out_38657891020581010[57] = 0.0;
   out_38657891020581010[58] = 0.0;
   out_38657891020581010[59] = 0.0;
   out_38657891020581010[60] = 1.0;
   out_38657891020581010[61] = 0.0;
   out_38657891020581010[62] = 0.0;
   out_38657891020581010[63] = 0.0;
   out_38657891020581010[64] = 0.0;
   out_38657891020581010[65] = 0.0;
   out_38657891020581010[66] = 0.0;
   out_38657891020581010[67] = 0.0;
   out_38657891020581010[68] = 0.0;
   out_38657891020581010[69] = 0.0;
   out_38657891020581010[70] = 1.0;
   out_38657891020581010[71] = 0.0;
   out_38657891020581010[72] = 0.0;
   out_38657891020581010[73] = 0.0;
   out_38657891020581010[74] = 0.0;
   out_38657891020581010[75] = 0.0;
   out_38657891020581010[76] = 0.0;
   out_38657891020581010[77] = 0.0;
   out_38657891020581010[78] = 0.0;
   out_38657891020581010[79] = 0.0;
   out_38657891020581010[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_558885094687163216) {
   out_558885094687163216[0] = state[0];
   out_558885094687163216[1] = state[1];
   out_558885094687163216[2] = state[2];
   out_558885094687163216[3] = state[3];
   out_558885094687163216[4] = state[4];
   out_558885094687163216[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_558885094687163216[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_558885094687163216[7] = state[7];
   out_558885094687163216[8] = state[8];
}
void F_fun(double *state, double dt, double *out_2583716049044893862) {
   out_2583716049044893862[0] = 1;
   out_2583716049044893862[1] = 0;
   out_2583716049044893862[2] = 0;
   out_2583716049044893862[3] = 0;
   out_2583716049044893862[4] = 0;
   out_2583716049044893862[5] = 0;
   out_2583716049044893862[6] = 0;
   out_2583716049044893862[7] = 0;
   out_2583716049044893862[8] = 0;
   out_2583716049044893862[9] = 0;
   out_2583716049044893862[10] = 1;
   out_2583716049044893862[11] = 0;
   out_2583716049044893862[12] = 0;
   out_2583716049044893862[13] = 0;
   out_2583716049044893862[14] = 0;
   out_2583716049044893862[15] = 0;
   out_2583716049044893862[16] = 0;
   out_2583716049044893862[17] = 0;
   out_2583716049044893862[18] = 0;
   out_2583716049044893862[19] = 0;
   out_2583716049044893862[20] = 1;
   out_2583716049044893862[21] = 0;
   out_2583716049044893862[22] = 0;
   out_2583716049044893862[23] = 0;
   out_2583716049044893862[24] = 0;
   out_2583716049044893862[25] = 0;
   out_2583716049044893862[26] = 0;
   out_2583716049044893862[27] = 0;
   out_2583716049044893862[28] = 0;
   out_2583716049044893862[29] = 0;
   out_2583716049044893862[30] = 1;
   out_2583716049044893862[31] = 0;
   out_2583716049044893862[32] = 0;
   out_2583716049044893862[33] = 0;
   out_2583716049044893862[34] = 0;
   out_2583716049044893862[35] = 0;
   out_2583716049044893862[36] = 0;
   out_2583716049044893862[37] = 0;
   out_2583716049044893862[38] = 0;
   out_2583716049044893862[39] = 0;
   out_2583716049044893862[40] = 1;
   out_2583716049044893862[41] = 0;
   out_2583716049044893862[42] = 0;
   out_2583716049044893862[43] = 0;
   out_2583716049044893862[44] = 0;
   out_2583716049044893862[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_2583716049044893862[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_2583716049044893862[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_2583716049044893862[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_2583716049044893862[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_2583716049044893862[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_2583716049044893862[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_2583716049044893862[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_2583716049044893862[53] = -9.8100000000000005*dt;
   out_2583716049044893862[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_2583716049044893862[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_2583716049044893862[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2583716049044893862[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2583716049044893862[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_2583716049044893862[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_2583716049044893862[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_2583716049044893862[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2583716049044893862[62] = 0;
   out_2583716049044893862[63] = 0;
   out_2583716049044893862[64] = 0;
   out_2583716049044893862[65] = 0;
   out_2583716049044893862[66] = 0;
   out_2583716049044893862[67] = 0;
   out_2583716049044893862[68] = 0;
   out_2583716049044893862[69] = 0;
   out_2583716049044893862[70] = 1;
   out_2583716049044893862[71] = 0;
   out_2583716049044893862[72] = 0;
   out_2583716049044893862[73] = 0;
   out_2583716049044893862[74] = 0;
   out_2583716049044893862[75] = 0;
   out_2583716049044893862[76] = 0;
   out_2583716049044893862[77] = 0;
   out_2583716049044893862[78] = 0;
   out_2583716049044893862[79] = 0;
   out_2583716049044893862[80] = 1;
}
void h_25(double *state, double *unused, double *out_6863161802963304328) {
   out_6863161802963304328[0] = state[6];
}
void H_25(double *state, double *unused, double *out_2872687221825540538) {
   out_2872687221825540538[0] = 0;
   out_2872687221825540538[1] = 0;
   out_2872687221825540538[2] = 0;
   out_2872687221825540538[3] = 0;
   out_2872687221825540538[4] = 0;
   out_2872687221825540538[5] = 0;
   out_2872687221825540538[6] = 1;
   out_2872687221825540538[7] = 0;
   out_2872687221825540538[8] = 0;
}
void h_24(double *state, double *unused, double *out_1110917950379322687) {
   out_1110917950379322687[0] = state[4];
   out_1110917950379322687[1] = state[5];
}
void H_24(double *state, double *unused, double *out_4545704411851386355) {
   out_4545704411851386355[0] = 0;
   out_4545704411851386355[1] = 0;
   out_4545704411851386355[2] = 0;
   out_4545704411851386355[3] = 0;
   out_4545704411851386355[4] = 1;
   out_4545704411851386355[5] = 0;
   out_4545704411851386355[6] = 0;
   out_4545704411851386355[7] = 0;
   out_4545704411851386355[8] = 0;
   out_4545704411851386355[9] = 0;
   out_4545704411851386355[10] = 0;
   out_4545704411851386355[11] = 0;
   out_4545704411851386355[12] = 0;
   out_4545704411851386355[13] = 0;
   out_4545704411851386355[14] = 1;
   out_4545704411851386355[15] = 0;
   out_4545704411851386355[16] = 0;
   out_4545704411851386355[17] = 0;
}
void h_30(double *state, double *unused, double *out_3820317013925574847) {
   out_3820317013925574847[0] = state[4];
}
void H_30(double *state, double *unused, double *out_7400383551953148736) {
   out_7400383551953148736[0] = 0;
   out_7400383551953148736[1] = 0;
   out_7400383551953148736[2] = 0;
   out_7400383551953148736[3] = 0;
   out_7400383551953148736[4] = 1;
   out_7400383551953148736[5] = 0;
   out_7400383551953148736[6] = 0;
   out_7400383551953148736[7] = 0;
   out_7400383551953148736[8] = 0;
}
void h_26(double *state, double *unused, double *out_3983562419870100788) {
   out_3983562419870100788[0] = state[7];
}
void H_26(double *state, double *unused, double *out_6614190540699596762) {
   out_6614190540699596762[0] = 0;
   out_6614190540699596762[1] = 0;
   out_6614190540699596762[2] = 0;
   out_6614190540699596762[3] = 0;
   out_6614190540699596762[4] = 0;
   out_6614190540699596762[5] = 0;
   out_6614190540699596762[6] = 0;
   out_6614190540699596762[7] = 1;
   out_6614190540699596762[8] = 0;
}
void h_27(double *state, double *unused, double *out_70918829331572549) {
   out_70918829331572549[0] = state[3];
}
void H_27(double *state, double *unused, double *out_5176789480769205519) {
   out_5176789480769205519[0] = 0;
   out_5176789480769205519[1] = 0;
   out_5176789480769205519[2] = 0;
   out_5176789480769205519[3] = 1;
   out_5176789480769205519[4] = 0;
   out_5176789480769205519[5] = 0;
   out_5176789480769205519[6] = 0;
   out_5176789480769205519[7] = 0;
   out_5176789480769205519[8] = 0;
}
void h_29(double *state, double *unused, double *out_4423632898602052364) {
   out_4423632898602052364[0] = state[1];
}
void H_29(double *state, double *unused, double *out_6890152207638756552) {
   out_6890152207638756552[0] = 0;
   out_6890152207638756552[1] = 1;
   out_6890152207638756552[2] = 0;
   out_6890152207638756552[3] = 0;
   out_6890152207638756552[4] = 0;
   out_6890152207638756552[5] = 0;
   out_6890152207638756552[6] = 0;
   out_6890152207638756552[7] = 0;
   out_6890152207638756552[8] = 0;
}
void h_28(double *state, double *unused, double *out_3204928983415884040) {
   out_3204928983415884040[0] = state[0];
}
void H_28(double *state, double *unused, double *out_4926521936073430301) {
   out_4926521936073430301[0] = 1;
   out_4926521936073430301[1] = 0;
   out_4926521936073430301[2] = 0;
   out_4926521936073430301[3] = 0;
   out_4926521936073430301[4] = 0;
   out_4926521936073430301[5] = 0;
   out_4926521936073430301[6] = 0;
   out_4926521936073430301[7] = 0;
   out_4926521936073430301[8] = 0;
}
void h_31(double *state, double *unused, double *out_597396961285583997) {
   out_597396961285583997[0] = state[8];
}
void H_31(double *state, double *unused, double *out_2842041259948580110) {
   out_2842041259948580110[0] = 0;
   out_2842041259948580110[1] = 0;
   out_2842041259948580110[2] = 0;
   out_2842041259948580110[3] = 0;
   out_2842041259948580110[4] = 0;
   out_2842041259948580110[5] = 0;
   out_2842041259948580110[6] = 0;
   out_2842041259948580110[7] = 0;
   out_2842041259948580110[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_8452016858633359142) {
  err_fun(nom_x, delta_x, out_8452016858633359142);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_6091261500618765150) {
  inv_err_fun(nom_x, true_x, out_6091261500618765150);
}
void car_H_mod_fun(double *state, double *out_38657891020581010) {
  H_mod_fun(state, out_38657891020581010);
}
void car_f_fun(double *state, double dt, double *out_558885094687163216) {
  f_fun(state,  dt, out_558885094687163216);
}
void car_F_fun(double *state, double dt, double *out_2583716049044893862) {
  F_fun(state,  dt, out_2583716049044893862);
}
void car_h_25(double *state, double *unused, double *out_6863161802963304328) {
  h_25(state, unused, out_6863161802963304328);
}
void car_H_25(double *state, double *unused, double *out_2872687221825540538) {
  H_25(state, unused, out_2872687221825540538);
}
void car_h_24(double *state, double *unused, double *out_1110917950379322687) {
  h_24(state, unused, out_1110917950379322687);
}
void car_H_24(double *state, double *unused, double *out_4545704411851386355) {
  H_24(state, unused, out_4545704411851386355);
}
void car_h_30(double *state, double *unused, double *out_3820317013925574847) {
  h_30(state, unused, out_3820317013925574847);
}
void car_H_30(double *state, double *unused, double *out_7400383551953148736) {
  H_30(state, unused, out_7400383551953148736);
}
void car_h_26(double *state, double *unused, double *out_3983562419870100788) {
  h_26(state, unused, out_3983562419870100788);
}
void car_H_26(double *state, double *unused, double *out_6614190540699596762) {
  H_26(state, unused, out_6614190540699596762);
}
void car_h_27(double *state, double *unused, double *out_70918829331572549) {
  h_27(state, unused, out_70918829331572549);
}
void car_H_27(double *state, double *unused, double *out_5176789480769205519) {
  H_27(state, unused, out_5176789480769205519);
}
void car_h_29(double *state, double *unused, double *out_4423632898602052364) {
  h_29(state, unused, out_4423632898602052364);
}
void car_H_29(double *state, double *unused, double *out_6890152207638756552) {
  H_29(state, unused, out_6890152207638756552);
}
void car_h_28(double *state, double *unused, double *out_3204928983415884040) {
  h_28(state, unused, out_3204928983415884040);
}
void car_H_28(double *state, double *unused, double *out_4926521936073430301) {
  H_28(state, unused, out_4926521936073430301);
}
void car_h_31(double *state, double *unused, double *out_597396961285583997) {
  h_31(state, unused, out_597396961285583997);
}
void car_H_31(double *state, double *unused, double *out_2842041259948580110) {
  H_31(state, unused, out_2842041259948580110);
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
