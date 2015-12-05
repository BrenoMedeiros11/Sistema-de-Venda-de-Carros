#include "cadastro_usuarios.h"
#include "ui_cadastro_usuarios.h"
#include "login.h"
#include "QByteArray"
#include <QCryptographicHash>
#include <QMessageBox>
#include <QSqlQuery>
#include <QString>

cadastro_usuarios::cadastro_usuarios(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cadastro_usuarios)
{
    ui->setupUi(this);

    login conexao;
    QMessageBox msg_sql_error;
    if( !conexao.conn_open() ) {
        msg_sql_error.setText("Erro de conexão com a base de dados - Erro #001 - cadastro_usuarios.cpp");
        msg_sql_error.exec();
        return;
    }
}

cadastro_usuarios::~cadastro_usuarios()
{
    delete ui;
}

void cadastro_usuarios::on_pushButton_c_usu_clicked()
{
    login conexao;
    int flag_nome = 0, flag_login = 0;

    QString name, username, password;
    QMessageBox msg_error;
    name = ui->lineEdit_name->text();
    username = ui->lineEdit_username->text();
    password = ui->lineEdit_password->text();

    if( !conexao.conn_open() ) {
        msg_error.setText("Erro de conexão com a base de dados - Erro #002 - cadastro_usuarios.cpp");
        msg_error.exec();
        return;
    }

    conexao.conn_open();
    QSqlQuery qry;

    bool isNum; int verif_name_numeric = name.toInt(&isNum);

    // Verificação dos dados enviados pelo usuário
    if( name.isEmpty() || isNum ){
        msg_error.setText("O nome não foi preenchido ou está incorreto.");
        msg_error.exec();
        return;
    }

    if( username.isEmpty() ){
        msg_error.setText("O login não foi preenchido.");
        msg_error.exec();
        return;
    }

   if( password.isEmpty() ){
        msg_error.setText("A senha não foi preenchida.");
        msg_error.exec();
        return;
   }

   // Verifica se o nome digitado existe no banco
   qry.prepare("select * from vendedor where nome = ?");
   qry.addBindValue(name);
   if( qry.exec() ) {
        int cont = 0;
        while( qry.next() ) {
            cont++;
        }

        if(cont >= 1){
            flag_nome = 1;
        }
   }

   // Verifica se o login digitado existe no banco
   qry.prepare("select * from vendedor where login = ?");
   qry.addBindValue(username);
   if( qry.exec() ) {
        int cont = 0;
        while( qry.next() ) {
            cont++;
        }

        if(cont >= 1){
            flag_login = 1;
        }
   }

   if(flag_nome == 1) {
        msg_error.setText("O usuário informado já possui cadastro no sistema.");
        msg_error.exec();
        return;
   }

   if(flag_login == 1) {
        msg_error.setText("O login digitado já existe no sistema. Escolha outro.");
        msg_error.exec();
        return;
   }

   QByteArray ba_password = QCryptographicHash::hash(ui->lineEdit_password->text().toLocal8Bit(),QCryptographicHash::Md5);
   password = QString(ba_password.toHex());

   // Insere o novo usuário no sistema
   qry.prepare("insert into vendedor (nome,login,senha) values (?,?,?)");
   qry.addBindValue(name);
   qry.addBindValue(username);
   qry.addBindValue(password);

   if( qry.exec() ) {
        msg_error.setText("Usuário inserido com sucesso.");
        msg_error.exec();
        conexao.conn_close();
        this->hide();
   } else {
        msg_error.setText("Erro de conexão com a base de dados - Erro #003 - cadastro_usuarios.cpp");
        msg_error.exec();
        return;
   }
}
