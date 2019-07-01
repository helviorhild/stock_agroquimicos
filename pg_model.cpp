#include "pg_model.h"
/*
pg_model::pg_model()
{

}
*/
void pg_model::update(Wt::WString pq_server,Wt::WString pq_query){
     r.clear();
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
       std::cerr<<e.what()<<std::endl;//TODO: must return some table with error inside
     }
}
