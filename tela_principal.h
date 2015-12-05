#ifndef TELA_PRINCIPAL_H
#define TELA_PRINCIPAL_H

#include <QMainWindow>
#include <QSqlQuery>
#include "alterar_senha.h"
#include "cadastro_usuarios.h"
#include "cadastro_veiculos.h"
#include "cadastro_vendas.h"
#include "edittableviewveiculos.h"
#include "veiculo_add_quant.h"
#include "view_veiculos.h"
#include "view_vendas.h"

namespace Ui {
class tela_principal;
}

class tela_principal : public QMainWindow
{
    Q_OBJECT

    /**
     * @brief posicao_usuario_txt
     *
     * Salva o valor do ID do vendedor logado.
     */
    int posicao_usuario_txt; // Sa

    /**
     * @brief action_table_view
     *
     * Salva o tipo de ação que está sendo executada.
     *
     * <ul>
     *      <li>1 - Usuários;</li>
     *      <li>2 - Veículos;</li>
     *      <li>3 - Vendas.</li>
     * </ul>
     */
    int action_table_view;

    /**
     * @brief row_table_selected
     *
     * Salva o número da linha na tabela selecionada pelo usuário.
     */
    int row_table_selected;

public:
    /**
     * @brief tela_principal::tela_principal
     *
     * Construtor da classe tela_principal, verifica se o banco de dados foi aberto com sucesso,
     * retornando uma mensagem de erro caso contrário. Também "esconde" o menu de editar/deletar dados.
     *
     * @param parent
     */
    explicit tela_principal(QWidget *parent = 0);

    /**
     * @brief tela_principal::~tela_principal
     *
     * Destrutor da classe tela_principal.
     */
    ~tela_principal();

    /**
     * @brief tela_principal::setPosicao
     *
     * Seta a variável interna "posição_usuario_txt", que corresponde a chave primária na tabela "vendedor" do banco de dados
     * para o usuário logado.
     *
     * @param _posicao
     */
    void setPosicao(int _posicao);

    /**
     * @brief tela_principal::getPosicao
     *
     * Retorna o ID do usuário logado.
     *
     * @return (int) posicao_usuario_txt O ID do usuário logado.
     */
    int getPosicao();

    /**
     * @brief tela_principal::getID
     *
     * Carrega do banco, o ID correspondente ao dado selecionado na tableview.
     * Os valores de column podem variar de acordo com os dados abaixo:
     *
     * <ul>
     *      <li>Para usuários: 0 para idusuario, 1 para nome, 2 para login.</li>
     *      <li>Para veículos: 0 para idveiculo, 1 para placa.</li>
     * </ul>
     *
     * @param _column
     * @param _data
     * @return id_return O ID do dado correspondente, ou 0 caso haja um erro (seguido de uma mensagem de erro).
     */
    int getID(int _column, QString _data);

    /**
     * @brief tela_principal::getNumVendas
     *
     * Recupera o número de vendas envolvendo o respectivo dado (Usuário ou Veículo), ou uma mensagem de erro caso contrário.
     *
     * @param tipo
     * @param _id_tabela
     * @return (int) num_vendas O número de vendas do respectivo dado. Caso retorne -1, significa um erro na função.
     */
    int getNumVendas(int tipo, int _id_tabela);

    /**
     * @brief tela_principal::apaga_vendas_vendedor
     *
     * Apaga todas as vendas relacionadas ao usuário especificado.
     *
     * @param (int) id_vendedor O ID do vendedor a ser pesquisado as vendas
     * @return (int) 1 caso as vendas tenham sido apagadas com sucesso | Um numero maior que 1 representando o erro ocorrido
     */
    int apaga_vendas_vendedor(int id_vendedor);
private slots:
    /**
     * @brief tela_principal::on_actionExit_triggered
     *
     * Realiza a saída do usuário do programa.
     */
    void on_actionExit_triggered();

    /**
     * @brief tela_principal::on_actionCadastro_de_Usu_rios_triggered
     *
     * Carrega a Dialog para o cadastro de usuários.
     */
    void on_actionCadastro_de_Usu_rios_triggered();

    /**
     * @brief tela_principal::on_actionCadastro_de_Ve_culos_triggered
     *
     * Carrega a Dialog para o cadastro de veículos.
     */
    void on_actionCadastro_de_Ve_culos_triggered();

    /**
     * @brief tela_principal::on_actionNova_Venda_triggered
     *
     * Carrega a Dialog para a nova venda.
     */
    void on_actionNova_Venda_triggered();

    /**
     * @brief tela_principal::on_actionLista_de_Usu_rios_triggered
     *
     * Carrega do banco os dados dos usuários cadastrados.
     */
    void on_actionLista_de_Usu_rios_triggered();

    /**
     * @brief tela_principal::on_actionLista_de_Ve_culos_triggered
     *
     * Carrega do banco os dados dos veículos cadastrados.
     */
    void on_actionLista_de_Ve_culos_triggered();

    /**
     * @brief tela_principal::on_actionLista_de_Vendas_triggered
     *
     * Carrega do banco os dados das vendas cadastradas.
     */
    void on_actionLista_de_Vendas_triggered();

    /**
     * @brief tela_principal::on_tableView_Dados_doubleClicked
     *
     * Mostra as opções de editar/deletar/visualizar o dado selecionado na tableview.
     *
     * @param index
     */
    void on_tableView_Dados_doubleClicked(const QModelIndex &index);

    /**
     * @brief tela_principal::on_pushButton_choice_edit_clicked
     *
     * Carrega a Dialog para editar o dado selecionado na tableview.
     */
    void on_pushButton_choice_edit_clicked();

    /**
     * @brief tela_principal::on_pushButton_choice_delete_clicked
     *
     * Deleta o dado selecionado na tableview (Se possível), ou retorna uma mensagem de erro.
     */
    void on_pushButton_choice_delete_clicked();

    /**
     * @brief tela_principal::on_actionAlterar_Senha_triggered
     *
     * Carrega a Dialog para editar a senha do usuário logado.
     */
    void on_actionAlterar_Senha_triggered();

    /**
     * @brief tela_principal::on_pushButton_choice_view_clicked
     *
     * Carrega a Dialog para visualizar detalhadamente o dado selecionado na tableview.
     */
    void on_pushButton_choice_view_clicked();

    /**
     * @brief tela_principal::on_actionAdicionar_no_estoque_triggered
     *
     * Carrega a Dialog para adicionar quantidades a veículos cadastrados.
     */
    void on_actionAdicionar_no_estoque_triggered();

private:
    Ui::tela_principal *ui;
};

#endif // TELA_PRINCIPAL_H
