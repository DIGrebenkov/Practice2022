#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void log (QString text);

private slots:
    void on_pushButton_clicked();

    void on_lineEdit_editingFinished();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_technicalButton_clicked();

    void on_comboBox_activated(int index);

    void on_technicalButton_2_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
