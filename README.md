# stock_agroquimicos
Aplicacion desarrollada usando wt (witty) y C++, para control de stock y aplicaiones de agroquimicos, sin terminar usada solamente para comunicar al cliente, la manera de trabajo;<br>
Esta aplicación esta pensada para un campo grande, con sectores y lotes. Inclusive puede definir lotes irregulares por coordenadas gps<br>
libzbar and opencv is used to read barcodes,using  cell phone camera<br>
This app don't use wt dbo driver, because I want to define all data in database, and the app must adapt to.<br>
Things todo:<br>
*user level, because is some intranet app, I'll be using the database user management.<br>
*session maybe son class with user data, or built in wt user session<br>
*Export to pdf, will be used to send the work order.<br>
*setup file, for database connections.<br>

<H1>por consultas:   helviorhild (arroba)   gmail (punto) com</H1><br>
agroquimicos.sql is postgres database dump. For recreation database definition for app test<br>
