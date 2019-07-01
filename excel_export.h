#ifndef EXCEL_EXPORT_H
#define EXCEL_EXPORT_H
#include <Wt/WString.h>
#include <xlnt/xlnt.hpp>
#include <pqxx/pqxx>
#include <string>
#include <cstdio>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <iostream>
class excel_export : public Wt::WContainerWidget
{
public:
  excel_export(Wt::WString pq_query);
};

#endif // EXCEL_EXPORT_H
