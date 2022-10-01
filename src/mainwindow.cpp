#include "mainwindow.h"
#include "extractwindow.h"
#include "license.h"
#include "ui_mainwindow.h"
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      compressWindow(new CompressWindow(this)),
      extractWindow(new ExtractWindow(this)) {
    ui->setupUi(this);
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
    const QString qt_ = QStringLiteral("qt_");
    const QStringList nameFilter ={ QStringLiteral("FrogArchiver_*.qm") };
    QDir i18nDir (QStringLiteral("%1/i18n/").arg(qApp->applicationDirPath()));
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
            QSettings settings(QStringLiteral("%1/settings.ini").arg(QCoreApplication::applicationDirPath()), QSettings::IniFormat);
            QString savedLang = settings.value("language", QStringLiteral("en")).toString();
            if (savedLang == langCode) {
                QApplication::installTranslator(qtTranslator);
                QApplication::installTranslator(appTranslator);
            }
        }
    }
}
