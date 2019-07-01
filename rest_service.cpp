#include "rest_service.h"

int YourWRun(int argc, char *argv[], Wt::ApplicationCreator createApplication)
{
  try {
     rest_service plaintextResource;
    // use argv[0] as the application name to match a suitable entry
    // in the Wt configuration file, and use the default configuration
    // file (which defaults to /etc/wt/wt_config.xml unless the environment
    // variable WT_CONFIG_XML is set)
    Wt::WServer server(argv[0],"");

    // WTHTTP_CONFIGURATION is e.g. "/etc/wt/wthttpd"
    server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
     server.addResource(&plaintextResource, "/bar_code/${tag}");
    // add a single entry point, at the default location (as determined
    // by the server configuration's deploy-path)
    server.addEntryPoint(Wt::EntryPointType::Application, createApplication);
   // server.addEntryPoint(Wt::Application, createSecondApplication,"/second");
    if (server.start()) {
      int sig =  server.waitForShutdown();
     // int sig = Wt::WServer::waitForShutdown(argv[0]);

      std::cerr << "Shutdown (signal = " << sig << ")" << std::endl;
      server.stop();

      /*
      if (sig == SIGHUP)
        WServer::restart(argc, argv, environ);
      */
      }
  } catch (Wt::WServer::Exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  } catch (std::exception& e) {
    std::cerr << "exception: " << e.what() << "\n";
    return 1;
  }
}
void decode(cv::Mat &im, std::vector<decodedObject>&decodedObjects){

  // Create zbar scanner
  zbar::ImageScanner scanner;

  // Configure scanner
  scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);

  // Convert image to grayscale
  cv::Mat imGray,imBin;
  cv::cvtColor(im, imGray,cv::COLOR_BGR2GRAY);
  int n=0;
 //zbar::Image *image;

  // Wrap image data in a zbar image
  auto image=std::make_unique<zbar::Image>(im.cols, im.rows, "Y800", (uchar *)imGray.data, im.cols * im.rows);
  // Scan the image for barcodes and QRCodes
  n = scanner.scan(*image);
  if(n==0){
    cv::adaptiveThreshold(imGray,imBin,255,cv::ADAPTIVE_THRESH_MEAN_C , cv::THRESH_BINARY,167,-3);
    image=std::make_unique<zbar::Image>(im.cols, im.rows, "Y800", (uchar *)imBin.data, im.cols * im.rows);
      // Scan the image for barcodes and QRCodes
      n = scanner.scan(*image);
   //   cv::imshow("Results", imBin);     cv::waitKey(0);
    }
 if(n==0){
      cv::threshold(imGray, imBin, 170, 128, cv::THRESH_BINARY);
     image=std::make_unique<zbar::Image>(im.cols, im.rows, "Y800", (uchar *)imBin.data, im.cols * im.rows);
       // Scan the image for barcodes and QRCodes
       n = scanner.scan(*image);
   }
std::cout<<"Encontre: "<<n<<" Codigos de barra"<<std::endl;
  // Print results
  for( zbar::Image::SymbolIterator symbol = image->symbol_begin(); symbol != image->symbol_end(); ++symbol)
  {
    decodedObject obj;

    obj.type = symbol->get_type_name();
    obj.data = symbol->get_data();

    // Print type and data
    std::cout << "Type : " << obj.type << std::endl;
    std::cout << "Data : " << obj.data <<std::endl <<std::endl;

    // Obtain location
    for(int i = 0; i< symbol->get_location_size(); i++)
    {
      obj.location.push_back(cv::Point(symbol->get_location_x(i),symbol->get_location_y(i)));
    }

    decodedObjects.push_back(obj);
  }
 //display(imBin, decodedObjects);
}
// Display barcode and QR code location
void display(cv::Mat &im, std::vector<decodedObject>&decodedObjects)
{
  // Loop over all decoded objects
  for(int i = 0; i < decodedObjects.size(); i++)
  {
    std::vector<cv::Point> points = decodedObjects[i].location;
    std::vector<cv::Point> hull;

    // If the points do not form a quad, find convex hull
    if(points.size() > 4)
      cv::convexHull(points, hull);
    else
      hull = points;

    // Number of points in the convex hull
    int n = hull.size();

    for(int j = 0; j < n; j++)
    {
      cv::line(im, hull[j], hull[ (j+1) % n], cv::Scalar(255,0,0), 3);
    }

  }

  // Display results
  cv::imshow("Results", im);
 // cv::waitKey(0);
}
