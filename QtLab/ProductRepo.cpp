#include "ProductRepo.h"
#include "Product.h"

const vector<Product> ProductRepo::getAllProducts()
{
	return productList;
}

void ProductRepo::addProduct(const Product& pr)
{
	productList.push_back(pr);
}

string ProductRepo::deleteProduct(int id)
{
	int pos = findProduct(id);

	if (pos == -1)
		return "ID inexistent";
	
	productList.erase(productList.begin() + pos);
	return "";
}

string ProductRepo::updateProduct(int id, const string new_val, int what)
{
	int pos = findProduct(id);

	if (pos == -1)
		return "ID inexistent";

	if (what == 1) // 1 -> name
		productList[pos].setName(new_val);
	else if (what == 2) // 2 -> type
		productList[pos].setType(new_val);
	else if (what == 3) // 3 -> price
		productList[pos].setPrice(std::stod(new_val));
	else if (what == 4) // 4 -> producer
		productList[pos].setProducer(new_val);
	return "";
}

int ProductRepo::findProduct(int id)
{
	vector<Product>::iterator it = std::find_if(productList.begin(), productList.end(),
		[id](Product pr) {return (pr.getId() == id); });
	
	if (it != productList.end())
		return it - productList.begin();
	return -1;
}


