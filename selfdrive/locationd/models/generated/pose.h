#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_2522441676304835175);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_497817490349892178);
void pose_H_mod_fun(double *state, double *out_8320576250642144753);
void pose_f_fun(double *state, double dt, double *out_368313394866714597);
void pose_F_fun(double *state, double dt, double *out_6193746690085684006);
void pose_h_4(double *state, double *unused, double *out_5598758435398906091);
void pose_H_4(double *state, double *unused, double *out_120271366668919986);
void pose_h_10(double *state, double *unused, double *out_2397420084850967746);
void pose_H_10(double *state, double *unused, double *out_6919231927501633514);
void pose_h_13(double *state, double *unused, double *out_3860918846742744675);
void pose_H_13(double *state, double *unused, double *out_7490359841647780943);
void pose_h_14(double *state, double *unused, double *out_148712011300453905);
void pose_H_14(double *state, double *unused, double *out_3842969489670564543);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}