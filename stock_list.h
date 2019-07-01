#ifndef STOCK_LIST_H
#define STOCK_LIST_H
#include <Wt/WContainerWidget.h>
#include <Wt/WTableView.h>
#include "pg_model.h"

class stock_list : public Wt::WContainerWidget
{
public:
    stock_list(WString Query);
private:
     Wt::WTableView *tdata;
};

#endif // STOCK_LIST_H
