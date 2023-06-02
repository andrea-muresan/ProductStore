#pragma once
#include<QAbstractTableModel>
#include "Product.h"
#include <vector>

class MyTableModel :public QAbstractTableModel {
	std::vector<Product> products;
public:
	MyTableModel() {}

	int rowCount(const QModelIndex& parent = QModelIndex()) const override {
		return products.size();
	}
	int columnCount(const QModelIndex& parent = QModelIndex()) const override {
		return 5;
	}
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
		if (role == Qt::DisplayRole) {
			Product p = products[index.row()];
			if (index.column() == 0) {
				return QString::number(p.getId());
			}
			else if (index.column() == 1) {
				return QString::fromStdString(p.getName());
			}
			else if (index.column() == 2) {
				return QString::fromStdString(p.getType());
			}
			else if (index.column() == 3) {
				return QString::number(p.getPrice());
			}
			else if (index.column() == 4) {
				return QString::fromStdString(p.getProducer());
			}
		}
		return QVariant{};
	}

	Product getPrduct(const QModelIndex& index) {
		return products[index.row()];
	}

	void setProducts(const std::vector<Product>& products) {
		this->products = products;
		QModelIndex topLeft = createIndex(0, 0);
		QModelIndex bottomRight = createIndex(rowCount(), columnCount());
		emit dataChanged(topLeft, bottomRight);
		emit layoutChanged();
	}
};