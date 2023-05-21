#pragma once
#include "Product.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

class Undo;

class ProductRepo
{
	vector<Product> productList;

public:
	ProductRepo() = default;

	// don't allow copying of objects
	ProductRepo(const ProductRepo& pr) = delete;

	// Return a list with all products
	const vector<Product> getAllProducts();

	// Add a product
	void addProduct(const Product& pr);

	// Delete a product by id
	// Return an error message if it does not exist, "" otherwise
	string deleteProduct(int id);

	// Update a product knowing its ID
	// Return an error message if the ID does not exist, "" otherwise
	string updateProduct(int id, const string new_val, int what);

	// Find a product by id
	// Return it's position. If it doesn't exist return -1
	int findProduct(int id);

	
};

