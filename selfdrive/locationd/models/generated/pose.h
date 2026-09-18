#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_2520069339484736335);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_3053044731700442871);
void pose_H_mod_fun(double *state, double *out_6611456164701546289);
void pose_f_fun(double *state, double dt, double *out_7258279731957877583);
void pose_F_fun(double *state, double dt, double *out_3804310599737068228);
void pose_h_4(double *state, double *unused, double *out_1042392600665946689);
void pose_H_4(double *state, double *unused, double *out_2890926242950183985);
void pose_h_10(double *state, double *unused, double *out_8809755638952996463);
void pose_H_10(double *state, double *unused, double *out_8718760487604320532);
void pose_h_13(double *state, double *unused, double *out_8270645238899386460);
void pose_H_13(double *state, double *unused, double *out_942829220352340039);
void pose_h_14(double *state, double *unused, double *out_891035561933187989);
void pose_H_14(double *state, double *unused, double *out_191862189345188311);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}