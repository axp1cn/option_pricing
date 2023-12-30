#include "asset.hpp"
#include "option.hpp"
#include <iostream>
#include <string>
using namespace std;

// Destructor
option::~option() {}

// Constructors
option::option(option& o) {
    this->strike = o.strike;
    this->maturity = o.maturity;
    this->position = o.position;
    this->underlying_asset = o.underlying_asset;
};

option::option(asset* underlying_asset, double strike, double maturity, string position) {
    this->strike = strike;
    this->maturity = maturity;
    this->position = position;
    this->underlying_asset = underlying_asset;
}

// Display overload function
istream& operator>>(istream& input, option& o) {
    cout <<"\nEnter the option's position (call/put): ";
    input >> o.position;
    cout <<"Enter the option's strike: ";
    input >> o.strike;
    cout << "Enter the option's maturity (in years): ";
    input >> o.maturity;

    asset* underlying_asset = new asset;
    input >> *underlying_asset;
    o.set_ptr_underlying(underlying_asset);
    cout << "\nThe option is now initialized!\n";
    return input;
};
