#ifndef VIEW_VEICULOS_H
#define VIEW_VEICULOS_H

#include <QDialog>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <QString>

namespace Ui {
class view_veiculos;
}

class view_veiculos : public QDialog
{
    Q_OBJECT
    int row_table_view;

public:
    /**
     * @brief view_veiculos::view_veiculos
     *
     * Construtor da classe view_veiculos, verifica se o banco de dados foi aberto com sucesso,
     * retornando uma mensagem de erro caso contrário.
     *
     * @param parent
     */
    explicit view_veiculos(QWidget *parent = 0);

    /**
     * @brief view_veiculos::~view_veiculos
     *
     * Destrutor da classe view_veiculos.
     */
    ~view_veiculos();

    /**
     * @brief view_veiculos::setRowTableView
     *
     * Salva o ID do veículo a ser editado.
     *
     * @param _row
     */
    void setRowTableView(int _row);

    /**
     * @brief view_veiculos::alternative_constructor
     *
     * Função carregada antes mesmo da chamada do exec(), carrega as informações do veículo a ser visualizado do banco,
     * assim como a imagem selecionada para ele (Caso não haja uma imagem, mostra a imagem default).
     */
    void alternative_constructor();
private:
    Ui::view_veiculos *ui;
    QFileDialog image_file;
    QGraphicsScene *image_scene;
    QImage *image_object;
    QPixmap image_pix;
    QString image_path;
};

#endif // VIEW_VEICULOS_H
