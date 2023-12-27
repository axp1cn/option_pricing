#pragma once
#include "option.hpp"
#include <iostream>
#include <string>
using namespace std;

class european : public option {
public :
    // Constructors
    european() {};
    european(european& euro) :option(euro) {};
    european(asset* ptr_underlying, double K, double T, string position) : option(ptr_underlying, K, T, position) {};

    // Destructor
    ~european() ;

    // Member function
    double price() const;
    string type() const;
    void replication() const;
};

