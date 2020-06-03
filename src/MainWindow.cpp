#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <sstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    initInsertDOMComboBox();
    disableControls();

    // set selected item as null
    m_selectedItem = nullptr;

    treeView = ui->treeView;
    treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Create model
    m_treeModel = new PageTree(DOMElement());


    // Set model and refresh display
    treeView->setModel(m_treeModel);
    treeView->expandAll();
    treeView->show();



    // Set delegate for style
    ui->styleList->setItemDelegate(new StyleDelegate());
    ui->attrList->setItemDelegate(new StyleDelegate());
    DOMDelegate *domDelegate = new DOMDelegate();
    treeView->setItemDelegate(domDelegate);


    // Set connects
    QObject::connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged,
                                                        this, &MainWindow::DOMElementChanged );

    // HTML button
    QObject::connect(ui->insertInDOMBtn, &QPushButton::clicked,
                     this, &MainWindow::insertItemInHTML );

    // Style button
    QObject::connect(ui->addNewStlBtn, &QPushButton::clicked,
                     this, &MainWindow::insertItemInStyle );

    QObject::connect(ui->delStlBtn, &QPushButton::clicked,
                     this, &MainWindow::removeItemInStyle );


    // Attribute button
    QObject::connect(ui->addNewAttrBtn, &QPushButton::clicked,
                     this, &MainWindow::insertItemInAttribute );

    QObject::connect(ui->delAttrBtn, &QPushButton::clicked,
                     this, &MainWindow::removeItemInAttribute );

    // DOM Button
    QObject::connect(ui->insertInDOMBtn, &QPushButton::clicked, this, &MainWindow::insertDOMElement);
    QObject::connect(ui->removeFromDOMBtn, &QPushButton::clicked, this, &MainWindow::removeDomElement);

    // Menu Button
    QObject::connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveToFile);
    QObject::connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFromFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DOMElementChanged(const QModelIndex &current, const QModelIndex &previous)
{
    HTMLNode *newElement = nullptr;
    // qDebug() << "change sel:" << current << "desel" << previous ;
    // Проверяем на валидность выделенный эл-т
    // В случае, если он совпадает с предыдущим, то выходим
    if (current.isValid()) {
        newElement = static_cast<HTMLNode*>(current.internalPointer());
        if (newElement == m_selectedItem) return;
        m_selectedItem = newElement;
    } else {
        m_selectedItem = nullptr;
    }


    // В случае если эл-т выбран, то заменяем текущий эл-т на новый
    // И отпавляем сигналы в блок управления стилем
    if (newElement != nullptr){
        m_selectedItem = newElement;
        enableControls();
        // I think it's a memory leak
        // set style model
        StyleModel *styleModel = new StyleModel( m_selectedItem->data().getStyles() );
        ui->styleList->setModel(styleModel);
        // set attribute model
        TagAttributeModel *attributeModel = new TagAttributeModel( m_selectedItem->data().getAttributes() );
        ui->attrList->setModel(attributeModel);
        return;
    }

    disableControls();
}

void MainWindow::insertItemInHTML(bool checked)
{
    if (!m_selectedItem){
        // TODO: Raise execption
        qDebug() << "Выберете объект";
        return;
    }
    //m_treeModel->insertColumns(ui->treeView->selectionModel()->selection()->, ui->treeView->selectionModel()->selection());
}

void MainWindow::insertItemInStyle(bool chached)
{
    if (!m_selectedItem) {
        qDebug() << "Выберете объект";
        return;
    }
    const QModelIndex index = ui->styleList->selectionModel()->currentIndex();
    std::list<std::string> fields;
    fields.emplace_back(std::string("Название стиля"));
    fields.emplace_back(std::string("Значение"));
    StyleModel* model = static_cast<StyleModel*>(ui->styleList->model());
    auto dialog = createDialogInput("Добавление стиля", fields);
    if (dialog.first->exec() == QDialog::Accepted) {
        QLineEdit *const style_name = dialog.second->at(0);
        QLineEdit *const style_value = dialog.second->at(1);
        model->cachedDataForNewItem(style_name->text(), style_value->text());
        if (model->insertRow(0, index)) {}
    }
}

void MainWindow::removeItemInStyle(bool changed)
{
    QListView* styleList = ui->styleList;
    if ( !styleList->isEnabled() ) {
        // raise not selected
        qDebug() << "not enabled";
    }
     QModelIndex current = styleList->selectionModel()->currentIndex();
     if( !current.isValid() ) {
        qDebug() << "not valid";
     }
     styleList->model()->removeRow(current.row());
}

void MainWindow::insertItemInAttribute(bool chached)
{
    qDebug() << "click";
    if (!m_selectedItem) {
        qDebug() << "Выберете объект";
        return;
    }
    const QModelIndex index = ui->attrList->selectionModel()->currentIndex();
    std::list<std::string> fields;
    fields.emplace_back(std::string("Название аттрибута"));
    fields.emplace_back(std::string("Значение"));
    TagAttributeModel* model = static_cast<TagAttributeModel*>(ui->attrList->model());
    auto dialog = createDialogInput("Добавление аттрибута", fields);
    if (dialog.first->exec() == QDialog::Accepted) {
        QLineEdit *const attribute_name = dialog.second->at(0);
        QLineEdit *const attribute_value = dialog.second->at(1);

        model->cachedDataForNewItem(attribute_name->text(), attribute_value->text());
        if (model->insertRow(0, index)) {}
    }
}

void MainWindow::removeItemInAttribute(bool changed)
{
    QListView* attrList = ui->attrList;
    if ( !attrList->isEnabled() ) {
        // raise not selected
        qDebug() << "not enabled";
    }
     QModelIndex current = attrList->selectionModel()->currentIndex();
     if( !current.isValid() ) {
        qDebug() << "not valid";
     }
     attrList->model()->removeRow(current.row());
}

void MainWindow::insertDOMElement()
{
    const QModelIndex index = treeView->selectionModel()->currentIndex();
    QAbstractItemModel* model = treeView->model();

    HTMLNode* node = m_treeModel->getItem(index);
    if (model->insertRow(0, index)) {
        auto comboBox = ui->insertTypeComboBox;
        const int tagID = comboBox->itemData(comboBox->currentIndex()).toInt();
        node->child(0)->data().setTag(tagID);
    }
}

void MainWindow::removeDomElement()
{
    const QModelIndex index = treeView->selectionModel()->currentIndex();
    QAbstractItemModel* model = treeView->model();

    if (!treeView->selectionModel()->hasSelection()) {
        return ;
    }

    if (model->removeRow(index.row(), index.parent())) {

    }
}

void MainWindow::saveToFile()
{
    QString fileName_html = QFileDialog::getSaveFileName(this,
                                                    tr("Сохранить DOM"), "",
                                                    tr("HTML (*.html);;All Files (*)"));

    QString fileName_css = QFileDialog::getSaveFileName(this,
                                                    tr("Сохранить стили"), "",
                                                    tr("CSS (*.css);;All Files (*)"));

    if (!fileName_html.isEmpty() && !fileName_css.isEmpty()) {
        std::ofstream file_html;
        std::ofstream file_css;
        try {
            file_html.open(fileName_html.toStdString(), std::ofstream::out);
            file_css.open(fileName_css.toStdString(), std::ofstream::out);
        }
        catch (std::system_error& e) {
            QMessageBox::information(this, tr("Не удалось открыть файл"),
                        e.what());
            return;
        }

        std::pair<std::string, std::string> data = m_treeModel->getSerializeData();
        file_html << data.first;
        file_css << data.second;
    }
    else {
        return;
    }
}

void MainWindow::openFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Сохранить DOM"), "",
                                                    tr("HTML (*.html);;All Files (*)"));

    if (!fileName.isEmpty()) {
        std::ifstream file;
        try {
            file.open(fileName.toStdString(), std::ofstream::in);
        }
        catch (std::system_error& e) {
            QMessageBox::information(this, tr("Не удалось открыть файл"),
                        e.what());
            return;
        }

        // Проверку можно убрать
        if (file.is_open()) {
            PageTree* tree = new PageTree(DOMElement());
            std::stringstream buffer;
            buffer << file.rdbuf();
            tree->readFromString(buffer.str());
            setTreeModel(tree);
            if (tree->rowCount() > 0) {
                enableControls();
            }
        }
    }
    else {
        return;
    }
}

void MainWindow::setTreeModel(PageTree* model)
{
    if (m_treeModel) {
        m_selectedItem = nullptr;
        delete m_treeModel;
    }
    m_treeModel = model;
    treeView->setModel(m_treeModel);
    treeView->expandAll();
    treeView->show();
    QObject::connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged,
                                                        this, &MainWindow::DOMElementChanged );
}

void MainWindow::enableControls()
{
    ui->addNewStlBtn->setEnabled(true);
    ui->delStlBtn->setEnabled(true);
    ui->addNewAttrBtn->setEnabled(true);
    ui->delAttrBtn->setEnabled(true);
    ui->insertInDOMBtn->setEnabled(true);
    ui->removeFromDOMBtn->setEnabled(true);
}

void MainWindow::disableControls()
{
    ui->addNewStlBtn->setEnabled(false);
    ui->delStlBtn->setEnabled(false);
    ui->addNewAttrBtn->setEnabled(false);
    ui->delAttrBtn->setEnabled(false);
    ui->insertInDOMBtn->setEnabled(false);
    ui->removeFromDOMBtn->setEnabled(false);
}

void MainWindow::initInsertDOMComboBox()
{
    QComboBox *comboBox = ui->insertTypeComboBox;
    std::vector<Tag> tags = Tags::getInstance()->getAllTags();
    for (Tag tag: tags) {
        comboBox->addItem(tag.second.c_str(), QVariant(tag.first));
    }
}

std::pair<QDialog* ,QList<QLineEdit *>*> MainWindow::createDialogInput(const std::string &name, std::list<std::string> fields)
{
    QDialog *dialog = new QDialog(this);
    // Use a layout allowing to have a label next to each field
    QFormLayout *form = new QFormLayout(dialog);

    // Add some text above the fields
    form->addRow(new QLabel(name.c_str()));

    // Add the lineEdits with their respective labels
    QList<QLineEdit *> *dialogFields = new QList<QLineEdit *>();

    for(auto it=fields.begin(); it!=fields.end(); ++it){
        QLineEdit *lineEdit = new QLineEdit(dialog);
        QString label = QString(it->c_str());
        form->addRow(label, lineEdit);

        *dialogFields << lineEdit;
    }


    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, dialog);
    form->addRow(buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    return std::pair<QDialog* ,QList<QLineEdit *>*>(dialog, dialogFields);
}
