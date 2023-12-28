#pragma once //unique inclusion to ensure the content of the file not to be included again if encountered in the same compilation
#include "asset.hpp"
#include <iostream>
#include <string>
using namespace std;

class option {
protected :

    double strike = 0 ; // strike
    double maturity = 0; // maturity (in years)
    string position = "";
    asset* underlying_asset = nullptr;

public :
    ~option();
    // Constructor
    option() {};
    option(option&);
    option(asset* underlying_asset, double strike, double maturity, string position);

    // Member functions
    virtual double price() const = 0;
    virtual string type() const = 0;
    virtual void replication() const = 0;
    
    // Getter
    double get_strike() const { return this->strike; } ;
    double get_T() const { return this->maturity; };
    string get_position() const { return this->position; };
    asset* get_ptr_underlying() const { return this->underlying_asset; } ;

    // Setter
    void set_strike(double strike) { this->strike = strike; };
    void set_T(double maturity) { this->maturity = maturity; };
    void set_position(string position) { this->position = position; };
    void set_ptr_underlying(asset* underlying_asset) { this->underlying_asset = underlying_asset; };



    // Non-member function
    friend istream& operator>>(istream& input, option& o);
};
