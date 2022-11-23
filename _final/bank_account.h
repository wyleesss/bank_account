#pragma once
#include <iostream>
#include <string>
#include <Windows.h>

//переліки для різноманітних типів валют/рахунків

enum CurrencyType {
	UAH = 980,
	USD = 840,
	EUR = 978,
	GPY = 392
};

enum AccountType {
	Current = 1,
	Card,
	Deposit,
	Credit
};

enum DepositType {
	Accumulation = 1,
	Savings,
	Universal
};
 
//структура дати для подальшої зручної роботи з нею

struct Date {
	int day, month, year;
	bool operator>(const Date&);
};

//перевантаження операторів потоків вводу/виводу для переліків/структур задля зручного виводу даних

std::ostream& operator<<(std::ostream& os, const CurrencyType& currency);

std::ostream& operator<<(std::ostream& os, const AccountType& type);

std::ostream& operator<<(std::ostream& os, const DepositType& deposit_type);

std::ostream& operator<<(std::ostream& os, const Date& date);

//статична змінна для реалізації ID

static int global_id;

//визначення класів

class Account {
protected:
	const int ID = ++global_id;
	std::string name;
	CurrencyType currency;
	std::string bank;
	AccountType type;
	double value;
public:
	Account(std::string name, CurrencyType currency, std::string bank, AccountType type, double value);
	virtual void income(double summ);
	virtual void write_off(double summ);
	double balance();
	virtual void output_info();
	virtual void operator+(double summ);
	virtual void operator-(double summ);
};

class CreditAccount :public Account {
	Date end_date;
	Date current_date;
	int credit_limit;
	int percentage;
public:
	CreditAccount(const std::string& name, const CurrencyType& currency, const std::string& bank, double value, const Date& current_date, const Date& end_date, int credit_limit, int percentage);
	void income(double summ) override;
	void write_off(double summ) override;
	void set_current_date(const Date& new_date);
	void output_info() override;
	void extend_end_date(const Date& new_end_date);
};

class DepositAccount :public Account {
	Date end_date;
	Date current_date;
	DepositType deposit_type;
	int percentage;
	double additional_value;
public:
	DepositAccount(const std::string& name, const CurrencyType& currency, const std::string& bank, double value, const Date& current_date,const Date& end_date, const DepositType& deposit_type, int percentage);
	void income(double summ) override;
	void write_off(double summ) override;
	void set_current_date(const Date& new_date);
	void output_info() override;
	void extend_end_date(Date new_date);
	void get_month_income();
};