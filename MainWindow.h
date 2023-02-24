#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "AdvMotApi.h"
#include "AdvMotDrv.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    Ui::MainWindowClass ui;
};
