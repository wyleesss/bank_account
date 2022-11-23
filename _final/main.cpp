#include "bank_account.h"
using namespace std;

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Account A("Перший", GPY, "Шостий", Card, 233;
	A.income(25000);
	A - 25000;

	Account B("Другий", GPY, "П'ятий", Current, 233);
	B.write_off(33000);
	B + 33000;
	B.output_info();

	CreditAccount C("Третій", GPY, "Четвертий", 233, { 25, 10, 2022 }, { 25, 11, 2022 }, 1000, 8);
	C.write_off(25000);
	C - 500;
	C.set_current_date({ 26, 11, 2022 });
	C + 600;
	C.output_info();

	DepositAccount D("П'ятий", GPY, "Другий", 1000, { 26, 11, 2022 }, { 26, 12, 2023 }, Savings, 15);
	DepositAccount D2("Шостий", GPY, "Перший", 1000, { 26, 11, 2022 }, { 26, 12, 2023 }, Universal, 15);
	D.write_off(100);
	D2 - 100;

	for(int i = 0; i<=6; i++)
		D2.get_month_income();
	D2.set_current_date({ 26, 5, 2023 });
	D2 - 500;
	D2 + 15000;
	D2.output_info();
}