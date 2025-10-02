#include "IOUtil.h"

void set_output_precision(int n) { std::cout << std::setprecision(n); }

void print_whitespace(int spaces) { for (int i = 0; i < spaces; i++) { std::cout << " "; } }

void print_blank_lines(int lines) { for (int i = 0; i < lines; i++) { std::cout << std::endl; } }

void println() {
	std::cout << std::endl;
}

std::string input::getString() {
	std::string s;
	getline(std::cin, s);
	return s;
}

char input::getChar() {
	char c;
	std::cin >> c;
	return c;
}

int input::getInt() {
	int i;
	std::cin >> i;
	return i;
}

double input::getDouble() {
	double d;
	std::cin >> d;
	return d;
}