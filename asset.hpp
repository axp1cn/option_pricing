#pragma once
#include <iostream>

int DividendCounter(double Delta, double Next, double Periods);
char* NameCopie(const char chaine[20]);
int euclidian_division(double x, double y);
double modulo(double x, double y);

class dividend {
protected:
    int Type;
    /*  An asset without dividends =0;
        An asset with lump payment of dividends =1;
        An asset with continuous dividend payment =2; */

    double Rate;

    double Periods;
    // The period between two dividend's payments for an asset with lump payment of dividends

    double Next;
    // Time before the next dividend's payment for an asset with lump payment of dividends

public:
    //************** Primary functions **************
    dividend();
    dividend(int DividendsType, double DividendsRate, double DividendsPeriods, double NextDividend);
    dividend(const dividend&);
    ~dividend(){};


    //************** Basic functions **************
    //*** Getter ***
    int get_Type() const;
    double get_Rate() const;
    double get_Periods() const;
    double get_Next() const;

    //*** Setter ***
    void set_Type(int);
    void set_Rate(double);
    void set_Periods(double);
    void set_Next(double);

    //*** Overload assignment operator ***
    void operator=(const dividend&);

};


class asset {
protected:
    //Asset Name
    char* AssetName; //We limit the AssetName at 19 characters


    //************** Basic features **************
    double CurrentTime;

    double SpotPrice;

    double Volatility;

    static double r ; // risk free rate

    //************** Dividends feature **************

    dividend Dividends;


public:
    //************** Primary functions **************
    asset();
    asset(char AssetName[20], double CurrentTime, double SpotPrice, double Volatility, dividend Dividends);
    asset(const asset&);
    ~asset();


    //************** Basic functions **************
    //*** Getter ***
    char* get_AssetName() const;
    double get_CurrentTime() const;
    double get_SpotPrice() const;
    double get_Volatility() const;
    dividend& get_alias_Dividends();
    dividend get_Dividends() const;
    double get_r() const;

    //*** Setter ***
    void set_AssetName(char Name[20]);
    void set_CurrentTime(double);
    void set_SpotPrice(double);
    void set_Volatility(double);
    void set_Dividends(dividend);
    void set_r(double r);

    //*** Overload assignment operator ***
    void operator=(const asset&);

    //************** Advanced functions **************

    void Asset_Actualization(double NewTime, double SpotPrice);
    asset Asset_Estimation(double Time, double RiskFreeRate) const;


    //************** Display overload **************
    
    friend std::ostream& operator<<(std::ostream& output, const asset& Asset);
    friend std::istream& operator>>(std::istream& input, asset& Asset);
    
};





