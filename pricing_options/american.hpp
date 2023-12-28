#pragma once
#include "tools.hpp"
#include "option.hpp"
#include <string>
using namespace std;

class american: public option {
public :
    // Constructors
    american() {};
    american(american& amer) :option(amer) {};
    american(asset* underlying_asset, double strike, double maturity, string position) : option(underlying_asset, strike, maturity, position) {};

    // Destructor
    ~american() ;

    // Member function
    double price() const;
    string type() const;
    void replication() const;
};
