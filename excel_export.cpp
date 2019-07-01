#include "excel_export.h"

excel_export::excel_export(Wt::WString pq_query)
{
  pqxx::result r;

  try{


    pqxx::connection c("dbname=agroquimicos user=wt password=ninguna host=localhost");
  pqxx::work w(c);
 r = w.exec(pq_query.toUTF8());
  w.commit();
  }
  catch(const std::exception &e)
  {
    this->addWidget(std::make_unique<Wt::WText>(e.what()));
  }
  xlnt::workbook wb;
      xlnt::worksheet ws = wb.active_sheet();
  for (int row_num=0;row_num<r.size();++row_num){
 for(int colum_num=0;colum_num<r[row_num].size();++colum_num){
     char cell[20];
     sprintf(cell,"%c%d",colum_num+65,row_num+1);
 //    std::cout<<cell<<"#"<<'\t';
      //std::string cell=(char)(colum_num+65)+

      ws.cell(cell).value(r[row_num][colum_num].c_str());
     // ws.cell("B2").value("string data");
      //ws.cell("C3").formula("=RAND()");
      //ws.merge_cells("C3:C4");
      //ws.freeze_panes("B2");
    }
    std::cout<<std::endl;
    }
      wb.save("/tmp/example.xlsx");


}
