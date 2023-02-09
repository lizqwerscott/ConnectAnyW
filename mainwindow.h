#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "utils/jsonfileutil.h"
#include "utils/httputil.h"

#include "logindialog.h"

#include "Service/connectservice.h"

#include "data/userinfo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_login_callback(const QString& username);

private slots:
    void on_pushButton_test_clicked();
    void on_pushButton_changehost_clicked();

    void on_pushButton_relogin_clicked();

private:
    bool loadUserInfo();
    void saveUserInfo();
    void refershUserInfo();

private:
    Ui::MainWindow *ui;
    LoginDialog * m_loginDialog;

    JsonFileUtil * m_userInfoFile;
    HttpUtil * m_httpUtil;

    UserInfo * m_userInfo = nullptr;

    ConnectService * m_connectService;
};
#endif // MAINWINDOW_H
