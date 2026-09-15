#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_3350640148905151952);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_5937141942059359877);
void pose_H_mod_fun(double *state, double *out_4176406737755320299);
void pose_f_fun(double *state, double dt, double *out_3911862024342109205);
void pose_F_fun(double *state, double dt, double *out_7558296450784675551);
void pose_h_4(double *state, double *unused, double *out_7569011205382429115);
void pose_H_4(double *state, double *unused, double *out_833561905108507896);
void pose_h_10(double *state, double *unused, double *out_1713401149757965889);
void pose_H_10(double *state, double *unused, double *out_4851606762710325905);
void pose_h_13(double *state, double *unused, double *out_4066253850474937797);
void pose_H_13(double *state, double *unused, double *out_4667317368411031920);
void pose_h_14(double *state, double *unused, double *out_1299243001364970001);
void pose_H_14(double *state, double *unused, double *out_8314707720388248320);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}