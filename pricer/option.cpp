#include "asset.hpp"
#include "option.hpp"
#include <iostream>
#include <string>
using namespace std;

option::~option() {}

// Constructors
option::option(option& o) {
    this->K = o.K;
    this->T = o.T;
    this->position = o.position;
    this->ptr_underlying = o.ptr_underlying;
};

option::option(asset* ptr_underlying, double K, double T, string position) {
    this->K = K;
    this->T = T;
    this->position = position;
    this->ptr_underlying = ptr_underlying;
}

// Overloading of the operators << and >> of iostream
std::ostream& operator<<(std::ostream& output, const option& o) {
    // manage of an option constructed by the default constructor and ptr_underlying not initialized
    if (o.ptr_underlying == nullptr) {
        output << "The option has not been initialised (the pointer of the underlying is not initialised). Thus, the price is not possible";
        return output;
    }

    asset* ptr_underlying = o.get_ptr_underlying();
    // manage of an option constructed by the default constructor and ptr_underlying not initialized
    if (ptr_underlying == nullptr) {
        output << "The option has not been initialised (the pointer of the underlying is not initialised).";
        return output;
    }

    double S = (*ptr_underlying).get_SpotPrice();
    double sigma = (*ptr_underlying).get_Volatility();
    double r = (*ptr_underlying).get_r();

    output << "an " << o.type() << " with : " << "\n    - Strike : " << o.K << "\n    - Spot price : " << S << "\n    - Maturity : " << o.T << " years" << "\n    - Volatility : " << sigma << "\n    - Risk free rate : " << r * 100 << "%" << "\n    - Price (Black-Scholes method) : " << o.price() << "\n";
    return output;
};

std::istream& operator>>(std::istream& input, option& o) {
    cout <<"\n - Enter the option's position (call/put) : ";
    input >> o.position;
    cout <<"\n - Enter the option's strike : ";
    input >> o.K;
    cout << " - Enter the option's maturity : ";
    input >> o.T;

    asset* ptr_underlying = new asset;
    input >> *ptr_underlying;
    o.set_ptr_underlying(ptr_underlying);
    cout << "\nThe price of the " << o.type() << " you have just defined is : " << o.price() << ".\n";

    return input;
};
