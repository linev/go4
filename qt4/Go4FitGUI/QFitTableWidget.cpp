#include "QFitTableWidget.h"

#include "QFitItem.h"
#include "TGo4Fitter.h"
#include "TGo4FitGuiTypes.h"

#include "Riostream.h"

QFitTableWidget::QFitTableWidget(QWidget *parent, const char* name)
         : QFitWidget(parent, name)
{
   setupUi(this);
   // put slot connections here!
   // note: Qt4 uic will add all existing connections
   // from ui file to the setupUI
}

void QFitTableWidget::FillSpecificData()
{
   Table->setColumnCount(0);
   Table->setRowCount(0);
   Table->setEnabled(false);
   if (GetItem())
     if (GetItem()->ObjectType()==FitGui::ot_results) {
        TGo4Fitter* fitter = dynamic_cast<TGo4Fitter*> (GetObject());
        if (fitter==0) return;
        Table->setColumnCount(1);
        Table->setHorizontalHeaderItem(0, new QTableWidgetItem("Value"));
        Table->setColumnWidth(0,150);
        Table->setRowCount(fitter->GetNumResults()+2);
        Table->setVerticalHeaderItem(0,  new QTableWidgetItem("Fit function"));
        Table->setItem(0,0, new QTableWidgetItem(QString::number(fitter->GetResultFF())));
        Table->setVerticalHeaderItem(1, new QTableWidgetItem("NDF"));
        Table->setItem(1,0, new QTableWidgetItem(QString::number(fitter->GetResultNDF())));

        for (Int_t n=0;n<fitter->GetNumResults();n++) {
           Table->setVerticalHeaderItem(n+2, new QTableWidgetItem(QString("Result") + QString::number(n)));
           Table->setItem(n+2, 0, new QTableWidgetItem(QString::number(fitter->GetResultValue(n))));
        }
     }
}
