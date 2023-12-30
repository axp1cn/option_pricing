#include "american.hpp"
#include "tools.hpp"
using namespace std;

// Destructor
american::~american(){};

// Member function
double american::price() const{
    if (underlying_asset == nullptr) {
        cout << "The option has not been initialised (the pointer of the underlying is not initialised). Thus, the price doesn't exist.\n";
        return 0;
    }
    
    if (position == "call") {
        if((*underlying_asset).get_alias_Dividends().get_Type() == 0){
            cout << "(Pricing may take a few minutes...)\n" << endl;

            double S_0 = (*underlying_asset).get_SpotPrice();
            double Vol = (*underlying_asset).get_Volatility();
            double Riskfree = (*underlying_asset).get_r();

            double Maturity = this->maturity;
            double Strike = this->strike;

            // Trajectories simulation of the underlying
            int noeuds = 1000;
            int branche = 10000;
            double noeuds_bis = noeuds;
            double delta_t = Maturity/noeuds_bis;
            
            Eigen::MatrixXd* Ptr_Mat_Simulation = Asset_Simulation(S_0, Vol, noeuds, branche, delta_t, Riskfree);
            
            
            // We declare all our pointers and Valuation variables
            
            // Pointer to the current column of the trajectories matrix
            Eigen::VectorXd* Ptr_Vec_SpotPrice;
            // Pointer to the future valuation of the option (so if we are at t this pointer is toward V_{t+1})
            Eigen::VectorXd* Ptr_Vec_Futur_Valuation;
            // Fot the iteration corresponding to the maturity time, the futur valuation of the Option is 0 because the option end now
            Eigen::VectorXd Zero_Val = Eigen::VectorXd::Zero(branche);
            // Pointer tot the futur estimated valuation of the Option (so if we are at t this pointer is toward E[V_{t+1}|S_t])
            Eigen::VectorXd* Estimated_Futur_Valuation = &Zero_Val;
            
            Eigen::VectorXd Copy_Vec(branche);
            Eigen::VectorXd Copy_Vec_bis(branche);
            
            for(int i=1; i<=noeuds; i++){
                
                Copy_Vec = (*Ptr_Mat_Simulation).col(noeuds - i);
                
                // We point the SpotPrice trajectory so this pointer points basically toward S_t
                Ptr_Vec_SpotPrice = &Copy_Vec;
                
                // From the last iteration we have the estimated Futur_Valuation E[V_{t+1}|S_t], so we valuate the asset V_t by the well known formula V_t = max((S_t - strike)^+ , E[V_{t+1}|S_t])
                Pricing_Spot(Ptr_Vec_SpotPrice, Strike, Estimated_Futur_Valuation, branche, delta_t, Riskfree);
                
                if(i > 1){
                    delete Estimated_Futur_Valuation; // We delete the useless Vector of FuturValuation dynamically created by the RegLin function
                }
                
                Matrix_Atribution(Ptr_Mat_Simulation, Ptr_Vec_SpotPrice, branche, noeuds-i);
                
                if(i == noeuds){
                    break; // We break if we have reach the first column
                }
                
                
                Copy_Vec_bis = Copy_Vec;
                // The SpotPrice has now the value of the Option so we give this value to the pointer Future Valuation
                Ptr_Vec_Futur_Valuation = &Copy_Vec_bis;
                Copy_Vec = (*Ptr_Mat_Simulation).col(noeuds - i - 1);
                
                // Here we estimate following the current SpotPrice of the underlying the estimated futur Valuation of the option. For that we used a second order polynomial regression on SpotPrice Trajectories and FuturPrice Valuated
                Estimated_Futur_Valuation = RegLin_Train(Ptr_Vec_Futur_Valuation, Ptr_Vec_SpotPrice);
            }
            // Now we have the Ptr_Mat_Simulation that is all the price of the Option following each nodes of times
            double V_0 = fmax(Mean((*Ptr_Mat_Simulation).col(1), branche)*exp(-Riskfree*delta_t), fmax((S_0-Strike), 0.0));
            
            if(Mean((*Ptr_Mat_Simulation).col(1), branche)*exp(-Riskfree*delta_t) <= fmax((S_0-Strike), 0.0)){
                delete Ptr_Mat_Simulation;
                return V_0; // Here we have that the price of the option is greater at emission than it could be (by simulations) after (it means that the option have to be exercise at emission)
            }
            
            
            // We need to discounted this matrix to be able to compare each price and determine the best strategy for each trajectory
            Actualization_Matrix(Ptr_Mat_Simulation, Riskfree, delta_t, noeuds, branche);
            // Now we cumul the all optimal exercice payoff
            Eigen::VectorXd Curr_row(noeuds);
            // Here we declare a vector that will take each trajectory
            Eigen::VectorXd* Ptr_Curr_row = &Curr_row;
            
            double Mean_value = 0;
            for(int i=0; i<branche; i++){
                Curr_row = (*Ptr_Mat_Simulation).transpose().col(i);
                Mean_value += (*Ptr_Curr_row)(0); //Mean_value += Max_Row(Ptr_Curr_row, noeuds);
            }
            double branche_d = branche; // Some specifications to avoid euclidian division
            delete Ptr_Mat_Simulation;
            return (1.0/branche_d)*Mean_value;
        }else{
            cout << "We do not price an american option where underlying asset pays dividends.\n";
            return 0;
        }
    }
    else {
        if (((*underlying_asset).get_alias_Dividends().get_Type()!=1) && ((*underlying_asset).get_alias_Dividends().get_Type() != 2)){
        cout << "(Pricing may take a few minutes...)\n";

        double S_0 = (*underlying_asset).get_SpotPrice();
        double Vol = (*underlying_asset).get_Volatility();
        double Riskfree = (*underlying_asset).get_r();

        double Maturity = this->maturity;
        double Strike = this->strike;

        // Trajectories simulation of the underlying
        int noeuds = 1000;
        int branche = 10000;
        double noeuds_bis = noeuds;
        double delta_t = Maturity/noeuds_bis;

        Eigen::MatrixXd* Ptr_Mat_Simulation = Asset_Simulation(S_0, Vol, noeuds, branche, delta_t, Riskfree);
        

        // We declare all our pointers and Valuation variables
            
        // Pointer to the current column of the trajectories matrix
        Eigen::VectorXd* Ptr_Vec_SpotPrice;
        

        // Pointer to the futur valuation of the Option (so if we are at t this pointer is toward V_{t+1})
        Eigen::VectorXd* Ptr_Vec_Futur_Valuation;
        // Fot the iteration corresponding to the maturity time, the futur valuation of the Option is 0 because the option end now
        Eigen::VectorXd Zero_Val = Eigen::VectorXd::Zero(branche);
        // Pointer tot the futur estimated valuation of the Option (so if we are at t this pointer is toward E[V_{t+1}|S_t])
        Eigen::VectorXd* Estimated_Futur_Valuation = &Zero_Val;

        Eigen::VectorXd Copy_Vec(branche);
        Eigen::VectorXd Copy_Vec_bis(branche);

        for(int i=1; i<=noeuds; i++){
            Copy_Vec = (*Ptr_Mat_Simulation).col(noeuds - i);
            
            // We point the SpotPrice trajectory so this pointer points basically toward S_t
            Ptr_Vec_SpotPrice = &Copy_Vec;

            // From the last iteration we have the estimated Futur_Valuation E[V_{t+1}|S_t], so we valuate the asset V_t by the well known formula V_t = max((S_t - strike)^+ , E[V_{t+1}|S_t])
            Pricing_Spot_bis(Ptr_Vec_SpotPrice, Strike, Estimated_Futur_Valuation, branche, delta_t, Riskfree);

            if(i > 1){
                delete Estimated_Futur_Valuation; // We delete the useless Vector of FuturValuation dynamically created by the RegLin function
            }

            Matrix_Atribution(Ptr_Mat_Simulation, Ptr_Vec_SpotPrice, branche, noeuds-i);

            if(i == noeuds){
                break; // We break if we have reach the first column
            }

            Copy_Vec_bis = Copy_Vec;
            // The SpotPrice has now the value of the Option so we give this value to the pointer Futur Valuation
            Ptr_Vec_Futur_Valuation = &Copy_Vec_bis;

            Copy_Vec = (*Ptr_Mat_Simulation).col(noeuds - i - 1);

            // Here we estimate following the current SpotPrice of the underlying the estimated futur Valuation of the option. For that we used a second order polynomial regression on SpotPrice Trajectories and FuturPrice Valuated
            Estimated_Futur_Valuation = RegLin_Train(Ptr_Vec_Futur_Valuation, Ptr_Vec_SpotPrice);
        }
            
        // Now we have the Ptr_Mat_Simulation that is all the price of the Option following each nodes of times
        double V_0 = fmax(Mean((*Ptr_Mat_Simulation).col(1), branche)*exp(-Riskfree*delta_t), fmax((Strike - S_0), 0.0));
        
        if(Mean((*Ptr_Mat_Simulation).col(1), branche)*exp(-Riskfree*delta_t) <= fmax((Strike - S_0), 0.0)){
            delete Ptr_Mat_Simulation;
            return V_0; // Here we have that the price of the option is greater at emission than it could be (by simulations) after (it means that the option have to be exercise at emission)
        }
        

        // We need to discounted this matrix to be able to compare each price and determine the best strategy for each trajectory
        Actualization_Matrix(Ptr_Mat_Simulation, Riskfree, delta_t, noeuds, branche);
        // Now we cumul the all optimal exercice payoff
        Eigen::VectorXd Curr_row(noeuds);
        // Here we declare a vector that will take each trajectory
        Eigen::VectorXd* Ptr_Curr_row = &Curr_row;
            
        double Mean_value = 0;
        for(int i=0; i<branche; i++){
            Curr_row = (*Ptr_Mat_Simulation).transpose().col(i);
            Mean_value += (*Ptr_Curr_row)(0); //Mean_value += Max_Row(Ptr_Curr_row, noeuds);
        }
        double branche_d = branche; // some specifications to avoid euclidian division
        delete Ptr_Mat_Simulation;
        return (1.0/branche_d)*Mean_value;
        }
        else{
            cout << "\nWe do not price an american option where underlying asset pays dividends.\n";
            return 0;
        }
    }
};

string american::type() const {
    string name = "american " + position;
    return name;
};
