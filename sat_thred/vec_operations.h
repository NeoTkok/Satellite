//
// Created by AS on 3/11/2022.
//

#ifndef TEST_SAT_VEC_OPERATIONS_H
#define TEST_SAT_VEC_OPERATIONS_H

#endif //TEST_SAT_VEC_OPERATIONS_H

#include "vector"
#include <iostream>
using std::vector;

std::ostream& operator<<(std::ostream& cout, std::vector<double>& one);

vector<double> sum(vector<double> a, vector<double> b);

vector<double> sum( vector<double> a, double b);

vector<double> mult(double alpha, vector<double> b);