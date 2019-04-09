#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qtreeview.h>

#include "actiontreemodel.h"
#include "parametercommentmodel.h"
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


private slots:
    void on_loadButton_clicked();

    void on_saveButton_clicked();

    void on_applyButton_clicked();

    void on_scriptTree_clicked(const QModelIndex &index);

    void on_parametersList_clicked(const QModelIndex &index);

    void on_clearButton_clicked();

private:
    Ui::MainWindow *ui;

    ActionTreeModel  *commandsModel_{nullptr};
    ScriptTreeModel  *scriptModel_{nullptr};
    ParametersListModel *parametersModel_{nullptr};
    ParameterCommentModel *parameterCommentModel_{nullptr};

};

#endif // MAINWINDOW_H
