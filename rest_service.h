#ifndef REST_SERVICE_H
#define REST_SERVICE_H
#include <Wt/Http/Request.h>
#include <Wt/Http/Response.h>
#include <Wt/WResource.h>
#include <Wt/WApplication.h>
#include <Wt/WServer.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core/mat.hpp>
#include <opencv4/opencv2/gapi/own/types.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>
#include <zbar.h>


int YourWRun(int argc, char *argv[], Wt::ApplicationCreator createApplication);
typedef struct
{
  std::string type;
  std::string data;
  std::vector <cv::Point> location;
} decodedObject;
void decode(cv::Mat &im, std::vector<decodedObject>&decodedObjects);
void display(cv::Mat &im, std::vector<decodedObject>&decodedObjects);
class rest_service :  public Wt::WResource
{
  virtual void handleRequest(const Wt::Http::Request &request, Wt::Http::Response &response) {

    response.setMimeType("text/html");
    response.addHeader("Server", "Wt");

    //request.urlParam()//->/tags/Wt, urlParam("tag") will return "Wt".
    auto codigo_val=request.getParameterValues("codigo");
   auto cant_val= request.getParameterValues("cantidad");
   if((cant_val.size()==1)&&(codigo_val.size()==1))
     {
       std::cout<<"codigo:"<<*(codigo_val.begin())<<std::endl;
       std::cout<<"cantidad:"<<*(cant_val.begin())<<std::endl;
     }
     /*for(auto it=codigo_val.begin();it!=codigo_val.end();++it)
   std::cout<<"codigo:"<<*it<<std::endl;//'?param=value1&param=value2'.
   for(auto it=cant_val.begin();it!=cant_val.end();++it)
   std::cout<<"cantidad:"<<*it<<std::endl;//'?param=value1&param=value2'.
   */
   if(request.urlParam("tag")=="img"){
       if(request.tooLarge()>0)
          std::cout<<"toolarge"<<std::endl;
       else{
       std::cout<<request.urlParam("tag")<<std::endl;
     auto image_file= request.getUploadedFile( "image");
     std::cout<<"image contentType:"<<image_file->contentType()<<std::endl;
     {
     // Read image
     //Mat im = imread("zbar-test.jpg");
     cv::Mat im = cv::imread(image_file->spoolFileName());
     // Variable for decoded objects
     std::vector<decodedObject> decodedObjects;

     // Find and decode barcodes and QR codes
     decode(im, decodedObjects);
if(decodedObjects.size()==1){
response.out() <<"tipo codigo barras:"<<(decodedObjects.begin())->type;
response.out() <<"<BR> n&uacute;mero :"<<(decodedObjects.begin())->data<<"<BR>";
  }else
  response.out() <<"<H1>No he podido  leer el c&oacute;digo de barras</H1>";

     // Display location
    // display(im, decodedObjects);
     }

         }
    }

    response.out() <<"  <form action=\"/bar_code/img\" method=\"post\" enctype=\"multipart/form-data\">";
      response.out() <<"  <input type=\"file\" name=\"image\" accept=\"image/*\" capture=\"environment\">";
      response.out() <<"  <input type=\"submit\" value=\"Upload\">";
   response.out() <<" </form>";
//<input type="file" name="image" accept="image/*"
  }

};

#endif // REST_SERVICE_H
