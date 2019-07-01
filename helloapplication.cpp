#include "helloapplication.h"
/*TODO: UNIQUE
 * CREATE UNIQUE INDEX CONCURRENTLY lista_quim_uniq on lista_quim(nombre_com,distribuidor);
 * ALTER TABLE  lista_quim ADD CONSTRAINT lista_quim_id UNIQUE USING INDEX lista_quim_uniq;
 * */
HelloApplication::HelloApplication(const WEnvironment& env)
  : WApplication(env)
{
  setTitle("Agroquimicos");     // application title
  //useStyleSheet("mi.css");
 useStyleSheet("CSSexample.css");
 //
 //root()->addWidget(std::make_unique<auto_complete_wline>("dbname=agroquimicos user=wt password=ninguna host=localhost","plaga","nombre"));
   container1 = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
    container = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
    {
      auto button = container->addWidget(std::make_unique<Wt::WPushButton>("Formularios"));      // create a button
       button->clicked().connect([=]{
          auto tb_show_con= container1->addWidget(std::make_unique<formularios>());
        auto buttonc = tb_show_con->addWidget(std::make_unique<Wt::WPushButton>("X"));      // create a button
         buttonc->clicked().connect([=]{tb_show_con->clear();
             tb_show_con->removeFromParent();
           });});
    }




    auto button = container->addWidget(std::make_unique<Wt::WPushButton>("Control Stock"));      // create a button
 button->clicked().connect(std::bind(&HelloApplication::stock_manager,this,button));
       //this, &HelloApplication::stock_manager);
 auto button2 = container->addWidget(std::make_unique<Wt::WPushButton>("Navegar tabla"));      // create a button
 //button2->clicked().connect(this, &HelloApplication::select_table);
 button2->clicked().connect(std::bind(&HelloApplication::select_table,this,button2));
 auto button3 = container->addWidget(std::make_unique<Wt::WPushButton>("blob_data"));      // create a button
  button3->clicked().connect([=]{
     auto tb_show_con= container1->addWidget(std::make_unique<blob_data>());
   auto button = tb_show_con->addWidget(std::make_unique<Wt::WPushButton>("X"));      // create a button
    button->clicked().connect([=]{tb_show_con->clear();
        tb_show_con->removeFromParent();
      });});

  auto button4= container->addWidget(std::make_unique<Wt::WPushButton>("try_export_excel"));      // create a button
   button4->clicked().connect([=]{
      auto tb_show_con= container1->addWidget(std::make_unique<excel_export>("select * from new_bayer;"));
    auto button = tb_show_con->addWidget(std::make_unique<Wt::WPushButton>("X"));      // create a button
     button->clicked().connect([=]{tb_show_con->clear();
         tb_show_con->removeFromParent();
       });});

   auto button5= container->addWidget(std::make_unique<Wt::WPushButton>("try_export_pdf"));      // create a button
    button5->clicked().connect([=]{
  //     auto tb_show_con= container1->addWidget(std::make_unique<export_pdf>("select nombre_com,distribuidor from new_bayer LIMIT 5;"));
             //auto tb_show_con= container1->addWidget(std::make_unique<export_pdf>("select * from new_bayer LIMIT 20;"));
             auto tb_show_con= container1->addWidget(std::make_unique<export_pdf>("select * from new_bayer ORDER BY nombre_com ASC;"));

     auto button = tb_show_con->addWidget(std::make_unique<Wt::WPushButton>("X"));      // create a button
      button->clicked().connect([=]{tb_show_con->clear();
          tb_show_con->removeFromParent();
        });});

}



std::unique_ptr<WApplication> createApplication(const WEnvironment& env)
{
  /*
   * You could read information from the environment to decide whether
   * the user has permission to start a new application
   */
  return std::make_unique<HelloApplication>(env);
}

void HelloApplication::select_table(Wt::WPushButton *mywidget){
  try{
    pqxx::connection c("dbname=agroquimicos user=wt password=ninguna host=localhost");
    pqxx::work w(c);
    //pqxx::result r = w.exec(" SELECT tablename FROM pg_catalog.pg_tables WHERE tableowner='wt';"); //SELECT * FROM pg_catalog.pg_tables;");
    pqxx::result r = w.exec("SELECT tablename    FROM pg_catalog.pg_tables WHERE schemaname='public';");
    w.commit();
    Wt::WPopupMenu popup;
    popup.addSectionHeader("Seleccione una tabla");
    popup.addSeparator();
    for (pqxx::result::const_iterator i = r.begin(); i != r.end(); ++i)
                        popup.addItem((*i)[0].c_str());
    Wt::WMenuItem *item = popup.exec(mywidget);
     if (item) {
        select_field( item->text().toUTF8());
     }
  }
  catch(const std::exception &e)
  {
    container1->addWidget(std::make_unique<Wt::WText>(e.what()));
  }


}

void HelloApplication::select_field(const std::string &stable){
  auto tb_show_con= container1->addWidget(std::make_unique<table_show>(stable));
auto button = tb_show_con->addWidget(std::make_unique<Wt::WPushButton>("X"));      // create a button
 button->clicked().connect([=]{tb_show_con->clear();
     tb_show_con->removeFromParent();
     });


}

void HelloApplication::Delete_tbshow(table_show *tbshow){
    tbshow->clear();
    tbshow->removeFromParent();//Destroy object
     }
void HelloApplication::stock_manager(Wt::WPushButton *mywidget){
   Wt::WPopupMenu popup;
   popup.addItem("Ver Faltantes");
   popup.addSectionHeader("Manejo Stock");
   popup.addSeparator();
 // auto subMenu = std::make_unique<Wt::WPopupMenu>();
   popup.addItem("Agregar por barras");
 popup.addItem("Agregar Stock");
  popup.addItem("Producto nuevo");
 //  popup.addMenu("Stock",std::move(subMenu));
    popup.addItem("Cambiar Stock mínimo");
    popup.addSectionHeader("Tablas");
    popup.addSeparator();
    popup.addItem("Crear Tabla");


    Wt::WMenuItem *item = popup.exec(mywidget);
    if (item) {
      if(item->text()=="Ver Faltantes")stock_view();
        else
          if(item->text()=="Agregar por barras"){
              stock_add_barras();
            }

      else
            if(item->text()=="Agregar Stock"){
                stock_add_call();
              }

        else

        if(item->text()=="Producto nuevo"){
            new_product();
           // popup.removeItem(item);
          } else if(item->text()=="Cambiar Stock mínimo"){
          stock_minimo();

        }else if(item->text()=="Crear Tabla"){
            tabla_add();
          }




      // select_field( item->text().toUTF8());
    }

  /*
    auto   button = root()->addWidget(std::make_unique<Wt::WPushButton>("Editar Stock"));      // create a button
    button->clicked().connect(this, &HelloApplication::stock_edit);
  //  button->clicked().operator delete((void *)button);
    auto button2 = root()->addWidget(std::make_unique<Wt::WPushButton>("Ver Faltantes"));      // create a button
    button2->clicked().connect(this, &HelloApplication::stock_view);


    container->clear();
   auto button = root()->addWidget(std::make_unique<Wt::WPushButton>("Agregar Stock"));      // create a button
    button->clicked().connect(this, &HelloApplication::stock_add_call);
    auto button2 = root()->addWidget(std::make_unique<Wt::WPushButton>("Producto nuevo"));      // create a button
    button2->clicked().connect(this, &HelloApplication::new_product);
    */
}
void HelloApplication::stock_add_call(){

  auto ptr_stock_add= container1->addWidget(std::make_unique<stock_add>(3));
auto button=ptr_stock_add->addWidget(std::make_unique<Wt::WPushButton>("X"));
button->clicked().connect([=]{ptr_stock_add->clear();
  ptr_stock_add->removeFromParent();
  });
}
void HelloApplication::stock_add_barras(){
auto ptr_stock_add= container1->addWidget(std::make_unique<stock_add_bar>());
auto button=ptr_stock_add->addWidget(std::make_unique<Wt::WPushButton>("X"));
button->clicked().connect([=]{ptr_stock_add->clear();
  ptr_stock_add->removeFromParent();
  });
}



void HelloApplication::stock_minimo(){
auto ptr_stock_add= container1->addWidget(std::make_unique<stock_add>(4));
auto button=ptr_stock_add->addWidget(std::make_unique<Wt::WPushButton>("X"));
button->clicked().connect([=]{ptr_stock_add->clear();
 ptr_stock_add->removeFromParent();
  });

}

void HelloApplication::new_product(){
  auto ptr_stock_add= container1->addWidget(std::make_unique<product_add>());
  auto button=ptr_stock_add->addWidget(std::make_unique<Wt::WPushButton>("X"));
  button->clicked().connect([=]{ptr_stock_add->clear();
      ptr_stock_add->removeFromParent();
    });
    /*
     * like wizard:
     * first view: Select table or made same new table
     * second view:Define new product, from some select over the table
     * second b: made some new table, the view must have a botton for add new fields, and type. the column's nombre_com	stock stock_min, are fixed

*/
}

void HelloApplication::stock_view(){

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
      QueryBuffer   +=  "SELECT nombre_com,stock,stock_min, (stock_min-stock) as faltante FROM ";
        QueryBuffer   +=(*i)[0].c_str();
          if((++i)!= r.end())QueryBuffer   +=  " WHERE stock<stock_min UNION ";
    }
  }
  catch(const std::exception &e)
  {
    container1->addWidget(std::make_unique<Wt::WText>(e.what()));
  }
    QueryBuffer   +=  " WHERE stock<stock_min ";

    auto tb_show_con= container1->addWidget(std::make_unique<stock_list>(QueryBuffer));
  auto button = tb_show_con->addWidget(std::make_unique<Wt::WPushButton>("X"));      // create a button
   button->clicked().connect(std::bind(&HelloApplication::Delete_stock_list,this,tb_show_con));
}
void HelloApplication::Delete_stock_list(stock_list  *tbshow){
    tbshow->clear();
    tbshow->removeFromParent();
    //TODO: object still in memory
  }
void HelloApplication::tabla_add(){
  auto ptr_stock_add= container1->addWidget(std::make_unique<create_tabla>());
  auto button=ptr_stock_add->addWidget(std::make_unique<Wt::WPushButton>("X"));
  button->clicked().connect([=]{ptr_stock_add->clear();
    ptr_stock_add->removeFromParent();
    });
}


int main(int argc, char **argv)
{

  /*
   * Your main method may set up some shared resources, but should then
   * start the server application (FastCGI or httpd) that starts listening
   * for requests, and handles all of the application life cycles.
   *
   * The last argument to WRun specifies the function that will instantiate
   * new application objects. That function is executed when a new user surfs
   * to the Wt application, and after the library has negotiated browser
   * support. The function should return a newly instantiated application
   * object.
   */
/*
   Wt::WServer server(argv[0]);
  server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
  rest_service plaintextResource;
  server.addResource(&plaintextResource, "/bar_code");
  server.start();
*/
 // return WRun(argc, argv, &createApplication);
 return YourWRun(argc, argv, &createApplication);
}

//TODO:USAR Lippincott para las excepciones
