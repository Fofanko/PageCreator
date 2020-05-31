#ifndef STYLEWIDGET_H
#define STYLEWIDGET_H

#include <QObject>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QHBoxLayout>

class StyleWidget: public QWidget
{
    Q_OBJECT
public:
    StyleWidget(QWidget *parent=nullptr)
        : QWidget(parent),
          nameEditor(new QLineEdit),
          valueEditor(new QLineEdit)
    {
        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(nameEditor);
        layout->addWidget(valueEditor);

        setLayout(layout);
    }

//    int currentIndex(){
//        return comboBox->currentIndex();
//    }
//    void setCurrentIndex(int index){
//        comboBox->setCurrentIndex(index);
//    }
//    QString text() const{
//        return  lineBox->text();
//    }
//    void setText(const QString &text){
//        lineBox->setText(text);
//    }
//    Qt::CheckState checkState() const{
//        return checkBox->checkState();
//    }
//    void setCheckState(Qt::CheckState state){
//        checkBox->setCheckState(state);
//    }

    QSize sizeHint() const override
    {
        return layout()->sizeHint();
    }
private:
    QLineEdit *nameEditor;
    QLineEdit *valueEditor;
};

#endif // STYLEWIDGET_H
