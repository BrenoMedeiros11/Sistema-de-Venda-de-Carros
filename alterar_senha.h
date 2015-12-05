#ifndef ALTERAR_SENHA_H
#define ALTERAR_SENHA_H

#include <QDialog>

namespace Ui {
class alterar_senha;
}

class alterar_senha : public QDialog
{
    Q_OBJECT
    int id_logado;

public:
    /**
     * @brief alterar_senha::alterar_senha
     *
     * Construtor da classe alterar_senha, verifica se o banco de dados foi aberto com sucesso,
     * retornando uma mensagem de erro caso contrário.
     *
     * @param parent
     */
    explicit alterar_senha(QWidget *parent = 0);

    /**
     * @brief alterar_senha::~alterar_senha
     *
     * Destrutor da classe alterar_senha
     */
    ~alterar_senha();

    /**
     * @brief alterar_senha::set_ID_logado
     *
     * Seta a variável interna "id_logado", que corresponde a chave primária na tabela "vendedor" do banco de dados
     * para o usuário logado.
     *
     * @param _id
     */
    void set_ID_logado(int _id);

    /**
     * @brief alterar_senha::get_ID_logado
     *
     * Retorna o ID do usuário logado.
     *
     * @return (int) id_logado O ID do usuário logado.
     */
    int get_ID_logado();
private slots:
    /**
     * @brief alterar_senha::on_pushButton_Alterar_clicked
     *
     * Verifica as informações passadas pelo usuário e, caso os dados estejam corretos,
     * salva a nova senha do usuário no banco de dados. Caso contrário retorna uma mensagem de erro.
     */
    void on_pushButton_Alterar_clicked();

private:
    Ui::alterar_senha *ui;
};

#endif // ALTERAR_SENHA_H
