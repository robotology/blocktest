#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qprocess.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_calibrate5kg_clicked();

    void on_load5kg_clicked();

    void on_home_clicked();

    void on_checkconfig_clicked();

    void on_Gazebo_stateChanged(int arg1);

    void on_testfolderstr_textChanged(const QString &arg1);

    void on_Validate_clicked();

private:
    QProcess test_;
    Ui::MainWindow *ui;
    const std::string calib5kg_{"kuka-4config5kg"};
    const std::string calib25kg_{"kuka-4config25kg"};
    const std::string load5kg_{"kuka-loadcalibration5kg"};
    const std::string load25kg_{"kuka-loadcalibration25kg"};
    const std::string validate5kg_{"kuka-validate5kg"};
    const std::string validate25kg_{"kuka-validate25kg"};
    const std::string home_{"kuka-home"};
    const std::string blocktest_{"./blocktestrunner"};
    const std::string gazebo_{"-gazebo"};
    std::string path_{"/home/triccyx/Documents/calibration-ft-sensors/ft-kuka/test/"};

    int gazeboSelected_{0};
    void testInExecution();

};
#endif // MAINWINDOW_H
