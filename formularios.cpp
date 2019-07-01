#include "formularios.h"

//select column_name from information_schema.columns where table_name='contratista';

formularios::formularios()
{
 // popup=nullptr;
  table_chooce_form=this->addWidget(std::make_unique<Wt::WContainerWidget>());
  this->setStyleClass("CSS-example");
      try{
    pqxx::connection c("dbname=agroquimicos user=wt password=ninguna host=localhost");
    pqxx::work w(c);
   // pqxx::result r = w.exec("SELECT table_name,leyenda FROM gui_table_form  WHERE contexto='ordenes_trabajo' ORDER BY priority DESC");
     pqxx::result r = w.exec("SELECT table_name,leyenda FROM gui_table_form   ORDER BY   contexto DESC,priority ASC");
 w.commit();
 for (pqxx::result::const_iterator i = r.begin(); i != r.end(); ++i)
   {
     Wt::WPushButton *button=table_chooce_form->addWidget(std::make_unique<Wt::WPushButton>((*i)[0].c_str()));
     button->setToolTip((*i)[1].c_str());
     button->clicked().connect(std::bind(&formularios::show_form,this,std::string((*i)[0].c_str())));

   }

  }catch(const std::exception &e)
  {
    this->addWidget(std::make_unique<Wt::WText>(e.what()));
  }
  leyenda=this->addWidget(std::make_unique<Wt::WText>());
  theader  = this->addWidget(Wt::cpp14::make_unique<Wt::WTable>());
}


void formularios::show_form(std::string table_name){
  theader->clear();
  theader->enable();

 //this->addWidget(std::make_unique<Wt::WText>());
 //std::cout<<table_name<<" nombre tabla\n";
  leyenda->setText(table_name);
  leyenda->setStyleClass("name");
  leyenda->setMargin(50);
  pqxx::connection c("dbname=agroquimicos user=wt password=ninguna host=localhost");
  pqxx::result r;
  try{
c.prepare("col_relvan","SELECT col_relevan FROM gui_table_form WHERE table_name=$1");
pqxx::transaction pqt(c);
      r = pqt.exec("select column_name,data_type from information_schema.columns where table_name='"+table_name+"'");


   theader->setHeaderCount(1);
   for (pqxx::result::const_iterator i = r.begin(); i != r.end(); ++i)
     {
   if((*i)[0].as<std::string>()!="id"){
    theader->elementAt(0,i.rownumber())->addWidget(std::make_unique<Wt::WText>((*i)[0].c_str()));
   if(!(i.rownumber()%2))theader->elementAt(0,i.rownumber())->setStyleClass("green");
   if((*i)[0].as<std::string>().find("id_")==0){
      // std::cout<<"data type:"<<(*i)[1].as<std::string>()<<'\n';
       auto column_type=theader->elementAt(1,i.rownumber())->addWidget(std::make_unique<Wt::WPushButton>((*i)[0].c_str()));
      auto popup=new Wt::WPopupMenu();
       vec_popup.push_back(popup);
   pqxx::result  rcolum_rele=   pqt.prepared("col_relvan")((*i)[0].as<std::string>().substr(3)).exec();
  if(rcolum_rele.size()){
       pqxx::result  r2 = pqt.exec("select id,"+rcolum_rele[0][0].as<std::string>()+" FROM "+(*i)[0].as<std::string>().substr(3));
for (pqxx::result::const_iterator j = r2.begin(); j != r2.end(); ++j){
    std::string buffi;
    if((*i)[1].as<std::string>()=="ARRAY")buffi+='{';
   for (auto z=j.begin();z!=j.end();++z)buffi+=z->as<std::string>()+".";
popup->addItem(buffi);
  }
     }else this->addWidget(std::make_unique<Wt::WText>("No se encontró una definición para la tabla:"+(*i)[0].as<std::string>().substr(3)+" "));
    column_type->clicked().connect([=]{
    column_type->disable();
    Wt::WMenuItem *item = popup->exec(column_type);
    if(item){
         column_type->setText(item->text());
      }
        column_type->enable();
  });
     }
else {
if((*i)[1].as<std::string>()=="text"){

auto column_name=theader->elementAt(1,i.rownumber())->addWidget(std::make_unique<Wt::WLineEdit>());
std::string buff =(*i)[0].c_str();
buff +="'";
  column_name->setObjectName(buff);
column_name->setPlaceholderText((*i)[0].c_str());
  }
else
  if((*i)[1].as<std::string>()=="integer"){
    //  theader->elementAt(0,i.rownumber())->addWidget(std::make_unique<Wt::WText>((*i)[0].c_str()));
  auto column_name=theader->elementAt(1,i.rownumber())->addWidget(std::make_unique<Wt::WSpinBox>());
       column_name->setObjectName((*i)[0].c_str());
       column_name->setPlaceholderText((*i)[0].c_str());
    }else
    if((*i)[1].as<std::string>()=="real"){
    //    theader->elementAt(0,i.rownumber())->addWidget(std::make_unique<Wt::WText>((*i)[0].c_str()));
    auto column_name=theader->elementAt(1,i.rownumber())->addWidget(std::make_unique<Wt::WDoubleSpinBox>());
    column_name->setMaximum(40000.00);
    column_name->setObjectName((*i)[0].c_str());
    column_name->setPlaceholderText((*i)[0].c_str());
      }else
      if((*i)[1].as<std::string>()=="timestamp without time zone"){
      //    theader->elementAt(0,i.rownumber())->addWidget(std::make_unique<Wt::WText>((*i)[0].c_str()));
      auto column_name=theader->elementAt(1,i.rownumber())->addWidget(std::make_unique<Wt::WDateEdit>());
        column_name->setObjectName((*i)[0].c_str());
        column_name->setPlaceholderText((*i)[0].c_str());
        }else
        if((*i)[1].as<std::string>()=="ARRAY"){
        //    theader->elementAt(0,i.rownumber())->addWidget(std::make_unique<Wt::WText>((*i)[0].c_str()));
        auto column_name=theader->elementAt(1,i.rownumber())->addWidget(std::make_unique<Wt::WPushButton>((*i)[0].c_str()));
          column_name->setObjectName((*i)[0].c_str());
        column_name->clicked().connect([=]{
             // auto dialo_coor=new coordenadas_dialog(this->parent()->parent()->parent(),column_name);
           dialo_coor=std::make_unique<coordenadas_dialog>(this->parent()->parent()->parent(),column_name);
          });

          }
}
}
     }
  pqt.commit();
  }

   catch(const std::exception &e)
  {
    this->addWidget(std::make_unique<Wt::WText>(e.what()));
  }
  auto spin_limit = theader->elementAt(2,2)->addWidget(Wt::cpp14::make_unique<Wt::WPushButton> ("<<CREAR>>"));
  spin_limit->setStyleClass("crear");
  spin_limit->clicked().connect(std::bind(&formularios::ejecutar,this,table_name));

}
//latitud -24.0404601 longuitud -61.3959389-->el colorado
 void formularios::ejecutar(std::string tabla)
 {
   std::string Query="INSERT INTO "+tabla+"(";
std::string Query2=" VALUES (";
   bool first=true;
   for (int i=0;i<theader->columnCount();++i){
       if(theader->elementAt(1,i)->children().size()>0){
           if(first) first=false;
            else {
                 Query+=",";
                 Query2+=",";
              }
          //std::cout<<dynamic_cast<Wt::WFormWidget*>( *theader->elementAt(1,i)->children().begin())->placeholderText().toUTF8()<<std::endl;
           if(dynamic_cast<Wt::WFormWidget*>( *theader->elementAt(1,i)->children().begin())->placeholderText().empty()){
            //   std::cout<<"need to filter button text information"<<std::endl;
               Query+= dynamic_cast<Wt::WText*>( *theader->elementAt(0,i)->children().begin())->text().toUTF8();
                if(Query.back()=='\'')
                  {
               Query2+='\'';
                Query2+=dynamic_cast<Wt::WPushButton*>(*theader->elementAt(1,i)->children().begin())->text().toUTF8();
                Query2+='\'';
                Query.pop_back();
                  }
                else{ std::string temp=dynamic_cast<Wt::WPushButton*>(*theader->elementAt(1,i)->children().begin())->text().toUTF8();
                    if(temp.back()=='}')Query2+="'"+dynamic_cast<Wt::WPushButton*>(*theader->elementAt(1,i)->children().begin())->text().toUTF8()+"'";
                      else{
                     auto pos_n=temp.find('.');
                    if(pos_n!=std::string::npos){
                        if(temp[0]=='{'){
                            temp[pos_n]='}';//if data is some array
                            ++pos_n;
                          }
                        Query2+="'"+temp.substr(0, pos_n)+"'";
                          }else Query2+= temp.substr(0, pos_n);
                      } }
             }else{
             Query+= dynamic_cast<Wt::WLineEdit*>( *theader->elementAt(1,i)->children().begin())->objectName();
             if(Query.back()=='\'')
               {
            Query2+='\'';
            Query2+=dynamic_cast<Wt::WLineEdit*>(*theader->elementAt(1,i)->children().begin())->text().toUTF8();
             Query2+='\'';
             Query.pop_back();
               }
             else {
            if(dynamic_cast<Wt::WText*>( *theader->elementAt(0,i)->children().begin())->text().toUTF8()=="fecha")
                             Query2+='\''+dynamic_cast<Wt::WLineEdit*>(*theader->elementAt(1,i)->children().begin())->text().toUTF8()+'\'';
                           else Query2+=dynamic_cast<Wt::WLineEdit*>(*theader->elementAt(1,i)->children().begin())->text().toUTF8();
               }
             }
         }
     }
 Query+=")"+ Query2+")";
  if(tabla=="ordenes_trab_app")Query+=" RETURNING id;";
   pqxx::connection c("dbname=agroquimicos user=wt password=ninguna host=localhost");


    pqxx::result r;
    try{
  pqxx::work w(c);
  r=w.exec(Query);
  w.commit();
  auto correct=theader->elementAt(2,1)->addWidget(std::make_unique<Wt::WText>("Creado correctamente"));
  correct->setStyleClass("green");
   }
   catch(const std::exception &e)
   {
     this->addWidget(std::make_unique<Wt::WText>(e.what()));
   }
   std::cout<<Query<<std::endl;
   theader->disable();
   if(tabla=="ordenes_trab_app"&&!r.empty()){
       auto ptr_stock_add= this->addWidget(std::make_unique<table_add_data>("aplicacion","id_orden",r[0][0].c_str()));
     auto button=ptr_stock_add->addWidget(std::make_unique<Wt::WPushButton>("X"));
     button->clicked().connect([=]{ptr_stock_add->clear();
       ptr_stock_add->removeFromParent();
       });
     }
    }

formularios::~formularios(){
  while(!vec_popup.empty()){
      delete(vec_popup.back());
      vec_popup.pop_back();

    }
}
