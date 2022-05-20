//
// Created by AS on 3/11/2022.
//

#ifndef TRY_WINSOCK_SERV_INTEGRATOR_H
#define TRY_WINSOCK_SERV_INTEGRATOR_H

#endif //TRY_WINSOCK_SERV_INTEGRATOR_H

#include "vector"
using std::vector;

vector<double> calc_force(double m, vector<double> q);
vector<double> calc_acceleration(double m, vector<double> q);
vector<double> find_q_deriv(double m, vector<double> q);
vector<double> make_iter_RK(double m, vector<double> q, double step);

