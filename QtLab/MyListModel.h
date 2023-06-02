#pragma once
#include <QAbstractListModel>
#include "Product.h"
#include <vector>
#include <qdebug.h>
#include "ProductService.h"
#include "ProductRepo.h"

class MyListModel :public QAbstractListModel {
	std::vector<Product> products;
public:
	MyListModel() {	}
	MyListModel(const vector<Product>& products) :products{ products } {
	}

	int rowCount(const QModelIndex& parent = QModelIndex()) const override {
		return products.size();
	}

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
		if (role == Qt::DisplayRole) {
			auto p = products[index.row()];
			return QString("- " + QString::number(p.getId()) + " : " + QString::fromStdString(p.getName()) + ", " + 
				QString::fromStdString(p.getType()) + ", " + QString::number(p.getPrice()) + ", " + QString::fromStdString(p.getProducer()));
		}

		return QVariant{};
	}

	Product getProduct(const QModelIndex& index) {
		return products[index.row()];
	}

	void setProducts(const vector<Product>& prods) {
		this->products = prods;
		QModelIndex topLeft = createIndex(0, 0);
		QModelIndex bottomRight = createIndex(rowCount(), 0);
		emit dataChanged(topLeft, bottomRight);
		emit layoutChanged();
	}
};