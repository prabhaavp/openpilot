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
void car_err_fun(double *nom_x, double *delta_x, double *out_5924875962979878419);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_3730797253670217675);
void car_H_mod_fun(double *state, double *out_8998384333727686430);
void car_f_fun(double *state, double dt, double *out_5904937932592495420);
void car_F_fun(double *state, double dt, double *out_874526194948226047);
void car_h_25(double *state, double *unused, double *out_5000351285338572771);
void car_H_25(double *state, double *unused, double *out_194803892184550807);
void car_h_24(double *state, double *unused, double *out_651987176130886021);
void car_H_24(double *state, double *unused, double *out_5015125396840539070);
void car_h_30(double *state, double *unused, double *out_8755349036136976630);
void car_H_30(double *state, double *unused, double *out_324142839327790877);
void car_h_26(double *state, double *unused, double *out_4689721769911729341);
void car_H_26(double *state, double *unused, double *out_3936307211058607031);
void car_h_27(double *state, double *unused, double *out_7961308757030146390);
void car_H_27(double *state, double *unused, double *out_2498906151128215788);
void car_h_29(double *state, double *unused, double *out_6510677364401263223);
void car_H_29(double *state, double *unused, double *out_186088504986601307);
void car_h_28(double *state, double *unused, double *out_4842705445598448391);
void car_H_28(double *state, double *unused, double *out_2248638606432440570);
void car_h_31(double *state, double *unused, double *out_902047987059200907);
void car_H_31(double *state, double *unused, double *out_164157930307590379);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}