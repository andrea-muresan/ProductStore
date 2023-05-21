#include <QtWidgets>
#include <iostream>

#include "ProductService.h"
#include "ProductValidator.h"
#include "UI.h"



void testAll() {
    testValidator();
    testService();
}
int main(int argc, char *argv[])
{
    testAll();

    QApplication a(argc, argv);

    ProductRepo repo;
    ProductValidator val;
    ProductService srv{ repo, val };

    UI gui{ srv };
    gui.buildUI();
    gui.connectSignalsSlots();

    gui.show();

    return a.exec();

}
