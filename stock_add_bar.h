#ifndef STOCK_ADD_BAR_H
#define STOCK_ADD_BAR_H
#include <pqxx/pqxx>
#include <Wt/WContainerWidget.h>
#include <string.h>
#include <Wt/WText.h>
#include <Wt/WFileUpload.h>
#include <Wt/WPushButton.h>
class stock_add_bar : public Wt::WContainerWidget
{
public:
  stock_add_bar();
private:
  void fileUploaded();
  void fileTooLarge();
};

#endif // STOCK_ADD_BAR_H
