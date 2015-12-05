#ifndef EDITTABLEVIEWVEICULOS_H
#define EDITTABLEVIEWVEICULOS_H

#include <QDialog>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <QString>

namespace Ui {
class EditTableViewVeiculos;
}

class EditTableViewVeiculos : public QDialog
{
    Q_OBJECT
    int row_table_view;
    QString row_table_view_string;

public:
    /**
     * @brief EditTableViewVeiculos::EditTableViewVeiculos
     *
     * Construtor da classe EditTableViewVeiculos, verifica se o banco de dados foi aberto com sucesso,
     * retornando uma mensagem de erro caso contrário.
     *
     * @param parent
     */
    explicit EditTableViewVeiculos(QWidget *parent = 0);

    /**
     * @brief EditTableViewVeiculos::~EditTableViewVeiculos
     *
     * Destrutor da classe EditTableViewVeiculos.
     */
    ~EditTableViewVeiculos();

    /**
     * @brief EditTableViewVeiculos::setRowTableView
     *
     * Salva o ID do veículo a ser editado.
     *
     * @param _row
     */
    void setRowTableView(int _row);

    /**
     * @brief EditTableViewVeiculos::alternative_constructor
     *
     * Função carregada antes mesmo da chamada do exec(), carrega as informações do veículo a ser editado do banco,
     * assim como a imagem selecionada para ele (Caso não haja uma imagem, mostra a imagem default).
     */
    void alternative_constructor();
private slots:
    /**
     * @brief EditTableViewVeiculos::on_pushButton_new_image_clicked
     *
     * Carrega uma nova imagem para o veículo que está sendo editado.
     */
    void on_pushButton_new_image_clicked();

    /**
     * @brief EditTableViewVeiculos::on_pushButton_edit_clicked
     *
     * Verifica as informações passadas pelo usuário e, caso os dados estejam corretos,
     * atualiza os dados do veículo no banco de dados. Caso contrário retorna uma mensagem de erro.
     */
    void on_pushButton_edit_clicked();

private:
    Ui::EditTableViewVeiculos *ui;
    QFileDialog image_file;
    QGraphicsScene *image_scene;
    QImage *image_object;
    QPixmap image_pix;
    QString image_path;
};

#endif // EDITTABLEVIEWVEICULOS_H
