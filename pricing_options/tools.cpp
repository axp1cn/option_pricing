#include "tools.hpp"
#include <iostream>
#include <random>

double cdf(double& d) {
    std::default_random_engine generator(time(0));
    std::normal_distribution<double> distribution(0.0, 1.0);

    int n = 100000;

    double sum = 0;
    for (int i = 0; i < n; i++) {
        double x = distribution(generator);
        if (x < d) {
            sum++;
        };
    };

    return sum/n;
};

void Pricing_Spot(Eigen::VectorXd* SpotPrice, double Strike, Eigen::VectorXd* FuturValuation, int taille, double delta, double Riskfree){
    double spot;
    for(int i=0; i<taille; i++){
        spot = (*SpotPrice)(i);
        (*SpotPrice)(i) = fmax(fmax(spot - Strike, 0.0), (*FuturValuation)(i) * exp(-delta*Riskfree));
    }
    //delete FuturValuation; // We delete the useless Vector of FuturValuation dynamically created by the RegLin function
}

void Pricing_Spot_bis(Eigen::VectorXd* SpotPrice, double Strike, Eigen::VectorXd* FuturValuation, int taille, double delta, double Riskfree){
    double spot;
    for(int i=0; i<taille; i++){
        spot = (*SpotPrice)(i);
        (*SpotPrice)(i) = fmax(fmax(Strike - spot, 0.0), (*FuturValuation)(i) * exp(-delta*Riskfree));
    }
    //delete FuturValuation; // We delete the useless Vector of FuturValuation dynamically created by the RegLin function
}

double Mean(Eigen::VectorXd Vec, int taille){
    double mean = 0;
    for(int i=0;i<taille;i++){
        mean += Vec(i);
    }
    double taille_bis = taille;
    return mean/taille_bis;
}

double Max_Row(Eigen::VectorXd* Row, int taille){
    double MadMax = (*Row)(0);
    for(int i=0; i<taille; i++){
        MadMax = fmax(MadMax, (*Row)(i));
    }
    return MadMax;
}

void Matrix_Atribution(Eigen::MatrixXd* Matrix, Eigen::VectorXd* Vector, int taille, int index_col){
    for(int i=0; i<taille; i++){
        (*Matrix)(i, index_col) = (*Vector)(i);
    }
}

void Actualization_Matrix(Eigen::MatrixXd* Matrix, double Riskfree, double Delta, int col_len, int row_len){
    Eigen::VectorXd Vec(row_len);
    Eigen::VectorXd* Ptr_Vec = &Vec;
    for(int i=0; i<col_len; i++){
        Vec = (*Matrix).col(i);
        Actualization_Vector(Ptr_Vec, -(Riskfree * Delta * (i+1)), row_len);
        Matrix_Atribution(Matrix, Ptr_Vec, row_len, i);
    }
}

void Actualization_Vector(Eigen::VectorXd* Vector, double factor, int taille){
    for(int i =0; i<taille; i++){
        (*Vector)(i) = (*Vector)(i) * exp(factor);
    }
}


Eigen::MatrixXd* Asset_Simulation(double S_0, double Vol, int noeuds, int branche, double delta, double riskfree){

    std::mt19937 generator(time(0));
    std::normal_distribution<double> simulator(0, 1);

    Eigen::MatrixXd* MatrixBS = new Eigen::MatrixXd(branche, noeuds);

    double spotprice;
    double r = riskfree;
    double W;

    for(int j=0; j<branche; j++){
        spotprice = S_0;

        for(int i=0; i<noeuds; i++){
        W = sqrt(delta) * simulator(generator);
        spotprice *= exp( ((r - pow(Vol, 2)/2) * (delta)) + Vol*W);
        (*MatrixBS)(j,i) = spotprice;
        }

    }
    return MatrixBS;
}


Eigen::VectorXd* Vec_square(const Eigen::VectorXd* Vec){
    int taille = (*Vec).rows();
    Eigen::VectorXd* Square = new Eigen::VectorXd(taille);
    (*Square) = ((*Vec) * (*Vec).transpose()).diagonal();
    return Square;
}



Eigen::VectorXd* RegLin_Train(Eigen::VectorXd* Label, Eigen::VectorXd* Data){
    
    Eigen::VectorXd* X_square = Vec_square(Data);

    int Length_data = (*Data).rows();

    Eigen::MatrixXd Design_matrix(Length_data, 3);

    for(int i=0; i<Length_data; i++){
        Design_matrix(i,0) = 1;
    }

    Design_matrix.col(1) = *Data;
    Design_matrix.col(2) = *X_square;

    delete X_square; //delete useless memory vector

    Eigen::VectorXd Coefficients = Eigen::VectorXd(3);

    Coefficients = (Design_matrix.transpose() * Design_matrix).inverse() * (Design_matrix.transpose() * (*Label));

    // Estimation following reg lin 2nd order

    Eigen::VectorXd* Estimated_Futur_Valuation = new Eigen::VectorXd(Length_data);

    *Estimated_Futur_Valuation = Design_matrix * Coefficients;


    return(Estimated_Futur_Valuation);

}
