#ifndef LOGIN_H
#define LOGIN_H

#include <QtDebug>
#include <QDialog>
#include <QFileInfo>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>

#include "tela_principal.h"

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief login::login
     *
     * Construtor da classe login, verifica se o banco de dados foi aberto com sucesso,
     * retornando uma mensagem de erro caso contrário.
     *
     * @param parent
     */
    explicit login(QWidget *parent = 0);

    /**
     * @brief login::~login
     *
     * Destrutor da classe login
     */
    ~login();

    QSqlDatabase mydb;

    /**
     * @brief login::conn_open
     *
     * Abre a conexão com o banco de dados (SQLite).
     * @return (bool) True caso a conexão tenha sido aberta com sucesso | False caso contrário.
     *
     */
    bool conn_open();

    /**
     * @brief login::conn_close
     *
     * Fecha a conexão com o banco de dados.
     */
    void conn_close();
private slots:
    /**
     * @brief login::on_pushButtonLogin_clicked
     *
     * Verifica as informações passadas pelo usuário e, caso os dados estejam corretos,
     * redireciona o usuário para a página principal. Caso contrário retorna uma mensagem de erro.
     */
    void on_pushButtonLogin_clicked();

private:
    Ui::login *ui;
    tela_principal principal;
};

#endif // LOGIN_H
