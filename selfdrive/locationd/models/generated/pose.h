#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_2282374485527159040);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_4495318315073688785);
void pose_H_mod_fun(double *state, double *out_8988321217165806383);
void pose_f_fun(double *state, double dt, double *out_7445325378273019703);
void pose_F_fun(double *state, double dt, double *out_1682818178324058156);
void pose_h_4(double *state, double *unused, double *out_4679628837592052355);
void pose_H_4(double *state, double *unused, double *out_43959551107985041);
void pose_h_10(double *state, double *unused, double *out_2324693601250545730);
void pose_H_10(double *state, double *unused, double *out_2927403466411711434);
void pose_h_13(double *state, double *unused, double *out_5301313385047798381);
void pose_H_13(double *state, double *unused, double *out_3256233376440317842);
void pose_h_14(double *state, double *unused, double *out_5419473069984494684);
void pose_H_14(double *state, double *unused, double *out_3038828881187387255);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}