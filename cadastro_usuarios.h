#ifndef CADASTRO_USUARIOS_H
#define CADASTRO_USUARIOS_H

#include <QDialog>

namespace Ui {
class cadastro_usuarios;
}

class cadastro_usuarios : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief cadastro_usuarios::cadastro_usuarios
     *
     * Construtor da classe cadastro_usuarios, verifica se o banco de dados foi aberto com sucesso,
     * retornando uma mensagem de erro caso contrário.
     *
     * @param parent
     */
    explicit cadastro_usuarios(QWidget *parent = 0);

    /**
     * @brief cadastro_usuarios::~cadastro_usuarios
     *
     * Destrutor da classe cadastro_usuarios
     */
    ~cadastro_usuarios();

private slots:
    /**
     * @brief cadastro_usuarios::on_pushButton_c_usu_clicked
     *
     * Verifica as informações passadas pelo usuário e, caso os dados estejam corretos,
     * salva o novo usuário no banco de dados. Caso contrário retorna uma mensagem de erro.
     */
    void on_pushButton_c_usu_clicked();

private:
    Ui::cadastro_usuarios *ui;
};

#endif // CADASTRO_USUARIOS_H
