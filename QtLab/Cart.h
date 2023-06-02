#pragma once
#include "Product.h"
#include "Observer.h"

#include <algorithm>  
#include <random>
#include <cstdlib>
#include <ctime>
#include <fstream>



class Cart : public Observable
{
private:
	std::vector<Product> cartList;

public:
	// Empty the cart
	void emptyCart() noexcept;

	// Add a product to the cart
	void add(Product x);

	// Add some random products to the cart
	void generate(int x, std::vector<Product> v);

	// Return the cart list
	std::vector<Product> getList();

	// Export the products from the cart to a html file
	void Export(std::string fileName);

};
