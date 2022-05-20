//
// Created by AS on 3/11/2022.
//

#include "includes.h"



std::ostream& operator<<(std::ostream& cout, std::vector<double>& one) {
    for (unsigned int i = 0; i < one.size() - 1; ++i) {
        std::cout << one[i]<< ", ";
    }
    std::cout<< one[one.size()-1];
    return cout;
}


vector<double> sum(vector<double> a, vector<double> b){
    if(a.size()!=b.size()){
        std::cout<< "Error line 14: vectors of different size"<< std::endl;
        abort();
    }
    else{
        int size = a.size();
        vector<double> summ(size);
        for(int i = 0; i < size; i++){
            summ[i] = a[i] + b[i];
        }
        return summ;
    }

}


vector<double> sum( vector<double> a, double b){
    vector<double> summ(a.size());
    for(int i = 0; i < a.size(); i++){
        summ[i] = a[i] + b;
    }
    return summ;
}

vector<double> mult(double alpha, vector<double> b){

    vector<double> summ(b.size());
    for(int i = 0; i < b.size(); i++){
        summ[i] = alpha * b[i];
    }
    return summ;
}