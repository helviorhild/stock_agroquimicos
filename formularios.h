#ifndef FORMULARIOS_H
#define FORMULARIOS_H
#include <pqxx/pqxx>
#include <Wt/WContainerWidget.h>
#include <Wt/WPushButton.h>
#include <Wt/WLineEdit.h>
#include <Wt/WSpinBox.h>
#include <Wt/WDoubleSpinBox.h>
#include <Wt/WTable.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WDateEdit.h>
#include <coordenadas_dialog.h>
#include <table_add_data.h>
#include <vector>

class formularios : public Wt::WContainerWidget
{
public:
  formularios();
 ~formularios();
private:
  void show_form(std::string table_name);
  void ejecutar(std::string tabla);
  Wt::WContainerWidget *table_chooce_form;
   Wt::WTable *theader;
   Wt::WString coordenadas_text;
  std::unique_ptr<coordenadas_dialog> dialo_coor;
 std::vector<Wt::WPopupMenu*>vec_popup;
  Wt::WText *leyenda;
  size_t id_app;
};

#endif // FORMULARIOS_H
