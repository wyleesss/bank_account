#pragma once
#include "bank_account.h"

 //опис перевантаження операторів потоку вводу/виводу

std::ostream& operator<<(std::ostream& os, const CurrencyType& currency) {
	return os << (currency == UAH ? "UAH" : currency == USD ? "USD" : currency == EUR ? "EUR" : "GPY");
}

std::ostream& operator<<(std::ostream& os, const AccountType& type) {
	return os << (type == Current ? "Поточний" : type == Card ? "Картковий" : type == Deposit ? "Депозитний" : "Кредитний");
}

std::ostream& operator<<(std::ostream& os, const DepositType& deposit_type) {
	return os << (deposit_type == Accumulation ? "Накопичувальний" : deposit_type == Savings ? "Ощадний" : "Універсальний");
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
	return os << date.day << "." << date.month << "." << date.year;
}

//оператор порівняння структур дат

bool Date::operator>(const Date& date) {
	long this_sum, other_sum;
	this_sum = year * 10000 + month * 100 + day;
	other_sum = date.year * 10000 + date.month * 100 + date.day;
	if (other_sum > this_sum) return false;
	return true;
}

//опис базового класу

Account::Account(std::string name, CurrencyType currency, std::string bank, AccountType type, double value) :name(name), currency(currency), bank(bank), type(type), value(value)
{
}

void Account::income(double summ) {
	value += summ;
	std::cout << "\nРахунок Поповнено Успішно! -> +" << summ << currency << ";\nЗалишок -> " << value << currency << std::endl;
}

void Account::write_off(double summ) {
	try {
		if (type != 4 && summ > value)
			throw new std::string("\nСписання з рахунку скасовано!\nОскільки рахунок не є кредитним та залишок недостатній, ви не зможете виконати операцію.");
		value -= summ;
		std::cout << "Списання з рахунку виконано успішно! -> -" << summ << currency << ";\nЗалишок -> " << value << currency << std::endl;
	}
	catch(std::string* ex){
		std::cerr << *ex << "\nСума -> " << summ << currency << "\nЗалишок -> " << value << currency << std::endl;
	}
}

double Account::balance() { return value; }

void Account::output_info() {
	std::cout << "\nІдентифікатор -> " << ID << std::endl
		<< "Назва -> " << name << std::endl
		<< "Установа -> " << bank << std::endl
		<< "Тип -> " << type << std::endl
		<< "Залишок -> " << value << std::endl
		<< "Валюта -> " << currency << std::endl;
}

void Account::operator+(double summ) {
	this->income(summ);
}
void Account::operator-(double summ) {
	this->write_off(summ);
}

//опис похідних класів

//клас кредитного рахунку

CreditAccount::CreditAccount(const std::string& name, const CurrencyType& currency, const std::string& bank, double value, const Date& current_date, const Date& end_date, int credit_limit, int percentage) :Account(name, currency, bank, Credit, value), current_date(current_date), end_date(end_date), credit_limit(credit_limit), percentage(percentage)
{
}

void CreditAccount::income(double summ) {
	if (value<0 && current_date>end_date) {
		value += summ = summ / 100 * (100 - percentage);
		std::cout << "\nТермін дії минув.\nРахунок поповнено успішно! (з урахуванням відсотків) -> +" << summ << currency << ";\nЗалишок -> " << value << currency << "\nВідсотки -> " << percentage << std::endl;
		return;
	}
	value += summ;
	std::cout << "\nРахунок поповнено успішно! -> +" << summ << currency << ";\nЗалишок -> " << value << currency << std::endl;
}

void CreditAccount::write_off(double summ) {
	try {
		if (value - summ < -credit_limit)
			throw new std::string("\nСписання з рахунку скасовано!\nКредитний ліміт вичерпано.");
		value -= summ;
		std::cout << "\nСписання з рахунку виконано успішно! -> -" << summ << currency << ";\nЗалишок -> " << value << currency << std::endl;
	}
	catch (std::string* ex) {
		std::cerr << *ex << "\nСума -> " << summ << currency << "\nЗалишок -> " << value << currency << "\nКредитний ліміт -> " << credit_limit << currency << std::endl;
		delete ex;
	}
}

void CreditAccount::output_info() {
	std::cout << "\nІдентифікатор -> " << ID << std::endl
		<< "Назва -> " << name << std::endl
		<< "Установа -> " << bank << std::endl
		<< "Залишок -> " << value << std::endl
		<< "Валюта -> " << currency << std::endl
		<< "Ліміт -> " << credit_limit << std::endl
		<< "Відсотки -> " << percentage << std::endl
		<< "Дата закінчення -> " << end_date << std::endl
		<< "Поточна дата -> " << current_date << std::endl;
}

void CreditAccount::set_current_date(const Date& new_date) {
	current_date = new_date;
	std::cout << "\nПоточну дату змінено на " << current_date << std::endl;
}

void CreditAccount::extend_end_date(const Date& new_end_date) {
	try {
		if (end_date > new_end_date)
			throw new std::string("\nНекоректна дата продовження дії кредитного рахунку\n");
		end_date = new_end_date;
		std::cout << "\nДату закінчення було продовжено до " << end_date << std::endl;
	}
	catch (std::string* ex) {
		std::cerr << *ex;
		delete ex;
	}
}

//клас депозитного рахунку

DepositAccount::DepositAccount(const std::string& name, const CurrencyType& currency, const std::string& bank, double value, const Date& current_date, const Date& end_date, const DepositType& deposit_type, int percentage):Account(name, currency, bank, Credit, value), current_date(current_date), end_date(end_date), deposit_type(deposit_type), percentage(percentage), additional_value(0)
{
}

void DepositAccount::get_month_income() { additional_value += value / 100 * percentage; }

void DepositAccount::income(double summ) {
	try {
		if (end_date > current_date && deposit_type == Savings)
			throw new std::string("\n\nОперацію рахунку скасовано!\nОскільки термін дії депозиту не вичерпаний\nта тип депозиту не дозволяє здійснювати поповнення вкладу, операцію скасовано.");
		value += summ;
		std::cout << "\nРахунок поповнено успішно! -> +" << summ << currency << ";\nЗалишок -> " << value + additional_value << currency << std::endl;
	}
	catch (std::string* ex) {
		std::cerr << ex << "\nТип депозиту -> " << deposit_type << "\nПоточна дата -> " << current_date << "\nДата закінчення депозиту -> " << end_date << std::endl;
		delete ex;
	}
}

void DepositAccount::write_off(double summ) {
	try {
		if (end_date > current_date && deposit_type != Universal)
			throw std::string("\n\nСписання з рахунку скасовано!\nОскільки термін дії депозиту не вичерпаний\nта тип депозиту не дозволяє здійснювати зняття вкладу, операцію скасовано.");
		if (summ > additional_value)
			throw new std::string("\n\nСписання з рахунку скасовано!\nВи не можете зняти основну частину свого вкладу.");
		std::cout << "\nСписання з рахунку виконано успішно! -> -" << summ << currency << ";\nЗалишок -> " << value + additional_value << currency << std::endl;
		additional_value -= summ;
	}
	catch (std::string* ex) {
		std::cerr << *ex << "\nСума -> " << summ << currency << "\nЗалишок -> " << additional_value + value << currency << "\nОсновна частина -> " << value << currency << std::endl;
		delete ex;
	}
	catch (std::string& ex) {
		std::cerr << ex << "\nТип депозиту -> " << deposit_type << "Поточна дата -> " << current_date << "Дата закінчення депозиту -> " << end_date << std::endl;
	}
}

void DepositAccount::set_current_date(const Date& new_date) {
	current_date = new_date;
	std::cout << "\nПоточну дату змінено на " << current_date << std::endl;
}

void DepositAccount::output_info() {
	std::cout << "\nІдентифікатор -> " << ID << std::endl
		<< "Назва -> " << name << std::endl
		<< "Установа -> " << bank << std::endl
		<< "Тип депозиту -> " << deposit_type << std::endl
		<< "Залишок -> " << value + additional_value << std::endl
		<< "Основна частина -> " << value << std::endl
		<< "Прибуток за місяць -> " << value / 100 * percentage << std::endl
		<< "Прибуток за ввесь час -> " << additional_value << std::endl
		<< "Валюта -> " << currency << std::endl
		<< "Відсотки -> " << percentage << std::endl
		<< "Дата закінчення -> " << end_date << std::endl
		<< "Поточна дата -> " << current_date << std::endl;
}

void DepositAccount::extend_end_date(Date new_end_date) {
	try {
		if (end_date > new_end_date)
			throw new std::string("\nНекоректна дата продовження дії кредитного рахунку\n");
		end_date = new_end_date;
		std::cout << "\nДату закінчення було продовжено до " << end_date << std::endl;
	}
	catch (std::string* ex) {
		std::cerr << *ex;
		delete ex;
	}
}