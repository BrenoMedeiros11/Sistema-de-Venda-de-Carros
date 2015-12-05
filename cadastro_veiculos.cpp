#include "cadastro_veiculos.h"
#include "ui_cadastro_veiculos.h"
#include "login.h"
#include <QMessageBox>
#include <QSize>
#include <QSqlQuery>

cadastro_veiculos::cadastro_veiculos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cadastro_veiculos)
{
    ui->setupUi(this);

    login conexao;
    QMessageBox msg_sql_error;
    if( !conexao.conn_open() ) {
        msg_sql_error.setText("Erro de conexão com a base de dados - Erro #001 - cadastro_veiculos.cpp");
        msg_sql_error.exec();
        return;
    }

    ui->comboBox_ano_fabricacao->addItem("Não Informado");

    for(int i=1950; i<=2050; i++){
        ui->comboBox_ano_fabricacao->addItem(QString::number(i));
    }

}

cadastro_veiculos::~cadastro_veiculos()
{
    delete ui;
}

void cadastro_veiculos::on_pushButton_open_picture_clicked()
{
    QMessageBox msg_size; QSize size;
    image_path = image_file.getOpenFileName(
                    this,
                    tr("Localizar foto de veículo"),
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

        ui->graphicsView_load_image->setScene(image_scene);
    }
}

void cadastro_veiculos::on_pushButton_Save_clicked()
{
    login conexao; bool save_image_result = false;
    QMessageBox msg_c_vei;
    QString placa = ui->lineEdit_placa->text();
    QString chassi = ui->lineEdit_chassi->text();
    QString renavam = ui->lineEdit_renavam->text();
    QString fabricante = ui->lineEdit_fabricante->text();
    QString modelo = ui->lineEdit_modelo->text();
    QString ano_fabricacao = ui->comboBox_ano_fabricacao->currentText();
    QString preco_string = ui->lineEdit_preco->text();
    QString last_id_row_string;

    bool preco_isNum; float preco = preco_string.toFloat(&preco_isNum);

    if( placa.isEmpty() ) {
        msg_c_vei.setText("Informe a placa do veículo.");
        msg_c_vei.exec();
        return;
    }

    if( fabricante.isEmpty() ) {
        msg_c_vei.setText("Informe o fabricante do veículo.");
        msg_c_vei.exec();
        return;
    }

    if( modelo.isEmpty() ) {
        msg_c_vei.setText("Informe o modelo do veículo.");
        msg_c_vei.exec();
        return;
    }

    if( ano_fabricacao == "Não Informado" ) {
        msg_c_vei.setText("Ano de fabricação inválido.");
        msg_c_vei.exec();
        return;
    }

    if( preco_string.isEmpty() || !preco_isNum ) {
        msg_c_vei.setText("Preço do veículo não foi informado ou está inválido.");
        msg_c_vei.exec();
        return;
    }

    if( !conexao.conn_open() ) {
        msg_c_vei.setText("Erro de conexão com a base de dados - Erro #002 - cadastro_veiculos.cpp");
        msg_c_vei.exec();
        return;
    }
    conexao.conn_open();
    QSqlQuery qry;

    // Verifica se existe algum veículo no sistema com o mesmo fabricante, modelo ou ano de fabricação
    qry.prepare("select * from veiculo where fabricante = ? and modelo = ? and ano_fabricacao = ?");
    qry.addBindValue(fabricante); qry.addBindValue(modelo); qry.addBindValue(ano_fabricacao);
    int num_rows_1 = 0;

    if( qry.exec() ) {
        while( qry.next() ){
            num_rows_1++;
        }

        if( num_rows_1 > 0 ) {
            msg_c_vei.setText("O veículo com as características informadas já existe no sistema.");
            msg_c_vei.exec();
            return;
        }
    } else {
        msg_c_vei.setText("Erro de conexão com a base de dados - Erro #003 - cadastro_veículos.cpp");
        msg_c_vei.exec();
        return;
    }

    // Verifica se existe algum veículo no sistema com o mesmo fabricante, modelo ou ano de fabricação
    qry.prepare("select * from veiculo where placa = ? or chassi = ? or renavam = ?");
    qry.addBindValue(placa); qry.addBindValue(chassi); qry.addBindValue(renavam);
    int num_rows_2 = 0;

    if( qry.exec() ) {
        while( qry.next() ){
            num_rows_2++;
        }

        if( num_rows_2 > 0 ) {
            msg_c_vei.setText("O veículo com as características informadas já existe no sistema.");
            msg_c_vei.exec();
            return;
        }
    } else {
        msg_c_vei.setText("Erro de conexão com a base de dados - Erro #004 - cadastro_veículos.cpp");
        msg_c_vei.exec();
        return;
    }

    qry.prepare("insert into veiculo (placa,chassi,renavam,fabricante,modelo,ano_fabricacao,quant_estoque,preco_estimado) values (?,?,?,?,?,?,?,?)");
    qry.addBindValue(placa); qry.addBindValue(chassi);
    qry.addBindValue(renavam); qry.addBindValue(fabricante);
    qry.addBindValue(modelo); qry.addBindValue(ano_fabricacao);
    qry.addBindValue(0); qry.addBindValue(preco);

    if( qry.exec() ) {

         // Recupera o id da query executada
         last_id_row_string = qry.lastInsertId().toString();

         // Salva a imagem selecionada, se teve alguma imagem selecionada
         if( !image_object->isNull() ) {
            *image_object = image_pix.toImage();
             save_image_result =  image_object->save("../sistema_venda_carros/resources/car_images/car_image_"+last_id_row_string+".png");

             if( !image_object->isNull() && save_image_result ) {
                msg_c_vei.setText("Veículo inserido com sucesso.");
                msg_c_vei.exec();
                conexao.conn_close();
                this->hide();
             } else {

                 msg_c_vei.setText("Erro ao salvar o arquivo - Erro #005 - cadastro_veículos.cpp");
                 msg_c_vei.exec();

                 // Deleta a inserção feita no banco sobre o veiculo
                 qry.prepare("delete from veiculo where idveiculo = ?"); qry.addBindValue(last_id_row_string);
                 qry.exec();

                 return;
             }
         } else {
             msg_c_vei.setText("Veículo inserido com sucesso.");
             msg_c_vei.exec();
             conexao.conn_close();
             this->hide();
         }
    } else {
         msg_c_vei.setText("Erro de conexão com a base de dados - Erro #006 - cadastro_veículos.cpp");
         msg_c_vei.exec();
         return;
    }
}
