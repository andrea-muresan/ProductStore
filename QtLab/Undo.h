#pragma once
#include "Product.h"
#include "ProductRepo.h"

class Undo
{
public:
	virtual void doUndo() = 0;
	virtual ~Undo() = default;
};

class UndoAdd : public Undo {
	Product product;
	ProductRepo& repo;
public:
	UndoAdd(ProductRepo& r, const Product p) : repo{r}, product{p}{}

	void doUndo() override {
		int id = repo.getAllProducts().back().getId();
		repo.deleteProduct(id);
	}
};

class UndoDelete : public Undo {
	Product product;
	ProductRepo& repo;
public:
	UndoDelete(ProductRepo& r, Product p) : repo{r}, product{p}{}

	void doUndo() override {
		repo.addProduct(product);
	}
};

class UndoUpdate : public Undo {
	int id, what;
	string new_val;
	ProductRepo& repo;
public:
	UndoUpdate(ProductRepo& r, int i, const string n_w, int w) : repo{ r }, id{ i }, new_val { n_w }, what{ w } {}
	void doUndo() override {
		repo.updateProduct(id, new_val, what);
	}
};