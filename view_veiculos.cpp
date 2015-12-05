#include "view_veiculos.h"
#include "ui_view_veiculos.h"
#include "login.h"
#include <QMessageBox>
#include <QSqlQuery>

view_veiculos::view_veiculos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::view_veiculos)
{
    ui->setupUi(this);

    login conexao;
    QMessageBox msg_sql_error;
    if( !conexao.conn_open() ) {
        msg_sql_error.setText("Erro de conexão com a base de dados - Erro #001 - view_veiculos.cpp");
        msg_sql_error.exec();
        return;
    }
}

view_veiculos::~view_veiculos()
{
    delete ui;
}

void view_veiculos::setRowTableView(int _row)
{
    row_table_view = _row;
}

void view_veiculos::alternative_constructor()
{
    QMessageBox msg_view_error; bool validate_image;
    QString placa, chassi, renavam, fabricante, modelo, ano_fabricacao, quant_estoque, preco;

    login conexao;
    if( !conexao.conn_open() ) {
        msg_view_error.setText("Erro de conexão com a base de dados - Erro #002 - view_veiculos.cpp");
        msg_view_error.exec();
        return;
    }

    conexao.conn_open();
    QSqlQuery qry;
    qry.prepare("select * from veiculo where idveiculo = ?");
    qry.addBindValue(row_table_view);

    if( qry.exec() ) {
        while(qry.next()) {
            placa = qry.value(1).toString();
            chassi = qry.value(2).toString();
            renavam = qry.value(3).toString();
            fabricante = qry.value(4).toString();
            modelo = qry.value(5).toString();
            ano_fabricacao = qry.value(6).toString();
            quant_estoque = qry.value(7).toString();
            preco = qry.value(8).toString();
        }
        conexao.conn_close();
    } else {
        msg_view_error.setText("Erro de conexão com a base de dados - Erro #003 - view_veiculos.cpp");
        msg_view_error.exec();
        return;
    }

    // Configura as labels do view de acordo com os dados recuperados do banco
    if(placa.isEmpty())
        ui->label_placa->setText("<h3><center><b>Placa = Não cadastrada.</b></center></h3>");
    else
        ui->label_placa->setText("<h3><center><b>Placa = "+placa+"</b></center></h3>");

    if(chassi.isEmpty())
        ui->label_chassi->setText("<h3><center><b>Chassi = Não cadastrado.</b></center></h3>");
    else
        ui->label_chassi->setText("<h3><center><b>Chassi = "+chassi+"</b></center></h3>");

    if(renavam.isEmpty())
        ui->label_renavam->setText("<h3><center><b>Renavam = Não cadastrado.</b></center></h3>");
    else
        ui->label_renavam->setText("<h3><center><b>Renavam = "+renavam+"</b></center></h3>");

    if(fabricante.isEmpty())
        ui->label_fabricante->setText("<h3><center><b>Fabricante = Não cadastrado.</b></center></h3>");
    else
        ui->label_fabricante->setText("<h3><center><b>Fabricante = "+fabricante+"</b></center></h3>");

    if(modelo.isEmpty())
        ui->label_modelo->setText("<h3><center><b>Modelo = Não cadastrado.</b></center></h3>");
    else
        ui->label_modelo->setText("<h3><center><b>Modelo = "+modelo+"</b></center></h3>");

    if(ano_fabricacao.isEmpty())
        ui->label_ano_fabricacao->setText("<h3><center><b>Ano de fabricação = Não cadastrado.</b></center></h3>");
    else
        ui->label_ano_fabricacao->setText("<h3><center><b>Ano de fabricação = "+ano_fabricacao+"</b></center></h3>");

        ui->label_quant_estoque->setText("<h3><center><b>Quantidade no estoque = "+quant_estoque+"</b></center></h3>");
        ui->label_preco->setText("<h3><center><b>Preço = R$"+preco+"</b></center></h3>");

    // Busca no sistema alguma imagem relacionada com o veículo (se tiver)
    image_object = new QImage();
    image_path = "../sistema_venda_carros/resources/car_images/car_image_"+QString::number(row_table_view)+".jpg";
    validate_image = image_object->load(image_path);

    if( validate_image ){
        image_pix = QPixmap::fromImage(*image_object);
    } else {

        image_object = new QImage();
        image_path = "../sistema_venda_carros/resources/car_images/car_image_"+QString::number(row_table_view)+".png";
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
            msg_view_error.setText("Erro ao abrir a imagem do veículo - Erro #004 - edittableviewveiculos.cpp");
            msg_view_error.exec();
            return;
            }
        }
    }

        image_scene = new QGraphicsScene(this);
        image_scene->addPixmap(image_pix);
        image_scene->setSceneRect(image_pix.rect());

        ui->graphicsView_image->setScene(image_scene);
}
