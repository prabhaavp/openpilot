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
void car_err_fun(double *nom_x, double *delta_x, double *out_6820148586248952230);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2700214348283633384);
void car_H_mod_fun(double *state, double *out_8028556290600921930);
void car_f_fun(double *state, double dt, double *out_896274243400428506);
void car_F_fun(double *state, double dt, double *out_6634580416456269043);
void car_h_25(double *state, double *unused, double *out_653594826425806173);
void car_H_25(double *state, double *unused, double *out_1224999557196392449);
void car_h_24(double *state, double *unused, double *out_276891562756827924);
void car_H_24(double *state, double *unused, double *out_7879205015304578206);
void car_h_30(double *state, double *unused, double *out_2983854701828216177);
void car_H_30(double *state, double *unused, double *out_1095660610053152379);
void car_h_26(double *state, double *unused, double *out_2082562816890126546);
void car_H_26(double *state, double *unused, double *out_2516503761677663775);
void car_h_27(double *state, double *unused, double *out_3258000147234503824);
void car_H_27(double *state, double *unused, double *out_1079102701747272532);
void car_h_29(double *state, double *unused, double *out_7985527969489061822);
void car_H_29(double *state, double *unused, double *out_1605891954367544563);
void car_h_28(double *state, double *unused, double *out_5380828712605819818);
void car_H_28(double *state, double *unused, double *out_828835157051497314);
void car_h_31(double *state, double *unused, double *out_8534867477179855358);
void car_H_31(double *state, double *unused, double *out_1255645519073352877);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}