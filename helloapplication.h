#ifndef HELLOAPPLICATION_H
#define HELLOAPPLICATION_H
#include <memory.h>
#include <pqxx/pqxx>
#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WTable.h>
#include <Wt/WBoxLayout.h>
#include <Wt/WCheckBox.h>
#include <vector>
#include <Wt/WSpinBox.h>
#include "helloapplication.h"
#include "table_show.h"
#include "stock_list.h"
#include "stock_add.h"
#include "product_add.h"
#include "create_tabla.h"
#include "rest_service.h"
#include "stock_add_bar.h"
#include "blob_data.h"
#include "excel_export.h"
#include "export_pdf.h"
#include "formularios.h"
#include "auto_complete_wline.h"
using namespace Wt;
class HelloApplication : public Wt::WApplication
{
public:
  HelloApplication(const WEnvironment& env);
private:
  //Functions
  void greet(WString &buff);
 void select_table(WPushButton *mywidget);
 void select_field(const std::string &);
 void select_field2(const std::string &);
 void make_query(const std::string &);
 void Delete_tbshow(table_show *tbshow);
 void stock_view();
void stock_minimo();
 void Delete_stock_list(stock_list  *tbshow);
void stock_manager(WPushButton *mywidget);
//void stock_edit();
void stock_add_call();
void new_product();
void tabla_add();
void  stock_add_barras();
 //General variables
  Wt::WContainerWidget *container1,*container;
std::unique_ptr<Wt::WLineEdit>  nameEdit_;
Wt::WText *greeting_;
  Wt::WTable *table,*theader;
   std::vector<Wt::WLineEdit *>ptrline;
   std::vector<Wt::WSpinBox *> ptrspbox;
};


#endif // HELLOAPPLICATION_H
