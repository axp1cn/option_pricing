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
    cout << "WELCOME to our options pricer !\n";
    cout << "Our program enables you to price basic options (european, american and asian) and even to estimate the future price of any asset you are interested in.\n";
    
    bool program_running = true;
    while (program_running){
        asset new_asset;
        string option_type, action, option_name;
        double price, risk_free_rate;
        bool valid_option_input = false;
        bool valid_action_input = false;
        
        cout << "\nLet's create your own option!\n";
        cout << "\nFirst, enter the risk free rate of your market (in %): ";
        cin >> risk_free_rate;
        new_asset.set_r(risk_free_rate/100);
        cout << "3 option types available:\n";
        cout << "- European options (type eu)\n- American options (type am)\n- Asian options (type as)";
        while (!valid_option_input){
            cout << "\nWhat type of option do you want to price? ";
            cin >> option_type;
            if (option_type == "eu" || option_type == "am" || option_type == "as") {
                valid_option_input = true;
            }
            else {
                cout << "Invalid option type entered. Please try again.\n";
            }
        }
            
        if (option_type=="eu"){
            european opt;
            cin >> opt;
            price = opt.price();
            option_name = opt.type();
        }
        else if (option_type=="am"){
            american opt;
            cin >> opt;
            price = opt.price();
            option_name = opt.type();
        }
        else {
            asian opt;
            cin >> opt;
            price = opt.price();
            option_name = opt.type();
        }
        cout << "\n4 actions available:\n";
        cout << "- Price the option (type p)\n- Estimate the future price of the asset (type e)\n- Generate a new option (type n)\n- Quit our program... (type q)";
        while (!valid_action_input) {
            // Prompt for action (price p or estimate e)
            cout << "\nWhat do you want to do next? ";
            cin >> action;

            if (action == "p") {
                cout << "Our program estimates your " << option_name << " option to be worth " << price << "$.\n";
            }
            else if (action == "e") {
                double time;
                cout << "Future date for the estimatation (in years, > current time): ";
                cin >> time;
                asset asset_estimated = new_asset.Asset_Estimation(time, new_asset.get_r());
                cout << "\nOur program estimates your asset to be worth " << asset_estimated.get_SpotPrice() << " in " << (time-new_asset.get_CurrentTime()) << " years.\n";
            }
            else if (action == "n") {
                break;
            }
            else if (action == "q") {
                program_running = false;
                break;
            } 
            else {
                cout << "Invalid action entered. Please try again.\n";
            }
        }
    }
    return 0;
};


