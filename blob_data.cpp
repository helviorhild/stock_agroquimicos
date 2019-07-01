#include "blob_data.h"
//create temp table mibloba(id int8,data bytea);
//create  table mibloba2(id int8,data pg_largeobject);
blob_data::blob_data()
{
  pq_server="dbname=agroquimicos user=wt password=ninguna host=localhost";
  auto button = this->addWidget(std::make_unique<Wt::WPushButton>("Crear blob"));      // create a button
   button->clicked().connect(this, &blob_data::blob_write);
    auto button2 = this->addWidget(std::make_unique<Wt::WPushButton>("leer Blob"));      // create a button
    button2->clicked().connect(this, &blob_data::blob_read);
}
void blob_data::blob_read(){
  try
  {
    pqxx::connection c(pq_server.toUTF8());

    pqxx::transaction pqt(c);

    pqxx::result r = pqt.exec("SELECT name,raster FROM image");
    if(r.size()){
       for (pqxx::result::const_iterator i = r.begin(); i != r.end();++i ){
        pqxx::largeobjectaccess pq_lo_acc(pqt,(*i)[1].as<int>());
        std::string temp="/tmp/imagen";
       temp+= (*i)[1].c_str();
          temp+= ".png";
        pq_lo_acc.to_file(temp);
      auto image = this->addWidget(std::make_unique<Wt::WImage>(temp));
image->setAlternateText((*i)[0].c_str());
        }
      }

 //pqxx::result::begin()::num()
    pqt.commit();
  }
 catch(const std::exception &e)
  {
    this->addWidget(std::make_unique<Wt::WText>(e.what()));
  }


}
void blob_data::blob_write(){
  try
  {
    pqxx::connection c(pq_server.toUTF8());
   c.prepare( "bloba_ins", " INSERT INTO image (name, raster)  VALUES ($1, $2)" );
    pqxx::transaction pqt(c);
   //pqxx::largeobject pq_lo(pqt,"/home/helvio/Descargas/rdamiento_lineal_doble.jpg");
    pqxx::largeobject pq_lo(pqt,"/usr/share/icons/Adwaita/48x48/categories/applications-multimedia.png");
  pqt.prepared( "bloba_ins" )( "rodamiento2" )(pq_lo.id()).exec();
   //pqt.prepared( "bloba_ins" )( 2 )(pq_lo.id()).exec();
    pqt.commit();
  }
 catch(const std::exception &e)
  {
    this->addWidget(std::make_unique<Wt::WText>(e.what()));
  }
}
