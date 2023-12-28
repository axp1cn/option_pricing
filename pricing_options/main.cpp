#include "asset.hpp"
#include "tools.hpp"
#include "european.hpp"
#include "asian.hpp"
#include "american.hpp"
#include <iostream>
#include <iomanip>
#include <string>

int main(){
    std::cout << "WELCOME to our options pricer !\n";
    std::cout << "Our program enables you to price basic options (european, american and asian) and even to estimate the future price of any asset you are interested in.\n";
    
    bool program_running = true;
    while (program_running){
        asset new_asset;
        string option_type, action, option_name;
        double price, risk_free_rate;
        bool valid_option_input = false;
        bool valid_action_input = false;
        
        std::cout << "\nLet's create your own option!\n";
        std::cout << "\nFirst, enter the risk free rate of your market (in %): ";
        std::cin >> risk_free_rate;
        new_asset.set_r(risk_free_rate/100);
        std::cout << "3 option types available:\n";
        std::cout << "- European options (type eu)\n- American options (type am)\n- Asian options (type as)";
        while (!valid_option_input){
            std::cout << "\nWhat type of option do you want to price? ";
            std::cin >> option_type;
            if (option_type == "eu" || option_type == "am" || option_type == "as") {
                valid_option_input = true;
            }
            else {
                cout << "Invalid option type entered. Please try again.\n";
            }
        }
            
        if (option_type=="eu"){
            european opt;
            std::cin >> opt;
            price = opt.price();
            option_name = opt.type();
        }
        else if (option_type=="am"){
            american opt;
            std::cin >> opt;
            price = opt.price();
            option_name = opt.type();
        }
        else {
            asian opt;
            std::cin >> opt;
            price = opt.price();
            option_name = opt.type();
        }
        std::cout << "\n4 actions available:\n";
        std::cout << "- Price the option (type p)\n- Estimate the future price of the asset (type e)\n- Generate a new option (type n)\n- Quit our program... (type q)";
        while (!valid_action_input) {
            // Prompt for action (price p or estimate e)
            std::cout << "\nWhat do you want to do next? ";
            std::cin >> action;

            if (action == "p") {
                std::cout << "Our program estimates your " << option_name << " option to be worth " << price << "$.\n";
            }
            else if (action == "e") {
                double time;
                std::cout << "Future date for the estimatation (in years, > current time): ";
                std::cin >> time;
                asset asset_estimated = new_asset.Asset_Estimation(time, new_asset.get_r());
                std::cout << "\nOur program estimates your asset to be worth " << asset_estimated.get_SpotPrice() << "in " << time << " years.\n";
            }
            else if (action == "n") {
                break;
            }
            else if (action == "q") {
                program_running = false;
                break;
            } 
            else {
                std::cout << "Invalid action entered. Please try again.\n";
            }
        }
    }
    return 0;
}
/*
    // Default values for options and underlying
    double strike = 40.0;
    double spot = 50.0;
    double maturity = 1;
    double vol = 0.30;
    string position = "put";

    // Assets construction
    asset new_asset;
    new_asset.set_SpotPrice(spot);
    new_asset.set_Volatility(vol);
    new_asset.set_CurrentTime(0.0) ;

    new_asset.get_alias_Dividends().set_Type(0);
    new_asset.set_r(0.05);

    // Options construction
    asset* ptr_underlying = &new_asset;

    european european_put(ptr_underlying, strike, maturity, position);
    std::cout << european_put << endl;
    european_put.replication();

    asian asian_put(ptr_underlying, strike, maturity, position);
    std::cout << asian_put << endl;
    asian_put.replication();

    american american_put(ptr_underlying, strike, maturity, position);
    std::cout << american_put << endl;

    // Option with underlying with dividend
    asset asset_div(new_asset);
    asset_div.get_alias_Dividends().set_Type(1);
    asset_div.get_alias_Dividends().set_Rate(0.0125);
    asset_div.get_alias_Dividends().set_Periods(0.25);
    asset_div.get_alias_Dividends().set_Next(0.1666);

    asset* ptr_underlying_div = &asset_div;

    european european_call_div(ptr_underlying_div, strike, maturity, "call");
    std::cout << european_call_div << endl;
*/


