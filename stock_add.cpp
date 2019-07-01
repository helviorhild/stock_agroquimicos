#include "stock_add.h"

stock_add::stock_add(int modo)//modo 3 para stock y 4 para stock_min
{
popup=new Wt::WPopupMenu();
Wt::WString  QueryBuffer ;
    try{

    pqxx::connection c("dbname=agroquimicos user=wt password=ninguna host=localhost");
    pqxx::work w(c);

    pqxx::result r = w.exec("\
     SELECT table_name FROM information_schema.columns WHERE table_schema = 'public' AND column_name='nombre_com'\
     INTERSECT  SELECT table_name FROM information_schema.columns WHERE table_schema = 'public' AND column_name='stock'\
     INTERSECT  SELECT table_name FROM information_schema.columns WHERE table_schema = 'public' AND column_name='stock_min'"  );
     w.commit();
      for (pqxx::result::const_iterator i = r.begin(); i != r.end(); ){
       popup->addItem((*i)[0].c_str());
       list_tables.push_back((*i)[0].c_str());
        QueryBuffer   +=  "SELECT "+w.quote((*i)[0].c_str())+" as tabla,nombre_com,distribuidor,stock,stock_min FROM ";
        QueryBuffer   +=(*i)[0].c_str();
          if((++i)!= r.end())QueryBuffer   +=  "  UNION ";
    }
    QueryBuffer+=" LIMIT 10";
    }
    catch(const std::exception &e)
    {
      this->addWidget(std::make_unique<Wt::WText>(e.what()));
    }
auto tabla_popup = this->addWidget(cpp14::make_unique<Wt::WPushButton>("Tabla"));
tabla_popup->setToolTip("Pulse para seleccionar una tabla de origen");
this->addWidget(cpp14::make_unique<Wt::WText>("\t nombre_com"));
nombre_com=this->addWidget(cpp14::make_unique<Wt::WLineEdit>());
nombre_com->setToolTip("Permite buscar por nombre el producto, con escribir un par de caracteres es suficiente");
nombre_com->keyWentUp().connect(this,&stock_add::update);
this->addWidget(cpp14::make_unique<Wt::WText>("\t  distribuidor"));
distribuidor=this->addWidget(cpp14::make_unique<Wt::WLineEdit>());
distribuidor->setToolTip("Buscar el producto por la marca, con un par de caracteres es suficiente");
distribuidor->keyWentUp().connect(this,&stock_add::update);
//Wt::WLineEdit::setToolTip()

table = this->addWidget(cpp14::make_unique<WTableView>());
// create model
Wt::WString server= "dbname=agroquimicos user=wt password=ninguna host=localhost";//TODO: take of this to user select
//Wt::WString buff ="SELECT * from lista_quim LIMIT 5";
model=std::make_shared<pg_model>(server,QueryBuffer,modo);// std::shared_ptr<pg_model> model;
// create table
table->setModel(model);
table->setEditTriggers(EditTrigger::SingleClicked);
std::shared_ptr<ComboDelegate> customdelegate =
    std::make_shared<ComboDelegate>(modo,this);
table->setItemDelegate(customdelegate);
//std::shared_ptr<Wt::WItemDelegate> default_delegate=std::make_shared<Wt::WItemDelegate>();
//table->setItemDelegate(default_delegate);
table->setSortingEnabled(false);
table->setColumnResizeEnabled(false);
table->setRowHeight(40);
table->setHeaderHeight(10);

const int WIDTH = 120;
for (int i = 0; i < table->model()->columnCount(); ++i)
    table->setColumnWidth(i, WIDTH);
table->setWidth((WIDTH + 7) * table->model()->columnCount() + 2);
tabla_popup->clicked().connect([=]{
    tabla_popup->disable();
    Wt::WMenuItem *item = popup->exec(tabla_popup);
if (!item->text().empty()) {
    list_tables.clear();
    list_tables.push_back(item->text());
    update();
    tabla_popup->enable();
  }});
}

void stock_add::update()
{
 Wt::WString  QueryBuffer ;
  for (std::vector<Wt::WString>::iterator it = list_tables.begin(); it != list_tables.end(); ){

   QueryBuffer   +=  "SELECT '"+*it+"' as tabla,nombre_com,distribuidor,stock,stock_min FROM ";
    QueryBuffer   +=*it;
    //setPlaceholderText(nada)
    if(!nombre_com->text().empty()){
      QueryBuffer   +=" WHERE nombre_com~'"+nombre_com->text()+"'";
      if(!distribuidor->text().empty())
        QueryBuffer   +=" AND distribuidor~'"+distribuidor->text()+"'";
      }else
      if(!distribuidor->text().empty())
         QueryBuffer   +=" WHERE distribuidor~'"+distribuidor->text()+"'";
      if((++it)!= list_tables.end())QueryBuffer   +=  "  UNION ";
}
  QueryBuffer+=" LIMIT 10";
  Wt::WString server= "dbname=agroquimicos user=wt password=ninguna host=localhost";//TODO: take of this to user select
  model->update(server,QueryBuffer);
  table->setModel(model);
}
void stock_add::head_item(Wt::WString  &query){
  //query+=" UNION ("+QueryBuffer+")";
  std::cout<<query.toUTF8()<<std::endl;
   Wt::WString server= "dbname=agroquimicos user=wt password=ninguna host=localhost";
  model->update(server,query);
  table->setModel(model);
  }



