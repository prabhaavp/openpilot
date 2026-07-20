#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_3585092682791090136);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_3602092163882833613);
void pose_H_mod_fun(double *state, double *out_9072779523166268742);
void pose_f_fun(double *state, double dt, double *out_8675559756142309217);
void pose_F_fun(double *state, double dt, double *out_4846751114242430980);
void pose_h_4(double *state, double *unused, double *out_5930999592870125556);
void pose_H_4(double *state, double *unused, double *out_5602157340699375302);
void pose_h_10(double *state, double *unused, double *out_5557120776990141355);
void pose_H_10(double *state, double *unused, double *out_1023238183635137507);
void pose_h_13(double *state, double *unused, double *out_6353105533166408740);
void pose_H_13(double *state, double *unused, double *out_2008473867617325627);
void pose_h_14(double *state, double *unused, double *out_3322199193100514196);
void pose_H_14(double *state, double *unused, double *out_8684945772994747598);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}