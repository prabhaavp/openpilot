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
void car_err_fun(double *nom_x, double *delta_x, double *out_8452016858633359142);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_6091261500618765150);
void car_H_mod_fun(double *state, double *out_38657891020581010);
void car_f_fun(double *state, double dt, double *out_558885094687163216);
void car_F_fun(double *state, double dt, double *out_2583716049044893862);
void car_h_25(double *state, double *unused, double *out_6863161802963304328);
void car_H_25(double *state, double *unused, double *out_2872687221825540538);
void car_h_24(double *state, double *unused, double *out_1110917950379322687);
void car_H_24(double *state, double *unused, double *out_4545704411851386355);
void car_h_30(double *state, double *unused, double *out_3820317013925574847);
void car_H_30(double *state, double *unused, double *out_7400383551953148736);
void car_h_26(double *state, double *unused, double *out_3983562419870100788);
void car_H_26(double *state, double *unused, double *out_6614190540699596762);
void car_h_27(double *state, double *unused, double *out_70918829331572549);
void car_H_27(double *state, double *unused, double *out_5176789480769205519);
void car_h_29(double *state, double *unused, double *out_4423632898602052364);
void car_H_29(double *state, double *unused, double *out_6890152207638756552);
void car_h_28(double *state, double *unused, double *out_3204928983415884040);
void car_H_28(double *state, double *unused, double *out_4926521936073430301);
void car_h_31(double *state, double *unused, double *out_597396961285583997);
void car_H_31(double *state, double *unused, double *out_2842041259948580110);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}