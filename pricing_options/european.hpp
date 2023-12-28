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
    european(asset* underlying_asset, double strike, double maturity, string position) : option(underlying_asset, strike, maturity, position) {};

    // Destructor
    ~european() ;

    // Member function
    double price() const;
    string type() const;
    void replication() const;
};

