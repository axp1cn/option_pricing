#pragma once
#include <iostream>

int DividendCounter(double Delta, double Next, double Periods);

class dividend {
protected:
    int Type;
    double Rate;
    double Periods; // The period between two dividend's payments for an asset with lump payment of dividends
    double Next; // Time before the next dividend's payment for an asset with lump payment of dividends

public:
    dividend();
    dividend(int DividendsType, double DividendsRate, double DividendsPeriods, double NextDividend);
    dividend(const dividend&);
    ~dividend(){};

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
    double CurrentTime;
    double SpotPrice;
    double Volatility;
    static double r ; // risk free rate
    dividend Dividends; //dividend features


public:
    asset();
    asset(double CurrentTime, double SpotPrice, double Volatility, dividend Dividends);
    asset(const asset&);
    ~asset();

    //*** Getter ***
    double get_CurrentTime() const;
    double get_SpotPrice() const;
    double get_Volatility() const;
    dividend& get_alias_Dividends();
    dividend get_Dividends() const;
    double get_r() const;

    //*** Setter ***
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
    
    friend std::istream& operator>>(std::istream& input, asset& Asset);
};





