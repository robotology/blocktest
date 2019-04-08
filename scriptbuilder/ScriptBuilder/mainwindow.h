#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qtreeview.h>

#include "actiontreemodel.h"
#include "parameterslistmodel.h"
#include "scripttreemodel.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void clicked(const QModelIndex &index);
    void buttonSaveClicked(bool checked);

private slots:
    void on_loadButton_clicked();

private:
    Ui::MainWindow *ui;

    ActionTreeModel  *commandsModel_{nullptr};
    ScriptTreeModel  *scriptModel_{nullptr};
    ParametersListModel *parametersModel_{nullptr};
};

#endif // MAINWINDOW_H
