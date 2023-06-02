#include "UI.h"

UI::UI(ProductService& srv) : srv{ srv }
{
	// delete window
	this->deleteProductUI = new DeleteUI(srv);
	deleteProductUI->buildUI();
	deleteProductUI->connectSignalsSlots();

	// update window
	this->updateProductUI = new UpdateUI(srv);
	updateProductUI->buildUI();
	updateProductUI->connectSignalsSlots();

	// search window
	this->searchProductUI = new SearchUI(srv);
	searchProductUI->buildUI();
	searchProductUI->connectSignalsSlots();

	// cart window
	/*this->cartProductUI = new CartUI(srv, srv.getCartObj());
	cartProductUI->buildUI();
	cartProductUI->connectSignalsSlots();*/

	// cart window drawings
	/*this->cartDrawings = new CartReadOnlyGUI(srv.getCartObj());*/

}

void UI::buildUI()
{

	this->setWindowTitle("Magazin");
	// main layout - orizontal
	QHBoxLayout* lyMain = new QHBoxLayout;
	this->setLayout(lyMain);
	lyMain->setSpacing(10);

	// LEFT PART of the window - vertical layout
	QWidget* left = new QWidget;
	QVBoxLayout* lyLeft = new QVBoxLayout;
	left->setLayout(lyLeft);

	QFormLayout* lyForm = new QFormLayout;
	groupBoxForm->setLayout(lyForm);
	
	// fields for entering product data 
	lyForm->addRow(lblName, editName);
	lyForm->addRow(lblType, editType);
	lyForm->addRow(lblPrice, editPrice);
	lyForm->addRow(lblProducer, editProducer);
	
	// buttons for managing the product list
	QWidget* btnSet = new QWidget;
	QHBoxLayout* lyBtnSet = new QHBoxLayout;
	lyBtnSet->addWidget(btnAddProduct);
	lyBtnSet->addWidget(btnUpdateProduct);
	lyBtnSet->addWidget(btnDeleteProduct);
	lyBtnSet->addWidget(btnSearchProduct);
	btnSet->setLayout(lyBtnSet);
	lyForm->addRow(btnSet);
	lyLeft->addWidget(groupBoxForm);


	// SORT
	QVBoxLayout* lySortBox = new QVBoxLayout;
	this->groupBoxSort->setLayout(lySortBox);

	// vertical layout for the radio buttons 
	QWidget* SortOptions = new QWidget;
	QHBoxLayout* lySort = new QHBoxLayout;
	lySort->addWidget(radioSrtName);
	lySort->addWidget(radioSrtType);
	lySort->addWidget(radioSrtNameType);
	SortOptions->setLayout(lySort);
	lySortBox->addWidget(SortOptions);

	btnSortProducts = new QPushButton("Sorteaza produse");
	lySortBox->addWidget(btnSortProducts);

	lyLeft->addWidget(groupBoxSort);


	// FILTER
	QVBoxLayout* layerGroupBoxFilter = new QVBoxLayout;
	QWidget* wGroupBoxFilter = new QWidget;

	groupBoxFilter->setLayout(layerGroupBoxFilter);
	layerGroupBoxFilter->addWidget(wGroupBoxFilter);
	lyLeft->addWidget(groupBoxFilter);

	QWidget* formFilter = new QWidget;
	QFormLayout* lyFormFilter = new QFormLayout;
	formFilter->setLayout(lyFormFilter);
	editFilter = new QLineEdit();
	lyFormFilter->addRow(lblFilterCriteria, editFilter);
	layerGroupBoxFilter->addWidget(formFilter);

	btnFilterType = new QPushButton("Filtreaza dupa tip");
	btnFilterProducer = new QPushButton("Filtreaza dupa producator");
	QWidget* filterBtns = new QWidget;
	QHBoxLayout* hlFilter = new QHBoxLayout;
	filterBtns->setLayout(hlFilter);
	hlFilter->addWidget(btnFilterType);
	hlFilter->addWidget(btnFilterProducer);

	layerGroupBoxFilter->addWidget(filterBtns);
	lyLeft->addStretch();

	// button used to reload data (eg. after a filtration)
	lyLeft->addWidget(btnReloadData);


	// add the left part
	lyMain->addWidget(left);

	// RIGHT PART of the window
	QWidget* right = new QWidget;
	QVBoxLayout* lyRight = new QVBoxLayout;
	right->setLayout(lyRight);

	// product table
	int noLines = 3;
	int noColumns = 5;
	this->productTable = new QTableWidget{ noLines, noColumns };

	// table header
	QStringList tblHeaderList;
	tblHeaderList << "ID" << "NUME" << "TIP" << "PRET" << "PRODUCATOR";
	this->productTable->setHorizontalHeaderLabels(tblHeaderList);

	// resize the table according to the content
	this->productTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	lyRight->addWidget(productTable);

	lyRight->addWidget(btnAddRandomProducts);
	lyRight->addWidget(btnUndo);
	lyRight->addWidget(btnReportType);

	// CART
	QVBoxLayout* lyCartBox = new QVBoxLayout;
	this->groupBoxCart->setLayout(lyCartBox);

	QFormLayout* lyCart = new QFormLayout;
	lyCartBox->addLayout(lyCart);

	lyCart->addRow(lblIdCart, editIdCart);
	lyCart->addWidget(btnAddToCart);

	lyCart->addRow(lblNrCart, editNrCart);
	lyCart->addWidget(btnAddRandomCart);
	
	lyCart->addRow(btnEmptyCart);
	lyCart->addRow(btnCart, btnCartDrawings);


	lyRight->addWidget(groupBoxCart);

	// add the right part
	lyMain->addWidget(right);

}

void UI::connectSignalsSlots()
{

	// button to add a product
	QObject::connect(btnAddProduct, &QPushButton::clicked, this, &UI::guiAddProduct);

	// button to delete a product
	QObject::connect(btnDeleteProduct, &QPushButton::clicked, [&]() {
		deleteProductUI->show();

		this->reloadList(srv.getAllProducts());
		});

	// button to update a product
	QObject::connect(btnUpdateProduct, &QPushButton::clicked, [&]() {
		updateProductUI->show();

		this->reloadList(srv.getAllProducts());
		}
	);

	// button to search a product
	QObject::connect(btnSearchProduct, &QPushButton::clicked, [&]() {
		searchProductUI->show();

		this->reloadList(srv.getAllProducts());
		});

	// button to sort a list
	QObject::connect(btnSortProducts, &QPushButton::clicked, [&]() {
		if (this->radioSrtName->isChecked())
			this->reloadList(srv.sortByName());
		else if (this->radioSrtType->isChecked())
			this->reloadList(srv.sortByType());
		else if (this->radioSrtNameType->isChecked())
			this->reloadList(srv.sortByNameType());
		});

	// button to filter a list by type
	QObject::connect(btnFilterType, &QPushButton::clicked, [&]() {
		string type = this->editFilter->text().toStdString();
		this->reloadList(srv.filterByType(type));
		});

	// button to filter a list by producer
	QObject::connect(btnFilterProducer, &QPushButton::clicked, [&]() {
		string producer = this->editFilter->text().toStdString();
		this->reloadList(srv.filterByProducer(producer));
		});

	// button to reload data in the table
	QObject::connect(btnReloadData, &QPushButton::clicked, [&]() {
		this->reloadList(srv.getAllProducts());
		});

	// button to undo the last action
	QObject::connect(btnUndo, &QPushButton::clicked, [&]() {
		string error = srv.undo();
		if (error != "") // error for invalid data
			QMessageBox::warning(this, QString::fromStdString("Eroare!"), QString::fromStdString(error));
		else this->reloadList(srv.getAllProducts());
		});

	// button to add some random products
	QObject::connect(btnAddRandomProducts, &QPushButton::clicked, [&]() {
		srv.addRandomProducts();
		this->reloadList(srv.getAllProducts());
		});

	// button to show the report (how many products of each type exist)
	QObject::connect(btnReportType, &QPushButton::clicked, [&]() {

		QWidget* window = new QWidget;
		window->setWindowTitle("Raport Produs");

		QHBoxLayout* ly = new QHBoxLayout;
		window->setLayout(ly);
		
		QTableWidget* reportTable = new QTableWidget{ 3, 2 };
		reportTable->setRowCount(1);
		
		// table header
		QStringList tblHeaderList;
		tblHeaderList << "TIP" << "NR ARTICOLE";
		reportTable->setHorizontalHeaderLabels(tblHeaderList);

		ly->addWidget(reportTable);
		window->show();

		// clean the table
		reportTable->clearContents();
		reportTable->setRowCount(srv.reportProducts().size());
		int lineNumber = 0;
		for (auto product : srv.reportProducts()) {
			reportTable->setItem(lineNumber, 0, new QTableWidgetItem(QString::fromStdString(product.first)));
			reportTable->setItem(lineNumber, 1, new QTableWidgetItem(QString::number(product.second)));;
			lineNumber++;
		}
		});

	// button to add random products to the cart
	QObject::connect(btnAddRandomCart, &QPushButton::clicked, [&]() {
		string nr = editNrCart->text().toStdString();
		editNrCart->clear();

		string error = srv.generate(nr);
		if (error != "") // error for invalid data
			QMessageBox::warning(this, QString::fromStdString("Eroare!"), QString::fromStdString(error));
		});

	// button to add to the cart
	QObject::connect(btnAddToCart, &QPushButton::clicked, [&]() {
		string id = editIdCart->text().toStdString();
		editIdCart->clear();

		string error = srv.addToCart(id);
		if (error != "") // error for invalid data
			QMessageBox::warning(this, QString::fromStdString("Eroare!"), QString::fromStdString(error));
		});

	// button to empty the cart
	QObject::connect(btnEmptyCart, &QPushButton::clicked, [&]() {
		srv.emptyCart();
		});

	// button to open the cart
	QObject::connect(btnCart, &QPushButton::clicked, [&]() {
		CartUI* cartProductUI = new CartUI(srv, srv.getCartObj());
		cartProductUI->buildUI();
		cartProductUI->connectSignalsSlots();
		cartProductUI->show();
		});

	// button to open the cart drawings
	QObject::connect(btnCartDrawings, &QPushButton::clicked, [&]() {
		CartReadOnlyGUI* cartDrawings = new CartReadOnlyGUI(srv.getCartObj());
		cartDrawings->show();
		});
}

void UI::reloadList(vector<Product> productList)
{
	// clean the text boxes
	this->editName->clear();
	this->editType->clear();
	this->editPrice->clear();
	this->editProducer->clear();

	// clean the table
	this->productTable->clearContents();
	this->productTable->setRowCount(productList.size());

	int lineNumber = 0;
	for (auto& product : productList) {
		this->productTable->setItem(lineNumber, 0, new QTableWidgetItem(QString::number(product.getId())));
		this->productTable->setItem(lineNumber, 1, new QTableWidgetItem(QString::fromStdString(product.getName())));
		this->productTable->setItem(lineNumber, 2, new QTableWidgetItem(QString::fromStdString(product.getType())));
		this->productTable->setItem(lineNumber, 3, new QTableWidgetItem(QString::number(product.getPrice())));
		this->productTable->setItem(lineNumber, 4, new QTableWidgetItem(QString::fromStdString(product.getProducer())));
		lineNumber++;
	}
}

void UI::guiAddProduct()
{
	string name = this->editName->text().toStdString();
	string type = this->editType->text().toStdString();
	string price = this->editPrice->text().toStdString();
	string producer = this->editProducer->text().toStdString();

	string error = srv.addProduct(name, type, price, producer);

	
	if (error != "") // error for invalid data
		QMessageBox::warning(this, QString::fromStdString("Eroare!"), QString::fromStdString(error));
	else
		this->reloadList(srv.getAllProducts());
}


// DELETE UI
void DeleteUI::buildUI()
{
	this->setWindowTitle("Elimina produs");
	QFormLayout* lyForm = new QFormLayout;
	this->setLayout(lyForm);

	lyForm->addRow(lblId, editId);
	lyForm->addWidget(btnDel);
}

void DeleteUI::connectSignalsSlots()
{
	QObject::connect(btnDel, &QPushButton::clicked, [&]() {
		string error = srv.deleteProduct(editId->text().toStdString());
		if (error != "")
			QMessageBox::warning(this, QString::fromStdString("Eorare!"), QString::fromStdString(error));
		this->editId->clear();
		this->close();
		});
	
}

// UPDATE UI
void UpdateUI::buildUI()
{
	this->setWindowTitle("Actualizeaza produs");
	QFormLayout* lyForm = new QFormLayout;
	this->setLayout(lyForm);

	lyForm->addRow(new QLabel("Modifica:"));

	lyForm->addWidget(radioUpdName);
	lyForm->addWidget(radioUpdType);
	lyForm->addWidget(radioUpdPrice);
	lyForm->addWidget(radioUpdProducer);

	lyForm->addRow(lblId, editId);
	lyForm->addRow(lblNewVal, editNewVal);

	lyForm->addWidget(btnUpd);

}

void UpdateUI::connectSignalsSlots()
{
	QObject::connect(btnUpd, &QPushButton::clicked, [&]() {
		string id = this->editId->text().toStdString();
		string newVal = this->editNewVal->text().toStdString();
		string error;

		if (this->radioUpdName->isChecked())
			error = srv.updateProduct(id, newVal, 1);
		else if (this->radioUpdType->isChecked())
			error = srv.updateProduct(id, newVal, 2);
		else if (this->radioUpdPrice->isChecked())
			error = srv.updateProduct(id, newVal, 3);
		else if (this->radioUpdProducer->isChecked())
			error = srv.updateProduct(id, newVal, 4);
		else QMessageBox::warning(this, QString::fromStdString("Eorare!"), QString::fromStdString("Selectati o optiune!"));
		
		if (error != "")
			QMessageBox::warning(this, QString::fromStdString("Eorare!"), QString::fromStdString(error));
		this->editId->clear();
		this->editNewVal->clear();
		this->close();
		});
}

// SEARCH UI
void SearchUI::buildUI()
{
	this->setWindowTitle("Cauta Produs");

	// main layout - orizontal
	QHBoxLayout* lyMain = new QHBoxLayout;
	this->setLayout(lyMain);
	lyMain->setSpacing(10);

	// LEFT PART 
	QWidget* left = new QWidget;
	QVBoxLayout* lyLeft = new QVBoxLayout;
	left->setLayout(lyLeft);

	QFormLayout* lyForm = new QFormLayout;
	this->groupBoxForm->setLayout(lyForm);

	lyForm->addRow(new QLabel("Introduceti datele cunsocute:"));

	lyForm->addRow(lblId, editId);
	lyForm->addRow(lblName, editName);
	lyForm->addRow(lblType, editType);
	lyForm->addRow(lblPrice, editPrice);
	lyForm->addRow(lblProducer, editProducer);

	lyForm->addWidget(btnSearch);

	lyLeft->addWidget(groupBoxForm);

	lyMain->addWidget(left);


	// RIGHT PART
	QWidget* right = new QWidget;
	QVBoxLayout* lyRight = new QVBoxLayout;
	right->setLayout(lyRight);

	// product table
	int noLines = 3;
	int noColumns = 5;
	this->productTable = new QTableWidget{ noLines, noColumns };

	// table header
	QStringList tblHeaderList;
	tblHeaderList << "ID" << "NUME" << "TIP" << "PRET" << "PRODUCATOR";
	this->productTable->setHorizontalHeaderLabels(tblHeaderList);

	// resize the table according to the content
	this->productTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	lyRight->addWidget(productTable);

	// add the right part
	lyMain->addWidget(right);
}


void SearchUI::connectSignalsSlots()
{
	QObject::connect(btnSearch, &QPushButton::clicked, [&]() {
		string id = this->editId->text().toStdString();
		string name = this->editName->text().toStdString();
		string type = this->editType->text().toStdString();
		string price = this->editPrice->text().toStdString();
		string producer = this->editProducer->text().toStdString();

		this->reloadList(srv.searchProduct(id, name, type, price, producer));
		});
}

void SearchUI::reloadList(vector<Product> productList)
{
	// clean the text boxes
	this->editId->clear();
	this->editName->clear();
	this->editType->clear();
	this->editPrice->clear();
	this->editProducer->clear();

	// clean the table
	this->productTable->clearContents();
	this->productTable->setRowCount(productList.size());

	int lineNumber = 0;
	for (auto& product : productList) {
		this->productTable->setItem(lineNumber, 0, new QTableWidgetItem(QString::number(product.getId())));
		this->productTable->setItem(lineNumber, 1, new QTableWidgetItem(QString::fromStdString(product.getName())));
		this->productTable->setItem(lineNumber, 2, new QTableWidgetItem(QString::fromStdString(product.getType())));
		this->productTable->setItem(lineNumber, 3, new QTableWidgetItem(QString::number(product.getPrice())));
		this->productTable->setItem(lineNumber, 4, new QTableWidgetItem(QString::fromStdString(product.getProducer())));
		lineNumber++;
	}
}

void CartUI::buildUI()
{
	this->setWindowTitle("Cos de cumparaturi");

	// main layout - orizontal
	QHBoxLayout* lyMain = new QHBoxLayout;
	this->setLayout(lyMain);
	lyMain->setSpacing(10);

	// LEFT PART 
	QWidget* left = new QWidget;
	QVBoxLayout* lyLeft = new QVBoxLayout;
	left->setLayout(lyLeft);

	// Add
	QFormLayout* lyAddBox = new QFormLayout;
	this->groupBoxAdd->setLayout(lyAddBox);
	lyAddBox->addRow(lblAdd, editAdd);
	lyAddBox->addRow(btnAdd);
	lyLeft->addWidget(groupBoxAdd);

	// Generate
	QFormLayout* lyGenerateBox = new QFormLayout;
	this->groupBoxGenerate->setLayout(lyGenerateBox);
	lyGenerateBox->addRow(lblGenerate, editGenerate);
	lyGenerateBox->addRow(btnGenerate);
	lyLeft->addWidget(groupBoxGenerate);

	// Export
	QFormLayout* lyExportBox = new QFormLayout;
	this->groupBoxExport->setLayout(lyExportBox);
	lyExportBox->addRow(lblExport, editExport);
	lyExportBox->addRow(btnExport);
	lyLeft->addWidget(groupBoxExport);

	// Empty cart button
	lyLeft->addWidget(btnEmptyCart);

	lyMain->addWidget(left);


	// RIGHT PART
	QWidget* right = new QWidget;
	QVBoxLayout* lyRight = new QVBoxLayout;
	right->setLayout(lyRight);

	productLst = new QListWidget;
	
	lyRight->addWidget(productLst);
	
	//// product table
	//int noLines = 3;
	//int noColumns = 5;
	//this->productTable = new QTableWidget{ noLines, noColumns };

	//// table header
	//QStringList tblHeaderList;
	//tblHeaderList << "ID" << "NUME" << "TIP" << "PRET" << "PRODUCATOR";
	//this->productTable->setHorizontalHeaderLabels(tblHeaderList);

	//// resize the table according to the content
	//this->productTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	///lyRight->addWidget(productTable);

	// add the right part
	lyMain->addWidget(right);
}

void CartUI::connectSignalsSlots()
{
	
	this->reloadList(srv.getCart());
	cart.addObserver(this);

	// button to add a product to the cart
	QObject::connect(btnAdd, &QPushButton::clicked, [&]() {
		string id = this->editAdd->text().toStdString();
		string error = srv.addToCart(id);

		if (error != "") // error for invalid data
			QMessageBox::warning(this, QString::fromStdString("Eroare!"), QString::fromStdString(error));
		else this->reloadList(srv.getCart());
		});

	// button to generate products to the cart
	QObject::connect(btnGenerate, &QPushButton::clicked, [&]() {
		string nr = this->editGenerate->text().toStdString();
		string error = srv.generate(nr);

		if (error != "") // error for invalid data
			QMessageBox::warning(this, QString::fromStdString("Eroare!"), QString::fromStdString(error));

		this->reloadList(srv.getCart());
		});

	// button to export the cart list
	QObject::connect(btnExport, &QPushButton::clicked, [&]() {
		string fileName = this->editExport->text().toStdString();

		srv.Export(fileName);

		this->reloadList(srv.getCart());
		});

	// button to empty the cart
	QObject::connect(btnEmptyCart, &QPushButton::clicked, [&]() {
		srv.emptyCart();
		this->reloadList(srv.getCart());
		});
}

void CartUI::reloadList(vector<Product> cartList)
{
	// clean the text boxes
	this->editAdd->clear();
	this->editGenerate->clear();
	this->editExport->clear();

	productLst->clear();
	for (auto& p : cartList)
	{
		productLst->addItem("- " + QString::number(p.getId()) + ": "+ QString::fromStdString(p.getName()) + ", " + QString::fromStdString(p.getType()) + ", " + QString::number(p.getPrice()) + ", " + QString::fromStdString(p.getProducer()));
	}

	//// clean the table
	//this->productTable->clearContents();
	//this->productTable->setRowCount(cartList.size());

	/*int lineNumber = 0;
	for (auto& product : cartList) {
		this->productTable->setItem(lineNumber, 0, new QTableWidgetItem(QString::number(product.getId())));
		this->productTable->setItem(lineNumber, 1, new QTableWidgetItem(QString::fromStdString(product.getName())));
		this->productTable->setItem(lineNumber, 2, new QTableWidgetItem(QString::fromStdString(product.getType())));
		this->productTable->setItem(lineNumber, 3, new QTableWidgetItem(QString::number(product.getPrice())));
		this->productTable->setItem(lineNumber, 4, new QTableWidgetItem(QString::fromStdString(product.getProducer())));
		lineNumber++;
	}*/
}
