#include "alterar_senha.h"
#include "ui_alterar_senha.h"
#include "login.h"
#include "QByteArray"
#include <QCryptographicHash>
#include <QMessageBox>
#include <QSqlQuery>
#include <QString>

alterar_senha::alterar_senha(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::alterar_senha)
{
    ui->setupUi(this);

    login conexao;
    QMessageBox msg_sql_error;
    if( !conexao.conn_open() ) {
        msg_sql_error.setText("Erro de conexão com a base de dados - Erro #001 - alterar_senha.cpp");
        msg_sql_error.exec();
        return;
    }
}

alterar_senha::~alterar_senha()
{
    delete ui;
}


void alterar_senha::set_ID_logado(int _id)
{
    id_logado = _id;
}


int alterar_senha::get_ID_logado()
{
    return id_logado;
}

void alterar_senha::on_pushButton_Alterar_clicked()
{
    login conexao;
    QString atual_senha, nova_senha, confirmar_senha;
    QMessageBox msg_alterar_senha;
    atual_senha = ui->lineEdit_atual_senha->text();
    nova_senha = ui->lineEdit_nova_senha->text();
    confirmar_senha = ui->lineEdit_confirmar_senha->text();

    if( atual_senha.isEmpty() || nova_senha.isEmpty() || confirmar_senha.isEmpty() ) {
        msg_alterar_senha.setText("Preencha todos os campos para alterar a sua senha.");
        msg_alterar_senha.exec();
        return;
    }

    QByteArray as_password = QCryptographicHash::hash(ui->lineEdit_atual_senha->text().toLocal8Bit(),QCryptographicHash::Md5);
    atual_senha = QString(as_password.toHex());
    QByteArray ns_password = QCryptographicHash::hash(ui->lineEdit_nova_senha->text().toLocal8Bit(),QCryptographicHash::Md5);
    nova_senha = QString(ns_password.toHex());
    QByteArray cs_password = QCryptographicHash::hash(ui->lineEdit_confirmar_senha->text().toLocal8Bit(),QCryptographicHash::Md5);
    confirmar_senha = QString(cs_password.toHex());

    if( nova_senha != confirmar_senha ) {
        msg_alterar_senha.setText("Os campos NOVA SENHA e CONFIRMAR SENHA não são iguais.");
        msg_alterar_senha.exec();

        ui->lineEdit_nova_senha->clear();
        ui->lineEdit_confirmar_senha->clear();
        return;
    }

    if( atual_senha == nova_senha && nova_senha == confirmar_senha ) {
        return;
    }

    if( !conexao.conn_open() ) {
        msg_alterar_senha.setText("Erro de conexão com a base de dados - Erro #002 - alterar_senha.cpp");
        msg_alterar_senha.exec();
        return;
    }

    QString senha_banco;
    conexao.conn_open();
    QSqlQuery qry;
    qry.prepare("select senha from vendedor where idvendedor = ?");
    qry.addBindValue(id_logado);
    if( qry.exec() ) {
        while( qry.next() ) {
            senha_banco = qry.value(0).toString();

            if( senha_banco == atual_senha ) {

                qry.prepare("update vendedor set senha = ? where idvendedor = ?");
                qry.addBindValue(nova_senha);
                qry.addBindValue(id_logado);
                if( qry.exec() ) {
                    msg_alterar_senha.setText("Senha alterada com sucesso.");
                    msg_alterar_senha.exec();
                    conexao.conn_close();
                    this->hide();
                } else {
                    msg_alterar_senha.setText("Erro de conexão com a base de dados - Erro #004 - alterar_senha.cpp");
                    msg_alterar_senha.exec();
                    return;
                }
            } else {
                msg_alterar_senha.setText("A atual senha digitada está errada.");
                msg_alterar_senha.exec();

                ui->lineEdit_nova_senha->clear();
                ui->lineEdit_confirmar_senha->clear();
                return;
            }
        }
    } else {
        msg_alterar_senha.setText("Erro de conexão com a base de dados - Erro #003 - alterar_senha.cpp");
        msg_alterar_senha.exec();
        return;
    }
}
