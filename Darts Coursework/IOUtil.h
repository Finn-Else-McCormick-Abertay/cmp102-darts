#pragma once

#include <iostream>
#include <iomanip>
#include <string>

void set_output_precision(int n);

void print_whitespace(int spaces);
void print_blank_lines(int lines);

void println();

// The variadic functions have to be defined in the header file for compiler reasons (you get linkage errors otherwise)
template <typename...Args>
void print(Args&&...args) {
	(std::cout << ... << args);
}
template <typename...Args>
void println(Args&&...args) {
	(std::cout << ... << args) << std::endl;
}


namespace input {
	std::string getString();
	char getChar();
	int getInt();
	double getDouble();
}