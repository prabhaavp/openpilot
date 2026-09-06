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
void car_err_fun(double *nom_x, double *delta_x, double *out_5731293444659899197);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_658884802146674275);
void car_H_mod_fun(double *state, double *out_2364024515690160793);
void car_f_fun(double *state, double dt, double *out_7312135134474090361);
void car_F_fun(double *state, double dt, double *out_1426178926713797570);
void car_h_25(double *state, double *unused, double *out_4322344263373120118);
void car_H_25(double *state, double *unused, double *out_6889531332107153586);
void car_h_24(double *state, double *unused, double *out_416399681732138117);
void car_H_24(double *state, double *unused, double *out_2020716467079446734);
void car_h_30(double *state, double *unused, double *out_4597538325657626007);
void car_H_30(double *state, double *unused, double *out_2361835001979545388);
void car_h_26(double *state, double *unused, double *out_7201943646466323658);
void car_H_26(double *state, double *unused, double *out_3148028013233097362);
void car_h_27(double *state, double *unused, double *out_5149894096224076937);
void car_H_27(double *state, double *unused, double *out_4585429073163488605);
void car_h_29(double *state, double *unused, double *out_5200854210334103524);
void car_H_29(double *state, double *unused, double *out_2872066346293937572);
void car_h_28(double *state, double *unused, double *out_7418564489926759591);
void car_H_28(double *state, double *unused, double *out_2210332670775593002);
void car_h_31(double *state, double *unused, double *out_684894735119097768);
void car_H_31(double *state, double *unused, double *out_6920177293984114014);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}