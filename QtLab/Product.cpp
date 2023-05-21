#include "Product.h"

bool cmpName(const Product& p1, const Product& p2) {
	return p1.getName() < p2.getName();
}

bool cmpType(const Product& p1, const Product& p2) {
	return p1.getType() < p2.getType();
}