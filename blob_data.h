#ifndef BLOB_DATA_H
#define BLOB_DATA_H
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WPushButton.h>
#include <Wt/WImage.h>}
#include <pqxx/pqxx>

class blob_data : public Wt::WContainerWidget
{
public:
  blob_data();
private:
  void blob_read();
  void blob_write();
  Wt::WString pq_server;
};

#endif // BLOB_DATA_H
