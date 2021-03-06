#include "launcherwindowhtml.h"
#include "ui_launcherwindowhtml.h"

#include "setting.h"
#include <QWebFrame>
#include <QCloseEvent>
#include <QDesktopServices>

LauncherWindowHtml::LauncherWindowHtml(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LauncherWindowHtml)
{
    ui->setupUi(this);
    connect(&contentSetting, SIGNAL(quitApp()), this, SLOT(quitApp()));
    connect(&contentSetting, SIGNAL(startGame()), this, SLOT(startGame()));
    connect(&contentSetting, SIGNAL(startLevelEditor()), this, SLOT(startLevelEditor()));
    connect(ui->webContentView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(initObjects()));
    connect(ui->webContentView->page(), SIGNAL(linkClicked(QUrl)), this, SLOT(linkClicked(QUrl)));

    ui->webContentView->page()->mainFrame()->setUrl(QUrl("http://engine.wohlnet.ru/docs/Reverse-Engineer/samplelauncher.html"));
    ui->webContentView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
}

LauncherWindowHtml::~LauncherWindowHtml()
{
    delete ui;
}

void LauncherWindowHtml::initObjects()
{
    ui->webContentView->page()->mainFrame()->addToJavaScriptWindowObject("Launcher", &contentSetting);
}

void LauncherWindowHtml::quitApp()
{
    selType = 0;
    close();
}

void LauncherWindowHtml::startGame()
{
    selType = 1;
    close();
}

void LauncherWindowHtml::startLevelEditor()
{
    selType = 2;
    close();
}

void LauncherWindowHtml::linkClicked(QUrl url)
{
    QDesktopServices::openUrl(url);
}

void LauncherWindowHtml::closeEvent(QCloseEvent *ev)
{
    Settings->result = selType;
    Settings->disableFrameskip = contentSetting.noframeskip();
    Settings->NoSound = contentSetting.nosound();
    ev->accept();
}
