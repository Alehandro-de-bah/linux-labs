#include <iostream>

double calcPower(double base, int exponent) {
	double result = 1.0;
	for (int i = 0; i < exponent; i++) {
		result *= base;
	}
	return result;
}

int main() {
	const int ITERATIONS = 1000000000;
	double total = 0.0;

	for (int i = 1; i <= ITERATIONS; i++) {
		int exp = i % 7;
		total += calcPower(i * 0.1, exp);
	}
	
	std::cout << "Total = " << total;
	return 0;
}
