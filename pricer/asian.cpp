#include <iostream>
#include "asian.hpp"
#include <random>
#include <string>
#include <cmath>
using namespace std;

// Destructor
asian::~asian() {};

// Advanced functions
double asian::price() const {
    // manage of an option constructed by the default constructor and underlying_asset not initialized
    if (underlying_asset == nullptr) {
        cout << "The option has not been initialised (the pointer of the underlying is not initialised). Thus, the price doesn't exist.";
        return 0;
    }
    
    if (position == "call") {
        // to valuate an asian (arithmetic) call, we need to compute the average of the price (S_mean) of the underlying asset
        double spot = (*underlying_asset).get_SpotPrice();
        double vol = (*underlying_asset).get_Volatility();
        
        if ((*underlying_asset).get_alias_Dividends().get_Type() == 0) {
            double V = 0;
            default_random_engine generator(time(0));
            normal_distribution<double> distribution(0.0, 1.0);
            
            int N = 10000; //number of price paths
            int n = 1000; //number of simulation of the borwnian motion for one price paths (from 0 till maturity)
            
            
            for (int i = 0; i < N; i++) {
                double S_mean = spot / n;
                double W = 0; //brownian motion
                
                for (int j = 0; j < n; j++) {
                    double delta_W = distribution(generator) * sqrt(maturity / n); // because W_{t+h}-W_{t} follows N(0,h)
                    
                    W += delta_W;
                    S_mean += spot * exp(((*underlying_asset).get_r() - pow(vol, 2) / 2) * maturity + vol * W) / n;
                };
                
                if (S_mean > strike) {
                    V += S_mean - strike;
                }
                
            };
            return V / N;
        }
        else {
            cout << "We do not price an asian option where underlying asset pays dividends.\n";
            return 0;
        }
    }
    else {
        // to valuate an asian (arithmetic) put, we need to compute the average of the price (S_mean) of the underlying asset
        double spot = (*underlying_asset).get_SpotPrice();
        double vol = (*underlying_asset).get_Volatility();
        double r = (*underlying_asset).get_r();

        if ( ((*underlying_asset).get_alias_Dividends().get_Type()!=1) &&((*underlying_asset).get_alias_Dividends().get_Type() != 2)){
            double V = 0;
            default_random_engine generator(time(0));
            normal_distribution<double> distribution(0.0, 1.0);

            int N = 10000; //number of price paths
            int n = 1000; //number of simulation of the borwnian motion for one price paths (from 0 till maturity)


            for (int i = 0; i < N; i++) {
                double S_mean = spot / n;
                double W = 0; //brownian motion
                
                for (int j = 0; j < n; j++) {
                    double delta_W = distribution(generator) * sqrt(maturity / n); // because W_{t+h}-W_{t} follows N(0,h)

                    W += delta_W;
                    S_mean += spot * exp((r - pow(vol, 2) / 2) * maturity + vol * W) / n;
                };

                if (strike > S_mean) {
                    V += strike - S_mean;
                }

            };
            return V / N;
        }
        else {
            cout << "We do not price an asian option where underlying asset pays dividends.\n";
            return 0;
        }
    }
};

string asian::type() const {
    string name = "asian " + position;
    return name;
};
