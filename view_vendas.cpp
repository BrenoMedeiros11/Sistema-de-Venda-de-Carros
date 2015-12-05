#include "view_vendas.h"
#include "ui_view_vendas.h"
#include "login.h"
#include <QMessageBox>
#include <QSqlQuery>

view_vendas::view_vendas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::view_vendas)
{
    ui->setupUi(this);

    login conexao;
    QMessageBox msg_sql_error;
    if( !conexao.conn_open() ) {
        msg_sql_error.setText("Erro de conexão com a base de dados - Erro #001 - view_vendas.cpp");
        msg_sql_error.exec();
        return;
    }
}

view_vendas::~view_vendas()
{
    delete ui;
}

void view_vendas::setRowTableView(int _row)
{
    row_table_view = _row;
}

void view_vendas::alternative_constructor()
{
    QMessageBox msg_view_error;
    QString  vendedor_idvendedor, veiculo_idveiculo, preco_final, forma_pagamento, tempo_garantia, tipo_juros,
            porcent_juros, numero_parcelas, prazo_primeira_parcela, cliente_nome, cliente_endereco, cliente_telefone,
            cliente_cpf, cliente_rg, cliente_idade, cliente_numero_cartao, valor_parcela_string;
    QString vendedor_nome, veiculo_fabricante, veiculo_modelo, veiculo_ano_fabricacao;
    float valor_parcela; int idvendageral;

    login conexao;
    if( !conexao.conn_open() ) {
        msg_view_error.setText("Erro de conexão com a base de dados - Erro #002 - view_vendas.cpp");
        msg_view_error.exec();
        return;
    }

    conexao.conn_open();
    QSqlQuery qry;
    qry.prepare("select * from vendageral where idvendageral = ?");
    qry.addBindValue(row_table_view);

    if( qry.exec() ) {
        while( qry.next() ) {
            idvendageral = qry.value(0).toInt(); vendedor_idvendedor = qry.value(1).toString();
            veiculo_idveiculo = qry.value(2).toString(); preco_final = qry.value(3).toString();
            forma_pagamento = qry.value(4).toString(); tempo_garantia = qry.value(5).toString();
            tipo_juros = qry.value(6).toString(); porcent_juros = qry.value(7).toString();
            numero_parcelas = qry.value(8).toString(); prazo_primeira_parcela = qry.value(9).toString();
        }
    } else {
        msg_view_error.setText("Erro de conexão com a base de dados - Erro #003 - view_vendas.cpp");
        msg_view_error.exec();
        return;
    }

    if( numero_parcelas != "0" ) {
        valor_parcela = preco_final.toInt()/numero_parcelas.toInt();
        valor_parcela_string = QString::number(valor_parcela, 'f', 2);
    }

    qry.prepare("select * from vendacliente where vendageral_idvendageral = ?");
    qry.addBindValue(idvendageral);

    if( qry.exec() ) {
        while( qry.next() ) {
            cliente_nome = qry.value(2).toString(); cliente_endereco = qry.value(3).toString();
            cliente_telefone = qry.value(4).toString(); cliente_cpf = qry.value(5).toString();
            cliente_rg = qry.value(6).toString(); cliente_idade = qry.value(7).toString();
            cliente_numero_cartao = qry.value(8).toString();
        }
    } else {
        msg_view_error.setText("Erro de conexão com a base de dados - Erro #004 - view_vendas.cpp");
        msg_view_error.exec();
        return;
    }

    qry.prepare("select nome from vendedor where idvendedor = ?");
    qry.addBindValue(vendedor_idvendedor);

    if( qry.exec() ) {
        while(qry.next()) {
            vendedor_nome = qry.value(0).toString();
        }
    } else {
        msg_view_error.setText("Erro de conexão com a base de dados - Erro #005 - view_vendas.cpp");
        msg_view_error.exec();
        return;
    }

    qry.prepare("select fabricante, modelo, ano_fabricacao from veiculo where idveiculo = ?");
    qry.addBindValue(veiculo_idveiculo);

    if( qry.exec() ) {
        while( qry.next() ) {
            veiculo_fabricante = qry.value(0).toString();
            veiculo_modelo = qry.value(1).toString();
            veiculo_ano_fabricacao = qry.value(2).toString();
        }
    } else {
        msg_view_error.setText("Erro de conexão com a base de dados - Erro #006 - view_vendas.cpp");
        msg_view_error.exec();
        return;
    }

    ui->label_introducao_venda->setText("<p align='center'><h2>Venda - Número de Registro "+QString::number(idvendageral)+"</h2></p>");
    ui->label_vendedor->setText("<p style='font-size: 13px; margin-left: 15px;'>Vendedor = "+vendedor_nome+"</p>");
    ui->label_porcent_juros->setText("<p style='font-size: 13px; margin-left: 15px;'>Porcentagem de juros = "+porcent_juros+"%</p>");
    ui->label_veiculo_fabricante->setText("<p style='font-size: 13px; margin-left: 20px;'>Fabricante = "+veiculo_fabricante+"</p>");
    ui->label_veiculo_modelo->setText("<p style='font-size: 13px; text-align: center;'>Modelo = "+veiculo_modelo+"</p>");
    ui->label_veiculo_ano_fabricacao->setText("<p style='font-size: 13px; text-align: right; margin-right: 20px;'>Ano de fabricação = "+veiculo_ano_fabricacao+"</p>");

    ui->label_cliente_nome->setText("<p style='font-size: 13px; margin-left: 10px;'>Nome = "+cliente_nome+"</p>");
    ui->label_cliente_endereco->setText("<p style='font-size: 13px; margin-left: 15px;'>Endereço = "+cliente_endereco+"</p>");
    ui->label_cliente_telefone->setText("<p style='font-size: 13px; text-align: left; margin-left: 198px;'>Telefone = "+cliente_telefone+"</p>");
    ui->label_cliente_cpf->setText("<p style='font-size: 13px; text-align: left; margin-left: 200px;'>CPF = "+cliente_cpf+"</p>");
    ui->label_cliente_rg->setText("<p style='font-size: 13px; margin-left: 15px;'>RG = "+cliente_rg+"</p>");
    ui->label_cliente_idade->setText("<p style='font-size: 13px; text-align: left; margin-left: 203px;'>Idade = "+cliente_idade+"</p>");

    ui->label_preco_final->setText("<p style='font-size: 13px; margin-left: 10px;'>Preço final = R$"+preco_final+"</p>");
    ui->label_forma_pagamento->setText("<p style='font-size: 13px; text-align: right; margin-right: 20px;'>Pagamento = "+forma_pagamento+"</p>");

    if( tipo_juros == "Inexistente" ) {
        ui->label_tipo_juros->setText("<p style='font-size: 13px; text-align: right; margin-right: 20px;'>Tipo de Juros = Não incluído.</p>");
    } else {
        ui->label_tipo_juros->setText("<p style='font-size: 13px; text-align: right; margin-right: 20px;'>Tipo de Juros = "+tipo_juros+"</p>");
    }

    if( valor_parcela_string.isEmpty() ) {
        ui->label_valor_parcela->setText("<p style='font-size: 13px; margin-left: 10px;'>Valor da Parcela = R$"+preco_final+"</p>");
    } else {
        ui->label_valor_parcela->setText("<p style='font-size: 13px; margin-left: 10px;'>Valor da Parcela = R$"+valor_parcela_string+"</p>");
    }

    if( numero_parcelas == "0" ) {
        ui->label_numero_parcelas->setText("<p style='font-size: 13px; text-align: right; margin-right: 20px;'>Número de parcelas = Não informado.</p>");
    } else {
        ui->label_numero_parcelas->setText("<p style='font-size: 13px; text-align: right; margin-right: 20px;'>Número de parcelas = "+numero_parcelas+"</p>");
    }

    if( prazo_primeira_parcela == "0" ) {
        ui->label_prazo_primeira_parcela->setText("<p style='font-size: 13px; text-align: center;'>Prazo para primeira parcela = Inexistente.</p>");
    } else {
        if( prazo_primeira_parcela == "1" ) {
            ui->label_prazo_primeira_parcela->setText("<p style='font-size: 13px; text-align: center;'>Prazo para primeira parcela = Próximo mês.</p>");
        } else {
            ui->label_prazo_primeira_parcela->setText("<p style='font-size: 13px; text-align: center;'>Prazo para primeira parcela = "+prazo_primeira_parcela+" meses</p>");
        }
    }

    if( tempo_garantia.isEmpty() ) {
        ui->label_garantia->setText("<p style='font-size: 13px; text-align: right; margin-right: 20px;'>Garantia = Não informada.</p>");
    } else {
        ui->label_garantia->setText("<p style='font-size: 13px; text-align: right; margin-right: 20px;'>Garantia = "+tempo_garantia+" meses</p>");
    }

    if( cliente_numero_cartao == "0" ) {
        ui->label_cliente_numero_cartao->setText("<p style='font-size: 13px; margin-left: 10px;'>Número do cartão = Não informado.</p>");
    } else {
        ui->label_cliente_numero_cartao->setText("<p style='font-size: 13px; margin-left: 10px;'>Número do cartão = "+cliente_numero_cartao+"</p>");
    }
}
