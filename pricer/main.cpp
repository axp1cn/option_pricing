#include "asset.hpp"
#include "tools.hpp"
#include "european.hpp"
#include "asian.hpp"
#include "american.hpp"
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

int main(){
    cout << "WELCOME to our pricer !\n";
    cout << "Let's introduce you some features of our program :\n";
    // Default values for options and underlying
    double K = 40.0;
    double S = 50.0;
    double T = 1;
    double sigma = 0.30;
    string position = "put";

    // Assets construction
    asset my_asset;
    my_asset.set_SpotPrice(S);
    my_asset.set_Volatility(sigma);
    my_asset.set_CurrentTime(0.0) ;

    my_asset.get_alias_Dividends().set_Type(0);
    my_asset.set_r(0.05);

    // Options construction
    asset* ptr_underlying = &my_asset;

    european my_european_put(ptr_underlying,K,T,position);
    cout << my_european_put << endl;
    my_european_put.replication();

    asian my_asian_put(ptr_underlying, K, T, position);
    cout << my_asian_put << endl;
    my_asian_put.replication();

    american my_american_put(ptr_underlying, K, T, position);
    cout << my_american_put << endl;

    // Option with underlying with dividend
    asset my_asset_div(my_asset);
    my_asset_div.get_alias_Dividends().set_Type(1);
    my_asset_div.get_alias_Dividends().set_Rate(0.0125);
    my_asset_div.get_alias_Dividends().set_Periods(0.25);
    my_asset_div.get_alias_Dividends().set_Next(0.1666);

    asset* ptr_underlying_div = &my_asset_div;

    european my_call_div(ptr_underlying_div, K, T, "call");
    cout << my_call_div << endl;

    //---------- Initialization of an option and its underlying by the user ----------
    cout << "\nCreate your own option :";
    european euro;
    cin >> euro;

    std::cout << "\nEnter the risk free rate of your market : ";
    double risk_free_rate;
    std::cin >> risk_free_rate;
    my_asset.set_r(risk_free_rate);

    
    cout << euro << endl;
    
    // To illustrate the management of a display of an option created by default
    //european_put default_put;
    //cout << default_put << endl;

    // To illustrate the estimation of the asset

    cout << "\n ================================= \n Estimation of the futur price of your asset : " << my_asset.get_AssetName() << endl;
    double time;
    cout << "Date for the estimatation (in years) : " << endl;
    cin >> time;
    if(time < my_asset.get_CurrentTime()){
        cout <<"Invalid date" << endl;
        time = my_asset.get_CurrentTime();
    }
    asset asset_estimated = my_asset.Asset_Estimation(time, my_asset.get_r());
    cout << "Asset estimated : " << asset_estimated << endl;

    return 0;
}
