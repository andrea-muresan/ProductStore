#pragma once
#include "ProductService.h"

#include <QtWidgets>

class CartUI : public QWidget
{
	friend class Service;
	friend class UI;

private:
	ProductService& srv;

	QPushButton* btnEmptyCart = new QPushButton("Goleste cos");
	QPushButton* btnAdd = new QPushButton("Adauga");
	QPushButton* btnGenerate = new QPushButton("Genereaza produse");
	QPushButton* btnGetCart = new QPushButton("Ia cosul");
	QPushButton* btnExport = new QPushButton("Exporta cosul");

	QGroupBox* groupBoxAdd = new QGroupBox(tr("Adauga"));
	QGroupBox* groupBoxGenerate = new QGroupBox(tr("Genereaza"));
	QGroupBox* groupBoxExport = new QGroupBox(tr("Exporta"));

	QLabel* lblAdd = new QLabel("ID-ul produsului: ");
	QLabel* lblGenerate = new QLabel("Numarul de elemente: ");
	QLabel* lblExport = new QLabel("Numele fisierului: ");

	QLineEdit* editAdd = new QLineEdit;
	QLineEdit* editGenerate = new QLineEdit;
	QLineEdit* editExport = new QLineEdit;

	QTableWidget* productTable;
	QListWidget* productLst;
public:
	CartUI(ProductService& srv) : srv{srv}{}
	void buildUI();
	void connectSignalsSlots();
	void reloadList(vector<Product> cartList);
};

class DeleteUI : public QWidget
{
	friend class Service;
	friend class UI;

private:
	ProductService& srv;

	QPushButton* btnDel = new QPushButton("Sterge");
	QLabel* lblId = new QLabel{ "ID-ul produsului:" };
	QLineEdit* editId = new QLineEdit;

public:
	DeleteUI(ProductService& srv) : srv{srv} {}
	void buildUI();
	void connectSignalsSlots();
};

class UpdateUI : public QWidget
{
	friend class ProductServive;
	friend class UI;
private:
	ProductService& srv;

	QPushButton* btnUpd = new QPushButton("Modifica");

	QLabel* lblId = new QLabel{ "ID-ul produsului:" };
	QLabel* lblNewVal = new QLabel{ "Noua valoare:" };

	QLineEdit* editId = new QLineEdit;	
	QLineEdit* editNewVal = new QLineEdit;	

	QRadioButton* radioUpdName = new QRadioButton(QString::fromStdString("numele"));
	QRadioButton* radioUpdType = new QRadioButton(QString::fromStdString("tipul"));
	QRadioButton* radioUpdPrice = new QRadioButton(QString::fromStdString("pretul"));
	QRadioButton* radioUpdProducer = new QRadioButton(QString::fromStdString("producatorul"));

public:
	UpdateUI(ProductService& srv) :srv{ srv } {}
	void buildUI();
	void connectSignalsSlots();
};

class SearchUI : public QWidget
{
	friend class ProductService;
	friend class UI;
private:
	ProductService& srv;
	
	QPushButton* btnSearch = new QPushButton("Cauta");

	QLabel* lblId = new QLabel{ "ID:" };
	QLabel* lblName = new QLabel{ "Nume:" };
	QLabel* lblType = new QLabel{ "Tip:" };
	QLabel* lblPrice = new QLabel{ "Pret:" };
	QLabel* lblProducer = new QLabel{ "Producator:" };

	QLineEdit* editId = new QLineEdit;
	QLineEdit* editName = new QLineEdit;
	QLineEdit* editType = new QLineEdit;
	QLineEdit* editPrice = new QLineEdit;
	QLineEdit* editProducer = new QLineEdit;

	QGroupBox* groupBoxForm = new QGroupBox();

	QTableWidget* productTable;

public:
	SearchUI(ProductService& srv) :srv{ srv } {}
	void buildUI();
	void connectSignalsSlots();
	void reloadList(vector<Product> productList);
};


class UI : public QWidget
{
	friend class PruductService;
private:
	ProductService& srv;

	DeleteUI* deleteProductUI;
	UpdateUI* updateProductUI;
	SearchUI* searchProductUI;
	CartUI*	cartProductUI;

	QLabel* lblId = new QLabel{ "ID-ul produsului:" };
	QLabel* lblName = new QLabel{ "Numele produsului:" };
	QLabel* lblType = new QLabel{ "Tipul produsului:" };
	QLabel* lblPrice = new QLabel{ "Pretul produsului:" };
	QLabel* lblProducer = new QLabel{ "Producatorul produsului:" };

	QLineEdit* editId = new QLineEdit;
	QLineEdit* editName = new QLineEdit;
	QLineEdit* editType = new QLineEdit;
	QLineEdit* editPrice = new QLineEdit;
	QLineEdit* editProducer = new QLineEdit;

	QPushButton* btnAddProduct = new QPushButton("Adauga");
	QPushButton* btnDeleteProduct = new QPushButton("Sterge");
	QPushButton* btnUpdateProduct = new QPushButton("Modifica");
	QPushButton* btnSearchProduct = new QPushButton("Cauta");

	QGroupBox* groupBoxForm = new QGroupBox();
	QGroupBox* groupBoxSort = new QGroupBox(tr("Sortare"));
	QGroupBox* groupBoxFilter= new QGroupBox(tr("Filtrare"));

	QRadioButton* radioSrtName = new QRadioButton(QString::fromStdString("nume"));
	QRadioButton* radioSrtType = new QRadioButton(QString::fromStdString("tip"));
	QRadioButton* radioSrtNameType = new QRadioButton(QString::fromStdString("nume si tip"));
	QPushButton* btnSortProducts;
	
	QLineEdit* editFilter;
	QPushButton* btnFilterType;
	QPushButton* btnFilterPrice;
	QPushButton* btnFilterProducer;
	QLabel* lblFilterCriteria = new QLabel{ "Ce cautam:" };
	
	QPushButton* btnReloadData = new QPushButton("Reincarca lista");

	QPushButton* btnAddRandomProducts = new QPushButton("Adauga cateva produse");
	QPushButton* btnUndo = new QPushButton("Anuleaza ultima operatie - UNDO");
	QPushButton* btnCart = new QPushButton("Deschide cosul de cumparaturi");
	QPushButton* btnReportType = new QPushButton("Raport produse - tip");


	QTableWidget* productTable;
	QPushButton* btnAddToCart = new QPushButton("+");

public:
	UI(ProductService& srv);

	void buildUI();
	void reloadList(vector<Product> productList);
	void connectSignalsSlots();
	void guiAddProduct();
};
