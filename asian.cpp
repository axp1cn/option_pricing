#include <iostream>
#include "asian.hpp"
#include <random>
#include <string>
#include <cmath>
using namespace std;

// Destructor
asian::~asian() {};

// Member function
double asian::price() const {
    // manage of an option constructed by the default constructor and ptr_underlying not initialized
    if (ptr_underlying == nullptr) {
        cout << "The option has not been initialised (the pointer of the underlying is not initialised). Thus, the price doesn't exist.";
        return 0;
    }
    
    if (position == "call") {
        // to valuate an asian (arithmetic) call, we need to compute the average of the price (S_mean) of the underlying asset
        double S = (*ptr_underlying).get_SpotPrice();
        double sigma = (*ptr_underlying).get_Volatility();
        
        if ((*ptr_underlying).get_alias_Dividends().get_Type() == 0) {
            double V = 0;
            std::default_random_engine generator(time(0));
            std::normal_distribution<double> distribution(0.0, 1.0);
            
            int N = 10000; //number of price paths
            int n = 1000; //number of simulation of the borwnian motion for one price paths (from 0 till maturity T)
            
            
            for (int i = 0; i < N; i++) {
                double S_mean = S / n;
                double W = 0; //brownian motion
                
                for (int j = 0; j < n; j++) {
                    double delta_W = distribution(generator) * sqrt(T / n); // because W_{t+h}-W_{t} follows N(0,h)
                    
                    W += delta_W;
                    S_mean += S * exp(((*ptr_underlying).get_r() - pow(sigma, 2) / 2) * T + sigma * W) / n;
                };
                
                if (S_mean > K) {
                    V += S_mean - K;
                }
                
            };
            return V / N;
        }
        else {
            cout << "We do not price an asian option where underlying asset pays dividends." << endl;
            return 0;
        }
    }
    else {
        // to valuate an asian (arithmetic) put, we need to compute the average of the price (S_mean) of the underlying asset
        double S = (*ptr_underlying).get_SpotPrice();
        double sigma = (*ptr_underlying).get_Volatility();
        double r = (*ptr_underlying).get_r();

        if ( ((*ptr_underlying).get_alias_Dividends().get_Type()!=1) &&((*ptr_underlying).get_alias_Dividends().get_Type() != 2)){
            double V = 0;
            std::default_random_engine generator(time(0));
            std::normal_distribution<double> distribution(0.0, 1.0);

            int N = 10000; //number of price paths
            int n = 1000; //number of simulation of the borwnian motion for one price paths (from 0 till maturity T)


            for (int i = 0; i < N; i++) {
                double S_mean = S / n;
                double W = 0; //brownian motion
                
                for (int j = 0; j < n; j++) {
                    double delta_W = distribution(generator) * sqrt(T / n); // because W_{t+h}-W_{t} follows N(0,h)

                    W += delta_W;
                    S_mean += S * exp((r - pow(sigma, 2) / 2) * T + sigma * W) / n;
                };

                if (K > S_mean) {
                    V += K - S_mean;
                }

            };
            return V / N;
        }
        else {
            cout << "We do not price an asian option where underlying asset pays dividends." << endl;
            return 0;
        }
    }
};

string asian::type() const {
    string name = "asian " + position;
    return name;
}

void asian::replication() const {
    cout << "Replication not implemented in this program. (An " << this->type() << " is not easily replicable)." << endl;
}

