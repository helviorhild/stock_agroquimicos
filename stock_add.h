
#ifndef STOCK_ADD_H
#define STOCK_ADD_H

#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
//#include <Wt/WContainerWidget.h>
#include <Wt/WDialog.h>
#include <Wt/WEnvironment.h>
#include <Wt/WLabel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WRegExpValidator.h>
#include <Wt/WText.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WMenuItem.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WPoint.h>
//#####################
#include <Wt/WStandardItem.h>
#include <Wt/WStandardItemModel.h>
#include <Wt/WStringListModel.h>
#include <Wt/WTableView.h>
#include <Wt/WItemDelegate.h>
//#include <Wt/WContainerWidget.h>
//#include <Wt/WComboBox.h>
#include <Wt/WAny.h>
#include <Wt/WSpinBox.h>
#include <Wt/WString.h>
#include <pg_model.h>
#include <iostream>
/*The selection for add stock must be nombre_com,distribuidor
 *  and prompt user for new stock for add or final
 */
//TODO: Must be same table for log transaction
class stock_add : public Wt::WContainerWidget
{
public:
    stock_add(int modo);
    void head_item(WString &query);
private:
  void update();
 // Wt::WString  QueryBuffer ;
 std::vector<Wt::WString> list_tables;
std::shared_ptr<pg_model> model;
 WTableView *table;
//Wt::WPushButton *tabla_popup;
Wt::WLineEdit *nombre_com,*distribuidor;
 Wt::WPopupMenu *popup;
};

#endif //EDIT_STOCK_MIN_H
#ifndef COMBODELEGATE_CLASS
#define COMBODELEGATE_CLASS
class ComboDelegate : public WItemDelegate {
public:
    ComboDelegate(int modo,stock_add *parent):modo_(modo),parent_(parent)

    {}

    virtual void setModelData(const cpp17::any &editState, WAbstractItemModel* model,
                      const WModelIndex &index) const override
    {

     WString Query="Update "+((WString)asString(model->data(index.row(),0)));
         if(modo_==3)Query+=" SET stock=stock+"+  (WString)asString(editState);  //(int)asNumber(editState);
         else Query+=" SET stock_min="+  (WString)asString(editState);  //(int)asNumber(editState);
         Query+=" WHERE nombre_com='"+((WString)asString(model->data(index.row(),1)));
         Query+="' AND distribuidor='"+((WString)asString(model->data(index.row(),2)))+"'";

         std::cout<<Query.toUTF8()<<std::endl;


         try{
         pqxx::connection c("dbname=agroquimicos user=wt password=ninguna host=localhost");
         pqxx::work w(c);
          w.exec(Query.toUTF8());
         w.commit();
         }

         catch(const std::exception &e){
        std::make_unique<Wt::WText>(e.what());
         }
       //  auto button = myContainer->addWidget(std::make_unique<Wt::WPushButton>("Constructor"));
      //int stringIdx = (int)asNumber(editState);
      //  model->setData(index, stringIdx, ItemDataRole::User);
     //   model->setData(index, items_->data(stringIdx, 0), ItemDataRole::Display);
         Query="SELECT '"+((WString)asString(model->data(index.row(),0)))+"' as tabla, nombre_com,distribuidor,stock,stock_min FROM ";
         Query+=((WString)asString(model->data(index.row(),0)));
         Query+=" WHERE nombre_com='"+((WString)asString(model->data(index.row(),1)));
         Query+="' AND distribuidor='"+((WString)asString(model->data(index.row(),2)))+"'";
    parent_->head_item(Query);
    }

    virtual cpp17::any editState(WWidget *editor, const WModelIndex& index) const override
    {
        WSpinBox* combo = dynamic_cast<WSpinBox*>
            (dynamic_cast<WContainerWidget*>(editor)->widget(0));
        return combo->value();
    }

    virtual void setEditState(WWidget *editor, const WModelIndex& index,
                  const cpp17::any& value) const override
    {
        WSpinBox* combo = dynamic_cast<WSpinBox*>
            (dynamic_cast<WContainerWidget*>(editor)->widget(0));
                combo->setValue((int)asNumber(value));
    }

protected:
    virtual std::unique_ptr<WWidget> createEditor(const WModelIndex &index,
                                      WFlags<ViewItemRenderFlag> flags) const override
    {
      //static int row_ant;
      auto container = cpp14::make_unique<WContainerWidget>();
     // if(index.row()!=row_ant){
        //auto combo =container->addWidget(cpp14::make_unique<WComboBox>());
        auto combo =container->addWidget(cpp14::make_unique<WSpinBox>());
        combo->setWidth(40);
        combo->setMaximum(999999);
        auto button = container->addWidget(std::make_unique<Wt::WPushButton>("->"));
       button->setWidth(1);
       // button->setHtmlTagName("enter_buttom");
        button->clicked().connect(std::bind(&ComboDelegate::doCloseEditor, this,container.get(), true));
	combo->enterPressed().connect(std::bind(&ComboDelegate::doCloseEditor,
						this, container.get(), true));
	combo->escapePressed().connect(std::bind(&ComboDelegate::doCloseEditor,
						 this, container.get(), false));
	//row_ant=index.row();
	//}
	return std::move(container);
    }

private:
int modo_;
stock_add *parent_;
    virtual void doCloseEditor(WWidget *editor, bool save) const
    {
        closeEditor().emit(editor, save);
    }
};
#endif // COMBODELEGATE_CLASS




