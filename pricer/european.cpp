#include "european.hpp"
#include "tools.hpp"
#include "asset.hpp"
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// Destructor
european::~european() {};

// Advanced functions
double european::price() const {
    if (underlying_asset == nullptr) {
        cout << "The option has not been initialised (the pointer of the underlying is not initialised). Thus, the price doesn't exist.";
        return 0;
    }
    if (position == "call") {
        double spot = (*underlying_asset).get_SpotPrice();
        double vol = (*underlying_asset).get_Volatility();
        double r = (*underlying_asset).get_r();
        
        // Lump dividends
        if (((*underlying_asset).get_alias_Dividends().get_Type() == 1)) {
            int n = DividendCounter(maturity, (*underlying_asset).get_alias_Dividends().get_Next(), (*underlying_asset).get_alias_Dividends().get_Periods()); // calculates the number of coupon detachments before the option's maturity
            double S_hat = spot*pow(1- (*underlying_asset).get_alias_Dividends().get_Rate(),n);
            
            /* we need to compute the price of an option,
             with the strike the same strike and the same maturity,
             on an underlying with the spot price S_hat and the same volatility, but WITH NO DIVIDENDS*/
            
            asset asset_for_computation; //asset without dividend
            asset_for_computation.set_SpotPrice(S_hat);
            asset_for_computation.set_Volatility(vol);
            asset* ptr_underlying_for_computation = &asset_for_computation;
            
            european call_for_computation(ptr_underlying_for_computation, strike, maturity,position);
            
            return call_for_computation.price();
        }
        // Continuous dividends
        else if (((*underlying_asset).get_alias_Dividends().get_Type() == 2)) {
            double rate = (*underlying_asset).get_alias_Dividends().get_Rate();
            double strike_for_computation = strike * exp(rate * maturity);
            
            /* we need to compute the price of an option,
             with the strike "strike_for_computation" and same maturity,
             on an underlying with the same spot price and volatility, but WITH NO DIVIDENDS*/
            
            asset asset_for_computation; //asset without dividend
            asset_for_computation.set_SpotPrice(spot);
            asset_for_computation.set_Volatility(vol);
            asset* ptr_underlying_for_computation = &asset_for_computation;
            
            european call_for_computation(ptr_underlying_for_computation,strike_for_computation,maturity,position);
            
            return exp(-rate * maturity) * call_for_computation.price();
        }
        // No dividends
        else{
            double d_1 = (log(spot / strike) + (r + pow(vol, 2) / 2) * maturity) / (vol * sqrt(maturity));
            double d_2 = d_1 - vol * sqrt(maturity);
            
            double V = spot * cdf(d_1) - strike * exp(-r * maturity) * cdf(d_2);
            
            return V;
        }
    }
    else {
        double spot = (*underlying_asset).get_SpotPrice();
        double r = (*underlying_asset).get_r();
        european equivalent_call(underlying_asset, strike, maturity, "call");
        // Lump dividends
        if (((*underlying_asset).get_alias_Dividends().get_Type() == 1)) {
            int n = DividendCounter(maturity, (*underlying_asset).get_alias_Dividends().get_Next(), (*underlying_asset).get_alias_Dividends().get_Periods()); // calculates the number of coupon detachments before the option's maturity
            double S_hat = spot * pow(1 - (*underlying_asset).get_alias_Dividends().get_Rate(), n); // S_hat = spot * (1-dividend_rate)^n

            return equivalent_call.price() + exp(-r * maturity) * strike - S_hat; // call-put parity for european with underlying with lump payments of dividends
        }
        // Continuous dividends and no dividend
        else {
            double rate = (*underlying_asset).get_alias_Dividends().get_Rate();
            double S_hat = spot * exp(- rate * maturity);
            
            return equivalent_call.price() - S_hat + exp(-r * maturity) * strike ; //We notice that if the rate is null, then we have the classical call-put parity.
        }
    }
};

string european::type() const {
    string name = "european " + position;
    return name;
};
