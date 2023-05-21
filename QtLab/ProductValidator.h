#pragma once

#include "Product.h"

#include <exception>

class ProductValidator
{
public:
	// check if the ID is valid
	bool validateId(std::string id);

	// check if the price is valid
	bool validatePrice(std::string price);

	// check if a string is valid
	bool validateString(std::string str);

	// check if a string is an integer
	bool validateInteger(std::string nr);

};


void testValidator();