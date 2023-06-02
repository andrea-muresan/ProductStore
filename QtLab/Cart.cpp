#include "Cart.h"
#include <random>

void Cart::emptyCart() noexcept {
	this->cartList.clear();

	notify();
}

void Cart::add(Product x)
{
	this->cartList.push_back(x);

	notify();
}

void Cart::generate(int x, std::vector<Product> v)
{
	/// generate x roducts
	for (int i = 0; i < x && !v.empty(); i++)
	{
		int rnd = rand() % v.size();
		this->add(v.at(rnd));
	}

	notify();
}

std::vector<Product> Cart::getList()
{
	return this->cartList;
}

void Cart::Export(std::string fileName)
{
	std::ofstream fout(fileName);

	fout << "<html>\n";
	fout << "<style> table, th, td {border:1px solid black} body{background-color: #E6E6FA;} </style>\n";
	fout << "<body>\n";
	fout << "<table><tr><th>ID</th><th>Nume</th> <th>Tip</th> <th>Pret </th><th>Producator</th></tr>\n";
	for (auto& x : this->cartList)
	{
		fout << "<tr><td>" << x.getId() << "</td><td>" << x.getName() << "</td><td>" << x.getType()
			<< "</td><td>" << x.getPrice() << "</td><td>" << x.getProducer() << "</td></tr>\n";
	}
	fout << "</table></body>\n";
	fout << "<html>\n";
}
