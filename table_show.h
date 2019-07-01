#ifndef TABLE_SHOW_H
#define TABLE_SHOW_H
#include <pqxx/pqxx>
#include <Wt/WText.h>
#include <Wt/WTable.h>
#include <Wt/WString.h>
#include <Wt/WTableView.h>
#include <Wt/WStandardItemModel.h>
#include <Wt/WString.h>
#include <Wt/WApplication.h>
#include <Wt/WSpinBox.h>
#include <Wt/WDoubleSpinBox.h>
#include <Wt/WPushButton.h>
#include <Wt/WMenuItem.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WSelectionBox.h>
#include <Wt/WStringListModel.h>
#include <memory>
#include <vector>
#include <iostream>
#include "pg_model.h"
class table_show:public Wt::WContainerWidget
{
public:
    //table_show(const Wt::WString DBtable,Wt::WContainerWidget *container);
    table_show(const Wt::WString DBtable);
    void table_data(const Wt::WString DBtable);
 //   ~table_show();
private:
     Wt::WTable *theader;
   Wt::WTableView *tdata;
  //  Wt::WContainerWidget *container1;
   std::vector<Wt::WLineEdit *> ptrline;
   std::vector<Wt::WLineEdit *> ptrline_esp;
   std::vector<Wt::WSpinBox *> ptrspbox;
   std::vector<Wt::WDoubleSpinBox *> ptrDspbox;
  std::shared_ptr<pg_model> db_model;
   Wt::WSpinBox *spin_limit;
   Wt::WPopupMenu *popup;
   std::vector<WString> columnas;
};

#endif // TABLE_SHOW_H
