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
void err_fun(double *nom_x, double *delta_x, double *out_5731293444659899197) {
   out_5731293444659899197[0] = delta_x[0] + nom_x[0];
   out_5731293444659899197[1] = delta_x[1] + nom_x[1];
   out_5731293444659899197[2] = delta_x[2] + nom_x[2];
   out_5731293444659899197[3] = delta_x[3] + nom_x[3];
   out_5731293444659899197[4] = delta_x[4] + nom_x[4];
   out_5731293444659899197[5] = delta_x[5] + nom_x[5];
   out_5731293444659899197[6] = delta_x[6] + nom_x[6];
   out_5731293444659899197[7] = delta_x[7] + nom_x[7];
   out_5731293444659899197[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_658884802146674275) {
   out_658884802146674275[0] = -nom_x[0] + true_x[0];
   out_658884802146674275[1] = -nom_x[1] + true_x[1];
   out_658884802146674275[2] = -nom_x[2] + true_x[2];
   out_658884802146674275[3] = -nom_x[3] + true_x[3];
   out_658884802146674275[4] = -nom_x[4] + true_x[4];
   out_658884802146674275[5] = -nom_x[5] + true_x[5];
   out_658884802146674275[6] = -nom_x[6] + true_x[6];
   out_658884802146674275[7] = -nom_x[7] + true_x[7];
   out_658884802146674275[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_2364024515690160793) {
   out_2364024515690160793[0] = 1.0;
   out_2364024515690160793[1] = 0.0;
   out_2364024515690160793[2] = 0.0;
   out_2364024515690160793[3] = 0.0;
   out_2364024515690160793[4] = 0.0;
   out_2364024515690160793[5] = 0.0;
   out_2364024515690160793[6] = 0.0;
   out_2364024515690160793[7] = 0.0;
   out_2364024515690160793[8] = 0.0;
   out_2364024515690160793[9] = 0.0;
   out_2364024515690160793[10] = 1.0;
   out_2364024515690160793[11] = 0.0;
   out_2364024515690160793[12] = 0.0;
   out_2364024515690160793[13] = 0.0;
   out_2364024515690160793[14] = 0.0;
   out_2364024515690160793[15] = 0.0;
   out_2364024515690160793[16] = 0.0;
   out_2364024515690160793[17] = 0.0;
   out_2364024515690160793[18] = 0.0;
   out_2364024515690160793[19] = 0.0;
   out_2364024515690160793[20] = 1.0;
   out_2364024515690160793[21] = 0.0;
   out_2364024515690160793[22] = 0.0;
   out_2364024515690160793[23] = 0.0;
   out_2364024515690160793[24] = 0.0;
   out_2364024515690160793[25] = 0.0;
   out_2364024515690160793[26] = 0.0;
   out_2364024515690160793[27] = 0.0;
   out_2364024515690160793[28] = 0.0;
   out_2364024515690160793[29] = 0.0;
   out_2364024515690160793[30] = 1.0;
   out_2364024515690160793[31] = 0.0;
   out_2364024515690160793[32] = 0.0;
   out_2364024515690160793[33] = 0.0;
   out_2364024515690160793[34] = 0.0;
   out_2364024515690160793[35] = 0.0;
   out_2364024515690160793[36] = 0.0;
   out_2364024515690160793[37] = 0.0;
   out_2364024515690160793[38] = 0.0;
   out_2364024515690160793[39] = 0.0;
   out_2364024515690160793[40] = 1.0;
   out_2364024515690160793[41] = 0.0;
   out_2364024515690160793[42] = 0.0;
   out_2364024515690160793[43] = 0.0;
   out_2364024515690160793[44] = 0.0;
   out_2364024515690160793[45] = 0.0;
   out_2364024515690160793[46] = 0.0;
   out_2364024515690160793[47] = 0.0;
   out_2364024515690160793[48] = 0.0;
   out_2364024515690160793[49] = 0.0;
   out_2364024515690160793[50] = 1.0;
   out_2364024515690160793[51] = 0.0;
   out_2364024515690160793[52] = 0.0;
   out_2364024515690160793[53] = 0.0;
   out_2364024515690160793[54] = 0.0;
   out_2364024515690160793[55] = 0.0;
   out_2364024515690160793[56] = 0.0;
   out_2364024515690160793[57] = 0.0;
   out_2364024515690160793[58] = 0.0;
   out_2364024515690160793[59] = 0.0;
   out_2364024515690160793[60] = 1.0;
   out_2364024515690160793[61] = 0.0;
   out_2364024515690160793[62] = 0.0;
   out_2364024515690160793[63] = 0.0;
   out_2364024515690160793[64] = 0.0;
   out_2364024515690160793[65] = 0.0;
   out_2364024515690160793[66] = 0.0;
   out_2364024515690160793[67] = 0.0;
   out_2364024515690160793[68] = 0.0;
   out_2364024515690160793[69] = 0.0;
   out_2364024515690160793[70] = 1.0;
   out_2364024515690160793[71] = 0.0;
   out_2364024515690160793[72] = 0.0;
   out_2364024515690160793[73] = 0.0;
   out_2364024515690160793[74] = 0.0;
   out_2364024515690160793[75] = 0.0;
   out_2364024515690160793[76] = 0.0;
   out_2364024515690160793[77] = 0.0;
   out_2364024515690160793[78] = 0.0;
   out_2364024515690160793[79] = 0.0;
   out_2364024515690160793[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_7312135134474090361) {
   out_7312135134474090361[0] = state[0];
   out_7312135134474090361[1] = state[1];
   out_7312135134474090361[2] = state[2];
   out_7312135134474090361[3] = state[3];
   out_7312135134474090361[4] = state[4];
   out_7312135134474090361[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_7312135134474090361[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_7312135134474090361[7] = state[7];
   out_7312135134474090361[8] = state[8];
}
void F_fun(double *state, double dt, double *out_1426178926713797570) {
   out_1426178926713797570[0] = 1;
   out_1426178926713797570[1] = 0;
   out_1426178926713797570[2] = 0;
   out_1426178926713797570[3] = 0;
   out_1426178926713797570[4] = 0;
   out_1426178926713797570[5] = 0;
   out_1426178926713797570[6] = 0;
   out_1426178926713797570[7] = 0;
   out_1426178926713797570[8] = 0;
   out_1426178926713797570[9] = 0;
   out_1426178926713797570[10] = 1;
   out_1426178926713797570[11] = 0;
   out_1426178926713797570[12] = 0;
   out_1426178926713797570[13] = 0;
   out_1426178926713797570[14] = 0;
   out_1426178926713797570[15] = 0;
   out_1426178926713797570[16] = 0;
   out_1426178926713797570[17] = 0;
   out_1426178926713797570[18] = 0;
   out_1426178926713797570[19] = 0;
   out_1426178926713797570[20] = 1;
   out_1426178926713797570[21] = 0;
   out_1426178926713797570[22] = 0;
   out_1426178926713797570[23] = 0;
   out_1426178926713797570[24] = 0;
   out_1426178926713797570[25] = 0;
   out_1426178926713797570[26] = 0;
   out_1426178926713797570[27] = 0;
   out_1426178926713797570[28] = 0;
   out_1426178926713797570[29] = 0;
   out_1426178926713797570[30] = 1;
   out_1426178926713797570[31] = 0;
   out_1426178926713797570[32] = 0;
   out_1426178926713797570[33] = 0;
   out_1426178926713797570[34] = 0;
   out_1426178926713797570[35] = 0;
   out_1426178926713797570[36] = 0;
   out_1426178926713797570[37] = 0;
   out_1426178926713797570[38] = 0;
   out_1426178926713797570[39] = 0;
   out_1426178926713797570[40] = 1;
   out_1426178926713797570[41] = 0;
   out_1426178926713797570[42] = 0;
   out_1426178926713797570[43] = 0;
   out_1426178926713797570[44] = 0;
   out_1426178926713797570[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_1426178926713797570[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_1426178926713797570[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_1426178926713797570[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_1426178926713797570[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_1426178926713797570[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_1426178926713797570[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_1426178926713797570[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_1426178926713797570[53] = -9.8100000000000005*dt;
   out_1426178926713797570[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_1426178926713797570[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_1426178926713797570[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1426178926713797570[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1426178926713797570[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_1426178926713797570[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_1426178926713797570[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_1426178926713797570[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1426178926713797570[62] = 0;
   out_1426178926713797570[63] = 0;
   out_1426178926713797570[64] = 0;
   out_1426178926713797570[65] = 0;
   out_1426178926713797570[66] = 0;
   out_1426178926713797570[67] = 0;
   out_1426178926713797570[68] = 0;
   out_1426178926713797570[69] = 0;
   out_1426178926713797570[70] = 1;
   out_1426178926713797570[71] = 0;
   out_1426178926713797570[72] = 0;
   out_1426178926713797570[73] = 0;
   out_1426178926713797570[74] = 0;
   out_1426178926713797570[75] = 0;
   out_1426178926713797570[76] = 0;
   out_1426178926713797570[77] = 0;
   out_1426178926713797570[78] = 0;
   out_1426178926713797570[79] = 0;
   out_1426178926713797570[80] = 1;
}
void h_25(double *state, double *unused, double *out_4322344263373120118) {
   out_4322344263373120118[0] = state[6];
}
void H_25(double *state, double *unused, double *out_6889531332107153586) {
   out_6889531332107153586[0] = 0;
   out_6889531332107153586[1] = 0;
   out_6889531332107153586[2] = 0;
   out_6889531332107153586[3] = 0;
   out_6889531332107153586[4] = 0;
   out_6889531332107153586[5] = 0;
   out_6889531332107153586[6] = 1;
   out_6889531332107153586[7] = 0;
   out_6889531332107153586[8] = 0;
}
void h_24(double *state, double *unused, double *out_416399681732138117) {
   out_416399681732138117[0] = state[4];
   out_416399681732138117[1] = state[5];
}
void H_24(double *state, double *unused, double *out_2020716467079446734) {
   out_2020716467079446734[0] = 0;
   out_2020716467079446734[1] = 0;
   out_2020716467079446734[2] = 0;
   out_2020716467079446734[3] = 0;
   out_2020716467079446734[4] = 1;
   out_2020716467079446734[5] = 0;
   out_2020716467079446734[6] = 0;
   out_2020716467079446734[7] = 0;
   out_2020716467079446734[8] = 0;
   out_2020716467079446734[9] = 0;
   out_2020716467079446734[10] = 0;
   out_2020716467079446734[11] = 0;
   out_2020716467079446734[12] = 0;
   out_2020716467079446734[13] = 0;
   out_2020716467079446734[14] = 1;
   out_2020716467079446734[15] = 0;
   out_2020716467079446734[16] = 0;
   out_2020716467079446734[17] = 0;
}
void h_30(double *state, double *unused, double *out_4597538325657626007) {
   out_4597538325657626007[0] = state[4];
}
void H_30(double *state, double *unused, double *out_2361835001979545388) {
   out_2361835001979545388[0] = 0;
   out_2361835001979545388[1] = 0;
   out_2361835001979545388[2] = 0;
   out_2361835001979545388[3] = 0;
   out_2361835001979545388[4] = 1;
   out_2361835001979545388[5] = 0;
   out_2361835001979545388[6] = 0;
   out_2361835001979545388[7] = 0;
   out_2361835001979545388[8] = 0;
}
void h_26(double *state, double *unused, double *out_7201943646466323658) {
   out_7201943646466323658[0] = state[7];
}
void H_26(double *state, double *unused, double *out_3148028013233097362) {
   out_3148028013233097362[0] = 0;
   out_3148028013233097362[1] = 0;
   out_3148028013233097362[2] = 0;
   out_3148028013233097362[3] = 0;
   out_3148028013233097362[4] = 0;
   out_3148028013233097362[5] = 0;
   out_3148028013233097362[6] = 0;
   out_3148028013233097362[7] = 1;
   out_3148028013233097362[8] = 0;
}
void h_27(double *state, double *unused, double *out_5149894096224076937) {
   out_5149894096224076937[0] = state[3];
}
void H_27(double *state, double *unused, double *out_4585429073163488605) {
   out_4585429073163488605[0] = 0;
   out_4585429073163488605[1] = 0;
   out_4585429073163488605[2] = 0;
   out_4585429073163488605[3] = 1;
   out_4585429073163488605[4] = 0;
   out_4585429073163488605[5] = 0;
   out_4585429073163488605[6] = 0;
   out_4585429073163488605[7] = 0;
   out_4585429073163488605[8] = 0;
}
void h_29(double *state, double *unused, double *out_5200854210334103524) {
   out_5200854210334103524[0] = state[1];
}
void H_29(double *state, double *unused, double *out_2872066346293937572) {
   out_2872066346293937572[0] = 0;
   out_2872066346293937572[1] = 1;
   out_2872066346293937572[2] = 0;
   out_2872066346293937572[3] = 0;
   out_2872066346293937572[4] = 0;
   out_2872066346293937572[5] = 0;
   out_2872066346293937572[6] = 0;
   out_2872066346293937572[7] = 0;
   out_2872066346293937572[8] = 0;
}
void h_28(double *state, double *unused, double *out_7418564489926759591) {
   out_7418564489926759591[0] = state[0];
}
void H_28(double *state, double *unused, double *out_2210332670775593002) {
   out_2210332670775593002[0] = 1;
   out_2210332670775593002[1] = 0;
   out_2210332670775593002[2] = 0;
   out_2210332670775593002[3] = 0;
   out_2210332670775593002[4] = 0;
   out_2210332670775593002[5] = 0;
   out_2210332670775593002[6] = 0;
   out_2210332670775593002[7] = 0;
   out_2210332670775593002[8] = 0;
}
void h_31(double *state, double *unused, double *out_684894735119097768) {
   out_684894735119097768[0] = state[8];
}
void H_31(double *state, double *unused, double *out_6920177293984114014) {
   out_6920177293984114014[0] = 0;
   out_6920177293984114014[1] = 0;
   out_6920177293984114014[2] = 0;
   out_6920177293984114014[3] = 0;
   out_6920177293984114014[4] = 0;
   out_6920177293984114014[5] = 0;
   out_6920177293984114014[6] = 0;
   out_6920177293984114014[7] = 0;
   out_6920177293984114014[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_5731293444659899197) {
  err_fun(nom_x, delta_x, out_5731293444659899197);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_658884802146674275) {
  inv_err_fun(nom_x, true_x, out_658884802146674275);
}
void car_H_mod_fun(double *state, double *out_2364024515690160793) {
  H_mod_fun(state, out_2364024515690160793);
}
void car_f_fun(double *state, double dt, double *out_7312135134474090361) {
  f_fun(state,  dt, out_7312135134474090361);
}
void car_F_fun(double *state, double dt, double *out_1426178926713797570) {
  F_fun(state,  dt, out_1426178926713797570);
}
void car_h_25(double *state, double *unused, double *out_4322344263373120118) {
  h_25(state, unused, out_4322344263373120118);
}
void car_H_25(double *state, double *unused, double *out_6889531332107153586) {
  H_25(state, unused, out_6889531332107153586);
}
void car_h_24(double *state, double *unused, double *out_416399681732138117) {
  h_24(state, unused, out_416399681732138117);
}
void car_H_24(double *state, double *unused, double *out_2020716467079446734) {
  H_24(state, unused, out_2020716467079446734);
}
void car_h_30(double *state, double *unused, double *out_4597538325657626007) {
  h_30(state, unused, out_4597538325657626007);
}
void car_H_30(double *state, double *unused, double *out_2361835001979545388) {
  H_30(state, unused, out_2361835001979545388);
}
void car_h_26(double *state, double *unused, double *out_7201943646466323658) {
  h_26(state, unused, out_7201943646466323658);
}
void car_H_26(double *state, double *unused, double *out_3148028013233097362) {
  H_26(state, unused, out_3148028013233097362);
}
void car_h_27(double *state, double *unused, double *out_5149894096224076937) {
  h_27(state, unused, out_5149894096224076937);
}
void car_H_27(double *state, double *unused, double *out_4585429073163488605) {
  H_27(state, unused, out_4585429073163488605);
}
void car_h_29(double *state, double *unused, double *out_5200854210334103524) {
  h_29(state, unused, out_5200854210334103524);
}
void car_H_29(double *state, double *unused, double *out_2872066346293937572) {
  H_29(state, unused, out_2872066346293937572);
}
void car_h_28(double *state, double *unused, double *out_7418564489926759591) {
  h_28(state, unused, out_7418564489926759591);
}
void car_H_28(double *state, double *unused, double *out_2210332670775593002) {
  H_28(state, unused, out_2210332670775593002);
}
void car_h_31(double *state, double *unused, double *out_684894735119097768) {
  h_31(state, unused, out_684894735119097768);
}
void car_H_31(double *state, double *unused, double *out_6920177293984114014) {
  H_31(state, unused, out_6920177293984114014);
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
