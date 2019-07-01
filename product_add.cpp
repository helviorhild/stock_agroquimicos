#include "product_add.h"

product_add::product_add()
{
 //popup=new Wt::WPopupMenu();
 std::vector<Wt::WString> columnas;
 pqxx::result r;
 try{
 pqxx::connection c("dbname=agroquimicos user=wt password=ninguna host=localhost");
 pqxx::work w(c);
  r = w.exec("\
   SELECT table_name FROM information_schema.columns WHERE table_schema = 'public' AND column_name='nombre_com'\
   INTERSECT  SELECT table_name FROM information_schema.columns WHERE table_schema = 'public' AND column_name='stock'\
   INTERSECT  SELECT table_name FROM information_schema.columns WHERE table_schema = 'public' AND column_name='stock_min'"  );
   w.commit();
 }

 catch(const std::exception &e)
 {

  this->addWidget(std::make_unique<Wt::WText>(e.what()));
 }


for (pqxx::result::const_iterator i = r.begin(); i != r.end(); ++i){
  //  popup->addItem((*i)[0].c_str());
     columnas.push_back((*i)[0].c_str());
  }
/*
  //TODO: Cambiar por un string list y sacar popup
  tabla_popup = this->addWidget(Wt::cpp14::make_unique<Wt::WPushButton>("Tabla"));
  tabla_popup->setToolTip("Pulse para seleccionar una tabla de origen");
  auto heigh_orig =tabla_popup->height();
tabla_popup->setHeight(120);
  tabla_popup->clicked().connect([=]{
      Wt::WMenuItem *item = popup->exec(tabla_popup);
  if (!item->text().empty()) {
      table_imput(item->text());
      tabla_popup->setHeight(heigh_orig);
    }});
*/
auto slModel = std::make_shared<Wt::WStringListModel>();
slModel->setStringList(columnas);
 auto selec_box = this->addWidget(Wt::cpp14::make_unique<Wt::WSelectionBox>());
selec_box->setModel(slModel);
selec_box->setVerticalSize(10);
selec_box->setSelectionMode(Wt::SelectionMode::Single);
selec_box->setToolTip("Seleccione la tabla donde quiere agregar el nuevo producto");
selec_box->clicked().connect(
      [=] {
    selec_box->hide();
    if(selec_box->currentIndex()!=-1){
       table_imput( selec_box->currentText());
      }
      });
theader=this->addWidget(Wt::cpp14::make_unique<Wt::WTable>());
}

void product_add::table_imput(Wt::WString DBtable){
   pqxx::result r;
  try {
  pqxx::connection c("dbname=agroquimicos user=wt password=ninguna host=localhost");
  pqxx::work w(c);
   r = w.exec(" SELECT * FROM "+w.quote_name(DBtable.toUTF8())+" WHERE false");
   w.commit();
  }
  catch(const std::exception &e)
  {
    this->addWidget(std::make_unique<Wt::WText>(e.what()));
  }

    for (unsigned int colnum=0; colnum < r.columns(); ++colnum){
        theader->elementAt(0,colnum)->addWidget( Wt::cpp14::make_unique<Wt::WText>(r.column_name(colnum)));
        if(r.column_type(colnum)==700){
              Wt::WDoubleSpinBox *button  = theader->elementAt(1,colnum)->addWidget(Wt::cpp14::make_unique<Wt::WDoubleSpinBox> ());
            button->setObjectName(r.column_name(colnum));
             ptrDspbox.push_back(button);
          }else
           if(r.column_type(colnum)>=20&&r.column_type(colnum)<=23){
           Wt::WSpinBox *button  = theader->elementAt(1,colnum)->addWidget(Wt::cpp14::make_unique<Wt::WSpinBox> ());
         button->setObjectName(r.column_name(colnum));
         ptrspbox.push_back(button);
           }else
           if(r.column_type(colnum)<1000)//not user define type
           {
         Wt::WLineEdit *button      =   theader->elementAt(1,colnum)->addWidget( Wt::cpp14::make_unique<Wt::WLineEdit>());
         button->setObjectName(r.column_name(colnum));
         ptrline.push_back(button);
}else{
         Wt::WLineEdit *button      =   theader->elementAt(1,colnum)->addWidget( Wt::cpp14::make_unique<Wt::WLineEdit>());
         button->setObjectName(r.column_name(colnum));
          ptrline_esp.push_back(button);
           }
         }
        {
            //theader->elementAt(0,r.columns())->addWidget( Wt::cpp14::make_unique<Wt::WPushButton>("Limite: 0->todo"));
          auto bt_ejecute = theader->elementAt(1,r.columns())->addWidget(Wt::cpp14::make_unique<Wt::WPushButton>("<<Crear>>"));

           bt_ejecute->clicked().connect(std::bind(&product_add::ejecute,this,DBtable));

        }

      }

void product_add::ejecute(Wt::WString &DBtable){
  Wt::WString buff="INSERT INTO "+DBtable+" (",buff_val,buff_query="SELECT * FROM "+DBtable+" WHERE ";
  bool first=true;

      for (std::vector<Wt::WLineEdit *>::iterator it=ptrline.begin();it!=ptrline.end();++it){
          if(!(*it)->text().empty()){
              if(first)       first=false;
              else{
                  buff_query+=" AND ";
                buff_val+=",";
                buff+=",";
                }
            buff+="\""+(*it)->objectName()+"\"";
            buff_val+="'"+(*it)->text()+"'";
            buff_query+="\""+(*it)->objectName()+"\"";
             buff_query+=" ~ ";
              buff_query+="'"+(*it)->text()+"'";
          }
      }
      for (std::vector<Wt::WLineEdit *>::iterator it=ptrline_esp.begin();it!=ptrline_esp.end();++it){
          if(!(*it)->text().empty()){
              if(first)first=false;
              else {
                    buff_query+=" AND ";
                  buff+=",";
                  buff_val+=",";
                  }
            buff+="\""+(*it)->objectName()+"\"";
            buff_val+="'"+(*it)->text()+"'";
            buff_query+="\""+(*it)->objectName()+"\"";
             buff_query+=" ~ ";
              buff_query+="'"+(*it)->text()+"'";
          }
      }

      for (std::vector<Wt::WSpinBox *>::iterator it=ptrspbox.begin();it!=ptrspbox.end();++it){
          if((*it)->value()!=0){
              if(first) first=false;
              else {
                    buff_query+=" AND ";
                  buff+=",";
                  buff_val+=",";
                  }
            buff+="\""+(*it)->objectName()+"\"";             
              buff_val+=(*it)->text();
              buff_query+="\""+(*it)->objectName()+"\"";
               buff_query+=" = ";
                buff_query+="'"+(*it)->text()+"'";

          }
      }
      for (std::vector<Wt::WDoubleSpinBox *>::iterator it=ptrDspbox.begin();it!=ptrDspbox.end();++it){
          if((*it)->value()!=0){
              if(first)   first=false;
             else {
                    buff_query+=" AND ";
                  buff+=",";
                  buff_val+=",";
                  }
            buff+="\""+(*it)->objectName()+"\"";
           buff_val+=(*it)->text();
           buff_query+="\""+(*it)->objectName()+"\"";
            buff_query+=" = ";
             buff_query+="'"+(*it)->text()+"'";
          }
      }
  buff+=") VALUES ("+buff_val+")";
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

theader->insertRow(0);
pqxx::result r;
try {
pqxx::connection c("dbname=agroquimicos user=wt password=ninguna host=localhost");
pqxx::work w(c);
 r = w.exec(buff_query.toUTF8());
 w.commit();
}
catch(const std::exception &e)
{
  this->addWidget(std::make_unique<Wt::WText>(e.what()));
}
for (unsigned int colnum=0; colnum < r.columns(); ++colnum){//TODO: usar css para colores de la tabla
    theader->elementAt(0,colnum)->addWidget( Wt::cpp14::make_unique<Wt::WText>(r[0][colnum].c_str()));
}
}
