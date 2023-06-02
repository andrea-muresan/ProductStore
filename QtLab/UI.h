#pragma once
#include "ProductService.h"
#include "Observer.h"
#include "MyListModel.h"
#include "MyTableModel.h"

#include <QtWidgets>

class CartUI : public QWidget, public Observer, public Observable
{
	friend class Service;
	friend class UI;

private:
	Cart& cart;
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

	/*QListWidget* productLst;*/
	QListView* listView = new QListView;
	MyListModel* listModel = new MyListModel;

public:
	CartUI(ProductService& srv, Cart& cart) : srv{ srv }, cart{ cart } {}
	void buildUI();
	void connectSignalsSlots();
	void reloadList(vector<Product> cartList);

	void update() override {
		reloadList(cart.getList());
	}

	~CartUI() {
		cart.removeObserver(this);
	}

};

class CartReadOnlyGUI : public QWidget, public Observer {
	friend class Cart;
	Cart& cart;

	QListWidget* cartList;
public:
	CartReadOnlyGUI(Cart& c) : cart(c)
	{
		//this->buildUI();
		cart.addObserver(this);
	}
	void buildUI()
	{
		cartList = new QListWidget();
		QHBoxLayout* mainly = new QHBoxLayout;
		mainly->addWidget(cartList);
		this->setLayout(mainly);
	}
	void update() override {
		//this->reloadList();
		this->repaint();
	}
	void reloadList()
	{
		cartList->clear();
		for (auto elem : cart.getList())
		{
			cartList->addItem(QString::fromStdString(elem.getName()));
		}
	}

	void paintEvent(QPaintEvent* ev) override
	{
		QPainter p(this);

		/*p.drawImage(0, 0, QImage("floare.jpg"));
		srand(time(0));*/

		int x = -20;
		for (auto elem : cart.getList())
		{
			x += 40;
			int forma = rand() % 4;
			int inaltime = rand() % 130;
			int start_y = rand() % 60;
			QColor color;

			int color_num = rand() % 10;
			switch (color_num)
			{
			case 0:
				color = Qt::red;
				break;
			case 1:
				color = Qt::green;
				break;
			case 2:
				color = Qt::black;
				break;
			case 3:
				color = Qt::blue;
				break;
			case 4:
				color = Qt::lightGray;
				break;
			case 5:
				color = Qt::yellow;
				break;
			case 6:
				color = Qt::cyan;
				break;
			case 7:
				color = Qt::magenta;
				break;
			case 8:
				color = Qt::darkGreen;
				break;
			case 9:
				color = Qt::darkBlue;
				break;
			default:
				break;
			}

			switch (forma)
			{
			case 0: // dreptunghi
				p.drawRect(x, start_y, 20, inaltime);
				break;
			case 1: // elipsa
				p.drawEllipse(x, start_y, 20, inaltime);
				break;
			case 2: // dreptunghi colorat
				p.fillRect(x, start_y, 20, inaltime, color);
				break;
			default: // dreptunghi colorat
				p.fillRect(x, start_y, 20, inaltime, color);
				break;
			}
		}
	}

	~CartReadOnlyGUI()
	{
		cart.removeObserver(this);
	}
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
	/*CartUI*	cartProductUI;*/
	//CartReadOnlyGUI* cartDrawings;

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
	QPushButton* btnReportType = new QPushButton("Raport produse - tip");

	// cart
	QPushButton* btnCart = new QPushButton("Deschide cosul de cumparaturi");
	QPushButton* btnCartDrawings = new QPushButton("Cos - desene");
	QPushButton* btnAddRandomCart = new QPushButton("Adauga cateva produse in cos");
	QPushButton* btnAddToCart = new QPushButton("Adauga in cos");
	QPushButton* btnEmptyCart = new QPushButton("Goleste cosul");
	QLabel* lblIdCart = new QLabel{ "ID-ul produsului:" };
	QLabel* lblNrCart = new QLabel{ "Numar de produse:" };
	QLineEdit* editIdCart = new QLineEdit;
	QLineEdit* editNrCart = new QLineEdit;
	QGroupBox* groupBoxCart = new QGroupBox(tr("Cos de cumparaturi"));


	// QTableWidget* productTable;
	QTableView* tableView = new QTableView;
	MyTableModel* tableModel = new MyTableModel;

public:
	UI(ProductService& srv);

	void buildUI();
	void reloadList(vector<Product> productList);
	void connectSignalsSlots();
	void guiAddProduct();
};
