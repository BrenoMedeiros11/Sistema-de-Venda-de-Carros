#include "veiculo_add_quant.h"
#include "ui_veiculo_add_quant.h"
#include "login.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

veiculo_add_quant::veiculo_add_quant(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::veiculo_add_quant)
{
    ui->setupUi(this);

    login conexao;
    QMessageBox msg_sql_error;
    if( !conexao.conn_open() ) {
        msg_sql_error.setText("Erro de conexão com a base de dados - Erro #001 - veiculo_add_quant.cpp");
        msg_sql_error.exec();
        return;
    }

    conexao.conn_open();
    QSqlQuery qry;

    // Busca no banco os dados dos fabricantes cadastrados
    qry.prepare("select fabricante from veiculo");

    if( qry.exec() ) {

        ui->comboBox_fabricante->addItem("Selecione um fabricante");

        while(qry.next()) {
            ui->comboBox_fabricante->addItem(qry.value(0).toString());
        }
        conexao.conn_close();

        ui->comboBox_fabricante->setCurrentIndex(0);

     } else {
        msg_sql_error.setText("Erro de conexão com a base de dados - Erro #002 - veiculo_add_quant.cpp");
        msg_sql_error.exec();
        return;
     }

    // Esconde a label/lineedit para adicionar quantidades e a quantidade atual
    ui->label_quantidade->hide(); ui->label_quantidade_atual->hide();
    ui->lineEdit_quantidade->hide();
}

veiculo_add_quant::~veiculo_add_quant()
{
    delete ui;
}

void veiculo_add_quant::on_comboBox_fabricante_currentIndexChanged(const QString &arg1)
{
    ui->comboBox_modelo->clear(); ui->comboBox_ano->clear();
    ui->lineEdit_quantidade->clear();

    if( arg1 == "Selecione um fabricante" ) {

        ui->lineEdit_quantidade->hide();
        ui->label_quantidade->hide(); ui->label_quantidade_atual->hide();

    } else {
        QMessageBox msg_add_quant_error;

        login conexao;
        if( !conexao.conn_open() ) {
            msg_add_quant_error.setText("Erro de conexão com a base de dados - Erro #003 - veiculo_add_quant.cpp");
            msg_add_quant_error.exec();
            return;
        }

        conexao.conn_open();
        QSqlQuery qry;
        qry.prepare("select modelo from veiculo where fabricante = ?");
        qry.addBindValue(arg1);

        if( qry.exec() ) {

            ui->comboBox_modelo->addItem("Selecione um modelo");

            while(qry.next()) {
                ui->comboBox_modelo->addItem(qry.value(0).toString());
            }
            conexao.conn_close();

            ui->comboBox_modelo->setCurrentIndex(0);

        } else {
            msg_add_quant_error.setText("Erro de conexão com a base de dados - Erro #004 - veiculo_add_quant.cpp");
            msg_add_quant_error.exec();
            return;
        }
    }
}

void veiculo_add_quant::on_comboBox_modelo_currentIndexChanged(const QString &arg1)
{
    ui->comboBox_ano->clear(); ui->lineEdit_quantidade->clear();

    if( arg1 == "Selecione um modelo" ) {

        ui->lineEdit_quantidade->hide();
        ui->label_quantidade->hide(); ui->label_quantidade_atual->hide();

    } else {
        QMessageBox msg_add_quant_error;

        login conexao;
        if( !conexao.conn_open() ) {
            msg_add_quant_error.setText("Erro de conexão com a base de dados - Erro #005 - veiculo_add_quant.cpp");
            msg_add_quant_error.exec();
            return;
        }

        conexao.conn_open();
        QSqlQuery qry;
        qry.prepare("select ano_fabricacao from veiculo where modelo = ?");
        qry.addBindValue(arg1);

        if( qry.exec() ) {

            ui->comboBox_ano->addItem("Selecione um ano");

            while(qry.next()) {
                ui->comboBox_ano->addItem(qry.value(0).toString());
            }
            conexao.conn_close();

            ui->comboBox_ano->setCurrentIndex(0);

        } else {
            msg_add_quant_error.setText("Erro de conexão com a base de dados - Erro #006 - veiculo_add_quant.cpp");
            msg_add_quant_error.exec();
            return;
        }
    }
}

void veiculo_add_quant::on_comboBox_ano_currentIndexChanged(const QString &arg1)
{
    ui->lineEdit_quantidade->clear();

    if( arg1 == "Selecione um ano" ) {

        ui->lineEdit_quantidade->hide();
        ui->label_quantidade->hide(); ui->label_quantidade_atual->hide();

    } else {
        QMessageBox msg_add_quant_error;

        QString fabricante, modelo, ano_fabricacao, quant_estoque;
        fabricante = ui->comboBox_fabricante->currentText();
        modelo = ui->comboBox_modelo->currentText();
        ano_fabricacao = arg1;

        login conexao;
        if( !conexao.conn_open() ) {
            msg_add_quant_error.setText("Erro de conexão com a base de dados - Erro #007 - veiculo_add_quant.cpp");
            msg_add_quant_error.exec();
            return;
        }

        conexao.conn_open();
        QSqlQuery qry;
        qry.prepare("select idveiculo, quant_estoque from veiculo where fabricante = ? and modelo = ? and ano_fabricacao = ?");
        qry.addBindValue(fabricante); qry.addBindValue(modelo); qry.addBindValue(ano_fabricacao);

        if( qry.exec() ) {

            while( qry.next() ) {
                row_affected = qry.value(0).toInt();
                quant_estoque = qry.value(1).toString();
            }

            ui->label_quantidade_atual->setText("<h3><center><b>Quantidade no estoque = "+quant_estoque+"</b></center></h3>");
            ui->label_quantidade_atual->show(); ui->label_quantidade->show(); ui->lineEdit_quantidade->show();

            conexao.conn_close();
        } else {
            msg_add_quant_error.setText("Erro de conexão com a base de dados - Erro #008 - veiculo_add_quant.cpp");
            msg_add_quant_error.exec();

            return;
        }
    }
}

void veiculo_add_quant::on_pushButton_add_clicked()
{
    QMessageBox msg_edit_quant_vei;
    QString quant_estoque_add_string;
    quant_estoque_add_string = ui->lineEdit_quantidade->text();
    bool isAdd; int quant_estoque_add = quant_estoque_add_string.toInt(&isAdd);
    int quant_estoque_atual = 0, quant_estoque_nova = 0;

    if( quant_estoque_add_string.isEmpty() || !isAdd ) {
        msg_edit_quant_vei.setText("A quantidade do veículo a ser adicionada não foi informada ou está incorreta.");
        msg_edit_quant_vei.exec();
        return;
    }

    login conexao;
    if( !conexao.conn_open() ) {
        msg_edit_quant_vei.setText("Erro de conexão com a base de dados - Erro #009 - veiculo_add_quant.cpp");
        msg_edit_quant_vei.exec();
        return;
    }

    conexao.conn_open();
    QSqlQuery qry;

    qry.prepare("select quant_estoque from veiculo where idveiculo = ?"); qry.addBindValue(row_affected);
    if( qry.exec() ) {
        while( qry.next() ) {
            quant_estoque_atual = qry.value(0).toInt();
        }
    } else {
        msg_edit_quant_vei.setText("Erro de conexão com a base de dados - Erro #010 - veiculo_add_quant.cpp");
        msg_edit_quant_vei.exec();
        return;
    }

    quant_estoque_nova = quant_estoque_atual + quant_estoque_add;

    qry.prepare("update veiculo set quant_estoque = ? where idveiculo = ?");
    qry.addBindValue(quant_estoque_nova); qry.addBindValue(row_affected);
    if( qry.exec() ) {
        msg_edit_quant_vei.setText("Quantidade adicionada com sucesso.");
        msg_edit_quant_vei.exec();
        conexao.conn_close();
        this->close();
        return;
    } else {
        msg_edit_quant_vei.setText("Erro de conexão com a base de dados - Erro #011 - veiculo_add_quant.cpp");
        msg_edit_quant_vei.exec();
        return;
    }
}
