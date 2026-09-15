#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_8219286415624560569);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_6195640368181513549);
void car_H_mod_fun(double *state, double *out_7959613188215390617);
void car_f_fun(double *state, double dt, double *out_3072344104277436358);
void car_F_fun(double *state, double dt, double *out_4300892242094410296);
void car_h_25(double *state, double *unused, double *out_3314714972500610637);
void car_H_25(double *state, double *unused, double *out_4251221245486041585);
void car_h_24(double *state, double *unused, double *out_1419049792901485991);
void car_H_24(double *state, double *unused, double *out_3406224636702346186);
void car_h_30(double *state, double *unused, double *out_4578224522383912722);
void car_H_30(double *state, double *unused, double *out_2665469096005575170);
void car_h_26(double *state, double *unused, double *out_3634714669482250287);
void car_H_26(double *state, double *unused, double *out_946695275725240984);
void car_h_27(double *state, double *unused, double *out_4810151999826627565);
void car_H_27(double *state, double *unused, double *out_490705784205150259);
void car_h_29(double *state, double *unused, double *out_3071120650927421620);
void car_H_29(double *state, double *unused, double *out_3175700440319967354);
void car_h_28(double *state, double *unused, double *out_5503836136891602899);
void car_H_28(double *state, double *unused, double *out_1906698576749563220);
void car_h_31(double *state, double *unused, double *out_8361193874858590043);
void car_H_31(double *state, double *unused, double *out_4220575283609081157);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}