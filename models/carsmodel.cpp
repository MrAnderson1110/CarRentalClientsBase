#include "carsmodel.h"

#include "../app_core/cstringdata.h"

#include <cstdlib>
#include <sstream>

CarsModel::CarsModel()
    : AbstractItemModel()
    , m_currentStorage()
    , m_showList()
{

}

CarsModel::~CarsModel()
{

}

void CarsModel::initHeader()
{
    setHeaderSize(1);
    char n[100] = "N";
    char numb[100] = "Гос. номер";
    char brand[100] = "Марка";
    char color[100] = "Цвет";
    char year[100] = "Год выпуска";
    char available[100] = "Доступен";
    char header[600] = "\0";
    std::strncat(header, n, 100);
    for(int i = 0; i < 3 - app_global::numberOfLetters(n); ++i)
        std::strncat(header, " ", 1);
    std::strncat(header, " | ", 3);
    std::strncat(header, numb, 100);
    for(int i = 0; i < 25 - app_global::numberOfLetters(numb); ++i)
        std::strncat(header, " ", 1);
    std::strncat(header, " | ", 3);
    std::strncat(header, brand, 100);
    for(int i = 0; i < 25 - app_global::numberOfLetters(brand); ++i)
        std::strncat(header, " ", 1);
    std::strncat(header, " | ", 3);
    std::strncat(header, color, 100);
    for(int i = 0; i < 25 - app_global::numberOfLetters(color); ++i)
        std::strncat(header, " ", 1);
    std::strncat(header, " | ", 3);
    std::strncat(header, year, 100);
    for(int i = 0; i < 25 - app_global::numberOfLetters(year); ++i)
        std::strncat(header, " ", 1);
    std::strncat(header, " | ", 3);
    std::strncat(header, available, 100);
    for(int i = 0; i < 25 - app_global::numberOfLetters(available); ++i)
        std::strncat(header, " ", 1);

    setHeaderData(CStringData(header, 600), 0);
}

CStringData CarsModel::data(const ModelIndex &index) const
{
    if(m_showList.size() < index.row() - 1
            || !index.isValid())
        return CStringData();

    if(index.column() != 0)
        return CStringData();

    int realRow = index.row() - 1;
    if(realRow < 0)
        return headerData(index.column());

    switch (index.column()) {
    case 0: {
        CStringData n(index.row());
        char nPresentation[100];

        std::strncat(nPresentation, n.data(), 3);
        for(int i = 0; i < 3 - app_global::numberOfLetters(n.data()); ++i)
            std::strncat(nPresentation, " ", 1);
        return CStringData(nPresentation, 100);
    }
    case 1: {
        const char *number = m_showList[realRow].number();
        char numberPresentation[100];
        std::strncat(numberPresentation, number, app_global::car::numberMaxLen);
        for(int i = 0; i < 25 - app_global::numberOfLetters(number); ++i)
            std::strncat(numberPresentation, " ", 1);
        return CStringData(number, app_global::car::numberMaxLen);
    }
    case 2:{
        const char *brand = m_showList[realRow].brand();
        char brandPresentation[100];
        std::strncat(brandPresentation, brand, app_global::car::brandMaxLen);
        for(int i = 0; i < 25 - app_global::numberOfLetters(brand); ++i)
            std::strncat(brandPresentation, " ", 1);
    }
    case 3: {
        const char *color = m_showList[realRow].color();
        char colorPresentation[100];
        std::strncat(colorPresentation, color, app_global::car::colorMaxLen);
        for(int i = 0; i < 25 - app_global::numberOfLetters(color); ++i)
            std::strncat(colorPresentation, " ", 1);
    }
    case 4: {
        CStringData year(m_showList[realRow].year());
        char yearPresentation[100];

        std::strncat(yearPresentation, year.data(), app_global::car::yearMaxLen);
        for(int i = 0; i < 25 - app_global::numberOfLetters(year.data()); ++i)
            std::strncat(yearPresentation, " ", 1);
    }
    case 5: {
        char availablePresentation[100];
        if(m_showList[realRow].available()) {
            std::strncat(availablePresentation, "Да", app_global::car::availableMaxLen);
            for(int i = 0; i < 25 - app_global::numberOfLetters("Да"); ++i)
                std::strncat(availablePresentation, " ", 1);
        }
        else {
            std::strncat(availablePresentation, "Нет", app_global::car::availableMaxLen);
            for(int i = 0; i < 25 - app_global::numberOfLetters("Нет"); ++i)
                std::strncat(availablePresentation, " ", 1);
        }
    }
    default:
        return CStringData();
    }
    return CStringData();
}

void CarsModel::setData(const CStringData &data, const ModelIndex &index)
{
    //    if(data.isEmpty()
    //            || m_showList.size() < index.row()
    //            || !index.isValid())
    //        return;

    //    int realRow = index.row() - 1;
    //    if(realRow < 0)
    //        return;

    //    switch (index.column()) {
    //    case 1: {
    //        Car editingData = m_showList[realRow];
    //        m_currentStorage.erase(editingData.number());
    //        editingData.setNumber(data.data());
    //        m_currentStorage.insert(editingData.number(), editingData);
    //        m_showList = m_currentStorage.values();
    //        break;
    //    }
    //    case 2: {
    //        const char *key = m_showList[realRow].number();
    //        Car &editingData = m_currentStorage[key];
    //        editingData.setBrand(data.data());
    //        m_showList = m_currentStorage.values();
    //        break;
    //    }
    //    case 3: {
    //        const char *key = m_showList[realRow].number();
    //        Car &editingData = m_currentStorage[key];
    //        editingData.setColor(data.data());
    //        m_showList = m_currentStorage.values();
    //        break;
    //    }
    //    case 4: {
    //        const char *key = m_showList[realRow].number();
    //        Car &editingData = m_currentStorage[key];
    //        editingData.setYear(data.toInt());
    //        m_showList = m_currentStorage.values();
    //        break;
    //    }
    //    case 5: {
    //        const char *key = m_showList[realRow].number();
    //        Car &editingData = m_currentStorage[key];
    //        if(std::strncmp(data.data(), "Да", 3))
    //            editingData.setAvailable(true);
    //        else
    //            editingData.setAvailable(false);
    //        m_showList = m_currentStorage.values();
    //    }
    //    }
}

void CarsModel::insertColumn(int column)
{
    (void)column;
}

void CarsModel::removeColumn(int column)
{
    (void)column;
}

int CarsModel::columnCount() const
{
    return 1;
}

int CarsModel::rowCount() const
{
    return m_showList.size();
}

void CarsModel::insertRow(int row)
{
    (void)row;
}

void CarsModel::insertRow(const Car &car)
{
    m_currentStorage.insert(car.number(), car);
    m_showList = m_currentStorage.values();
}

void CarsModel::clearModel()
{
    m_currentStorage.clear();
    m_showList = m_currentStorage.values();
}

void CarsModel::removeRow(int row)
{
    if(m_showList.size() < row)
        return;

    Car &removedVal = m_showList[row];
    m_currentStorage.erase(removedVal.number());
    m_showList = m_currentStorage.values();
}
