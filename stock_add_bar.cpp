#include "stock_add_bar.h"

stock_add_bar::stock_add_bar()
{
  /*
  std::string Query;
  try{
  pqxx::connection c("dbname=agroquimicos user=wt password=ninguna host=localhost");
  pqxx::work w(c);
   w.exec(Query);
  w.commit();
  }

  catch(const std::exception &e){
 std::make_unique<Wt::WText>(e.what());
  }*/

  Wt::WFileUpload *upload =addWidget(std::make_unique<Wt::WFileUpload>());
  upload->setFileTextSize(40);
  // Provide a button
  Wt::WPushButton *uploadButton =
    addWidget(std::make_unique<Wt::WPushButton>("Send"));
  // Upload when the button is clicked.
  uploadButton->clicked().connect(upload, &Wt::WFileUpload::upload);
  uploadButton->clicked().connect(uploadButton, &Wt::WPushButton::disable);
  // Upload automatically when the user entered a file.
  upload->changed().connect(upload, &Wt::WFileUpload::upload);
  upload->changed().connect(uploadButton, &Wt::WPushButton::disable);
  // React to a succesfull upload.
  upload->uploaded().connect(this, &stock_add_bar::fileUploaded);
  // React to a fileupload problem.
  upload->fileTooLarge().connect(this, &stock_add_bar::fileTooLarge);
}
void stock_add_bar::fileUploaded(){
addWidget(std::make_unique<Wt::WText>("succesfull upload"));

}
void stock_add_bar::fileTooLarge(){
  addWidget(std::make_unique<Wt::WText>("fileupload problem"));
}
