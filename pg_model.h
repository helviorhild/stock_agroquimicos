#ifndef PG_MODEL_H
#define PG_MODEL_H
#include <Wt/WAbstractTableModel.h>
#include <Wt/WStandardItemModel.h>
#include <Wt/WModelIndex.h>
#include <Wt/WString.h>
#include <Wt/WItemDelegate.h>
#include <string>
#include <pqxx/pqxx>
#include <any>

using namespace Wt;
//pq_server->"dbname=agroquimicos user=wt password=ninguna host=localhost"
//pq_query->" SELECT * FROM "+w.quote_name(stable.c_str())+" LIMIT 0"
class pg_model : public Wt::WAbstractTableModel
{
public:
  pg_model(Wt::WString &pq_server,Wt::WString &pq_query,int modo)//-1 en general modo 3 para stock y 4 para stock_min
    :modo_(modo)
  {
    try{
      pqxx::connection c(pq_server.toUTF8());
      pqxx::work w(c);
      r = w.exec(pq_query.toUTF8());
      w.commit();
      rows_=r.size();
      columns_=r.columns();
      pg_error="";
    }
    catch(const std::exception &e)
    {

      std::string temp=e.what();
      auto pos_n=temp.find('\n');
      if(pos_n!=std::string::npos){
     pg_error_h= temp.substr(0, pos_n);
     pg_error=temp.substr(pos_n+1,temp.length());
        }
          rows_=1;
      columns_=1;
      std::cerr<<e.what()<<std::endl;
    }
    }
virtual WFlags< ItemFlag> flags(const WModelIndex &  	index	)const
{

if(index.column()==modo_)return Wt::ItemFlag::Editable;
else return  Wt::ItemFlag::Selectable;
}

  virtual int rowCount(const WModelIndex& parent = WModelIndex()) const
  {
    if (!parent.isValid())
      return rows_;
    else
      return 0;
  }

  virtual int columnCount(const WModelIndex& parent = WModelIndex()) const
  {
    if (!parent.isValid())
      return columns_;
    else
      return 0;
  }

  virtual Wt::cpp17::any data(const WModelIndex& index, ItemDataRole role = ItemDataRole::ItemDataRole::Display ) const
  {

    if(role== ItemDataRole::ItemDataRole::Display){
      //if (index.column() == 0)
        if(pg_error.empty())   return r[index.row()][index.column()].c_str();
        else return pg_error;
    }
    else return Wt::cpp17::any();

  }

  virtual Wt::cpp17::any headerData(int section,
                                Orientation orientation = Orientation::Horizontal,
                                ItemDataRole role = ItemDataRole::ItemDataRole::Display) const
  {
    if (orientation == Orientation::Horizontal) {
     if(role== ItemDataRole::ItemDataRole::Display){
          if(pg_error.empty()) return  r.column_name(section);
          else  return pg_error_h;

     }else  return Wt::cpp17::any();

    } else
      return Wt::cpp17::any();
  }
void update(Wt::WString pq_server,Wt::WString pq_query);
private:
  int rows_, columns_,modo_;
  pqxx::result r;
Wt::WString pg_error,pg_error_h;
};

#endif // PG_MODEL_H
