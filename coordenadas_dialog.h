#ifndef COORDENADAS_DIALOG_H
#define COORDENADAS_DIALOG_H
#include <Wt/WDialog.h>
#include <Wt/WDoubleSpinBox.h>
#include <Wt/WText.h>
#include <Wt/WDoubleSpinBox.h>
#include <Wt/WPushButton.h>
#include <Wt/WBreak.h>

class coordenadas_dialog
{
public:
  coordenadas_dialog(Wt::WObject *powner,Wt::WPushButton *parent_but);
  void setPlaceholderText(Wt::WString textin);
  Wt::WString placeholderText();
 Wt::WString resultText();
private:
  void dialogDone();
  Wt::WLineEdit *edit_;
  Wt::WString placeholder_text,coordinates_text;
Wt::WDialog *dialog;
Wt::WPushButton *my_parent_b;
};

#endif // COORDENADAS_DIALOG_H
