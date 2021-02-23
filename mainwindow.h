#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "global.h"

#include "./views/tableview.h"
#include "./models/carsmodel.h"
#include "./delegates/tabledelegate.h"

class MainWindow
{
    enum consoleCommands {
        Exit = 0,
        ShowHint,
        HideHint,
        ShowAllCars,
        ShowAllClients,
        ShowRentInfo,
        InsertNewCar,
        EraseCar,
        InsertNewClient,
        EraseClient,
        InsertRentInfo,
        EraseRentInfo,
        UserCommand = 10000
    };

public:
    MainWindow();
    ~MainWindow();

    void open();
    void update();

private:
    bool switchCommand(const int &command);
    void showHint();

private:
    int m_command;
    bool m_exit;
    bool m_showHint;

    TableView *m_table;
    TableDelegate *m_delegate;
    CarsModel *m_carModel;
};

#endif // MAINWINDOW_H
