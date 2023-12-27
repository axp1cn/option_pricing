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
    american(asset* ptr_underlying, double K, double T, string position) : option(ptr_underlying, K, T, position) {};

    // Destructor
    ~american() ;

    // Member function
    double price() const;
    string type() const;
    void replication() const;
};
