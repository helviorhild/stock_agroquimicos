#include "table_show.h"


table_show::table_show(const Wt::WString DBtable)//:container1(container)
{

     //std::vector<WString> columnas;
     columnas.clear();
     pqxx::result r;
     try{
        pqxx::connection c("dbname=agroquimicos user=wt password=ninguna host=localhost");
        pqxx::work w(c);
         r = w.exec(" SELECT * FROM "+w.quote_name(DBtable.toUTF8())+" WHERE false");
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
             theader->elementAt(0,colnum)->addWidget( Wt::cpp14::make_unique<Wt::WText>(r.column_name(colnum)));
        if(r.column_type(colnum)==700){
              Wt::WDoubleSpinBox *button  = theader->elementAt(1,colnum)->addWidget(Wt::cpp14::make_unique<Wt::WDoubleSpinBox> ());
            button->setObjectName(r.column_name(colnum));
            button->changed().connect(std::bind(&table_show::table_data,this,DBtable));
            ptrDspbox.push_back(button);
          }else
           if(r.column_type(colnum)>=20&&r.column_type(colnum)<=23){
           Wt::WSpinBox *button  = theader->elementAt(1,colnum)->addWidget(Wt::cpp14::make_unique<Wt::WSpinBox> ());
         button->setObjectName(r.column_name(colnum));
         button->changed().connect(std::bind(&table_show::table_data,this,DBtable));
         ptrspbox.push_back(button);
           }else
           if(r.column_type(colnum)<1000)//not user define type
           {
         Wt::WLineEdit *button      =   theader->elementAt(1,colnum)->addWidget( Wt::cpp14::make_unique<Wt::WLineEdit>());
         button->setObjectName(r.column_name(colnum));
         button->keyWentUp().connect(std::bind(&table_show::table_data,this,DBtable));
           ptrline.push_back(button);
}else{
         Wt::WLineEdit *button      =   theader->elementAt(1,colnum)->addWidget( Wt::cpp14::make_unique<Wt::WLineEdit>());
         button->setObjectName(r.column_name(colnum));
         button->keyWentUp().connect(std::bind(&table_show::table_data,this,DBtable));
           ptrline_esp.push_back(button);
           }
         }
        {
            theader->elementAt(0,r.columns())->addWidget( Wt::cpp14::make_unique<Wt::WText>("Limite: 0->todo"));
           spin_limit = theader->elementAt(1,r.columns())->addWidget(Wt::cpp14::make_unique<Wt::WSpinBox> ());
           //spin_limit->setObjectName(r.column_name(colnum));
          spin_limit->changed().connect(std::bind(&table_show::table_data,this,DBtable));
          spin_limit->setValue(5);
        }
      tdata=this->addWidget(Wt::cpp14::make_unique<Wt::WTableView>());
       Wt::WString server= "dbname=agroquimicos user=wt password=ninguna host=localhost";//TODO: take of this to user select
       Wt::WString buff ="SELECT * from "+DBtable+" LIMIT 5";
       db_model=std::make_shared<pg_model>(server,buff,-1);

       tdata->setModel(db_model);
      //tdata->setModel(Wt::cpp14::make_unique<pg_model>(server,buff));


//combo setup
       auto slModel = std::make_shared<Wt::WStringListModel>();
       slModel->setStringList(columnas);
       auto scroll_left=this->addWidget(cpp14::make_unique<Wt::WPushButton>("<<"));
        auto combo = this->addWidget(cpp14::make_unique<Wt::WSelectionBox>());
        auto scroll_rigth=this->addWidget(cpp14::make_unique<Wt::WPushButton>(">>"));
       combo->setModel(slModel);
       combo->setVerticalSize(2);
       combo->setSelectionMode(Wt::SelectionMode::Single);
    combo->clicked().connect(
          [=] {
        if(combo->currentIndex()!=-1){
        auto it=columnas.begin();
        while((*it!=combo->currentText())&&(it!=columnas.end()))++it;
        if(it!=columnas.end()){
        std::cout<<"Seleccionado"<<it->toUTF8()<<" "<<combo->currentText()<<std::endl;
         columnas.erase(it);
         table_data(DBtable);
          }
          }
          });

    scroll_left->clicked().connect([=] {
          if(combo->currentIndex()-2>0)combo->setCurrentIndex(combo->currentIndex()-2);
          else combo->setCurrentIndex(0);
        //  combo->clearSelection();
} );
    scroll_rigth->clicked().connect([=] {
          if(combo->currentIndex()+2<combo->count())combo->setCurrentIndex(combo->currentIndex()+2);
          else combo->setCurrentIndex(combo->count()-1);
      } );

      }

  void table_show::table_data(const Wt::WString DBtable)
  {
      Wt::WString buff="SELECT ";
      for(std::vector<WString>::iterator it=columnas.begin();it!=columnas.end();)
        {
          buff+=*it;
          if(++it!=columnas.end()) buff+=",";
        }
      bool first=true;
      buff+=" FROM "+DBtable;
          for (std::vector<Wt::WLineEdit *>::iterator it=ptrline.begin();it!=ptrline.end();++it){
              if(!(*it)->text().empty()){
                  if(!first)    buff+=" AND ";
                  else  buff+=" WHERE ";
                buff+="\""+(*it)->objectName()+"\"";
                 buff+=" ~ ";
                  buff+="'"+(*it)->text()+"'";
               first=false;
              }
          }
          for (std::vector<Wt::WLineEdit *>::iterator it=ptrline_esp.begin();it!=ptrline_esp.end();++it){
              if(!(*it)->text().empty()){
                  if(!first)                buff+=" AND ";
                  else  buff+=" WHERE ";
                buff+="\""+(*it)->objectName()+"\"";
                 buff+=" = ";
                  buff+="'"+(*it)->text()+"'";
               first=false;
              }
          }

          for (std::vector<Wt::WSpinBox *>::iterator it=ptrspbox.begin();it!=ptrspbox.end();++it){
              if(((*it)->text()!="0")&&(!(*it)->text().empty())){
                  if(!first)                buff+=" AND ";
                 else  buff+=" WHERE ";
                buff+="\""+(*it)->objectName()+"\"";
                 buff+=" = ";
                  buff+=(*it)->text();
               first=false;
              }
          }
          for (std::vector<Wt::WDoubleSpinBox *>::iterator it=ptrDspbox.begin();it!=ptrDspbox.end();++it){
              if(((*it)->value()!=0)&&(!(*it)->text().empty())){//TODO: compare .value!=0
                  if(!first)                buff+=" AND ";
                 else  buff+=" WHERE ";
                buff+="\""+(*it)->objectName()+"\"";
                 buff+=" = ";
                  buff+=(*it)->text();
               first=false;
              }
          }
          if(spin_limit->value())  buff+= " LIMIT "+spin_limit->text();
Wt::WString server= "dbname=agroquimicos user=wt password=ninguna host=localhost";//TODO: take of this to user select
std::cout << buff.toUTF8()<< std::endl;
db_model->update(server,buff);
tdata->setModel(db_model);
  }
/*
table_show::~table_show(){
    std::cout<<"table_show.cpp Destructor called, form someone\n";
  }
*/
