#ifndef EXPORT_PDF_H
#define EXPORT_PDF_H
#include <podofo/podofo.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <pqxx/pqxx>
class export_pdf : public Wt::WContainerWidget
{
public:
  export_pdf(std::string query);
};

#endif // EXPORT_PDF_H
