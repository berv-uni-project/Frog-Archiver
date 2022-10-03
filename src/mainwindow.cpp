#include "mainwindow.h"
#include "extractwindow.h"
#include "license.h"
#include "ui_mainwindow.h"
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      compressWindow(new CompressWindow(this)),
      extractWindow(new ExtractWindow(this)) {
    ui->setupUi(this);
    if (QFile(QStringLiteral("%1/settings.ini").arg(QCoreApplication::applicationDirPath())).exists())
        m_settings = new QSettings(QStringLiteral("%1/settings.ini").arg(QCoreApplication::applicationDirPath()), QSettings::IniFormat);
    else
        m_settings = new QSettings(QSettings::NativeFormat, QSettings::UserScope, QStringLiteral("berv-uni"), QStringLiteral("frogarchiver"));
    detectTranslations();

    QSize iconSize = QSize(fontMetrics().height(), fontMetrics().height());
    ui->buttonCompress->setIconSize(iconSize);
    ui->buttonExtract->setIconSize(iconSize);

    ui->mainToolBar->addAction(ui->actionCompress);

    connect(ui->actionCompress, &QAction::triggered, this, &MainWindow::Compress);
    connect(ui->buttonCompress, &QPushButton::pressed, this,
            &MainWindow::Compress);

    ui->mainToolBar->addAction(ui->actionExtract);
    connect(ui->actionExtract, &QAction::triggered, this, &MainWindow::Extract);
    connect(ui->buttonExtract, &QPushButton::pressed, this, &MainWindow::Extract);

    connect(ui->actionExit, &QAction::triggered, this, &QApplication::quit);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);
    connect(ui->actionLicense, &QAction::triggered, this, &MainWindow::showLicense);
    connect(ui->menuLanguage, &QMenu::triggered, this, &MainWindow::changeLanguage);
}

MainWindow::~MainWindow() {
    delete compressWindow;
    delete extractWindow;
    delete ui;
}

void MainWindow::showAbout() {
    About Ab1;
    Ab1.setModal(true);
    Ab1.exec();
}

void MainWindow::Extract() { extractWindow->show(); }

void MainWindow::Compress() { compressWindow->show(); }

void MainWindow::showLicense() {
    License license;
    license.setModal(true);
    license.exec();
}

void MainWindow::detectTranslations()
{
    ui->menuLanguage->clear();
    m_translations.clear();
    const QStringList nameFilter{QStringLiteral("FrogArchiver_*.qm")};
    const QString qt_ = QStringLiteral("qt_");

    QDir i18nDir(QStringLiteral("%1/%2").arg(QCoreApplication::applicationDirPath(), QStringLiteral("i18n")));
    if (i18nDir.entryList(nameFilter, QDir::Files, QDir::Name).isEmpty()) {
        i18nDir.setPath(QStringLiteral("%1/../i18n").arg(QCoreApplication::applicationDirPath()));
        if (i18nDir.entryList(nameFilter, QDir::Files, QDir::Name).isEmpty()) {
            i18nDir.setPath(QStringLiteral("%1/../share/frogarchiver/i18n").arg(QCoreApplication::applicationDirPath()));
            if (i18nDir.entryList(nameFilter, QDir::Files, QDir::Name).isEmpty()) {
                i18nDir.setPath(QStringLiteral("/usr/share/frogarchiver/i18n"));
            }
        }
    }

    const QStringList langList = i18nDir.entryList(nameFilter,QDir::Files, QDir::Name);
    for (const QString &translation : langList) {
        auto *appTranslator = new QTranslator;
        if (appTranslator->load(translation, i18nDir.absolutePath())) {
            QString langCode = appTranslator->language().mid(0, 2);
            auto *qtTranslator = new QTranslator;
            QLocale locale = QLocale(appTranslator->language());

            if (!langCode.startsWith(QStringLiteral("en")) && !qtTranslator->load(locale, qt_, QString(), i18nDir.absolutePath())) {
                if (!qtTranslator->load(locale, qt_, QString(), QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
                    qWarning() << "Can't find qt ts";
                }
            }
            m_translations.insert(langCode, {qtTranslator, appTranslator});

            if (m_settings->value("language", QStringLiteral("en")).toString() == langCode) {
                QApplication::installTranslator(qtTranslator);
                QApplication::installTranslator(appTranslator);
            }
        }
    }

    for (const QString &translation : langList) {
        QTranslator translator;
        translator.load(translation, i18nDir.absolutePath());
        QString lang = translation.mid(13,2);
        auto langAction = ui->menuLanguage->addAction(translator.translate("MainWindow","TRANSLATE TO YOUR LANGUAGE NAME"));
        langAction->setData(lang);
        langAction->setCheckable(true);
        langAction->setChecked(m_settings->value("language").toString() == lang);
    }
}

void MainWindow::changeLanguage(QAction *action)
{
    QVariant data = action->data();
    if(!m_translations.contains(data.toString()))
        detectTranslations();
    const QList<QAction*> allActions = ui->menuLanguage->actions();
    for (QAction *other : allActions)
        other->setChecked(false);
    for(auto translation : m_translations.value(m_settings->value("language").toString()))
        QApplication::removeTranslator(translation);
    m_settings->setValue("language", data);
    for(auto translation : m_translations.value(data.toString()))
        QApplication::installTranslator(translation);
    action->setChecked(true);
    QTranslator *translator = new QTranslator(this);
    translator->load(QString("FrogArchiver_%1.qm").arg(m_settings->value("language").toString()), QString("%1/i18n").arg(QCoreApplication::applicationDirPath()));
    qApp->installTranslator(translator);
}

void MainWindow::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}
