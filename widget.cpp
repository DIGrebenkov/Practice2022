#include "widget.h"
#include "ui_widget.h"

#include "words.h"

#include <QFile>
QFile file;
bool fileOpened = false;

QVector <Reangor *> RAGs;
int currentRAGidx = 0;

Widget::Widget (QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->lineEdit->hide ();
    ui->comboBox->addItem ("Свой");
    ui->comboBox->addItem ("По правилам русского языка");
    ui->comboBox->setCurrentIndex (0);
    RAGs.append (new RAGConstantTheoretical);
    RAGs.append (new RAGAdaptiveRussianSyllables);
}

Widget::~Widget ()
{
    file.close ();
    delete ui;
}

void Widget::log (QString text) {
    ui->textEdit->setText (ui->textEdit->toPlainText () + text);
}

void Widget::on_pushButton_clicked()
{
    QString fullLog = "Сгенерированные слова:\n";
    QString buffer;
    for (int i = 0; i < 10; i ++) {
        buffer = RAGs [currentRAGidx]->generate ();
        while (!RAGs [currentRAGidx]->filter.check (buffer.left (buffer.size ()).toLower ())) {
            buffer = RAGs [currentRAGidx]->generate ();
        }
        fullLog += buffer + '\n';
    }
    ui->textEdit->setText ("  ");
    log (fullLog);
}

void Widget::on_lineEdit_editingFinished ()
{
    if (fileOpened) {
        return;
    }
    QString fileName = ui->lineEdit->text ();
    fileName.remove ('\n');
    fileName.remove ('\r');
    file.setFileName (fileName);
    if (!file.open (QIODevice::ReadOnly)) {
        log ("ОШИБКА: Не удалось открыть этот файл.\n");
    }
    else {
        fileOpened = true;
        log ("Файл " + fileName + " успешно открыт.\n");
    }
    ui->lineEdit->clear ();
    ui->lineEdit->hide ();
}

void Widget::on_pushButton_2_clicked ()
{
    int recognizedSuccessfully = 0;
    int notRecognized = 0;
    QString unrecognizedWords;
    QString buffer;
    while (!file.atEnd ()) {
        buffer = file.readLine ();
        buffer.remove ('\n');
        buffer.remove ('\r');
        buffer.remove ('\t');
        buffer.remove (' ');
        buffer.remove ('-');
        buffer.remove ('\'');
        buffer.remove (QChar (QString ("ё") [0]));
        if (RAGs [currentRAGidx]->recognize (buffer.left (buffer.size ()).toLower ())) {
            ++ recognizedSuccessfully;
        }
        else {
            ++ notRecognized;
            if (notRecognized != 1) {
                unrecognizedWords += ", ";
            }
            unrecognizedWords += RAGs [currentRAGidx]->lastRecognition;
        }
    }
    //log (fullLog); // debug
    buffer = QString ("Результат:\n  %1 слов из %2 распознано успешно.\n").arg (
                recognizedSuccessfully).arg (
                recognizedSuccessfully + notRecognized);
    if (notRecognized != 0) {
        buffer += "  Нераспознанные слова: " + unrecognizedWords + ".\n"; // debug
    }
    log (buffer);
    RAGs [currentRAGidx]->analyze ();
}

void Widget::on_pushButton_3_clicked ()
{
    ui->lineEdit->show ();
    file.close ();
    fileOpened = false;
}

void Widget::on_technicalButton_clicked ()
{
    QString buffer;
    buffer = RAGs [currentRAGidx]->generate ();
    while (!RAGs [currentRAGidx]->filter.check (buffer.left (buffer.size ()).toLower ())) {
        buffer = RAGs [currentRAGidx]->generate ();
    }
    buffer += '\n';
    log (buffer);
}

void Widget::on_comboBox_activated (int index)
{
    currentRAGidx = index;
}

void Widget::on_technicalButton_2_clicked()
{
    ui->textEdit->clear ();
    for (int i = 0; i < RAGs.size (); i ++) {
        delete RAGs [i];
    }
    RAGs.clear ();
    RAGs.append (new RAGConstantTheoretical);
    RAGs.append (new RAGAdaptiveRussianSyllables);
}
