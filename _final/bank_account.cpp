#pragma once
#include "bank_account.h"

 //���� �������������� ��������� ������ �����/������

std::ostream& operator<<(std::ostream& os, const CurrencyType& currency) {
	return os << (currency == UAH ? "UAH" : currency == USD ? "USD" : currency == EUR ? "EUR" : "GPY");
}

std::ostream& operator<<(std::ostream& os, const AccountType& type) {
	return os << (type == Current ? "��������" : type == Card ? "���������" : type == Deposit ? "����������" : "���������");
}

std::ostream& operator<<(std::ostream& os, const DepositType& deposit_type) {
	return os << (deposit_type == Accumulation ? "���������������" : deposit_type == Savings ? "�������" : "������������");
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
	return os << date.day << "." << date.month << "." << date.year;
}

//�������� ��������� �������� ���

bool Date::operator>(const Date& date) {
	long this_sum, other_sum;
	this_sum = year * 10000 + month * 100 + day;
	other_sum = date.year * 10000 + date.month * 100 + date.day;
	if (other_sum > this_sum) return false;
	return true;
}

//���� �������� �����

Account::Account(std::string name, CurrencyType currency, std::string bank, AccountType type, double value) :name(name), currency(currency), bank(bank), type(type), value(value)
{
}

void Account::income(double summ) {
	value += summ;
	std::cout << "\n������� ��������� ������! -> +" << summ << currency << ";\n������� -> " << value << currency << std::endl;
}

void Account::write_off(double summ) {
	try {
		if (type != 4 && summ > value)
			throw new std::string("\n�������� � ������� ���������!\n������� ������� �� � ��������� �� ������� ����������, �� �� ������� �������� ��������.");
		value -= summ;
		std::cout << "�������� � ������� �������� ������! -> -" << summ << currency << ";\n������� -> " << value << currency << std::endl;
	}
	catch(std::string* ex){
		std::cerr << *ex << "\n���� -> " << summ << currency << "\n������� -> " << value << currency << std::endl;
	}
}

double Account::balance() { return value; }

void Account::output_info() {
	std::cout << "\n������������� -> " << ID << std::endl
		<< "����� -> " << name << std::endl
		<< "�������� -> " << bank << std::endl
		<< "��� -> " << type << std::endl
		<< "������� -> " << value << std::endl
		<< "������ -> " << currency << std::endl;
}

void Account::operator+(double summ) {
	this->income(summ);
}
void Account::operator-(double summ) {
	this->write_off(summ);
}

//���� �������� �����

//���� ���������� �������

CreditAccount::CreditAccount(const std::string& name, const CurrencyType& currency, const std::string& bank, double value, const Date& current_date, const Date& end_date, int credit_limit, int percentage) :Account(name, currency, bank, Credit, value), current_date(current_date), end_date(end_date), credit_limit(credit_limit), percentage(percentage)
{
}

void CreditAccount::income(double summ) {
	if (value<0 && current_date>end_date) {
		value += summ = summ / 100 * (100 - percentage);
		std::cout << "\n����� 䳿 �����.\n������� ��������� ������! (� ����������� �������) -> +" << summ << currency << ";\n������� -> " << value << currency << "\n³������ -> " << percentage << std::endl;
		return;
	}
	value += summ;
	std::cout << "\n������� ��������� ������! -> +" << summ << currency << ";\n������� -> " << value << currency << std::endl;
}

void CreditAccount::write_off(double summ) {
	try {
		if (value - summ < -credit_limit)
			throw new std::string("\n�������� � ������� ���������!\n��������� ��� ���������.");
		value -= summ;
		std::cout << "\n�������� � ������� �������� ������! -> -" << summ << currency << ";\n������� -> " << value << currency << std::endl;
	}
	catch (std::string* ex) {
		std::cerr << *ex << "\n���� -> " << summ << currency << "\n������� -> " << value << currency << "\n��������� ��� -> " << credit_limit << currency << std::endl;
		delete ex;
	}
}

void CreditAccount::output_info() {
	std::cout << "\n������������� -> " << ID << std::endl
		<< "����� -> " << name << std::endl
		<< "�������� -> " << bank << std::endl
		<< "������� -> " << value << std::endl
		<< "������ -> " << currency << std::endl
		<< "˳�� -> " << credit_limit << std::endl
		<< "³������ -> " << percentage << std::endl
		<< "���� ��������� -> " << end_date << std::endl
		<< "������� ���� -> " << current_date << std::endl;
}

void CreditAccount::set_current_date(const Date& new_date) {
	current_date = new_date;
	std::cout << "\n������� ���� ������ �� " << current_date << std::endl;
}

void CreditAccount::extend_end_date(const Date& new_end_date) {
	try {
		if (end_date > new_end_date)
			throw new std::string("\n���������� ���� ����������� 䳿 ���������� �������\n");
		end_date = new_end_date;
		std::cout << "\n���� ��������� ���� ���������� �� " << end_date << std::endl;
	}
	catch (std::string* ex) {
		std::cerr << *ex;
		delete ex;
	}
}

//���� ����������� �������

DepositAccount::DepositAccount(const std::string& name, const CurrencyType& currency, const std::string& bank, double value, const Date& current_date, const Date& end_date, const DepositType& deposit_type, int percentage):Account(name, currency, bank, Credit, value), current_date(current_date), end_date(end_date), deposit_type(deposit_type), percentage(percentage), additional_value(0)
{
}

void DepositAccount::get_month_income() { additional_value += value / 100 * percentage; }

void DepositAccount::income(double summ) {
	try {
		if (end_date > current_date && deposit_type == Savings)
			throw new std::string("\n\n�������� ������� ���������!\n������� ����� 䳿 �������� �� ����������\n�� ��� �������� �� �������� ���������� ���������� ������, �������� ���������.");
		value += summ;
		std::cout << "\n������� ��������� ������! -> +" << summ << currency << ";\n������� -> " << value + additional_value << currency << std::endl;
	}
	catch (std::string* ex) {
		std::cerr << ex << "\n��� �������� -> " << deposit_type << "\n������� ���� -> " << current_date << "\n���� ��������� �������� -> " << end_date << std::endl;
		delete ex;
	}
}

void DepositAccount::write_off(double summ) {
	try {
		if (end_date > current_date && deposit_type != Universal)
			throw std::string("\n\n�������� � ������� ���������!\n������� ����� 䳿 �������� �� ����������\n�� ��� �������� �� �������� ���������� ������ ������, �������� ���������.");
		if (summ > additional_value)
			throw new std::string("\n\n�������� � ������� ���������!\n�� �� ������ ����� ������� ������� ����� ������.");
		std::cout << "\n�������� � ������� �������� ������! -> -" << summ << currency << ";\n������� -> " << value + additional_value << currency << std::endl;
		additional_value -= summ;
	}
	catch (std::string* ex) {
		std::cerr << *ex << "\n���� -> " << summ << currency << "\n������� -> " << additional_value + value << currency << "\n������� ������� -> " << value << currency << std::endl;
		delete ex;
	}
	catch (std::string& ex) {
		std::cerr << ex << "\n��� �������� -> " << deposit_type << "������� ���� -> " << current_date << "���� ��������� �������� -> " << end_date << std::endl;
	}
}

void DepositAccount::set_current_date(const Date& new_date) {
	current_date = new_date;
	std::cout << "\n������� ���� ������ �� " << current_date << std::endl;
}

void DepositAccount::output_info() {
	std::cout << "\n������������� -> " << ID << std::endl
		<< "����� -> " << name << std::endl
		<< "�������� -> " << bank << std::endl
		<< "��� �������� -> " << deposit_type << std::endl
		<< "������� -> " << value + additional_value << std::endl
		<< "������� ������� -> " << value << std::endl
		<< "�������� �� ����� -> " << value / 100 * percentage << std::endl
		<< "�������� �� ����� ��� -> " << additional_value << std::endl
		<< "������ -> " << currency << std::endl
		<< "³������ -> " << percentage << std::endl
		<< "���� ��������� -> " << end_date << std::endl
		<< "������� ���� -> " << current_date << std::endl;
}

void DepositAccount::extend_end_date(Date new_end_date) {
	try {
		if (end_date > new_end_date)
			throw new std::string("\n���������� ���� ����������� 䳿 ���������� �������\n");
		end_date = new_end_date;
		std::cout << "\n���� ��������� ���� ���������� �� " << end_date << std::endl;
	}
	catch (std::string* ex) {
		std::cerr << *ex;
		delete ex;
	}
}