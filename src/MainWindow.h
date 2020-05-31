#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <fstream>

#include "Models/PageTree.h"
#include "Models/AttributeModel.h"
#include "Delegates/StyleDelegate.h"
#include "Delegates/DOMDelegate.h"
#include "utils.h"

#include <QMainWindow>
#include <QAbstractItemView>
#include <QItemSelection>

#include <QTreeView>
#include <QDialog>
#include <QDialogButtonBox>
#include <QList>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>

typedef AttributeModel<utils::unordered_set_StyleRecord, utils::StyleRecord> StyleModel;
typedef AttributeModel<utils::unordered_set_StyleRecord, utils::StyleRecord> TagAttributeModel;

namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    // Слот для изменения выбранного элемента
    void DOMElementChanged(const QModelIndex &current, const QModelIndex &previous);
    // Слот для вставки/удаления элемента в dom
    void insertItemInHTML(bool checked = false);
    // вставка/удаление нового стиля
    void insertItemInStyle(bool chached = false);
    void removeItemInStyle(bool changed = false);
    // вставка/удаление нового аттрибута
    void insertItemInAttribute(bool chached = false);
    void removeItemInAttribute(bool changed = false);
    // вставка/удаление элементов dom
    void insertDOMElement();
    void removeDomElement();
    // menu
    void saveToFile();
    void openFromFile();

    void setTreeModel(PageTree* model);
private:
    Ui::MainWindow *ui;

    PageTree *m_treeModel = nullptr;
    HTMLNode *m_selectedItem = nullptr;
    QTreeView *treeView = nullptr;



    // enable controls
    void enableControls();
    void disableControls();

    void initInsertDOMComboBox();

    std::pair<QDialog* ,QList<QLineEdit *>*> createDialogInput(const std::string &name, std::list<std::string> fields);
};

#endif // MAINWINDOW_H
