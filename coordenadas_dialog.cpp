#include "coordenadas_dialog.h"

coordenadas_dialog::coordenadas_dialog(Wt::WObject *powner,Wt::WPushButton *parent_but)
{
  my_parent_b=parent_but;
  auto dialog = powner->addChild(Wt::cpp14::make_unique<Wt::WDialog>("Ingrese dos coordenadas (4valóres), o más"));
  dialog->setToolTip("Use dos coordenadas para las esquinas, a partir de la tercer coordenada, el sistema lo toma como una poligonal");
  dialog->contents()->addWidget(std::make_unique<Wt::WText>("latitud"));
   dialog->contents()->addWidget(std::make_unique<Wt::WDoubleSpinBox>());
   dialog->contents()->addWidget(std::make_unique<Wt::WText>("loguitud"));
   dialog->contents()->addWidget(std::make_unique<Wt::WDoubleSpinBox>());
  dialog->contents()->addWidget(std::make_unique<Wt::WBreak>());
  dialog->contents()->addWidget(std::make_unique<Wt::WText>("latitud"));
   dialog->contents()->addWidget(std::make_unique<Wt::WDoubleSpinBox>());
   dialog->contents()->addWidget(std::make_unique<Wt::WText>("loguitud"));
   dialog->contents()->addWidget(std::make_unique<Wt::WDoubleSpinBox>());
  dialog->contents()->addWidget(std::make_unique<Wt::WBreak>());
  Wt::WPushButton *mas = dialog->contents()->addWidget(std::make_unique<Wt::WPushButton>("+"));
Wt::WPushButton *ok = dialog->contents()->addWidget(std::make_unique<Wt::WPushButton>("Ok"));
dialog->contents()->addWidget(std::make_unique<Wt::WBreak>());
  mas->clicked().connect([=]{
      dialog->contents()->addWidget(std::make_unique<Wt::WText>("latitud"));
      dialog->contents()->addWidget(std::make_unique<Wt::WDoubleSpinBox>());
      dialog->contents()->addWidget(std::make_unique<Wt::WText>("loguitud"));
      dialog->contents()->addWidget(std::make_unique<Wt::WDoubleSpinBox>());
     dialog->contents()->addWidget(std::make_unique<Wt::WBreak>());
                         });

ok->clicked().connect([&] {
       coordinates_text+="{";
          int j=0;
          for(int i=1;i<dialog->contents()->count();i+=2,++j)
            {
              if(i!=1) coordinates_text+=",";
              coordinates_text+= dynamic_cast<Wt::WDoubleSpinBox*>(dialog->contents()->widget(i))->text();
              if(j==1){
                  ++i;
                  j=-1;
                }
              if(i==9){
                  i=12;
                  j=-1;
                }}
            dialog->accept();
             coordinates_text+="}";
             std::cout<<coordinates_text<<std::endl;
             my_parent_b->setText(coordinates_text);

   });
dialog->show();
}
void coordenadas_dialog::dialogDone()
{
  std::cout<<"BUTPARTEXT"<<my_parent_b->text();
 my_parent_b->setText(coordinates_text);
dialog->removeFromParent();
}

void coordenadas_dialog::setPlaceholderText(Wt::WString textin){
  placeholder_text=textin;
}

Wt::WString coordenadas_dialog::placeholderText(){
  return placeholder_text;
  }
Wt::WString coordenadas_dialog::resultText(){
  return coordinates_text;
  }
