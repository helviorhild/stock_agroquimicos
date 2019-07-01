#ifndef PRODUCT_ADD_H
#define PRODUCT_ADD_H
#include <Wt/WPopupMenu.h>
#include <Wt/WString.h>
#include <Wt/WSelectionBox.h>
#include <Wt/WPushButton.h>
#include <Wt/WLineEdit.h>
#include <Wt/WSpinBox.h>
#include <Wt/WDoubleSpinBox.h>
#include <Wt/WTable.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WStringListModel.h>
#include <vector>
#include <iostream>
#include <pqxx/pqxx>

class product_add : public Wt::WContainerWidget
{
public:
  product_add();
private:
  void ejecute(Wt::WString &DBtable);
  void table_imput(Wt::WString DBtable);
  Wt::WTable *theader;
  //Vector's for automatic generate  imput menu
  std::vector<Wt::WLineEdit *> ptrline;
  std::vector<Wt::WLineEdit *> ptrline_esp;
  std::vector<Wt::WSpinBox *> ptrspbox;
  std::vector<Wt::WDoubleSpinBox *> ptrDspbox;
  Wt::WPopupMenu *popup;
  Wt::WPushButton *tabla_popup;
};

#endif // PRODUCT_ADD_H
