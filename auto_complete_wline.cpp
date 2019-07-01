#include "auto_complete_wline.h"

auto_complete_wline::auto_complete_wline(std::string server_, std::string table_, std::string column_)
  :server(server_),table(table_),column(column_)
{
 //le = this->addWidget(Wt::cpp14::make_unique<Wt::WLineEdit>());
 this->textInput().connect(this,&auto_complete_wline::update);
}
void auto_complete_wline::update(){
    std::string query="SELECT "+column+" FROM "+table+" WHERE "+column+"~'"+this->text().toUTF8()+"' LIMIT 5";
// std::cout<<query<<std::endl;
  pqxx::result r;
  try{
  pqxx::connection c(server);
  pqxx::work w(c);
  r=w.exec(query);
    w.commit();
  }
  catch(const std::exception &e)
  {
    this->setPlaceholderText(e.what());
  }
  if(!r.empty()){
   popup.reset(new Wt::WPopupMenu());
  for (pqxx::result::const_iterator i = r.begin(); i != r.end(); ++i)
  {
      popup->addItem( i[0].c_str());
    }
    Wt::WMenuItem *item = popup->exec(this);
  if(item){
       this->setText(item->text());
    }
    }
}
