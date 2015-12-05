#include "cadastro_vendas.h"
#include "ui_cadastro_vendas.h"
#include "login.h"
#include <QDateEdit>
#include <QDateTime>
#include <QLineEdit>
#include <QMessageBox>
#include <QtMultimedia/QMediaContent>
#include <QtMultimedia/QMediaPlayer>
#include <QSqlQuery>
#include <QString>
#include <QUrl>

cadastro_vendas::cadastro_vendas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cadastro_vendas)
{
    ui->setupUi(this);

    login conexao;
    QMessageBox msg_sql_error;
    if( !conexao.conn_open() ) {
        msg_sql_error.setText("Erro de conexão com a base de dados - Erro #001 - cadastro_vendas.cpp");
        msg_sql_error.exec();
        return;
    }

    // Configura as opções para o prazo da primeira parcela do pagamento
    ui->comboBox_prazo_parcela->addItem("Selecione um prazo"); ui->comboBox_prazo_parcela->addItem("Próximo mês");
    ui->comboBox_prazo_parcela->addItem("2"); ui->comboBox_prazo_parcela->addItem("3");
    ui->comboBox_prazo_parcela->addItem("6");
    ui->comboBox_prazo_parcela->setCurrentIndex(0);

    // Esconde a opção de divisão do pagamento por parcelas, para que o cliente possa optar(ou não) por ela.
    // NOTA: O cliente pode comprar o carro a vista ou direto no débito ou em cheque.
    ui->label_num_parcelas->hide(); ui->lineEdit_num_parcelas->hide();
    ui->label_valor_parcela->hide(); ui->lineEdit_valor_parcela->hide();
    ui->pushButton_calcula_parcelas->hide();
    ui->label_prazo_parcela->hide(); ui->comboBox_prazo_parcela->hide();

    // Por default, o usuário não irá dividir o pagamento do veículo em parcelas
    dividir_pagamento_parcelas = 0;

    // Esconde a label para numeracao do cartao (credito ou debito)
    ui->label_num_cartao->hide(); ui->lineEdit_num_cartao->hide();
}

cadastro_vendas::~cadastro_vendas()
{
    delete ui;
}

void cadastro_vendas::setPosicao(int _posicao)
{
    posicao_usuario_txt = _posicao;
}

int cadastro_vendas::getPosicao()
{
    return posicao_usuario_txt;
}

void cadastro_vendas::alternative_constructor(){

    QMessageBox msg_venda_error;

    login conexao;
    if( !conexao.conn_open() ) {
        msg_venda_error.setText("Erro de conexão com a base de dados - Erro #002 - cadastro_vendas.cpp");
        msg_venda_error.exec();
        return;
    }

    conexao.conn_open();
    QSqlQuery qry;
    qry.prepare("select nome from vendedor where idvendedor = ?");
    qry.addBindValue(posicao_usuario_txt);

    if( qry.exec() ) {
        while(qry.next()) {
            ui->label_vendedor->setText("<h3><center><b>Nome do vendedor: "+qry.value(0).toString()+"</b></center></h3>");
        }
    } else {
        msg_venda_error.setText("Erro de conexão com a base de dados - Erro #003 - cadastro_vendas.cpp");
        msg_venda_error.exec();
        return;
    }

    // Configura as formas de pagamento disponiveis
    ui->comboBox_forma_pagamento->addItem("Selecione abaixo");
    ui->comboBox_forma_pagamento->addItem("A vista"); ui->comboBox_forma_pagamento->addItem("Crédito");
    ui->comboBox_forma_pagamento->addItem("Débito"); ui->comboBox_forma_pagamento->addItem("Cheque");
    ui->comboBox_forma_pagamento->setCurrentIndex(0);

    // Configura os tipos de juros disponiveis
    ui->comboBox_tipo_juros->addItem("Inexistente");
    ui->comboBox_tipo_juros->addItem("Mensal"); ui->comboBox_tipo_juros->addItem("Anual");
    ui->comboBox_porcent_juros->setCurrentIndex(0);

    // Configura os valores (%) de juros
    ui->comboBox_porcent_juros->addItem("0"); ui->comboBox_porcent_juros->addItem("0.5");
    ui->comboBox_porcent_juros->addItem("1.0"); ui->comboBox_porcent_juros->addItem("1.5");
    ui->comboBox_porcent_juros->setCurrentIndex(0);

    // Busca no banco os dados dos fabricantes cadastrados
    // NOTA: Existe uma verificação em tela_principal.cpp, portanto assuma aqui que EXISTE veiculos cadastrados.
    qry.prepare("select fabricante from veiculo where quant_estoque > ?"); qry.addBindValue(0);

    if( qry.exec() ) {

        ui->comboBox_veiculo_fabricante->addItem("Selecione um fabricante");

        while(qry.next()) {
            ui->comboBox_veiculo_fabricante->addItem(qry.value(0).toString());
        }
        conexao.conn_close();

        ui->comboBox_veiculo_fabricante->setCurrentIndex(0);

    } else {
        msg_venda_error.setText("Erro de conexão com a base de dados - Erro #004 - cadastro_vendas.cpp");
        msg_venda_error.exec();
        return;
    }
}

bool cadastro_vendas::modifica_quant_veiculo(int id_veiculo)
{
    QMessageBox msg_sql_error;
    int quant_atual_estoque, quant_nova_estoque;

    login conexao;
    if( !conexao.conn_open() ) {
        msg_sql_error.setText("Erro de conexão com a base de dados - Erro #005 - cadastro_vendas.cpp");
        msg_sql_error.exec();
        return false;
    }

    conexao.conn_open();
    QSqlQuery qry;

    qry.prepare("select quant_estoque from veiculo where idveiculo = ?"); qry.addBindValue(id_veiculo);
    if( qry.exec() ){
        while( qry.next() ) {
            quant_atual_estoque = qry.value(0).toInt();
        }
    } else {
        msg_sql_error.setText("Erro de conexão com a base de dados - Erro #006 - cadastro_vendas.cpp");
        msg_sql_error.exec();
        return false;
    }

    quant_nova_estoque = quant_atual_estoque - 1;

    qry.prepare("update veiculo set quant_estoque = ? where idveiculo = ?");
    qry.addBindValue(quant_nova_estoque); qry.addBindValue(id_veiculo);

    if( qry.exec() ) {
        return true;
    } else {
        msg_sql_error.setText("Erro de conexão com a base de dados - Erro #007 - cadastro_vendas.cpp");
        msg_sql_error.exec();
        return false;
    }
}

void cadastro_vendas::on_pushButton_cancel_clicked()
{
    this->close();
}

void cadastro_vendas::on_comboBox_veiculo_fabricante_currentIndexChanged(const QString &arg1)
{
    ui->comboBox_veiculo_modelo->clear(); ui->comboBox_ano_fabricacao->clear(); ui->lineEdit_preco_final->clear();

    if( arg1 != "Selecione um fabricante" ) {

        QMessageBox msg_venda_error;

        login conexao;
        if( !conexao.conn_open() ) {
            msg_venda_error.setText("Erro de conexão com a base de dados - Erro #008 - cadastro_vendas.cpp");
            msg_venda_error.exec();
            return;
        }

        conexao.conn_open();
        QSqlQuery qry;
        qry.prepare("select modelo from veiculo where fabricante = ?");
        qry.addBindValue(arg1);

        if( qry.exec() ) {

            ui->comboBox_veiculo_modelo->addItem("Selecione um modelo");

            while(qry.next()) {
                ui->comboBox_veiculo_modelo->addItem(qry.value(0).toString());
            }
            conexao.conn_close();

            ui->comboBox_veiculo_modelo->setCurrentIndex(0);

        } else {
            msg_venda_error.setText("Erro de conexão com a base de dados - Erro #009 - cadastro_vendas.cpp");
            msg_venda_error.exec();
            return;
        }
    }
}

void cadastro_vendas::on_comboBox_veiculo_modelo_currentIndexChanged(const QString &arg1)
{
    ui->comboBox_ano_fabricacao->clear(); ui->lineEdit_preco_final->clear();

    if( arg1 != "Selecione um modelo" ) {

        QMessageBox msg_venda_error;

        login conexao;
        if( !conexao.conn_open() ) {
            msg_venda_error.setText("Erro de conexão com a base de dados - Erro #010 - cadastro_vendas.cpp");
            msg_venda_error.exec();
            return;
        }

        conexao.conn_open();
        QSqlQuery qry;
        qry.prepare("select ano_fabricacao from veiculo where modelo = ?");
        qry.addBindValue(arg1);

        if( qry.exec() ) {

            ui->comboBox_ano_fabricacao->addItem("Selecione um ano");

            while(qry.next()) {
                ui->comboBox_ano_fabricacao->addItem(qry.value(0).toString());
            }
            conexao.conn_close();

            ui->comboBox_ano_fabricacao->setCurrentIndex(0);

        } else {
            msg_venda_error.setText("Erro de conexão com a base de dados - Erro #011 - cadastro_vendas.cpp");
            msg_venda_error.exec();
            return;
        }
    }
}

void cadastro_vendas::on_comboBox_ano_fabricacao_currentIndexChanged(const QString &arg1)
{
    ui->lineEdit_preco_final->clear();

    if( arg1 != "Selecione um ano" ) {

        QMessageBox msg_venda_error;
        QString fabricante, modelo, ano_fabricacao, preco_estimado;
        fabricante = ui->comboBox_veiculo_fabricante->currentText();
        modelo = ui->comboBox_veiculo_modelo->currentText();
        ano_fabricacao = ui->comboBox_ano_fabricacao->currentText();

        login conexao;
        if( !conexao.conn_open() ) {
            msg_venda_error.setText("Erro de conexão com a base de dados - Erro #012 - cadastro_vendas.cpp");
            msg_venda_error.exec();
            return;
        }

        conexao.conn_open();
        QSqlQuery qry;
        qry.prepare("select idveiculo, preco_estimado from veiculo where fabricante = ? and modelo = ? and ano_fabricacao = ?");
        qry.addBindValue(fabricante); qry.addBindValue(modelo); qry.addBindValue(ano_fabricacao);

        if( qry.exec() ) {
            while(qry.next()) {
                id_veiculo_selected = qry.value(0).toInt();
                preco_estimado = qry.value(1).toString();
                preco_atual = preco_estimado.toInt();
            }

            ui->lineEdit_preco_final->setText(preco_estimado);
            conexao.conn_close();

        } else {
            msg_venda_error.setText("Erro de conexão com a base de dados - Erro #013 - cadastro_vendas.cpp");
            msg_venda_error.exec();
            return;
        }
    }
}

void cadastro_vendas::on_comboBox_forma_pagamento_currentIndexChanged(const QString &arg1)
{
    if( arg1 == "Crédito" || arg1 == "Débito" ) {
        ui->label_num_cartao->show(); ui->lineEdit_num_cartao->show();
    } else {
        ui->label_num_cartao->hide(); ui->lineEdit_num_cartao->hide(); ui->lineEdit_num_cartao->clear();
    }

    if( arg1 == "Crédito" ) {

        ui->label_num_parcelas->show(); ui->lineEdit_num_parcelas->show();
        ui->label_valor_parcela->show(); ui->lineEdit_valor_parcela->show();
        ui->pushButton_calcula_parcelas->show();
        ui->label_prazo_parcela->show(); ui->comboBox_prazo_parcela->show();

        dividir_pagamento_parcelas = 1;

    } else {

        ui->label_num_parcelas->hide(); ui->lineEdit_num_parcelas->hide();
        ui->label_valor_parcela->hide(); ui->lineEdit_valor_parcela->hide();
        ui->pushButton_calcula_parcelas->hide();
        ui->label_prazo_parcela->hide(); ui->comboBox_prazo_parcela->hide();

        ui->lineEdit_num_parcelas->clear(); ui->lineEdit_valor_parcela->clear();
        ui->comboBox_prazo_parcela->setCurrentIndex(0);

        dividir_pagamento_parcelas = 0;

    }
}

void cadastro_vendas::on_pushButton_calcula_parcelas_clicked()
{
    QMessageBox msg_venda_error;
    QString preco_final_string, quant_parcela_string;
    preco_final_string = ui->lineEdit_preco_final->text();
    quant_parcela_string = ui->lineEdit_num_parcelas->text();

    bool preco_isNum; float preco_final = preco_final_string.toFloat(&preco_isNum);
    bool q_parc_isNum; int quant_parcela = quant_parcela_string.toInt(&q_parc_isNum);
    float valor_parcelas;

    if( preco_final_string.isEmpty() || !preco_isNum ) {
        msg_venda_error.setText("O preço final do veículo não foi preenchido ou está incorreto.");
        msg_venda_error.exec();
        return;
    }

    if( quant_parcela_string.isEmpty() || !q_parc_isNum ) {
        msg_venda_error.setText("A quantidade de parcelas não foi preenchida ou está incorreta.");
        msg_venda_error.exec();
        return;
    }

    valor_parcelas = preco_final/quant_parcela;

    ui->lineEdit_valor_parcela->setText(QString::number(valor_parcelas, 'f', 2));
}

void cadastro_vendas::on_pushButton_save_clicked()
{
    login conexao;
    QMessageBox msg_venda_error;
    QString cliente_nome, cliente_endereco, cliente_cpf_string, cliente_rg_string, cliente_idade_string, cliente_telefone_string,
            fabricante, modelo, ano_fabricacao, garantia_string, preco_final_string, porcent_juros, tipo_juros, forma_pagamento,
            quant_parcela_string, prazo_primeira_parcela_string, num_cartao_string;
    double cliente_cpf, cliente_rg, cliente_telefone;
    int cliente_idade, garantia, quant_parcela, prazo_primeira_parcela, num_cartao,
            last_insert_row;
    float preco_final;

    if( !conexao.conn_open() ) {
        msg_venda_error.setText("Erro de conexão com a base de dados - Erro #014 - cadastro_vendas.cpp");
        msg_venda_error.exec();
        return;
    }

    conexao.conn_open();
    QSqlQuery qry;

    cliente_nome = ui->lineEdit_cliente_nome->text(); cliente_endereco = ui->lineEdit_cliente_endereco->text();
    cliente_cpf_string = ui->lineEdit_cliente_cpf->text(); cliente_rg_string = ui->lineEdit_cliente_rg->text();
    cliente_idade_string = ui->lineEdit_cliente_idade->text(); cliente_telefone_string = ui->lineEdit_cliente_telefone->text();
    fabricante = ui->comboBox_veiculo_fabricante->currentText();
    modelo = ui->comboBox_veiculo_modelo->currentText();
    ano_fabricacao = ui->comboBox_ano_fabricacao->currentText();
    garantia_string = ui->lineEdit_veiculo_garantia->text();
    preco_final_string = ui->lineEdit_preco_final->text();
    forma_pagamento = ui->comboBox_forma_pagamento->currentText();
    porcent_juros = ui->comboBox_porcent_juros->currentText();
    tipo_juros = ui->comboBox_tipo_juros->currentText();
    quant_parcela_string = ui->lineEdit_num_parcelas->text();
    prazo_primeira_parcela_string = ui->comboBox_prazo_parcela->currentText();
    num_cartao_string = ui->lineEdit_num_cartao->text();

    bool cpf_isNum; cliente_cpf = cliente_cpf_string.toDouble(&cpf_isNum);
    bool rg_isNum; cliente_rg = cliente_rg_string.toDouble(&rg_isNum);
    bool idade_isNum; cliente_idade = cliente_idade_string.toInt(&idade_isNum);
    bool telefone_isNum; cliente_telefone = cliente_telefone_string.toDouble(&telefone_isNum);
    bool garantia_isNum; garantia = garantia_string.toInt(&garantia_isNum);
    bool preco_isNum; preco_final = preco_final_string.toFloat(&preco_isNum);
    bool q_parc_isNum; quant_parcela = quant_parcela_string.toInt(&q_parc_isNum);
    bool num_cartao_isNum; num_cartao = num_cartao_string.toInt(&num_cartao_isNum);

    if( cliente_nome.isEmpty() ) {
        msg_venda_error.setText("Preencha o nome do cliente.");
        msg_venda_error.exec();
        return;
    }

    if( cliente_endereco.isEmpty() ) {
        msg_venda_error.setText("Preencha o endereço do cliente.");
        msg_venda_error.exec();
        return;
    }

    if( cliente_cpf_string.isEmpty() || !cpf_isNum ) {
        msg_venda_error.setText("Preencha o CPF do cliente.");
        msg_venda_error.exec();
        return;
    }

    if( cliente_rg_string.isEmpty() || !rg_isNum ) {
        msg_venda_error.setText("O RG do cliente não foi preenchido ou está incorreto.");
        msg_venda_error.exec();
        return;
    }

    if( cliente_idade_string.isEmpty() || !idade_isNum ) {
        msg_venda_error.setText("A idade do cliente não foi preenchida ou está incorreta.");
        msg_venda_error.exec();
        return;
    }

    if( cliente_telefone_string.isEmpty() || !telefone_isNum ) {
        msg_venda_error.setText("O telefone do cliente não foi preenchido ou está incorreto.");
        msg_venda_error.exec();
        return;
    }

    if( fabricante == "Selecione um fabricante" ) {
        msg_venda_error.setText("Selecione o fabricante do veículo a ser vendido.");
        msg_venda_error.exec();
        return;
    }

    if( modelo == "Selecione um modelo" ) {
        msg_venda_error.setText("Selecione o modelo do veículo a ser vendido.");
        msg_venda_error.exec();
        return;
    }

    if( ano_fabricacao == "Selecione um ano" ) {
        msg_venda_error.setText("Selecione o ano de fabricação do veículo a ser vendido.");
        msg_venda_error.exec();
        return;
    }

    if( preco_final_string.isEmpty() || !preco_isNum ) {
        msg_venda_error.setText("O preço final do veículo não foi preenchido ou está incorreto.");
        msg_venda_error.exec();
        return;
    }

    if( forma_pagamento == "Selecione abaixo" ) {
        msg_venda_error.setText("Defina uma forma de pagamento para essa venda.");
        msg_venda_error.exec();
        return;
    }

    if( forma_pagamento == "Crédito" && num_cartao_string.isEmpty() ||
        forma_pagamento == "Débito" && num_cartao_string.isEmpty()
    ) {
        msg_venda_error.setText("Preencha o número do cartão do cliente.");
        msg_venda_error.exec();
        return;
    }

    if( tipo_juros == "Inexistente" && porcent_juros != "0" ||
        tipo_juros != "Inexistente" && porcent_juros == "0"
    ) {
        msg_venda_error.setText("Os valores do tipo de juros e sua porcentagem estão incorretos.");
        msg_venda_error.exec();
        return;
    }

    if( cliente_idade_string.isEmpty() || !idade_isNum ) {
        msg_venda_error.setText("A idade do cliente não foi preenchida ou está incorreta.");
        msg_venda_error.exec();
        return;
    }

    // Caso o cliente tenha optado por pagar o veículo em parcelas
    if( dividir_pagamento_parcelas == 1 ) {

        if( quant_parcela_string.isEmpty() || !q_parc_isNum ) {
            msg_venda_error.setText("A quantidade de parcelas não foi preenchida ou está incorreta.");
            msg_venda_error.exec();
            return;
        }

        if( prazo_primeira_parcela_string == "Selecione um prazo" ) {
            msg_venda_error.setText("Selecione um prazo para a primeira parcela do pagamento.");
            msg_venda_error.exec();
            return;
        }

        if( prazo_primeira_parcela_string == "Próximo mês" ) {
            prazo_primeira_parcela = 1;
        } else {
            prazo_primeira_parcela = prazo_primeira_parcela_string.toInt();
        }

    } else {
        prazo_primeira_parcela = 0;
    }

    // Caso o preço final do veículo seja diferente do preço salvo no banco
    if( preco_final != preco_atual ) {
        int ret = QMessageBox::warning(this, tr("Concluir Venda"),
                                            tr("O preço final do veículo é diferente do preço de fábrica. Deseja concluir a venda mesmo assim?"),
                                            QMessageBox::Yes |
                                            QMessageBox::No);

        if(ret == QMessageBox::No)
            return;
    }

    // Caso o vendedor não tenha preenchido a garantia
    if( garantia_string.isEmpty() ) {
        int ret = QMessageBox::warning(this, tr("Concluir Venda"),
                                            tr("A garantia não foi preenchida. Deseja concluir a venda mesmo assim?"),
                                            QMessageBox::Yes |
                                            QMessageBox::No);

        // Não foi colocada a garantia para essa venda
        if(ret == QMessageBox::Yes) {

            // Salva a nova venda na tabela vendageral
            qry.prepare("insert into vendageral (vendedor_idvendedor,veiculo_idveiculo,preco_final,forma_pagamento,tipo_juros,porcent_juros,numero_parcelas,prazo_primeira_parcela) values (?,?,?,?,?,?,?,?)");
            qry.addBindValue(posicao_usuario_txt); qry.addBindValue(id_veiculo_selected);
            qry.addBindValue(preco_final); qry.addBindValue(forma_pagamento);
            qry.addBindValue(tipo_juros); qry.addBindValue(porcent_juros);
            qry.addBindValue(quant_parcela); qry.addBindValue(prazo_primeira_parcela);

        }
    } else { // O vendedor preencheu a garantia
        if( !garantia_isNum ) {
            msg_venda_error.setText("A garantia informada está incorreta.");
            msg_venda_error.exec();
            return;
        } else {

            // Salva a nova venda na tabela vendageral
            qry.prepare("insert into vendageral (vendedor_idvendedor,veiculo_idveiculo,preco_final,forma_pagamento,tempo_garantia,tipo_juros,porcent_juros,numero_parcelas,prazo_primeira_parcela) values (?,?,?,?,?,?,?,?,?)");
            qry.addBindValue(posicao_usuario_txt); qry.addBindValue(id_veiculo_selected);
            qry.addBindValue(preco_final); qry.addBindValue(forma_pagamento);
            qry.addBindValue(garantia); qry.addBindValue(tipo_juros);
            qry.addBindValue(porcent_juros); qry.addBindValue(quant_parcela);
            qry.addBindValue(prazo_primeira_parcela);

        }
    }

    if( qry.exec() ) {

        // Recupera o id da venda inserida no banco
        last_insert_row = qry.lastInsertId().toInt();

        // Salva a nova venda na tabela vendacliente
        qry.prepare("insert into vendacliente (vendageral_idvendageral,nome,endereco,telefone,cpf,rg,idade,numero_cartao) values (?,?,?,?,?,?,?,?)");
        qry.addBindValue(last_insert_row); qry.addBindValue(cliente_nome);
        qry.addBindValue(cliente_endereco); qry.addBindValue(cliente_telefone);
        qry.addBindValue(cliente_cpf); qry.addBindValue(cliente_rg);
        qry.addBindValue(cliente_idade); qry.addBindValue(num_cartao);

        if( qry.exec() ) {

            QMediaPlayer caixa_registradora;
            caixa_registradora.setMedia(QUrl::fromLocalFile("../sistema_venda_carros/resources/music/som_caixa_registradora.mp3"));
            caixa_registradora.setVolume(100);
            caixa_registradora.play();

            // Modifica a quantidade do veículo do banco
            this->modifica_quant_veiculo(id_veiculo_selected);

            } else {
                msg_venda_error.setText("Erro de conexão com a base de dados - Erro #016 - cadastro_vendas.cpp");
                msg_venda_error.exec();
                return;
            }

            msg_venda_error.setText("Venda realizada com sucesso!");
            msg_venda_error.exec();
            conexao.conn_close();

            this->close();
        } else {
            msg_venda_error.setText("Erro de conexão com a base de dados - Erro #015 - cadastro_vendas.cpp");
            msg_venda_error.exec();
            return;
        }
    }
