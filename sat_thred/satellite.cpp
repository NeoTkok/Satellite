//
// Created by 79042 on 11.03.2022.
//

#include "satellite.h"
#include "vec_operations.h"

satellite::satellite(): id(0), mass(0), gen_coord({0, 0, 0, 0, 0, 0}) {};

satellite::satellite( int id_, double m, vector<double> set_q): id{id_}, mass{m}, gen_coord{set_q} {};
satellite::~satellite(){

};
std::vector<double> satellite::get_genCoord() const {
    return gen_coord;
}

std::vector<double> satellite::get_Coord() const {
    std::vector<double> coord = {this->gen_coord[0], this->gen_coord[1], this->gen_coord[2]};
    return coord;
}

std::vector<double> satellite::get_Vel() const {
    std::vector<double> veloc = {this->gen_coord[3], this->gen_coord[4], this->gen_coord[5]};
    return veloc;
}

double satellite::get_Mass() const {
    return this->mass;
}

int satellite::get_ID() const {
    return this->id;
}


void satellite::change_genCoord(vector<double> new_genCoords){
    this->gen_coord = new_genCoords;
}