#include <pqxx/pqxx>
#include <iostream>
#include <optional>
int main()
{
 std::optional<pqxx::result> r=[]()->std::optional<pqxx::result>
 {
  try{
        pqxx::connection c("dbname=agroquimicos user=wt password=ninguna host=localhost");
        pqxx::work w(c);
   return  w.exec(" SELECT * FROM lista_quim");
  //pqxx::result r= w.exec(" SELECT * FROM lista_quim WHERE false");
         w.commit();
     }
     catch(const std::exception &e)
     {
      std::cerr<<e.what();
     }
     return std::nullopt;
 }();
 // if(r.value()!=std::nullopt)
 std::cout<<r->size()<<std::endl;
 for (pqxx::result::const_iterator i = r->begin(); i != r->end(); ++i)
 std::cout<<(*i)[1].c_str()<<std::endl;
}
