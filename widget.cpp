#include "widget.h"
#include "ui_widget.h"
#include "string.h"
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebEngineWidgets/QWebEnginePage>
#include <QtWebEngineWidgets/QWebEngineSettings>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

       QWebEngineView* webview = new QWebEngineView;
       QUrl url = QUrl("https://repp.azurewebsites.net/maps/?lat=-34.397&long=150.644");
       webview->page()->load(url);
       ui->verticalLayout->addWidget(webview);


       /* Create Object the Class QSerialPort*/
       devserial = new QSerialPort(this);      

       /* Create Object the Class comserial to manipulate read/write of the my way */
       procSerial = new comserial(devserial);

       /* Load Device PortSerial available */
       QStringList DispSeriais = procSerial->CarregarDispositivos();

       /* Inserting in ComboxBox the Devices */
       ui->comboBox->addItems(DispSeriais);

       /* Enable PushButton "Conectar" if any port is found.
        * If an error occurs, it is reported in the Log
        */
       if(DispSeriais.length() > 0) {
           ui->pushButton->setEnabled(true);
           ui->textEdit_2->append("### Porta serial pronta para ser utilizada.");
       }
       else { ui->textEdit_2->append("### Nenhuma porta serial foi detectada!"); }


       /* Connect Objects -> Signal and Slots
        * DevSerial with Read Data Serial
        * TextEdit "teLog" with getData() after send data WriteData() [Not apply here in version 5.X]
        */
       connect(devserial, SIGNAL(readyRead()), this, SLOT(ReadData()));
       //connect(ui->teLog, SIGNAL(getData(QByteArray)), this, SLOT(WriteData(QByteArray)));
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief Widget::CarregarInfoDispSerial()
 *
 * Função que cria uma QStringList e baseado no device selecionado na Porta em Porta Serial
 * carrega algumas informações como Fabricante e Descrição e já envia para o EditText no
 * frame Porta Serial
 *
 * @param void
 * @return void
 */
/*
void Widget::CarregarInfoDispSerial()
{
    QStringList InfoDisp = procSerial->Info(ui->comboBox->currentText());
}
*/

void Widget::WriteData(const QByteArray data)
{
    procSerial->Write(data);
}

void Widget::ReadData()
{
    //QByteArray a;
    //a = devserial->readAll();
    //QString data = procSerial->Read();
    //qDebug() << "Input: " << QString(a) << endl;
    //ui->textEdit_2->append(QString(a));
    //if(a.toDouble() > 0){
        //ui->lcdNumber->display(QString(a));
    //}
    QByteArray datas = devserial->readLine();
    QString data2 = "";
        if (datas.size() == 0) {
            qDebug() << "Arrived data: 0";
            //ui->textEdit_2->append(datas);
        } else {
            for (int i = 0; i < datas.size(); i++){
                if(datas[i] == '\n'){
                    break;
                }
                if (datas.at(i)) {
                    //qDebug() << datas[i];
                    if(datas[i] == '\r') break;
                    data2 += datas[i];
                }
            }
            //QString data2 = tr(datas);
            //int n = data2.toInt();
            qDebug() << data2;
            ui->textEdit_2->append(data2);
            if(data2.toFloat()>0){
                ui->lcdNumber->display(data2);                
            }
        }

}

void Widget::on_pushButton_clicked()
{
    bool statusOpenSerial;

    statusOpenSerial = procSerial->Conectar(ui->comboBox->currentText(),
                                             ui->comboBox_2->currentText().toInt()
                                             );
    if (statusOpenSerial) {
        ui->pushButton_2->setEnabled(true);
        ui->pushButton->setEnabled(false);
        ui->textEdit_2->append("### Porta serial aberta com sucesso!");
    }
    else {
        ui->textEdit_2->append("Falha ao abrir conexão serial.");
    }
}

void Widget::on_pushButton_2_clicked()
{
    bool statusCloseSerial;


    statusCloseSerial = procSerial->Desconectar();

    /* BUG: Existe um bug no close do QtSerialPort, já conhecido pela comunidade onde
     * quando usado com waitForReadyRead, da um erro 12 Timeout no SerialPort e não encerra a conexão
     * porém é reportado o erro mas o device é encerrado.
     * Para contornar o problema no Desconectar eu verifiquei com isOpen logo apos fechar a conexão
     * serial.
     */

    if (statusCloseSerial) {
        ui->pushButton_2->setEnabled(false);
        ui->pushButton->setEnabled(true);
        ui->textEdit_2->append("### Porta serial fechada com sucesso!");
    }
    else {
        ui->textEdit_2->append("### Falha ao fechar conexão serial.");
    }
}

void Widget::on_pushButton_3_clicked()
{
    QString Cmd = ui->lineEdit->text()+"\n";
    qDebug() << "Output: " << Cmd << endl;
    WriteData(Cmd.toUtf8());
}


