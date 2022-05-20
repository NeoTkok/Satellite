//
// Created by 79042 on 11.03.2022.
//

#ifndef MAIN_CPP_SATT_H
#define MAIN_CPP_SATT_H

#include "vector"
#include <iostream>
using std::vector;

class satellite {
protected:
    //bad - std::array
    vector<double> gen_coord;
    double mass;
    int id;
public:
    satellite();
    satellite(int id_, double m, vector<double> set_q);
    ~satellite();
    [[nodiscard]] vector<double> get_genCoord() const;
    [[nodiscard]] vector<double> get_Coord() const;
    [[nodiscard]] vector<double> get_Vel() const;
    [[nodiscard]] double get_Mass() const;
    [[nodiscard]] int get_ID() const;
    void change_genCoord(vector<double> new_genCoords);
};


#endif //MAIN_CPP_SATT_H
