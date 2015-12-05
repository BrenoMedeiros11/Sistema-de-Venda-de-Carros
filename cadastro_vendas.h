#ifndef CADASTRO_VENDAS_H
#define CADASTRO_VENDAS_H

#include <QDialog>

namespace Ui {
class cadastro_vendas;
}

class cadastro_vendas : public QDialog
{
    Q_OBJECT
    int posicao_usuario_txt;
    int dividir_pagamento_parcelas; // 1 para dividir, 0 para não dividir
    int id_veiculo_selected;
    int preco_atual;

public:
    /**
     * @brief cadastro_vendas::cadastro_vendas
     *
     * Construtor da classe cadastro_vendas, verifica se o banco de dados foi aberto com sucesso,
     * retornando uma mensagem de erro caso contrário. Também adiciona os anos de fabricação para o ComboBox_prazo_parcela,
     * além de esconder o label/lineedit para numeração de cartão e relacionados a parcelas.
     *
     * @param parent
     */
    explicit cadastro_vendas(QWidget *parent = 0);

    /**
     * @brief cadastro_vendas::~cadastro_vendas
     *
     * Destrutor da classe cadastro_vendas
     */
    ~cadastro_vendas();

    /**
     * @brief cadastro_vendas::setPosicao
     *
     * Seta a variável interna "posição_usuario_txt", que corresponde a chave primária na tabela "vendedor" do banco de dados
     * para o usuário logado.
     *
     * @param _posicao
     */
    void setPosicao(int _posicao);

    /**
     * @brief cadastro_vendas::getPosicao
     *
     * Retorna o ID do usuário logado.
     *
     * @return (int) posicao_usuario_txt O ID do usuário logado.
     */
    int getPosicao();

    /**
     * @brief cadastro_vendas::alternative_constructor
     *
     * Função carregada antes mesmo da chamada do exec(), configura o nome do vendedor (usuário logado),
     * além  de configurar os comboBox: forma_pagamento, tipo_juros e porcent_juros.
     */
    void alternative_constructor();

    /**
     * @brief cadastro_vendas::modifica_quant_veiculo
     *
     * Altera em 1 a quantidade do veículo envolvido na venda em questão.
     * NOTA: Assuma que cada venda modifica em 1 a quantidade do veículo.
     *
     * @param id_veiculo
     * @return  (bool) True caso a quantidade do veículo tenha sido alterada com sucesso | False caso contrário
     */
    bool modifica_quant_veiculo(int id_veiculo);
private slots:
    /**
     * @brief cadastro_vendas::on_pushButton_cancel_clicked
     *
     * Cancela a venda.
     */
    void on_pushButton_cancel_clicked();

    /**
     * @brief cadastro_vendas::on_comboBox_veiculo_fabricante_currentIndexChanged
     *
     * Carrega os modelos existentes no banco para o fabricante do veículo selecionado pelo usuário.
     *
     * @param arg1
     */
    void on_comboBox_veiculo_fabricante_currentIndexChanged(const QString &arg1);

    /**
     * @brief cadastro_vendas::on_comboBox_veiculo_modelo_currentIndexChanged
     *
     * Carrega os anos de fabricação existentes no banco para o modelo do veículo selecionado pelo usuário.
     *
     * @param arg1
     */
    void on_comboBox_veiculo_modelo_currentIndexChanged(const QString &arg1);

    /**
     * @brief cadastro_vendas::on_comboBox_ano_fabricacao_currentIndexChanged
     *
     * Busca no banco o id do veículo e seu preço, baseado no fabricante, modelo e ano de fabricação selecionados.
     *
     * @param arg1
     */
    void on_comboBox_ano_fabricacao_currentIndexChanged(const QString &arg1);

    /**
     * @brief cadastro_vendas::on_comboBox_forma_pagamento_currentIndexChanged
     *
     * Mostra (ou não) as labels/lineEdit's para numeração do cartão e pagamento em parcelas.
     *
     * @param arg1
     */
    void on_comboBox_forma_pagamento_currentIndexChanged(const QString &arg1);

    /**
     * @brief cadastro_vendas::on_pushButton_calcula_parcelas_clicked
     *
     * Calcula o valor das parcelas (Caso o usuário tenha selecionado o pagamento por parcelas)
     */
    void on_pushButton_calcula_parcelas_clicked();

    /**
     * @brief cadastro_vendas::on_pushButton_save_clicked
     *
     * Verifica as informações passadas pelo usuário e, caso os dados estejam corretos,
     * salva a nova venda no banco de dados. Caso contrário retorna uma mensagem de erro.
     *
     * NOTA: Vendas cadastradas nas tabelas vendageral e vendacliente.
     */
    void on_pushButton_save_clicked();

private:
    Ui::cadastro_vendas *ui;
};

#endif // CADASTRO_VENDAS_H
