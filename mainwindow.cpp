#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSystemTrayIcon>
#include <QDebug>
#include <QUuid>
#include <QInputDialog>
#include <QMessageBox>

#include "logindialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_loginDialog(new LoginDialog(this))
    , m_userInfoFile(new JsonFileUtil("./userinfo.json"))
    , m_httpUtil(new HttpUtil(this, new QNetworkAccessManager(this)))
    , m_userInfo(new UserInfo())
    , m_connectService(new ConnectService(this, m_userInfo, m_httpUtil))

{
    ui->setupUi(this);

    QApplication::setQuitOnLastWindowClosed(false);

    // Tray
    QSystemTrayIcon *icon = new QSystemTrayIcon(this);
    icon->setIcon(QIcon(":/icon/images/icon.png"));
    icon->setToolTip(QString("我的托盘"));

    QMenu * menu = new QMenu(this);
    QAction * normal, * quit;
    normal = new QAction("显示主界面");
    connect(normal, &QAction::triggered, [&]() {
        showNormal();
    });
    menu->addAction(normal);

    quit = new QAction("退出");
    connect(quit, &QAction::triggered, [&]() {
        QCoreApplication::quit();
    });
    menu->addAction(quit);

    icon->setContextMenu(menu);

    connect(icon, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason r) {
        if (r == QSystemTrayIcon::ActivationReason::Trigger) {
            emit normal->triggered();
        }
    });

    icon->show();

    connect(m_loginDialog, SIGNAL(login(QString)), this, SLOT(on_login_callback(QString)));

    if (!loadUserInfo()) {
        m_loginDialog->show();
    } else {
        m_connectService->start();
    }

    refershUserInfo();
}

MainWindow::~MainWindow()
{
    delete ui;

    m_connectService->stop();

    delete m_userInfoFile;
    delete m_httpUtil;
    delete m_userInfo;
}

void MainWindow::on_login_callback(const QString& username)
{
    qDebug() << username << ": Login ";

    m_httpUtil->login(m_userInfo->defaultHost, username, m_userInfo->deviceId);

    connect(m_httpUtil, &HttpUtil::loginResult, [=](bool ok) {
        if (ok) {
            m_userInfo->userName = username;
            saveUserInfo();
            refershUserInfo();
            m_connectService->start();
        } else {
            QMessageBox::warning(this,tr("警告！"), tr("用户名错误"), QMessageBox::Yes);
            m_loginDialog->show();
        }
    });
}

bool MainWindow::loadUserInfo()
{
    auto userInfo = this->m_userInfoFile->getJson();

    auto userName = userInfo["username"].toString("-1");
    auto deviceId = userInfo["deviceid"].toString("-1");
    auto defaultHost = userInfo["host"].toString("-1");

    if (deviceId != "-1") {
        m_userInfo->deviceId = deviceId;
    } else {
        auto uuid = QUuid::createUuid();
        QString id = uuid.toString();

        id.remove("{");
        id.remove("}");
        m_userInfo->deviceId = id;
    }

    if (defaultHost != "-1") {
        m_userInfo->defaultHost = defaultHost;
    } else {
        m_userInfo->defaultHost = "101.42.233.83";
    }

    if (userName != "-1") {
        m_userInfo->userName = userName;
    }

    saveUserInfo();

    return userName != "-1";
}

void MainWindow::saveUserInfo()
{
    m_userInfoFile->writeJson("username", m_userInfo->userName);
    m_userInfoFile->writeJson("deviceid", m_userInfo->deviceId);
    m_userInfoFile->writeJson("host", m_userInfo->defaultHost);
    m_userInfoFile->saveJson();
}

void MainWindow::refershUserInfo()
{
    ui->label_username->setText(m_userInfo->userName);
    ui->label_deviceid->setText(m_userInfo->deviceId);
    ui->label_defaulthost->setText(m_userInfo->defaultHost);
}

void MainWindow::on_pushButton_test_clicked()
{
    QString address = m_userInfo->defaultHost + "/user/adduser";
    QJsonObject data;
    data["name"] = "lizqwer";
    QJsonObject device;
    device["id"] = "asdsad";
    device["type"] = "Window";
    device["name"] = "SD-adS";
    data["device"] = device;

//    m_httpUtil->httpPost(address, data);
}


void MainWindow::on_pushButton_changehost_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("修改地址"), tr("User name:"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) {
        m_userInfo->defaultHost = text;
        saveUserInfo();
        refershUserInfo();
    }
}

void MainWindow::on_pushButton_relogin_clicked()
{
    m_loginDialog->show();
}

