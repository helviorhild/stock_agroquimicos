#ifndef CREATE_TABLA_H
#define CREATE_TABLA_H
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WTable.h>
#include <Wt/WPopupMenu.h>
#include <string.h>
#include <pqxx/pqxx>
#include <memory.h>
class create_tabla : public Wt::WContainerWidget
{
public:
  create_tabla();
private:
  void crear(Wt::WTable *table, const Wt::WString tablename);
  Wt::WPopupMenu *popup;
};

#endif // CREATE_TABLA_H
