#include "export_pdf.h"
using namespace PoDoFo;

// typedef PdfPage *(* CreatePageCallback) (PdfRect &rClipRect, void *pCustom)
// void SetAutoPageBreak (bool bPageBreak, CreatePageCallback callback, void *pCustomData=NULL)
struct mypointer
{
  PdfDocument *document;
  PdfPainter *painter;
  };


  PdfPage* callback(PdfRect &rClipRect,void  *document)//tabla
{
  PdfPage* page=(( mypointer*)document)->document->InsertPage((( mypointer*)document)->document->GetPage(0)->GetPageSize(),(( mypointer*)document)->document->GetPageCount());
(( mypointer*)document)->painter->SetPage( page );
  return page;
}

export_pdf::export_pdf(std::string query)
{
  mypointer point_call;
  pqxx::result r;
  try{
    pqxx::connection c("dbname=agroquimicos user=wt password=ninguna host=localhost");
  pqxx::work w(c);
 r = w.exec(query);
  w.commit();
  }
  catch(const std::exception &e)
  {
    this->addWidget(std::make_unique<Wt::WText>(e.what()));
  }
  PdfStreamedDocument document( "/tmp/outputfile.pdf" );
  //document.SetLanguage();
  //const PdfEncoding* pEncoding = new PdfIdentityEncoding();
  PdfFont* pFont = document.CreateFont( "Arial");
  //PdfFont* pFont = document.CreateFont( "LiberationSerif",false,false,pEncoding);
  //document.CreateFont()
  //pFont->SetEncoding(PdfIso88592Encoding);
 //document.SetDisplayDocTitle();
  //document.GetInfo()
  PdfSimpleTableModel model(r.begin().size(),r.size());
  model.SetFont(pFont);
 // model.SetForegroundColor(PdfColor(0.3,1,0.8));//verde lindo
   model.SetForegroundColor(PdfColor(0,0,0));
  PdfTable table(r.begin().size(),r.size());
   table.SetModel(&model);
   PdfPage* pPage = document.CreatePage( PdfPage::CreateStandardPageSize( ePdfPageSize_A4 ) );
    table.SetTableHeight(pPage->GetPageSize().GetHeight()-70);
    table.SetTableWidth(pPage->GetPageSize().GetWidth()-20);
    table.SetRowHeight(20);
   PdfPainter painter;
  painter.SetPage( pPage );
  point_call.document=&document;
  point_call.painter=&painter;
  table.SetAutoPageBreak(true,callback,(void *)&point_call);

  for (pqxx::row::size_type row_num=0;row_num<r.size();++row_num){
 for(pqxx::row::size_type colum_num=0;colum_num<r[row_num].size();++colum_num){
    PdfString pString(reinterpret_cast<const pdf_utf8*>(r[row_num][colum_num].c_str()));
   model.SetText(colum_num,row_num,pString);
//model.SetText(colum_num,row_num,r[row_num][colum_num].c_str());//me imprime mal la tilde
   }}
painter.SetFont( pFont );
painter.DrawText( 5, pPage->GetPageSize().GetHeight() - 10, "Hello World!" );
table.Draw(10,pPage->GetPageSize().GetHeight() -35,&painter);
painter.FinishPage();
document.Close();
}
