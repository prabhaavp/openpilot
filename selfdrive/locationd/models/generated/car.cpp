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
void err_fun(double *nom_x, double *delta_x, double *out_8219286415624560569) {
   out_8219286415624560569[0] = delta_x[0] + nom_x[0];
   out_8219286415624560569[1] = delta_x[1] + nom_x[1];
   out_8219286415624560569[2] = delta_x[2] + nom_x[2];
   out_8219286415624560569[3] = delta_x[3] + nom_x[3];
   out_8219286415624560569[4] = delta_x[4] + nom_x[4];
   out_8219286415624560569[5] = delta_x[5] + nom_x[5];
   out_8219286415624560569[6] = delta_x[6] + nom_x[6];
   out_8219286415624560569[7] = delta_x[7] + nom_x[7];
   out_8219286415624560569[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_6195640368181513549) {
   out_6195640368181513549[0] = -nom_x[0] + true_x[0];
   out_6195640368181513549[1] = -nom_x[1] + true_x[1];
   out_6195640368181513549[2] = -nom_x[2] + true_x[2];
   out_6195640368181513549[3] = -nom_x[3] + true_x[3];
   out_6195640368181513549[4] = -nom_x[4] + true_x[4];
   out_6195640368181513549[5] = -nom_x[5] + true_x[5];
   out_6195640368181513549[6] = -nom_x[6] + true_x[6];
   out_6195640368181513549[7] = -nom_x[7] + true_x[7];
   out_6195640368181513549[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_7959613188215390617) {
   out_7959613188215390617[0] = 1.0;
   out_7959613188215390617[1] = 0.0;
   out_7959613188215390617[2] = 0.0;
   out_7959613188215390617[3] = 0.0;
   out_7959613188215390617[4] = 0.0;
   out_7959613188215390617[5] = 0.0;
   out_7959613188215390617[6] = 0.0;
   out_7959613188215390617[7] = 0.0;
   out_7959613188215390617[8] = 0.0;
   out_7959613188215390617[9] = 0.0;
   out_7959613188215390617[10] = 1.0;
   out_7959613188215390617[11] = 0.0;
   out_7959613188215390617[12] = 0.0;
   out_7959613188215390617[13] = 0.0;
   out_7959613188215390617[14] = 0.0;
   out_7959613188215390617[15] = 0.0;
   out_7959613188215390617[16] = 0.0;
   out_7959613188215390617[17] = 0.0;
   out_7959613188215390617[18] = 0.0;
   out_7959613188215390617[19] = 0.0;
   out_7959613188215390617[20] = 1.0;
   out_7959613188215390617[21] = 0.0;
   out_7959613188215390617[22] = 0.0;
   out_7959613188215390617[23] = 0.0;
   out_7959613188215390617[24] = 0.0;
   out_7959613188215390617[25] = 0.0;
   out_7959613188215390617[26] = 0.0;
   out_7959613188215390617[27] = 0.0;
   out_7959613188215390617[28] = 0.0;
   out_7959613188215390617[29] = 0.0;
   out_7959613188215390617[30] = 1.0;
   out_7959613188215390617[31] = 0.0;
   out_7959613188215390617[32] = 0.0;
   out_7959613188215390617[33] = 0.0;
   out_7959613188215390617[34] = 0.0;
   out_7959613188215390617[35] = 0.0;
   out_7959613188215390617[36] = 0.0;
   out_7959613188215390617[37] = 0.0;
   out_7959613188215390617[38] = 0.0;
   out_7959613188215390617[39] = 0.0;
   out_7959613188215390617[40] = 1.0;
   out_7959613188215390617[41] = 0.0;
   out_7959613188215390617[42] = 0.0;
   out_7959613188215390617[43] = 0.0;
   out_7959613188215390617[44] = 0.0;
   out_7959613188215390617[45] = 0.0;
   out_7959613188215390617[46] = 0.0;
   out_7959613188215390617[47] = 0.0;
   out_7959613188215390617[48] = 0.0;
   out_7959613188215390617[49] = 0.0;
   out_7959613188215390617[50] = 1.0;
   out_7959613188215390617[51] = 0.0;
   out_7959613188215390617[52] = 0.0;
   out_7959613188215390617[53] = 0.0;
   out_7959613188215390617[54] = 0.0;
   out_7959613188215390617[55] = 0.0;
   out_7959613188215390617[56] = 0.0;
   out_7959613188215390617[57] = 0.0;
   out_7959613188215390617[58] = 0.0;
   out_7959613188215390617[59] = 0.0;
   out_7959613188215390617[60] = 1.0;
   out_7959613188215390617[61] = 0.0;
   out_7959613188215390617[62] = 0.0;
   out_7959613188215390617[63] = 0.0;
   out_7959613188215390617[64] = 0.0;
   out_7959613188215390617[65] = 0.0;
   out_7959613188215390617[66] = 0.0;
   out_7959613188215390617[67] = 0.0;
   out_7959613188215390617[68] = 0.0;
   out_7959613188215390617[69] = 0.0;
   out_7959613188215390617[70] = 1.0;
   out_7959613188215390617[71] = 0.0;
   out_7959613188215390617[72] = 0.0;
   out_7959613188215390617[73] = 0.0;
   out_7959613188215390617[74] = 0.0;
   out_7959613188215390617[75] = 0.0;
   out_7959613188215390617[76] = 0.0;
   out_7959613188215390617[77] = 0.0;
   out_7959613188215390617[78] = 0.0;
   out_7959613188215390617[79] = 0.0;
   out_7959613188215390617[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_3072344104277436358) {
   out_3072344104277436358[0] = state[0];
   out_3072344104277436358[1] = state[1];
   out_3072344104277436358[2] = state[2];
   out_3072344104277436358[3] = state[3];
   out_3072344104277436358[4] = state[4];
   out_3072344104277436358[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_3072344104277436358[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_3072344104277436358[7] = state[7];
   out_3072344104277436358[8] = state[8];
}
void F_fun(double *state, double dt, double *out_4300892242094410296) {
   out_4300892242094410296[0] = 1;
   out_4300892242094410296[1] = 0;
   out_4300892242094410296[2] = 0;
   out_4300892242094410296[3] = 0;
   out_4300892242094410296[4] = 0;
   out_4300892242094410296[5] = 0;
   out_4300892242094410296[6] = 0;
   out_4300892242094410296[7] = 0;
   out_4300892242094410296[8] = 0;
   out_4300892242094410296[9] = 0;
   out_4300892242094410296[10] = 1;
   out_4300892242094410296[11] = 0;
   out_4300892242094410296[12] = 0;
   out_4300892242094410296[13] = 0;
   out_4300892242094410296[14] = 0;
   out_4300892242094410296[15] = 0;
   out_4300892242094410296[16] = 0;
   out_4300892242094410296[17] = 0;
   out_4300892242094410296[18] = 0;
   out_4300892242094410296[19] = 0;
   out_4300892242094410296[20] = 1;
   out_4300892242094410296[21] = 0;
   out_4300892242094410296[22] = 0;
   out_4300892242094410296[23] = 0;
   out_4300892242094410296[24] = 0;
   out_4300892242094410296[25] = 0;
   out_4300892242094410296[26] = 0;
   out_4300892242094410296[27] = 0;
   out_4300892242094410296[28] = 0;
   out_4300892242094410296[29] = 0;
   out_4300892242094410296[30] = 1;
   out_4300892242094410296[31] = 0;
   out_4300892242094410296[32] = 0;
   out_4300892242094410296[33] = 0;
   out_4300892242094410296[34] = 0;
   out_4300892242094410296[35] = 0;
   out_4300892242094410296[36] = 0;
   out_4300892242094410296[37] = 0;
   out_4300892242094410296[38] = 0;
   out_4300892242094410296[39] = 0;
   out_4300892242094410296[40] = 1;
   out_4300892242094410296[41] = 0;
   out_4300892242094410296[42] = 0;
   out_4300892242094410296[43] = 0;
   out_4300892242094410296[44] = 0;
   out_4300892242094410296[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_4300892242094410296[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_4300892242094410296[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4300892242094410296[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4300892242094410296[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_4300892242094410296[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_4300892242094410296[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_4300892242094410296[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_4300892242094410296[53] = -9.8100000000000005*dt;
   out_4300892242094410296[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_4300892242094410296[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_4300892242094410296[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4300892242094410296[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4300892242094410296[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_4300892242094410296[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_4300892242094410296[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_4300892242094410296[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4300892242094410296[62] = 0;
   out_4300892242094410296[63] = 0;
   out_4300892242094410296[64] = 0;
   out_4300892242094410296[65] = 0;
   out_4300892242094410296[66] = 0;
   out_4300892242094410296[67] = 0;
   out_4300892242094410296[68] = 0;
   out_4300892242094410296[69] = 0;
   out_4300892242094410296[70] = 1;
   out_4300892242094410296[71] = 0;
   out_4300892242094410296[72] = 0;
   out_4300892242094410296[73] = 0;
   out_4300892242094410296[74] = 0;
   out_4300892242094410296[75] = 0;
   out_4300892242094410296[76] = 0;
   out_4300892242094410296[77] = 0;
   out_4300892242094410296[78] = 0;
   out_4300892242094410296[79] = 0;
   out_4300892242094410296[80] = 1;
}
void h_25(double *state, double *unused, double *out_3314714972500610637) {
   out_3314714972500610637[0] = state[6];
}
void H_25(double *state, double *unused, double *out_4251221245486041585) {
   out_4251221245486041585[0] = 0;
   out_4251221245486041585[1] = 0;
   out_4251221245486041585[2] = 0;
   out_4251221245486041585[3] = 0;
   out_4251221245486041585[4] = 0;
   out_4251221245486041585[5] = 0;
   out_4251221245486041585[6] = 1;
   out_4251221245486041585[7] = 0;
   out_4251221245486041585[8] = 0;
}
void h_24(double *state, double *unused, double *out_1419049792901485991) {
   out_1419049792901485991[0] = state[4];
   out_1419049792901485991[1] = state[5];
}
void H_24(double *state, double *unused, double *out_3406224636702346186) {
   out_3406224636702346186[0] = 0;
   out_3406224636702346186[1] = 0;
   out_3406224636702346186[2] = 0;
   out_3406224636702346186[3] = 0;
   out_3406224636702346186[4] = 1;
   out_3406224636702346186[5] = 0;
   out_3406224636702346186[6] = 0;
   out_3406224636702346186[7] = 0;
   out_3406224636702346186[8] = 0;
   out_3406224636702346186[9] = 0;
   out_3406224636702346186[10] = 0;
   out_3406224636702346186[11] = 0;
   out_3406224636702346186[12] = 0;
   out_3406224636702346186[13] = 0;
   out_3406224636702346186[14] = 1;
   out_3406224636702346186[15] = 0;
   out_3406224636702346186[16] = 0;
   out_3406224636702346186[17] = 0;
}
void h_30(double *state, double *unused, double *out_4578224522383912722) {
   out_4578224522383912722[0] = state[4];
}
void H_30(double *state, double *unused, double *out_2665469096005575170) {
   out_2665469096005575170[0] = 0;
   out_2665469096005575170[1] = 0;
   out_2665469096005575170[2] = 0;
   out_2665469096005575170[3] = 0;
   out_2665469096005575170[4] = 1;
   out_2665469096005575170[5] = 0;
   out_2665469096005575170[6] = 0;
   out_2665469096005575170[7] = 0;
   out_2665469096005575170[8] = 0;
}
void h_26(double *state, double *unused, double *out_3634714669482250287) {
   out_3634714669482250287[0] = state[7];
}
void H_26(double *state, double *unused, double *out_946695275725240984) {
   out_946695275725240984[0] = 0;
   out_946695275725240984[1] = 0;
   out_946695275725240984[2] = 0;
   out_946695275725240984[3] = 0;
   out_946695275725240984[4] = 0;
   out_946695275725240984[5] = 0;
   out_946695275725240984[6] = 0;
   out_946695275725240984[7] = 1;
   out_946695275725240984[8] = 0;
}
void h_27(double *state, double *unused, double *out_4810151999826627565) {
   out_4810151999826627565[0] = state[3];
}
void H_27(double *state, double *unused, double *out_490705784205150259) {
   out_490705784205150259[0] = 0;
   out_490705784205150259[1] = 0;
   out_490705784205150259[2] = 0;
   out_490705784205150259[3] = 1;
   out_490705784205150259[4] = 0;
   out_490705784205150259[5] = 0;
   out_490705784205150259[6] = 0;
   out_490705784205150259[7] = 0;
   out_490705784205150259[8] = 0;
}
void h_29(double *state, double *unused, double *out_3071120650927421620) {
   out_3071120650927421620[0] = state[1];
}
void H_29(double *state, double *unused, double *out_3175700440319967354) {
   out_3175700440319967354[0] = 0;
   out_3175700440319967354[1] = 1;
   out_3175700440319967354[2] = 0;
   out_3175700440319967354[3] = 0;
   out_3175700440319967354[4] = 0;
   out_3175700440319967354[5] = 0;
   out_3175700440319967354[6] = 0;
   out_3175700440319967354[7] = 0;
   out_3175700440319967354[8] = 0;
}
void h_28(double *state, double *unused, double *out_5503836136891602899) {
   out_5503836136891602899[0] = state[0];
}
void H_28(double *state, double *unused, double *out_1906698576749563220) {
   out_1906698576749563220[0] = 1;
   out_1906698576749563220[1] = 0;
   out_1906698576749563220[2] = 0;
   out_1906698576749563220[3] = 0;
   out_1906698576749563220[4] = 0;
   out_1906698576749563220[5] = 0;
   out_1906698576749563220[6] = 0;
   out_1906698576749563220[7] = 0;
   out_1906698576749563220[8] = 0;
}
void h_31(double *state, double *unused, double *out_8361193874858590043) {
   out_8361193874858590043[0] = state[8];
}
void H_31(double *state, double *unused, double *out_4220575283609081157) {
   out_4220575283609081157[0] = 0;
   out_4220575283609081157[1] = 0;
   out_4220575283609081157[2] = 0;
   out_4220575283609081157[3] = 0;
   out_4220575283609081157[4] = 0;
   out_4220575283609081157[5] = 0;
   out_4220575283609081157[6] = 0;
   out_4220575283609081157[7] = 0;
   out_4220575283609081157[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_8219286415624560569) {
  err_fun(nom_x, delta_x, out_8219286415624560569);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_6195640368181513549) {
  inv_err_fun(nom_x, true_x, out_6195640368181513549);
}
void car_H_mod_fun(double *state, double *out_7959613188215390617) {
  H_mod_fun(state, out_7959613188215390617);
}
void car_f_fun(double *state, double dt, double *out_3072344104277436358) {
  f_fun(state,  dt, out_3072344104277436358);
}
void car_F_fun(double *state, double dt, double *out_4300892242094410296) {
  F_fun(state,  dt, out_4300892242094410296);
}
void car_h_25(double *state, double *unused, double *out_3314714972500610637) {
  h_25(state, unused, out_3314714972500610637);
}
void car_H_25(double *state, double *unused, double *out_4251221245486041585) {
  H_25(state, unused, out_4251221245486041585);
}
void car_h_24(double *state, double *unused, double *out_1419049792901485991) {
  h_24(state, unused, out_1419049792901485991);
}
void car_H_24(double *state, double *unused, double *out_3406224636702346186) {
  H_24(state, unused, out_3406224636702346186);
}
void car_h_30(double *state, double *unused, double *out_4578224522383912722) {
  h_30(state, unused, out_4578224522383912722);
}
void car_H_30(double *state, double *unused, double *out_2665469096005575170) {
  H_30(state, unused, out_2665469096005575170);
}
void car_h_26(double *state, double *unused, double *out_3634714669482250287) {
  h_26(state, unused, out_3634714669482250287);
}
void car_H_26(double *state, double *unused, double *out_946695275725240984) {
  H_26(state, unused, out_946695275725240984);
}
void car_h_27(double *state, double *unused, double *out_4810151999826627565) {
  h_27(state, unused, out_4810151999826627565);
}
void car_H_27(double *state, double *unused, double *out_490705784205150259) {
  H_27(state, unused, out_490705784205150259);
}
void car_h_29(double *state, double *unused, double *out_3071120650927421620) {
  h_29(state, unused, out_3071120650927421620);
}
void car_H_29(double *state, double *unused, double *out_3175700440319967354) {
  H_29(state, unused, out_3175700440319967354);
}
void car_h_28(double *state, double *unused, double *out_5503836136891602899) {
  h_28(state, unused, out_5503836136891602899);
}
void car_H_28(double *state, double *unused, double *out_1906698576749563220) {
  H_28(state, unused, out_1906698576749563220);
}
void car_h_31(double *state, double *unused, double *out_8361193874858590043) {
  h_31(state, unused, out_8361193874858590043);
}
void car_H_31(double *state, double *unused, double *out_4220575283609081157) {
  H_31(state, unused, out_4220575283609081157);
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
