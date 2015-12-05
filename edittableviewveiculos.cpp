#include "edittableviewveiculos.h"
#include "ui_edittableviewveiculos.h"
#include "login.h"
#include "QFile"
#include "QMessageBox"
#include <QSize>
#include "QSqlQuery"
#include "QTextStream"

EditTableViewVeiculos::EditTableViewVeiculos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTableViewVeiculos)
{
    ui->setupUi(this);

    login conexao;
    QMessageBox msg_error;
    if( !conexao.conn_open() ) {
        msg_error.setText("Erro de conexão com a base de dados - Erro #001 - edittableviewveiculos.cpp");
        msg_error.exec();
        return;
    }
}

EditTableViewVeiculos::~EditTableViewVeiculos()
{
    delete ui;
}

void EditTableViewVeiculos::setRowTableView(int _row)
{
    row_table_view = _row;
}

void EditTableViewVeiculos::alternative_constructor()
{
    QMessageBox msg_edit_error; bool validate_image;
    QString idveiculo, chassi, renavam, fabricante, modelo, ano_fabricacao, preco;
    int ano_fabricacao_localizacao, ano_fabricacao_int;

    login conexao;
    if( !conexao.conn_open() ) {
        msg_edit_error.setText("Erro de conexão com a base de dados - Erro #002 - edittableviewveiculos.cpp");
        msg_edit_error.exec();
        return;
    }

    conexao.conn_open();
    QSqlQuery qry;
    qry.prepare("select * from veiculo where idveiculo = ?");
    qry.addBindValue(row_table_view);

    if( qry.exec() ) {
        while(qry.next()) {
            idveiculo = qry.value(0).toString();
            row_table_view_string = idveiculo;
            chassi = qry.value(2).toString();
            renavam = qry.value(3).toString();
            fabricante = qry.value(4).toString();
            modelo = qry.value(5).toString();
            ano_fabricacao = qry.value(6).toString();
            preco = qry.value(8).toString();

            ano_fabricacao_int = ano_fabricacao.toInt();
        }
        conexao.conn_close();
    } else {
        msg_edit_error.setText("Erro de conexão com a base de dados - Erro #003 - edittableviewveiculos.cpp");
        msg_edit_error.exec();
        return;
    }

    ui->label_fabricante->setText("<h3><center><b>Fabricante = "+fabricante+"</b></center></h3>");
    ui->label_modelo->setText("<h3><center><b>Modelo = "+modelo+"</b></center></h3>");
    ui->label_preco->setText("<h3><center><b>Preço = R$"+preco+"</b></center></h3>");
    ui->lineEdit_chassi->setText(chassi);
    ui->lineEdit_renavam->setText(renavam);

    int flag_default_item = 0;
    int i_antes = 1950; int i_depois = 2050;
    ui->comboBox_ano_fabricacao->addItem("Não Informado");

    while(i_antes <= i_depois) {

        int tempo_de_diferenca = 101 - (2050 - i_antes);

        ui->comboBox_ano_fabricacao->addItem(QString::number(i_antes));

        if( i_antes == ano_fabricacao_int ) {
            ano_fabricacao_localizacao = tempo_de_diferenca;
            flag_default_item = 1;
        }

        i_antes++;
    }

    if( flag_default_item == 1 ) {
        ui->comboBox_ano_fabricacao->setCurrentIndex(ano_fabricacao_localizacao);
    }

    // Busca no sistema alguma imagem relacionada com o veículo (se tiver)
    image_object = new QImage();
    image_path = "../sistema_venda_carros/resources/car_images/car_image_"+idveiculo+".jpg";
    validate_image = image_object->load(image_path);

    if( validate_image ){
        image_pix = QPixmap::fromImage(*image_object);
    } else {

        image_object = new QImage();
        image_path = "../sistema_venda_carros/resources/car_images/car_image_"+idveiculo+".png";
        validate_image = image_object->load(image_path);

        if( validate_image ) {
            image_pix = QPixmap::fromImage(*image_object);
        } else {

            image_object = new QImage();
            image_path = "../sistema_venda_carros/resources/car_images/no_image_selected.png";
            validate_image = image_object->load(image_path);

            if ( validate_image ) {
                image_pix = QPixmap::fromImage(*image_object);
            } else {
                msg_edit_error.setText("Erro ao abrir a imagem do veículo - Erro #004 - edittableviewveiculos.cpp");
                msg_edit_error.exec();
                return;
            }
        }
    }

    image_scene = new QGraphicsScene(this);
    image_scene->addPixmap(image_pix);
    image_scene->setSceneRect(image_pix.rect());

    ui->graphicsView_image->setScene(image_scene);
}

void EditTableViewVeiculos::on_pushButton_new_image_clicked()
{
    QMessageBox msg_size; QSize size;
    image_path = image_file.getOpenFileName(
                    this,
                    tr("Localizar foto do veículo"),
                    "",
                    tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
                );

    image_object = new QImage();

    if( !image_path.isEmpty() ) {

        image_object->load(image_path);

        image_pix = QPixmap::fromImage(*image_object);

        size = image_pix.size();
        int size_1 = size.height(); // Altura
        int size_2 = size.width(); // Largura

        if( size_1 > 256 || size_2 > 256 ) {
            msg_size.setText("A imagem selecionada é maior do que o tamanho máximo permitido. Escolha outra.");
            msg_size.exec();

            image_path.clear(); image_object = new QImage();
            return;
        }

        image_scene = new QGraphicsScene(this);
        image_scene->addPixmap(image_pix);
        image_scene->setSceneRect(image_pix.rect());

        ui->graphicsView_image->setScene(image_scene);
        //ui->graphicsView_image->resize(image_pix.size());
    }
}

void EditTableViewVeiculos::on_pushButton_edit_clicked()
{
    login conexao; bool validate_image, save_image_result;
    QMessageBox msg_edit_error;
    QString chassi, renavam, ano_fabricacao;
    chassi = ui->lineEdit_chassi->text(); renavam = ui->lineEdit_renavam->text();
    ano_fabricacao = ui->comboBox_ano_fabricacao->currentText();

    if(ano_fabricacao == "Não Informado") {
        msg_edit_error.setText("Informe o ano de fabricação do veículo");
        msg_edit_error.exec();
        return;
    }

    if( !conexao.conn_open() ) {
        msg_edit_error.setText("Erro de conexão com a base de dados - Erro #005 - edittableviewveiculos.cpp");
        msg_edit_error.exec();
        return;
    }

    conexao.conn_open();
    QSqlQuery qry;

    qry.prepare("update veiculo set chassi = ?, renavam = ?, ano_fabricacao = ? where idveiculo = ?");
    qry.addBindValue(chassi);
    qry.addBindValue(renavam);
    qry.addBindValue(ano_fabricacao);
    qry.addBindValue(row_table_view);

    if( qry.exec() ) {

        if( !image_object->isNull() ) {
            // Verifica se existe alguma imagem setada anteriormente
            image_object = new QImage();
            image_path = "../sistema_venda_carros/resources/car_images/car_image_"+row_table_view_string+".jpg";
            validate_image = image_object->load(image_path);

            // DESCOBRIR COMO APAGAR ARQUIVOS DO SISTEMA!
            if( validate_image ){
                QFile image_file;
                image_file.setFileName(image_path);
                image_file.remove();
            } else {

                image_object = new QImage();
                image_path = "../sistema_venda_carros/resources/car_images/car_image_"+row_table_view_string+".png";
                validate_image = image_object->load(image_path);

                if( validate_image ) {
                    QFile image_file;
                    image_file.setFileName(image_path);
                    image_file.remove();
                }
            }

            // Salva a nova imagem
            *image_object = image_pix.toImage();
            save_image_result =  image_object->save("../sistema_venda_carros/resources/car_images/car_image_"+row_table_view_string+".png");

            if( !image_object->isNull() && save_image_result ) {
                msg_edit_error.setText("Veículo atualizado com sucesso.");
                msg_edit_error.exec();
                conexao.conn_close();
                this->hide();
            } else {
                msg_edit_error.setText("Erro ao salvar o arquivo - Erro #006 - edittableviewveiculos.cpp");
                msg_edit_error.exec();
                return;
            }
        } else {
            msg_edit_error.setText("Veículo atualizado com sucesso.");
            msg_edit_error.exec();
            conexao.conn_close();
            this->hide();
        }
    } else {
        msg_edit_error.setText("Erro de conexão com a base de dados - Erro #007 - edittableviewveiculos.cpp");
        msg_edit_error.exec();
        return;
    }
}
