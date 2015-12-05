#ifndef CADASTRO_VEICULOS_H
#define CADASTRO_VEICULOS_H

#include <QDialog>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <QString>

namespace Ui {
class cadastro_veiculos;
}

class cadastro_veiculos : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief cadastro_veiculos::cadastro_veiculos
     *
     * Construtor da classe cadastro_veiculos, verifica se o banco de dados foi aberto com sucesso,
     * retornando uma mensagem de erro caso contrário. Também adiciona os anos de fabricação para o ComboBox.
     *
     * @param parent
     */
    explicit cadastro_veiculos(QWidget *parent = 0);

    /**
     * @brief cadastro_veiculos::~cadastro_veiculos
     *
     * Destrutor da classe cadastro_veiculos
     */
    ~cadastro_veiculos();

private slots:
    /**
     * @brief cadastro_veiculos::on_pushButton_open_picture_clicked
     *
     * Carrega a imagem do veículo a ser cadastrado, selecionada pelo usuário,
     * verificando se ela atende os requisitos mínimos:
     *
     * <ul>
     *      <li>* Tamanho máximo = 256x256;</li>
     *      <li>* Formatos = JPEG/JPG ou PNG</li>
     * </ul>
     */
    void on_pushButton_open_picture_clicked();

    /**
     * @brief cadastro_veiculos::on_pushButton_Save_clicked
     *
     * Verifica as informações passadas pelo usuário e, caso os dados estejam corretos,
     * salva o novo veículo no banco de dados. Caso contrário retorna uma mensagem de erro.
     *
     * NOTA: Chassi, renavam e a imagem do veículo são optativos.
     */
    void on_pushButton_Save_clicked();

private:
    Ui::cadastro_veiculos *ui;
    QFileDialog image_file;
    QGraphicsScene *image_scene;
    QImage *image_object;
    QPixmap image_pix;
    QString image_path;
};

#endif // CADASTRO_VEICULOS_H
