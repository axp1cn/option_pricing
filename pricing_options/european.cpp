#include "european.hpp"
#include "tools.hpp"
#include "asset.hpp"
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// Destructor
european::~european() {};

// Member function
double european::price() const {
    // manage of an option constructed by the default constructor and underlying_asset not initialized
    if (underlying_asset == nullptr) {
        cout << "The option has not been initialised (the pointer of the underlying is not initialised). Thus, the price doesn't exist.";
        return 0;
    }
    if (position == "call") {
        double spot = (*underlying_asset).get_SpotPrice();
        double vol = (*underlying_asset).get_Volatility();
        double r = (*underlying_asset).get_r();
        
        // -------------- Lump dividends --------------
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
            
            european call_for_computation(ptr_underlying_for_computation, strike, maturity, position);
            
            return call_for_computation.price();
        }
        //----------- Continuous dividend ---------------
        if (((*underlying_asset).get_alias_Dividends().get_Type() == 2)) {
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
        // -------------- No dividend --------------------
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
        // -------------- Lump dividends --------------
        if (((*underlying_asset).get_alias_Dividends().get_Type() == 1)) {
            int n = DividendCounter(maturity, (*underlying_asset).get_alias_Dividends().get_Next(), (*underlying_asset).get_alias_Dividends().get_Periods()); // calculates the number of coupon detachments before the option's maturity
            double S_hat = spot * pow(1 - (*underlying_asset).get_alias_Dividends().get_Rate(), n); // S_hat = spot * (1-dividend_rate)^n

            return equivalent_call.price() + exp(-r * maturity) * strike - S_hat; // call-put parity for european with underlying with lump payments of dividends
        }
        // -------------- Continuous dividends and no dividend -------------
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
/*
void european::replication() const {
    // manage of an option constructed by the default constructor and underlying_asset not initialized
    if (underlying_asset == nullptr) {
        cout << "The option has not been initialised (the pointer of the underlying is not initialised). Thus, replication is not possible";
    }

    if (position == "call") {
        european put(underlying_asset, strike, maturity, "put");
        double r = (*underlying_asset).get_r();
        
        // ------------ no dividends ----------------
        if ((*underlying_asset).get_alias_Dividends().get_Type() == 0) {
            cout << "The replication of the buy of this " << this->type() << ", which value is " << this->price() << ", is : ";
            cout << "\n * The buy on the same underlying asset of an " << put;
            cout << "\n * The buy of the underlying asset.";
            cout << "\n * The borrow of " << exp(-r * maturity) * strike << " on the risk free rate market. ";
            cout << "\n The balance of the replication is " << put.price() + (*underlying_asset).get_SpotPrice() - exp(-r * maturity) * strike << " (which is indeed equal to the spot price of the replicated option).\n\n\n";
        }
        // ------------ continuous dividends ----------------
        if ((*underlying_asset).get_alias_Dividends().get_Type() == 2) {
            double rate = (*underlying_asset).get_alias_Dividends().get_Rate();
            
            cout << "The replication of the buy of this " << this->type() << ", which value is " << this->price() << ", is : ";
            cout << "\n * The buy on the same underlying asset of an " << put;
            cout << "\n * The buy of the underlying asset.";
            cout << "\n * The borrow of " << exp(-r * maturity) * strike + (*underlying_asset).get_SpotPrice() * (1 - exp(-rate * maturity)) << " on the risk free rate market. ";
            cout << "\n The balance of the replication is " << put.price() + (*underlying_asset).get_SpotPrice() -( exp(-r * maturity) * strike + (*underlying_asset).get_SpotPrice() * (1 - exp(-rate * maturity))) << " (which is indeed equal to the spot price of the replicated option).\n\n\n";
        }
        // ------------ lump dividends ----------------
        else {
            double rate = (*underlying_asset).get_alias_Dividends().get_Rate();
            int n = DividendCounter(maturity, (*underlying_asset).get_alias_Dividends().get_Next(), (*underlying_asset).get_alias_Dividends().get_Periods()); // calculates the number of coupon detachments before the option's maturity
            
            cout << "The replication of the buy of this " << this->type() << ", which value is " << this->price() << ", is : ";
            cout << "\n * The buy on the same underlying asset of an " << put;
            cout << "\n * The buy of the underlying asset.";
            cout << "\n * The borrow of " << exp(-r * maturity) * strike + (*underlying_asset).get_SpotPrice() * (1 - pow(1 - rate, n)) << " on the risk free rate market. ";
            cout << "\n The balance of the replication is " << put.price() + (*underlying_asset).get_SpotPrice() - (exp(-r * maturity) * strike + (*underlying_asset).get_SpotPrice() * (1 - pow(1 - rate, n))) << " (which is indeed equal to the spot price of the replicated option).\n\n\n";
        }
    }
    else{
        european call(underlying_asset, strike, maturity, "call");
        double r = (*underlying_asset).get_r();

        // ------------- no dividends ----------------
        if ((*underlying_asset).get_alias_Dividends().get_Type() == 0) {
            cout << "The replication of the buy of this " << this->type() << ", which value is " << this->price() << ", is : ";
            cout << "\n * The buy, on the same underlying asset, of an " << call;
            cout << "\n * The sell of the underlying asset.";
            cout << "\n * The investment of " << exp(-r * maturity) * strike << " on the risk free rate market. ";
            cout << "\n The balance of the replication is " << call.price() - (*underlying_asset).get_SpotPrice() + exp(-r * maturity) * strike << " (which is indeed equal to the spot price of the replicated option).\n\n\n";
        }
        // ------------- continuous dividends ----------------
        if ((*underlying_asset).get_alias_Dividends().get_Type() == 2) {
            double rate = (*underlying_asset).get_alias_Dividends().get_Rate();

            cout << "The replication of the buy of this " << this->type() << ", which value is " << this->price() << ", is : ";
            cout << "\n * The buy, on the same underlying asset, of an " << call;
            cout << "\n * The sell of the underlying asset. (It is a short selling, you will have to pay the continuous dividends)";
            cout << "\n * The investment of " << exp(-r * maturity) * strike + (*underlying_asset).get_SpotPrice() * (1 - exp(-rate * maturity)) << " on the risk free rate market. "; // strike*e^(-r*maturity) - spot(e^(-rate*maturity)-1)
            cout << "\n The balance of the replication is " << call.price() - (*underlying_asset).get_SpotPrice() + exp(-r * maturity) * strike + (*underlying_asset).get_SpotPrice()*(1-exp(-rate*maturity)) << " (which is indeed equal to the spot price of the replicated option).\n\n\n";

        }
        // ------------- lump dividends ----------------
        else {
            double rate = (*underlying_asset).get_alias_Dividends().get_Rate();
            int n = DividendCounter(maturity, (*underlying_asset).get_alias_Dividends().get_Next(), (*underlying_asset).get_alias_Dividends().get_Periods()); // calculates the number of coupon detachments before the option's maturity

            cout << "The replication of the buy of this " << this->type() << ", which value is " << this->price() << ", is : ";
            cout << "\n * The buy, on the same underlying asset, of an " << call;
            cout << "\n * The sell of the underlying asset. (It is a short selling, you will have to pay the continuous dividends)";
            cout << "\n * The investment of " << exp(-r * maturity) * strike + (*underlying_asset).get_SpotPrice() * (1 - pow(1 - rate, n)) << " on the risk free rate market. "; // strike*e^(-r*maturity) - spot(e^(-rate*maturity)-1)
            cout << "\n The balance of the replication is " << call.price() - (*underlying_asset).get_SpotPrice() + exp(-r * maturity) * strike + (*underlying_asset).get_SpotPrice() *(1- pow((1 - rate), n)) << " (which is indeed equal to the spot price of the replicated option).\n\n\n";

        }
    }
};
*/
