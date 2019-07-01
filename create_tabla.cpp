#include "create_tabla.h"

create_tabla::create_tabla()
{
 this->addWidget(std::make_unique<Wt::WText>("<p>Nombre que desea darle a la tabla</p>"));
 // this->setContentAlignment(1);
auto tablename =this->addWidget(std::make_unique<Wt::WLineEdit>());
this->addWidget(std::make_unique<Wt::WText>("<p>Campos que desea que la tabla tenga, por nombre de campo, y tipo de campo, en general se usa \"text\", para la mayoria, y los campos de cantidad \"integer\", y los campos porcentuales o con comas \"real\", presione (+) para agregar otro</p>"));
auto container=this->addWidget(std::make_unique<Wt::WContainerWidget>());
container->setStyleClass("CSS-example");
//container->setContentAlignment(Wt::AlignmentFlag::Right);
auto create_show = container->addWidget(Wt::cpp14::make_unique<Wt::WTable>());
create_show->elementAt(0,0)->addWidget(std::make_unique<Wt::WText>("Nombre Columna"));
create_show->elementAt(1,0)->addWidget(std::make_unique<Wt::WText>("Tipo Dato"));
{
auto column_name=create_show->elementAt(0,1)->addWidget(std::make_unique<Wt::WLineEdit>());
column_name->setToolTip("Nombre que desea darle a la columna");
auto column_type=create_show->elementAt(1,1)->addWidget(std::make_unique<Wt::WPushButton>("text"));
column_type->setToolTip("Tipo de dato que desea alojar en la columna, en general se usa text, y para números int o real, y date para fecha");
popup=new Wt::WPopupMenu();
popup->addItem("text(texto generalmente)");
popup->addItem("int4(número entero)");
popup->addItem("agr_quim_cat(N° romanos I al X)");
popup->addItem("money(dinero)");
popup->addItem("real(numeros decimales)");
popup->addItem("date (fecha)");
popup->addItem("TIMESTAMP(fecha y hora)");
popup->addItem("bool(SI/NO)");
popup->addItem("oid(archivo multimedia)");
popup->addItem("json(Formato de intercambio)");
popup->addItem("xml(Formato de intercambio)");
column_type->clicked().connect([=]{
    column_type->disable();
    Wt::WMenuItem *item = popup->exec(column_type);
    if(item){
        std::string type_name(item->text().toUTF8());
         auto pos_n=type_name.find('(');
        column_type->setText(type_name.substr(0,pos_n));
      }
        column_type->enable();


  });

}

auto column_add=create_show->elementAt(1,2)->addWidget(std::make_unique<Wt::WPushButton>("+"));
create_show->columnAt(2)->setStyleClass("red");
create_show->setStyleClass("table table-bordered");
create_show->rowAt(1)->setStyleClass("info");
create_show->elementAt(0,0)->setStyleClass("green");
column_add->clicked().connect([=]{
    auto column=create_show->columnCount()-1;
    create_show->insertColumn(column);
    auto column_name=create_show->elementAt(0,column)->addWidget(std::make_unique<Wt::WLineEdit>());
    auto column_type=create_show->elementAt(1,column)->addWidget(std::make_unique<Wt::WPushButton>("text"));
    if(!(column%2))create_show->elementAt(0,column)->setStyleClass("green");
    //else create_show->elementAt(0,column)->setStyleClass("info");
    column_type->clicked().connect([=]{
       column_type->disable();
        Wt::WMenuItem *item = popup->exec(column_type);
        if(item){
            std::string type_name(item->text().toUTF8());
             auto pos_n=type_name.find('(');
            column_type->setText(type_name.substr(0,pos_n));
            column_type->enable();
          }
});});


auto crear_bt=this->addWidget(std::make_unique<Wt::WPushButton>("<<CREAR>>"));
crear_bt->clicked().connect(std::bind(&create_tabla::crear,this,create_show,tablename->text()));
}



void create_tabla::crear(Wt::WTable *table_format,const Wt::WString tablename)
{
  Wt::WString Query="CREATE TABLE "+tablename+" (";
  //for (int j=0;i<table_format->rowCount();++j)
  for (int i=1;i<table_format->columnCount()-1;++i){
    //  std::cout<<"row0"<<table_format->elementAt(0,i)->children().size()<<"row1"<<table_format->elementAt(1,i)->children().size()<<std::endl;
      if(i>1)Query+= ",";
      Query+= dynamic_cast<Wt::WLineEdit*>(table_format->elementAt(0,i)->children().at(0))->text()+" ";
      Query+= dynamic_cast<Wt::WPushButton*>(table_format->elementAt(1,i)->children().at(0))->text();

    }

  Query+=")";

std::cerr<<Query.toUTF8()<<std::endl;

  try{
    pqxx::connection c("dbname=agroquimicos user=wt password=ninguna host=localhost");
    pqxx::work w(c);
    w.exec("SELECT tablename    FROM pg_catalog.pg_tables WHERE false;");
    w.commit();
  }
  catch(const std::exception &e)
  {
    this->addWidget(std::make_unique<Wt::WText>(e.what()));
  }
}




