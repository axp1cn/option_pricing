#include "asset.hpp"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>
#include <string>
// #include <termcolor/termcolor.hpp>

//************** Dividend functions **************

// Primary functions

dividend::dividend(){
    this->Type = 0;
    this->Rate = 0.0;
    this->Periods = 0.0;
    this->Next = 0.0;
}

dividend::dividend(int DividendsType, double DividendsRate, double DividendsPeriods, double NextDividend){
    this->Type = DividendsType;
    this->Rate = DividendsRate;
    this->Periods = DividendsPeriods;
    this->Next = fmod(NextDividend, DividendsPeriods);
}

dividend::dividend(const dividend& Div){
    this->Type = Div.Type;
    this->Rate = Div.Rate;
    this->Periods = Div.Periods;
    this->Next = Div.Next;
}


// Basic functions

int dividend::get_Type() const{
    return Type;
}

double dividend::get_Rate() const{
    return Rate;
}

double dividend::get_Periods() const{
    return Periods;
}

double dividend::get_Next() const{
    return Next;
}

void dividend::set_Type(int Type){
    if(Type != 0 && Type !=1 && Type !=2){
        this->Type = 0; // Default value
    }else{
        this->Type = Type;
    }
}

void dividend::set_Rate(double Rate){
    this->Rate = Rate;
}

void dividend::set_Periods(double Periods){
    this->Periods = Periods;
}

void dividend::set_Next(double Next){
    this->Next = fmod(Next, this->Periods);
}

void dividend::operator=(const dividend& Div){
    this->Type = Div.Type;
    this->Rate = Div.Rate;
    this->Periods = Div.Periods;
    this->Next = Div.Next;
}

//************** Asset functions **************

// Primary functions
double asset::r = 0.05;

asset::asset(){
    dividend Div;

    this->CurrentTime = 0.0;
    this->SpotPrice = 0.0;
    this->Volatility = 0.0;
    this->Dividends = Div;
}

asset::asset(double CurrentTime, double SpotPrice, double Volatility, dividend Dividends){
    this->CurrentTime = CurrentTime;
    this->SpotPrice = SpotPrice;
    this->Volatility = Volatility;
    this->Dividends = Dividends;
}

asset::asset(const asset& Asset1){
    this->CurrentTime = Asset1.CurrentTime;
    this->SpotPrice = Asset1.SpotPrice;
    this->Volatility = Asset1.Volatility;
    this->Dividends = Asset1.Dividends;
}

asset::~asset(){}


// Basic functions

double asset::get_CurrentTime() const{
    return CurrentTime;
}

double asset::get_SpotPrice() const{
    return SpotPrice;
}

double asset::get_Volatility() const{
    return Volatility;
}

dividend& asset::get_alias_Dividends(){
    return this->Dividends;
}

dividend asset::get_Dividends() const{
    return this->Dividends;
}

double asset::get_r() const { return r; };


void asset::set_CurrentTime(double CurrentTime){
    this->CurrentTime = CurrentTime;
}

void asset::set_SpotPrice(double SpotPrice){
    this->SpotPrice = SpotPrice;
}

void asset::set_Volatility(double Volatility){
    this->Volatility = Volatility;
}

void asset::set_Dividends(dividend Dividends){
    this->Dividends = Dividends;
}

void asset::set_r(double r) { asset::r = r; };

void asset::operator=(const asset& Asset1){
    this->CurrentTime = Asset1.CurrentTime;
    this->SpotPrice = Asset1.SpotPrice;
    this->Volatility = Asset1.Volatility;
    this->Dividends = Asset1.Dividends;
}

//Advanced functions


void asset::Asset_Actualization(double NewTime, double SpotPrice){
    double OldTime = this->CurrentTime;

    this->CurrentTime = NewTime;
    this->SpotPrice = SpotPrice;

    //Actualization of dividends
    double OldNext = this->Dividends.get_Next();
    double Periods = this->Dividends.get_Periods();
    double Delta = NewTime - OldTime;


    if(this->Dividends.get_Type() != 0 && this->Dividends.get_Periods() != 0){
    //double NextDividend = (Periods - Delta%Periods + OldNext)%Periods;
    double NextDividend = fmod((Periods - fmod(Delta, Periods) + OldNext),Periods);

    if(NextDividend == 0.0){
        this->Dividends.set_Next(Periods);
    }else{
        this->Dividends.set_Next(NextDividend);
    }
    }


}

asset asset::Asset_Estimation(double Time, double RiskFreeRate) const{
    asset AssetEstimate = *this;
    // Computation of the expected spot price of the asset at time t=Time
    // Following the dividends type, the pricing is different
    dividend DivAsEs = AssetEstimate.get_Dividends();
    int DivType = DivAsEs.get_Type();
    double ExpectedPrice;
    double OldTime = AssetEstimate.get_CurrentTime();
    double Next = DivAsEs.get_Next();
    double DivRate = DivAsEs.get_Rate();
    double Periods = DivAsEs.get_Periods();
    double PriceOldTime = AssetEstimate.get_SpotPrice();

    if(DivType == 0){
        ExpectedPrice = PriceOldTime * exp( RiskFreeRate * (Time - OldTime) );
    }else if(DivType == 1){
        int DivCount = DividendCounter( Time - OldTime, Next, Periods);
        ExpectedPrice = PriceOldTime * exp( RiskFreeRate * (Time - OldTime)) * pow((1 - DivRate), DivCount);
    }else{
        ExpectedPrice = PriceOldTime * exp( (RiskFreeRate - DivRate ) * (Time - OldTime));
    }

    AssetEstimate.Asset_Actualization(Time, ExpectedPrice);
    return AssetEstimate;
}

//************** Tools functions **************

int DividendCounter(double Delta, double Next, double Periods){
    return (Delta + Periods - Next) / Periods;
}


std::istream& operator>>(std::istream& input, asset& Asset){
    std::cout << "\nAsset builder:\n";
    std::cout << "Enter the current time (in years): ";
    input >> Asset.CurrentTime;
    std::cout << "Enter the asset's current spot price: ";
    input >> Asset.SpotPrice;
    std::cout << "Enter the asset's volatility: ";
    input >> Asset.Volatility;

    // Dividends settings
    std::cout << "\nDividends settings:\n";
    std::cout << "3 dividends types available (for european options only!):\n";
    std::cout << "- Asset without dividends (type 0)\n- Asset with lump dividend payments (type 1)\n- Asset with continuous dividend payments (type 2)\n";
    std::cout << "What type of dividends do you want? ";
    int DividendsType;
    double Next = 0;
    double Periods = 0;
    double RateDiv = 0;
    input >> DividendsType;
    if (!(DividendsType==0 || DividendsType==1 || DividendsType==2)) {
        std::cout << "This type isn't available, please choose between:/n-type 0 : no dividends/n-type 1 : lump dividend/n-type 2 : continuous dividend";
    }
    if(DividendsType != 0){
        std::cout << "Enter the dividends' rate (in %): ";
        input >> RateDiv;
        std::cout << "Enter the dividends' period (in years): ";
        input >> Periods;
        std::cout << "Enter the next dividends term (in years): ";
        input >> Next;
    }

    dividend DivG(DividendsType, RateDiv/100, Periods, Next);
    Asset.set_Dividends(DivG);
    std::cout << "\nYour asset have been initialized.\n";
    return input;
}





