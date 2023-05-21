#include "Product.h"
#include "ProductService.h"

#include <assert.h>
#include <algorithm>

const vector<Product> ProductService::getAllProducts()
{
    return repo.getAllProducts();
}

static int id_count = 0;
string ProductService::addProduct(const string name, const string type, const string price, const string producer)
{
    if (!(val.validateString(name) && val.validateString(type) && val.validateString(producer)))
        return "Toate casutele trebuie completate";
    
    if (!val.validatePrice(price))
        return "Pret invalid";

    double pr = std::stod(price);

    Product p{ id_count++, name, type, pr, producer };
    repo.addProduct(p);

    // add the action to the undo list
    undoList.push_back(std::make_unique<UndoAdd>(repo, p)); 
    return "";
}

string ProductService::deleteProduct(const string id)
{
    if (!val.validateId(id))
        return "ID invalid";

    int idVal = std::stoi(id);

    // add the action to the undo list
    int pos = repo.findProduct(idVal);
    if (pos != -1)
    {
        Product p = repo.getAllProducts()[pos];
        undoList.push_back(std::make_unique<UndoDelete>(repo, p));
    }
        

    // delete the product
    return repo.deleteProduct(idVal);
}

string ProductService::updateProduct(const string id, const string newVal, int what)
{
    // validate input data
    if (!val.validateId(id))
        return "ID invalid";

    if (what == 3) { // price
        if (!val.validatePrice(newVal))
            return "Pret invalid";
    }
    else if (!val.validateString(newVal))
            return "Casuta pentru valoarea nu poate fi goala";
    
    // add the action to the undo list
    int pos = repo.findProduct(stoi(id));
    if (pos != -1)
    {
        Product p = repo.getAllProducts()[pos];
        if (what == 1)
            undoList.push_back(std::make_unique<UndoUpdate>(repo, stoi(id), p.getName(), what));
        else if (what == 2)
            undoList.push_back(std::make_unique<UndoUpdate>(repo, stoi(id), p.getType(), what));
        else if (what == 3)
            undoList.push_back(std::make_unique<UndoUpdate>(repo, stoi(id), std::to_string(p.getPrice()), what));
        else if (what == 4)
            undoList.push_back(std::make_unique<UndoUpdate>(repo, stoi(id), p.getProducer(), what));
    }

    // update the product
    return repo.updateProduct(stoi(id), newVal, what);
}

vector<Product> ProductService::filterByType(const string type) 
{
    vector<Product> rez;
    for (const auto& p : repo.getAllProducts()) {
        if (p.getType() == type)
            rez.push_back(p);
    }
    return rez;
}

vector<Product> ProductService::filterByPrice(const string pMin, const string pMax)
{
    vector<Product> rez;
    for (const auto& p : repo.getAllProducts()) {
        if (stod(pMin)<=p.getPrice() && p.getPrice() <= stod(pMax))
            rez.push_back(p);
    }
    return rez;
}

vector<Product> ProductService::filterByProducer(const string producer)
{
    vector<Product> rez;
    for (const auto& p : repo.getAllProducts()) {
        if (p.getProducer() == producer)
            rez.push_back(p);
    }
    return rez;
}

vector<Product> ProductService::searchProduct(const string id, const string name, const string type, const string price, const string producer)
{
    vector<Product>current = repo.getAllProducts();
    vector<Product>rez;
    copy_if(current.begin(), current.end(), back_inserter(rez), [=](Product p) {
        if (val.validateId(id) && p.getId() != stod(id))
            return false;
       if (val.validateString(name) && p.getName() != name)
            return false;
        if (val.validateString(type) && p.getType() != type)
            return false;
        if (val.validatePrice(price) && p.getPrice() != stod(price))
            return false;
        if (val.validateString(producer) && p.getProducer() != producer)
            return false;
        return true;
        });
    return rez;
}

vector<Product> ProductService::sortByName() {
    auto copyAll = repo.getAllProducts();
    std::sort(copyAll.begin(), copyAll.end(), cmpName);
    return copyAll;
}

vector<Product> ProductService::sortByType() {
    auto copyAll = repo.getAllProducts();
    std::sort(copyAll.begin(), copyAll.end(), cmpType);
    return copyAll;
}

vector<Product> ProductService::sortByNameType() {
    auto copyAll = repo.getAllProducts();
    std::sort(copyAll.begin(), copyAll.end(), [](const Product& p1, const Product& p2) {
        if (p1.getName() == p2.getName()) {
            return p1.getType() < p2.getType();
        }
        return p1.getName() < p2.getName();
        });
    return copyAll;
}

void ProductService::addRandomProducts()
{
    addProduct("mere", "fructe", "4.6", "Romania");
    addProduct("miere", "alimente", "15", "Stupul Vesel");
    addProduct("masline", "fructe", "30", "Grecia");
    addProduct("faina", "neperisabile", "5", "Baneasa");
    addProduct("lapte", "lactate", "4.8", "Dorna");
    addProduct("cascaval", "lactate", "30.55", "Delaco");
    addProduct("orez", "neperisabile", "9.15", "Deroni");
}

// UNDO
string ProductService::undo()
{
    if (this->undoList.empty())
        return "Nu avem la ce face undo!";

    this->undoList.back()->doUndo();
    this->undoList.pop_back();
    return "";
}

// CART
void ProductService::emptyCart()
{
    this->cart.emptyCart();
}

string ProductService::addToCart(string id)
{
    if (!val.validateId(id))
        return "ID invalid";
    Product x = this->repo.getAllProducts().at(repo.findProduct(stoi(id)));
    this->cart.add(x);
    return "";
}

string ProductService::generate(string nr)
{
    if (!val.validateInteger(nr))
        return "Trebuie introdus un numar!";
    int nrVal = stoi(nr);
    if (nrVal < 0)
        return "Numarul nu pate fi negativ!";
    this->cart.generate(nrVal, this->getAllProducts());
    return "";

}

vector<Product> ProductService::getCart()
{
    return this->cart.getList();
}

string ProductService::Export(string fileName)
{
    int n = fileName.length(), nr = 0;
    for (int i = 0; i < n; i++)
        if (fileName.at(i) == '.')
            nr++;

    if (nr != 1)
        fileName += ".html";

    cart.Export(fileName);
    return "";
}

const std::map<std::string, int> ProductService::reportProducts()
{
    auto products = repo.getAllProducts();
    std::map<std::string, int> map;
    for (int i = 0; i < products.size(); i++)
    {
        Product p = products[i];
        auto it = map.find(p.getType());

        if (it != map.end())
            it->second++;
        else
            map.insert(std::pair <std::string, int>(p.getType(), 1));
    }

    return map;
}

// TESTS
void testAddDeleteProduct()
{
    ProductRepo repo;
    ProductValidator val;
    ProductService srv{ repo, val };

    // try to delete some invalid IDs
    srv.deleteProduct("1");
    srv.deleteProduct("aa");
    assert(srv.getAllProducts().size() == 0);

    // add some products
    srv.addProduct("a", "a", "7", "a");
    srv.addProduct("b", "b", "17.9", "b");
    srv.addProduct("c", "c", "99.99", "c");
    assert(srv.getAllProducts().size() == 3);

    // add some invalid products
    srv.addProduct("", "", "0", "");
    srv.addProduct("a", "a", "- 2", "a");
    assert(srv.getAllProducts().size() == 3);

    // delete all elements
    int id = srv.getAllProducts()[0].getId();
    srv.deleteProduct(std::to_string(id));
    assert(srv.getAllProducts().size() == 2);

    id = srv.getAllProducts()[0].getId();
    srv.deleteProduct(std::to_string(id));
    assert(srv.getAllProducts().size() == 1);

    id = srv.getAllProducts()[0].getId();
    srv.deleteProduct(std::to_string(id));
    assert(srv.getAllProducts().size() == 0);
}

void testUpdateProduct() 
{
    ProductRepo repo;
    ProductValidator val;
    ProductService srv{ repo, val };

    // update an inexistent product
    assert(srv.updateProduct("1", "b", 1) != "");

    // add a product and update its data
    srv.addProduct("a", "a", "7", "a");
    int id = srv.getAllProducts()[0].getId();
    srv.updateProduct(std::to_string(id), "b", 1); // name
    srv.updateProduct(std::to_string(id), "b", 2); // type
    srv.updateProduct(std::to_string(id), "3", 3); // price
    srv.updateProduct(std::to_string(id), "b", 4); // producer

    Product pr = srv.getAllProducts()[0];
    assert(pr.getName() == "b");
    assert(pr.getType() == "b");
    assert(pr.getPrice() == 3);
    assert(pr.getProducer() == "b");

    // try for invalid data
    assert(srv.updateProduct(std::to_string(id), "", 1) != "");
    assert(srv.updateProduct("id", "b", 1) != "");
    assert(srv.updateProduct(std::to_string(id), "-7", 3) != "");
}

void testUndo()
{
    ProductRepo repo;
    ProductValidator val;
    ProductService srv{ repo, val };

    assert(srv.undo() != "");

    // add
    srv.addProduct("a", "a", "3", "a");
    srv.addProduct("b", "b", "9", "b");
    srv.addProduct("c", "c", "9", "c");
    srv.addProduct("d", "d", "10", "d");
    assert(srv.getAllProducts().size() == 4);
    srv.undo();
    srv.undo();
    assert(srv.getAllProducts().size() == 2);

    // delete
    srv.deleteProduct(std::to_string(srv.getAllProducts()[0].getId()));
    srv.deleteProduct(std::to_string(srv.getAllProducts()[0].getId()));
    assert(srv.getAllProducts().size() == 0);
    srv.undo();
    assert(srv.getAllProducts().size() == 1);
    srv.undo();
    assert(srv.getAllProducts().size() == 2);

    // update
    Product p = srv.getAllProducts().at(0);
    srv.updateProduct(std::to_string(p.getId()), "x", 1);
    assert(srv.getAllProducts().at(0).getName() == "x");
    srv.undo();
    assert(srv.getAllProducts().at(0).getName() == p.getName());
}

void testFilter()
{
    ProductRepo repo;
    ProductValidator val;
    ProductService srv{ repo, val };

    // add
    srv.addProduct("a", "a", "6", "b");
    srv.addProduct("b", "a", "60", "b");
    srv.addProduct("c", "b", "600", "a");

    // filter by price
    assert(srv.filterByPrice("6", "70").size() == 2);
    assert(srv.filterByPrice("2", "3").size() == 0);
    assert(srv.filterByPrice("70", "4").size() == 0);

    // filter by type
    assert(srv.filterByType("a").size() == 2);
    assert(srv.filterByType("c").size() == 0);

    // filter by producer
    assert(srv.filterByProducer("b").size() == 2);
    assert(srv.filterByProducer("c").size() == 0);
}

void testSort()
{
    ProductRepo repo;
    ProductValidator val;
    ProductService srv{ repo,val };
    srv.addProduct("z", "z", "6", "d");
    srv.addProduct("b", "a", "60", "d");
    srv.addProduct("b", "d", "600", "d");

    // sort by type
    auto firstP = srv.sortByType()[0];
    assert(firstP.getType() == "a");

    // sort by name
    firstP = srv.sortByName()[0];
    assert(firstP.getName() == "b");

    // sort by name+type
    firstP = srv.sortByNameType()[0];
    assert(firstP.getPrice() == 60);
}

void testAddRandomProducts()
{
    ProductRepo repo;
    ProductValidator val;
    ProductService srv{ repo, val };

    srv.addRandomProducts();
    assert(srv.getAllProducts().size() != 0);
}

void testSearchProduct()
{
    ProductRepo repo;
    ProductValidator val;
    ProductService srv{ repo, val };

    srv.addProduct("mere", "fructe", "6", "Romania");
    srv.addProduct("miere", "alimente", "15", "Stupul Vesel");
    srv.addProduct("masline", "fructe", "25", "Grecia");
    srv.addProduct("pere", "fructe", "6", "Grecia");
    srv.addProduct("faina", "neperisabile", "5", "Baneasa");

    assert(srv.searchProduct("", "", "fructe", "6", "").size() == 2);
    assert(srv.searchProduct("", "", "", "5", "").size() == 1);
}

void testCartList()
{
    ProductRepo rep;
    ProductValidator val;
    ProductService srv{ rep, val };
    srv.addProduct("a", "a", "6", "a");
    srv.addProduct("b", "a", "6", "a");
    srv.addProduct("c", "a", "6", "a");

    // add
    assert(srv.addToCart(std::to_string(srv.getAllProducts()[0].getId())) == "");
    assert(srv.addToCart("a") != "");
    assert(srv.getCart().size() == 1);
   
    //empty
    srv.emptyCart();
    assert(srv.getCart().size() == 0);

    // generate
    srv.generate("3");
    assert(srv.getCart().size() == 3);
    srv.emptyCart();
    assert(srv.getCart().size() == 0);
}

void testReportProducts()
{
    ProductRepo repo;
    ProductValidator val;
    ProductService srv{ repo, val };
    srv.addProduct("a", "a", "3", "a");
    srv.addProduct("a", "b", "3", "a");
    srv.addProduct("a", "a", "3", "a");
    srv.addProduct("a", "a", "3", "a");
    assert(srv.reportProducts().size() == 2);
}

void testService()
{
    testAddDeleteProduct();
    testUpdateProduct();
    testFilter();
    testSort();
    testAddRandomProducts();
    testSearchProduct();
    testUndo();
    testCartList();
    testReportProducts();
}
