#pragma once
#include "option.hpp"
#include <string>
using namespace std;

class asian : public option {
public:
    // Constructors
    asian() {};
    asian(asian& asia) :option(asia) {};
    asian(asset* ptr_underlying, double K, double T, string position) : option(ptr_underlying, K, T, position) {};

    // Destructor
    ~asian();

    // Member function
    double price() const;
    string type() const;
    void replication() const;
};
