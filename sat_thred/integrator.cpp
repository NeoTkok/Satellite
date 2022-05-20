//
// Created by AS on 3/11/2022.
//
#include <chrono>
#include "integrator.h"
#include "vec_operations.h"
#include "cmath"

using std::sqrt;
using std::pow;
double const Mu = 398600441500000.0;

vector<double> calc_force( double m, vector<double> q){
    vector<double> r(3);

    for(int i = 0; i<3; i++){
        r.at(i) = q.at(i);
    }
    double radius = sqrt(r[0]*r[0] + r[1]*r[1] + r[2]*r[2]);
    vector<double> f = mult( -( Mu * m / pow(radius, 3)), r);
    return f;
}

vector<double> calc_acceleration(double m, vector<double> q){
    return mult(1/m ,calc_force(m , q));
}

vector<double> find_q_deriv(double m, vector<double> q){

    vector<double> q_deriv_calculated({
                                              q[3],
                                              q[4],
                                              q[5],

                                              calc_acceleration(m, q)[0],
                                              calc_acceleration(m, q)[1],
                                              calc_acceleration(m, q)[2]
                                      });
    return  q_deriv_calculated;
}

vector<double> make_iter_RK(double m, vector<double> q, double step){

    vector<double> k1(6,0), k2(6,0),
            k3(6,0), k4(6,0);
    vector<double> q_new(6,0);

    k1 = find_q_deriv( m, q);
    k2 = find_q_deriv( m, sum(q, mult(step/2, k1)));
    k3 = find_q_deriv( m, sum(q, mult(step/2, k2)));
    k4 = find_q_deriv( m, sum(q, mult(step, k3)));

    return sum(q, mult(step/6,
                       sum(sum(k1, k4),
                           mult(2, sum(k2, k3))))) ;
}
