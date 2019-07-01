#ifndef AUTO_COMPLETE_WLINE_H
#define AUTO_COMPLETE_WLINE_H
#include <Wt/WLineEdit>
#include <Wt/WContainerWidget.h>
#include <pqxx/pqxx>
#include <Wt/WPopupMenu.h>

class auto_complete_wline : public Wt::WLineEdit
{

public:
  auto_complete_wline(std::string server_, std::string table_, std::string column_);
private:
  void update();
  std::string server,table,column,fill;
 // Wt::WLineEdit *le;
  std::shared_ptr<Wt::WPopupMenu> popup;
};

#endif // AUTO_COMPLETE_WLINE_H
