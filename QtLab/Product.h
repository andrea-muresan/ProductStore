#pragma once

#include <string>
#include <iostream>


class Product {
	int id;
	std::string name;
	std::string type;
	double price;
	std::string producer;

public:
	Product(int i, const std::string n, const std::string t, double price, const std::string p) :
		id{ i }, name{ n }, type{ t }, price{ price }, producer{ p } {}

	// copy const
	Product(const Product& pr) :
		id{ pr.id }, name{ pr.name }, type{ pr.type }, price{ pr.price }, producer{ pr.producer } {}

	// getters
	int getId() const {
		return this->id;
	}
	std::string getName() const {
		return this->name;
	}
	std::string getType() const {
		return this->type;
	}
	double getPrice() const {
		return this->price;
	}
	std::string getProducer() const {
		return this->producer;
	}

	//setters
	void setName(std::string new_val) {
		this->name = new_val;
	}
	void setType(std::string new_val) {
		this->type = new_val;
	}
	void setPrice(double new_val) {
		this->price = new_val;
	}
	void setProducer(std::string new_val) {
		this->producer = new_val;
	}

};

/*
Compare by name
: return: true if p1.name is lower than p2.tip
*/
bool cmpName(const Product& p1, const Product& p2);

/*
Compare by type
: return: true if p1.type is lower than p2.type
*/
bool cmpType(const Product& p1, const Product& p2);
