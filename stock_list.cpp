#include "stock_list.h"

stock_list::stock_list( Wt::WString Query)
{
 Wt::WString server= "dbname=agroquimicos user=wt password=ninguna host=localhost";
 tdata=this->addWidget(Wt::cpp14::make_unique<Wt::WTableView>());
auto db_model=std::make_shared<pg_model>(server,Query,-1);
 // auto db_model=std::make_unique<pg_model>(server,Query);
 tdata->setModel(db_model);
}
