#ifndef VEICULO_ADD_QUANT_H
#define VEICULO_ADD_QUANT_H

#include <QDialog>

namespace Ui {
class veiculo_add_quant;
}

class veiculo_add_quant : public QDialog
{
    Q_OBJECT
    int row_table_view;
    int row_affected;

public:
    /**
     * @brief veiculo_add_quant::veiculo_add_quant
     *
     * Construtor da classe veiculo_add_quant, verifica se o banco de dados foi aberto com sucesso,
     * retornando uma mensagem de erro caso contrário. Também carrega na comboBox os dados dos fabricantes cadastrados.
     *
     * @param parent
     */
    explicit veiculo_add_quant(QWidget *parent = 0);

    /**
     * @brief veiculo_add_quant::~veiculo_add_quant
     *
     * Destrutor da classe veiculo_add_quant.
     */
    ~veiculo_add_quant();

private slots:
    /**
     * @brief veiculo_add_quant::on_comboBox_fabricante_currentIndexChanged
     *
     * Carrega os modelos existentes no banco para o fabricante do veículo selecionado pelo usuário.
     *
     * @param arg1
     */
    void on_comboBox_fabricante_currentIndexChanged(const QString &arg1);

    /**
     * @brief veiculo_add_quant::on_comboBox_modelo_currentIndexChanged
     *
     * Carrega os anos de fabricação existentes no banco para o modelo do veículo selecionado pelo usuário.
     *
     * @param arg1
     */
    void on_comboBox_modelo_currentIndexChanged(const QString &arg1);

    /**
     * @brief veiculo_add_quant::on_comboBox_ano_currentIndexChanged
     *
     * Busca no banco o id do veículo e sua quantidade no estoque, baseado no fabricante, modelo e ano de fabricação selecionados.
     *
     * @param arg1
     */
    void on_comboBox_ano_currentIndexChanged(const QString &arg1);

    /**
     * @brief veiculo_add_quant::on_pushButton_add_clicked
     *
     * Verifica as informações passadas pelo usuário e, caso os dados estejam corretos,
     * atualiza a quantidade do veículo em estoque no banco de dados. Caso contrário retorna uma mensagem de erro.
     */
    void on_pushButton_add_clicked();

private:
    Ui::veiculo_add_quant *ui;
};

#endif // VEICULO_ADD_QUANT_H
