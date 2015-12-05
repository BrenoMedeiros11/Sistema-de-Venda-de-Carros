#include "login.h"
#include "ui_login.h"
#include "tela_principal.h"
#include "QByteArray"
#include <QCryptographicHash>
#include <QMessageBox>
#include <QPixmap>
#include <QString>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    QPixmap pix(":/initial_image/initial_image");
    ui->label_picture->setPixmap(QPixmap(pix));
    ui->label_picture->resize(pix.size());

    QMessageBox msg_sql_error;
    if(!conn_open()) {
        msg_sql_error.setText("Erro de conexão com a base de dados - Erro #001 - login.cpp");
        msg_sql_error.exec();
        return;
    }
}

login::~login()
{
    delete ui;
}

bool login::conn_open()
{
    mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("../sistema_venda_carros/database/sistema_venda_carros.db");

    if(!mydb.open()) {
        return false;
    } else {
        return true;
    }
}

void login::conn_close()
{
    mydb.close();
    mydb.removeDatabase(QSqlDatabase::defaultConnection);
}

void login::on_pushButtonLogin_clicked()
{
    QString username, password;
    QMessageBox msg_login;
    username = ui->lineEdit_username->text();
    password = ui->lineEdit_password->text();

    if( username.isEmpty() || password.isEmpty() ) {
        msg_login.setText("Preencha o login e/ou senha para entrar no sistema.");
        msg_login.exec();
        return;
    }

    QByteArray ba_password = QCryptographicHash::hash(ui->lineEdit_password->text().toLocal8Bit(),QCryptographicHash::Md5);
    password = QString(ba_password.toHex());

    if(!mydb.isOpen()) {
        msg_login.setText("Erro de conexão com a base de dados - Erro #002 - login.cpp");
        msg_login.exec();
        return;
    }

    QSqlQuery qry;
    qry.prepare("select * from vendedor where login = ? and senha = ?");
    qry.addBindValue(username);
    qry.addBindValue(password);
    int idusu;
    if( qry.exec() ) {
        int cont = 0;
        while( qry.next() ) {
            idusu = qry.value(0).toInt();
            cont++;
        }

        if(cont == 0){
            msg_login.setText("Login e/ou senha incorretos.");
            msg_login.exec();
            ui->lineEdit_password->clear();
            return;
        } else {
            if(cont == 1){
                principal.setPosicao(idusu); // Guarda o ID do usuário logado
                conn_close();
                this->hide(); // Esconde a tela de login
                principal.show(); // Revela a tela principal
            }
        }
    } else {
        msg_login.setText("Erro de conexão com a base de dados - Erro #003 - login.cpp");
        msg_login.exec();
        return;
    }
}
