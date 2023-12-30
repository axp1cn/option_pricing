#pragma once //unique inclusion to ensure the content of the file not to be included again if encountered in the same compilation
#include "asset.hpp"
#include <iostream>
#include <string>
using namespace std;

class option {
protected :

    double strike = 0 ; // strike
    double maturity = 0; // maturity (in years)
    string position = ""; // call or put
    asset* underlying_asset = nullptr;

public :
    //Destructor
    ~option();
    
    // Constructors
    option() {};
    option(option&);
    option(asset* underlying_asset, double strike, double maturity, string position);

    // Member functions
    virtual double price() const = 0;
    virtual string type() const = 0;
    
    // Getters
    double get_strike() const { return this->strike; } ;
    double get_T() const { return this->maturity; };
    string get_position() const { return this->position; };
    asset* get_ptr_underlying() const { return this->underlying_asset; } ;

    // Setters
    void set_strike(double strike) { this->strike = strike; };
    void set_T(double maturity) { this->maturity = maturity; };
    void set_position(string position) { this->position = position; };
    void set_ptr_underlying(asset* underlying_asset) { this->underlying_asset = underlying_asset; };


    // Display overload function
    friend istream& operator>>(istream& input, option& o);
};
