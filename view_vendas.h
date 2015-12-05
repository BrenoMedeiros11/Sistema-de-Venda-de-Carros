#ifndef VIEW_VENDAS_H
#define VIEW_VENDAS_H

#include <QDialog>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <QString>

namespace Ui {
class view_vendas;
}

class view_vendas : public QDialog
{
    Q_OBJECT
    int row_table_view;

public:
    /**
     * @brief view_vendas::view_vendas
     * @param parent
     *
     * Construtor da classe view_vendas, verifica se o banco de dados foi aberto com sucesso,
     * retornando uma mensagem de erro caso contrário.
     */
    explicit view_vendas(QWidget *parent = 0);

    /**
     * @brief view_vendas::~view_vendas
     *
     * Destrutor da classe view_vendas.
     */
    ~view_vendas();

    /**
     * @brief view_vendas::setRowTableView
     *
     * Salva o ID da venda a ser visualizada.
     *
     * @param _row
     */
    void setRowTableView(int _row);

    /**
     * @brief view_vendas::alternative_constructor
     *
     * Função carregada antes mesmo da chamada do exec(), carrega as informações da venda a ser visualizada do banco.
     */
    void alternative_constructor();
private:
    Ui::view_vendas *ui;
    QFileDialog image_file;
    QGraphicsScene *image_scene;
    QImage *image_object;
    QPixmap image_pix;
    QString image_path;
};

#endif // VIEW_VENDAS_H
