#pragma once

#include "ProductRepo.h"
#include "ProductValidator.h"
#include "Undo.h"
#include "Cart.h"
#include <map>


class ProductService
{
	Cart cart;
	ProductRepo& repo;
	ProductValidator val;
	vector <std::unique_ptr<Undo>>undoList;

public:
	ProductService(ProductRepo& r, ProductValidator v) noexcept: repo{r}, val{v}{}

	// do not allow copying of ProdusStore objects
	ProductService(const ProductService& srv) = delete;

	//Return a list with all Products
	const vector<Product> getAllProducts();

	// Cerate a product and add it to the list
	// Validate the input data and return an error message if they are not valid, "" otherwise
	string addProduct(const string name, const string type, const string price, const string producer);
	
	// Delete a product by its ID
	// Validate input data
	// Return an error message if the ID does not exist, "" otherwise
	string deleteProduct(const string id);

	// Update a product knowing its ID
	// Validate input data
	// Return an error message if the ID does not exist, "" otherwise
	string updateProduct(const string id, const string new_val, int what);
	
	// Return only products of a certain type
	vector<Product> filterByType(const string type);
	
	// Return only the products with a price between the two prices
	vector<Product> filterByPrice(const string pretMin, const string pretMax);

	// Return only products of a certain producer
	vector<Product> filterByProducer(const string producer);

	// Search for a product knowing one or more of its attributes
	vector<Product> searchProduct(const string id,const string name,const string type, const string price,const string producer);

	// Sort by type
	vector<Product> sortByType();

	// Sort by name
	vector<Product> sortByName();

	// Sort by name+type
	vector<Product> sortByNameType();

	// Add some random products
	void addRandomProducts();


	// CART
	// Empty the cart
	void emptyCart();
	// Add a product to the cart
	string addToCart(string id);
	// Add some random products to the cart
	string generate(string nr);
	// Return the cart list
	vector<Product> getCart();
	// Export the products from the cart to a html file
	string Export(string fileName);

	// UNDO function
	// Return an error message if we cannot undo the action, "" otherwise
	string undo();


	const std::map<std::string, int> reportProducts();
};

// tests
void testService();
