#ifndef TABLE_ADD_DATA_H
#define TABLE_ADD_DATA_H
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
#include <Wt/WDateEdit.h>
#include <memory>
#include <vector>
#include <iostream>
#include "pg_model.h"
#include <auto_complete_wline.h>

class table_add_data : public  Wt::WContainerWidget
{
public:
  table_add_data(const Wt::WString DBtable_,const Wt::WString tfield_,const Wt::WString value_);
private:
  void table_data();
  Wt::WTable *theader;
Wt::WTableView *tdata;
//  Wt::WContainerWidget *container1;
std::shared_ptr<pg_model> db_model;
Wt::WSpinBox *spin_limit;
Wt::WPopupMenu *popup;
std::vector<Wt::WString> columnas;
Wt::WString DBtable,tfield,value;
};

#endif // TABLE_ADD_DATA_H
