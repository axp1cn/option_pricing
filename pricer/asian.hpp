#pragma once
#include "option.hpp"
#include <string>
using namespace std;

class asian : public option {
public:
    // Constructors
    asian() {};
    asian(asian& asia) :option(asia) {};
    asian(asset* underlying_asset, double strike, double maturity, string position) : option(underlying_asset, strike, maturity, position) {};

    // Destructor
    ~asian();

    // Advanced functions
    double price() const;
    string type() const;
};
