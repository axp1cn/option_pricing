#pragma once
#include <string>
#include <iostream>
#include <random>
#include <cmath>
#include <iomanip>
#include <limits>
#include <eigen3/Eigen/Dense>

double cdf(double& d); //cdf = cumulative distribution function
int DividendCounter(double Delta, double Next, double Periods);
char* NameCopie(const char chaine[20]);
int euclidian_division(double x, double y);
double modulo(double x, double y);

Eigen::VectorXd* RegLin_Train(Eigen::VectorXd* Label, Eigen::VectorXd* Data);
char* transformation(char chaine[20]);
char* copieee(const char chaine[20]);
double* simulation_asset(double S_0, double Sigma, int noeuds, double delta, double riskfree);
void Pricing_Spot(Eigen::VectorXd* SpotPrice, double Strike, Eigen::VectorXd* FuturValuation, int taille, double delta, double Riskfree);
void Pricing_Spot_bis(Eigen::VectorXd* SpotPrice, double Strike, Eigen::VectorXd* FuturValuation, int taille, double delta, double Riskfree);
double Max_Row(Eigen::VectorXd* Row, int taille);
void Matrix_Atribution(Eigen::MatrixXd* Matrix, Eigen::VectorXd* Vector, int taille, int index_col);
void Actualization_Vector(Eigen::VectorXd* Vector, double factor, int taille);
void Actualization_Matrix(Eigen::MatrixXd* Matrix, double Riskfree, double Delta, int col_len, int row_len);
Eigen::VectorXd* Vec_square(const Eigen::VectorXd* Vec);
Eigen::MatrixXd* Asset_Simulation(double S_0, double Sigma, int noeuds, int branche, double delta, double riskfree);
double Mean(Eigen::VectorXd Vec, int taille);

