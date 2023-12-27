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
    // manage of an option constructed by the default constructor and ptr_underlying not initialized
    if (ptr_underlying == nullptr) {
        cout << "The option has not been initialised (the pointer of the underlying is not initialised). Thus, the price doesn't exist.";
        return 0;
    }
    
    if (position == "call") {
        double S = (*ptr_underlying).get_SpotPrice();
        double sigma = (*ptr_underlying).get_Volatility();
        double r = (*ptr_underlying).get_r();
        
        // -------------- Lump dividends --------------
        if (((*ptr_underlying).get_alias_Dividends().get_Type() == 1)) {
            int n = DividendCounter(T, (*ptr_underlying).get_alias_Dividends().get_Next(), (*ptr_underlying).get_alias_Dividends().get_Periods()); // calculates the number of coupon detachments before the option's maturity
            double S_hat = S*pow(1- (*ptr_underlying).get_alias_Dividends().get_Rate(),n);
            
            /* we need to compute the price of an option,
             with the strike the same strike and the same maturity,
             on an underlying with the spot price S_hat and the same volatility, but WITH NO DIVIDENDS*/
            
            asset asset_for_computation; //asset without dividend
            asset_for_computation.set_SpotPrice(S_hat);
            asset_for_computation.set_Volatility(sigma);
            asset* ptr_underlying_for_computation = &asset_for_computation;
            
            european call_for_computation(ptr_underlying_for_computation, K, T, position);
            
            return call_for_computation.price();
        }
        //----------- Continuous dividend ---------------
        if (((*ptr_underlying).get_alias_Dividends().get_Type() == 2)) {
            double rate = (*ptr_underlying).get_alias_Dividends().get_Rate();
            double K_for_computation = K * exp(rate * T);
            
            /* we need to compute the price of an option,
             with the strike "K_for_computation" and same maturity,
             on an underlying with the same spot price and volatility, but WITH NO DIVIDENDS*/
            
            asset asset_for_computation; //asset without dividend
            asset_for_computation.set_SpotPrice(S);
            asset_for_computation.set_Volatility(sigma);
            asset* ptr_underlying_for_computation = &asset_for_computation;
            
            european call_for_computation(ptr_underlying_for_computation,K_for_computation,T,position);
            
            return exp(-rate * T) * call_for_computation.price();
        }
        // -------------- No dividend --------------------
        else{
            double d_1 = (log(S / K) + (r + pow(sigma, 2) / 2) * T) / (sigma * sqrt(T));
            double d_2 = d_1 - sigma * sqrt(T);
            
            double V = S * cdf(d_1) - K * exp(-r * T) * cdf(d_2);
            
            return V;
        }
    }
    else{
        double S = (*ptr_underlying).get_SpotPrice();
        double r = (*ptr_underlying).get_r();
        european equivalent_call(ptr_underlying, K, T, position);
        // -------------- Lump dividends --------------
        if (((*ptr_underlying).get_alias_Dividends().get_Type() == 1)) {
            int n = DividendCounter(T, (*ptr_underlying).get_alias_Dividends().get_Next(), (*ptr_underlying).get_alias_Dividends().get_Periods()); // calculates the number of coupon detachments before the option's maturity
            double S_hat = S * pow(1 - (*ptr_underlying).get_alias_Dividends().get_Rate(), n); // S_hat = S * (1-dividend_rate)^n

            return equivalent_call.price() + exp(-r * T) * K - S_hat; // call-put parity for european with underlying with lump payments of dividends
        }
        // -------------- Continuous dividends and no dividend -------------
        else {
            double rate = (*ptr_underlying).get_alias_Dividends().get_Rate();
            double S_hat = S * exp(- rate * T);
            
            return equivalent_call.price() - S_hat + exp(-r * T) * K ; //We notice that if the rate is null, then we have the classical call-put parity.
        }
    }
};

string european::type() const {
    string name = "european " + position;
    return name;
};

void european::replication() const {
    // manage of an option constructed by the default constructor and ptr_underlying not initialized
    if (ptr_underlying == nullptr) {
        cout << "The option has not been initialised (the pointer of the underlying is not initialised). Thus, replication is not possible";
    }

    if (position == "call") {
        european put(ptr_underlying, K, T, "put");
        double r = (*ptr_underlying).get_r();
        
        // ------------ no dividends ----------------
        if ((*ptr_underlying).get_alias_Dividends().get_Type() == 0) {
            cout << "The replication of the buy of this " << this->type() << ", which value is " << this->price() << ", is : ";
            cout << "\n * The buy on the same underlying asset of an " << put;
            cout << "\n * The buy of the underlying asset.";
            cout << "\n * The borrow of " << exp(-r * T) * K << " on the risk free rate market. ";
            cout << "\n The balance of the replication is " << put.price() + (*ptr_underlying).get_SpotPrice() - exp(-r * T) * K << " (which is indeed equal to the spot price of the replicated option).\n\n\n";
        }
        // ------------ continuous dividends ----------------
        if ((*ptr_underlying).get_alias_Dividends().get_Type() == 2) {
            double rate = (*ptr_underlying).get_alias_Dividends().get_Rate();
            
            cout << "The replication of the buy of this " << this->type() << ", which value is " << this->price() << ", is : ";
            cout << "\n * The buy on the same underlying asset of an " << put;
            cout << "\n * The buy of the underlying asset.";
            cout << "\n * The borrow of " << exp(-r * T) * K + (*ptr_underlying).get_SpotPrice() * (1 - exp(-rate * T)) << " on the risk free rate market. ";
            cout << "\n The balance of the replication is " << put.price() + (*ptr_underlying).get_SpotPrice() -( exp(-r * T) * K + (*ptr_underlying).get_SpotPrice() * (1 - exp(-rate * T))) << " (which is indeed equal to the spot price of the replicated option).\n\n\n";
        }
        // ------------ continuous dividends ----------------
        else {
            double rate = (*ptr_underlying).get_alias_Dividends().get_Rate();
            int n = DividendCounter(T, (*ptr_underlying).get_alias_Dividends().get_Next(), (*ptr_underlying).get_alias_Dividends().get_Periods()); // calculates the number of coupon detachments before the option's maturity
            
            cout << "The replication of the buy of this " << this->type() << ", which value is " << this->price() << ", is : ";
            cout << "\n * The buy on the same underlying asset of an " << put;
            cout << "\n * The buy of the underlying asset.";
            cout << "\n * The borrow of " << exp(-r * T) * K + (*ptr_underlying).get_SpotPrice() * (1 - pow(1 - rate, n)) << " on the risk free rate market. ";
            cout << "\n The balance of the replication is " << put.price() + (*ptr_underlying).get_SpotPrice() - (exp(-r * T) * K + (*ptr_underlying).get_SpotPrice() * (1 - pow(1 - rate, n))) << " (which is indeed equal to the spot price of the replicated option).\n\n\n";
        }
    }
    else{
        european call(ptr_underlying, K, T, "call");
        double r = (*ptr_underlying).get_r();

        // ------------- no dividends ----------------
        if ((*ptr_underlying).get_alias_Dividends().get_Type() == 0) {
            cout << "The replication of the buy of this " << this->type() << ", which value is " << this->price() << ", is : ";
            cout << "\n * The buy, on the same underlying asset, of an " << call;
            cout << "\n * The sell of the underlying asset.";
            cout << "\n * The investment of " << exp(-r * T) * K << " on the risk free rate market. ";
            cout << "\n The balance of the replication is " << call.price() - (*ptr_underlying).get_SpotPrice() + exp(-r * T) * K << " (which is indeed equal to the spot price of the replicated option).\n\n\n";
        }
        // ------------- continuous dividends ----------------
        if ((*ptr_underlying).get_alias_Dividends().get_Type() == 2) {
            double rate = (*ptr_underlying).get_alias_Dividends().get_Rate();

            cout << "The replication of the buy of this " << this->type() << ", which value is " << this->price() << ", is : ";
            cout << "\n * The buy, on the same underlying asset, of an " << call;
            cout << "\n * The sell of the underlying asset. (It is a short selling, you will have to pay the continuous dividends)";
            cout << "\n * The investment of " << exp(-r * T) * K + (*ptr_underlying).get_SpotPrice() * (1 - exp(-rate * T)) << " on the risk free rate market. "; // Ke^(-r*t) - S(e^(-rate*T)-1)
            cout << "\n The balance of the replication is " << call.price() - (*ptr_underlying).get_SpotPrice() + exp(-r * T) * K + (*ptr_underlying).get_SpotPrice()*(1-exp(-rate*T)) << " (which is indeed equal to the spot price of the replicated option).\n\n\n";

        }
        // ------------- lump dividends ----------------
        else {
            double rate = (*ptr_underlying).get_alias_Dividends().get_Rate();
            int n = DividendCounter(T, (*ptr_underlying).get_alias_Dividends().get_Next(), (*ptr_underlying).get_alias_Dividends().get_Periods()); // calculates the number of coupon detachments before the option's maturity

            cout << "The replication of the buy of this " << this->type() << ", which value is " << this->price() << ", is : ";
            cout << "\n * The buy, on the same underlying asset, of an " << call;
            cout << "\n * The sell of the underlying asset. (It is a short selling, you will have to pay the continuous dividends)";
            cout << "\n * The investment of " << exp(-r * T) * K + (*ptr_underlying).get_SpotPrice() * (1 - pow(1 - rate, n)) << " on the risk free rate market. "; // Ke^(-r*t) - S(e^(-rate*T)-1)
            cout << "\n The balance of the replication is " << call.price() - (*ptr_underlying).get_SpotPrice() + exp(-r * T) * K + (*ptr_underlying).get_SpotPrice() *(1- pow((1 - rate), n)) << " (which is indeed equal to the spot price of the replicated option).\n\n\n";

        }
    }
};
