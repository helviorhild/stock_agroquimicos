#include "table_add_data.h"

table_add_data::table_add_data(const WString DBtable_, const WString tfield_, const WString value_)
  :DBtable(DBtable_),tfield(tfield_),value(value_)
{
  columnas.clear();
  pqxx::result r;
  try{
     pqxx::connection c("dbname=agroquimicos user=wt password=ninguna host=localhost");
     pqxx::work w(c);
      r = w.exec(" SELECT * FROM "+w.quote_name(DBtable_.toUTF8())+" WHERE false");
      w.commit();
  }
  catch(const std::exception &e)
  {
    this->addWidget(std::make_unique<Wt::WText>(e.what()));
  }
     // container1->addWidget(std::make_unique<Wt::WText>("What table, please ? "));
      theader  = this->addWidget(Wt::cpp14::make_unique<Wt::WTable>());
      theader->setHeaderCount(1);
      //theader->setScrollVisibilityEnabled(1);
      theader->setWidth(Wt::WLength("100%"));
      for (unsigned int colnum=0; colnum < r.columns(); ++colnum){
           columnas.push_back(r.column_name(colnum));
          if(r.column_name(colnum)!=tfield){
         theader->elementAt(0,colnum)->addWidget( Wt::cpp14::make_unique<Wt::WText>(r.column_name(colnum)));
     if(r.column_type(colnum)==700){
           Wt::WDoubleSpinBox *button  = theader->elementAt(1,colnum)->addWidget(Wt::cpp14::make_unique<Wt::WDoubleSpinBox> ());
         button->setObjectName(r.column_name(colnum));
         }else
        if(r.column_type(colnum)>=20&&r.column_type(colnum)<=23){
        Wt::WSpinBox *button  = theader->elementAt(1,colnum)->addWidget(Wt::cpp14::make_unique<Wt::WSpinBox> ());
      button->setObjectName(r.column_name(colnum));
                                       }else
        if(r.column_type(colnum)<1000)//not user define type
        {

        auto    button      =   theader->elementAt(1,colnum)->addWidget(std::make_unique<auto_complete_wline>("dbname=agroquimicos user=wt password=ninguna host=localhost",DBtable.toUTF8(),r.column_name(colnum)));
      //Wt::WLineEdit *button      =   theader->elementAt(1,colnum)->addWidget( Wt::cpp14::make_unique<Wt::WLineEdit>());
      button->setObjectName(r.column_name(colnum));
      }else{
            if(r.column_type(colnum)==1114){
                Wt::WDateEdit *button      =   theader->elementAt(1,colnum)->addWidget( Wt::cpp14::make_unique<Wt::WDateEdit>());
                button->setObjectName(r.column_name(colnum));
                button->setDate(Wt::WDate(2019,1,1));
              }else{
      Wt::WLineEdit *button      =   theader->elementAt(1,colnum)->addWidget( Wt::cpp14::make_unique<Wt::WLineEdit>());
      button->setObjectName(r.column_name(colnum));
                   }
        }
      }}
      auto bt_ejecute = theader->elementAt(2,1)->addWidget(Wt::cpp14::make_unique<Wt::WPushButton>("<<Crear>>"));
       bt_ejecute->clicked().connect(this,&table_add_data::table_data);
    tdata=this->addWidget(Wt::cpp14::make_unique<Wt::WTableView>());
    Wt::WString server= "dbname=agroquimicos user=wt password=ninguna host=localhost";//TODO: take of this to user select
  //  Wt::WString buff ="SELECT * from "+DBtable_+" WHERE ";
    Wt::WString buff="SELECT * FROM "+DBtable+" WHERE "+tfield+" ="+value;
    std::cout<<buff.toUTF8()<<'\n';
    db_model=std::make_shared<pg_model>(server,buff,-1);
    tdata->setModel(db_model);
}
void table_add_data::table_data(){
  Wt::WString buff="INSERT INTO "+DBtable+" (";
  for(std::vector<Wt::WString>::iterator it=columnas.begin();it!=columnas.end();)
    {
      buff+=*it;
      if(++it!=columnas.end()) buff+=",";
    }

  buff+=") VALUES (";
  bool first=true;
  for(std::vector<Wt::WString>::iterator it=columnas.begin();it!=columnas.end();++it)
    {
      if(first)  buff+="'";
      else   buff+=",'";
      if(*it!=tfield){
       buff+=dynamic_cast<Wt::WLineEdit *>(this->find(it->toUTF8()))->text();
        }else{
          buff+=value;
        }
      buff+="'";
      first=false;
          }
buff+=")";
Wt::WString server= "dbname=agroquimicos user=wt password=ninguna host=localhost";//TODO: take of this to user select
try{
pqxx::connection c("dbname=agroquimicos user=wt password=ninguna host=localhost");
pqxx::work w(c);
  w.exec(buff.toUTF8());
  w.commit();
}

catch(const std::exception &e)
{
  this->addWidget(std::make_unique<Wt::WText>(e.what()));
}

//std::cout << buff.toUTF8()<< std::endl;
 Wt::WString consulta="SELECT * FROM "+DBtable+" WHERE "+tfield+" ="+value;
//INSERT INTO lote(nombre,id_campo,cordenadas,obs) VALUES ('nada','1','{0.00,0.00,0.00,0.00}','nadie')

db_model->update(server,consulta);
tdata->setModel(db_model);

}
