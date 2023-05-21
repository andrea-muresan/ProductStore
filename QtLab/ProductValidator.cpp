#include "ProductValidator.h"

#include <assert.h>

bool ProductValidator::validateId(std::string id)
{
	// check if it is an integer
	int nr;
	try {
		nr = std::stoi(id);
	}
	catch (std::exception e) {
		return false; }

	// check if it is pozitive
	return nr >= 0;
}

bool ProductValidator::validatePrice(std::string price)
{
	// check if it is a decimal
	double pr;
	try {
		pr = std::stod(price);
	}
	catch (std::exception e) {
		return false; }

	// check if it is positive
	return pr >= 0;

}

bool ProductValidator::validateString(std::string str)
{
	// check the length
	return str.length() > 0;
}

bool ProductValidator::validateInteger(std::string nr)
{
	// check if it is an integer
	int nrVal;
	try {
		nrVal = std::stoi(nr);
	}
	catch (std::exception e) {
		return false;
	}

	return true;
}

// TESTS
void testId() {
	ProductValidator val;
	
	std::string id1 = "abcd";
	assert(val.validateId(id1) == false);

	std::string id2 = "";
	assert(val.validateId(id2) == false);

	std::string id3 = "111";
	assert(val.validateId(id3) == true);

	std::string id4 = "-1";
	assert(val.validateId(id4) == false);
}

void testPrice() {
	ProductValidator val;
	
	std::string pr1 = "1";
	assert(val.validatePrice(pr1) == true);
	
	std::string pr2 = "23.7";
	assert(val.validatePrice(pr2) == true);

	std::string pr3 = "-7";
	assert(val.validatePrice(pr3) == false);

	std::string pr4 = "-7.9";
	assert(val.validatePrice(pr4) == false);

	std::string pr5 = "abc";
	assert(val.validatePrice(pr5) == false);
}

void testString() {
	ProductValidator val;
	
	std::string str1 = "";
	assert(val.validateString(str1) == false);

	std::string str2 = "ana";
	assert(val.validateString(str2) == true);
}

void testInteger() {
	ProductValidator val;

	std::string str1 = "";
	assert(val.validateInteger(str1) == false);

	std::string str2 = "7";
	assert(val.validateInteger(str2) == true);
}

void testValidator()
{
	testId();
	testPrice();
	testString();
	testInteger();
}
