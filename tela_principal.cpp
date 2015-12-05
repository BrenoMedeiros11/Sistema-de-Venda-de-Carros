#include "tela_principal.h"
#include "ui_tela_principal.h"
#include "login.h"
#include "QMessageBox"
#include "QPushButton"
#include "QStandardItemModel"
#include "QSqlQuery"
#include "QSqlQueryModel"
#include "QString"

tela_principal::tela_principal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::tela_principal)
{   
    ui->setupUi(this);
    ui->tableView_Dados->hide();
    row_table_selected = 0; // Reseta

    /*
    login conexao;
    QMessageBox msg_sql_error;
    if( !conexao.conn_open() ) {
        msg_sql_error.setText("Erro de conexão com a base de dados - Erro #001 - tela_principal.cpp");
        msg_sql_error.exec();
        return;
    }
    */

    // "Esconde" o menu de editar/deletar dados
    ui->label_choice->hide();
    ui->textBrowser_choice->hide();
    ui->pushButton_choice_delete->hide();
    ui->pushButton_choice_edit->hide();
    ui->pushButton_choice_view->hide();

}

tela_principal::~tela_principal()
{
    delete ui;
}

void tela_principal::setPosicao(int _posicao)
{
    posicao_usuario_txt = _posicao;
}

int tela_principal::getPosicao()
{
    return posicao_usuario_txt;
}

int tela_principal::getNumVendas(int tipo, int _id_tabela) {
    QMessageBox msg_vendas_error;
    int num_vendas = -1;

    login conexao;
    if( !conexao.conn_open() ) {
        msg_vendas_error.setText("Erro de conexão com a base de dados - Erro #002 - tela_principal.cpp");
        msg_vendas_error.exec();
        return num_vendas;
    }

    conexao.conn_open();
    QSqlQuery qry;

    if( tipo == 1 ) {
        qry.prepare("select * from vendageral where vendedor_idvendedor = ?");
    } else {
        if( tipo == 2 ) {
            qry.prepare("select * from vendageral where veiculo_idveiculo = ?");
        }
    }

    qry.addBindValue(_id_tabela);

    if( qry.exec() ) {
        num_vendas = 0;
        while( qry.next() ) {
            num_vendas++;
        }
        return num_vendas;
    } else {
        msg_vendas_error.setText("Erro de conexão com a base de dados - Erro #003 - tela_principal.cpp");
        msg_vendas_error.exec();
        return num_vendas;
    }
}

void tela_principal::on_actionExit_triggered()
{
    int ret = QMessageBox::warning(this, tr("Sair"),
                                      tr("Tem certeza que deseja sair?"),
                                      QMessageBox::Yes |
                                      QMessageBox::No);

    if(ret == QMessageBox::Yes)
        exit(0);
}

void tela_principal::on_actionCadastro_de_Usu_rios_triggered()
{
    cadastro_usuarios c_usu;
    c_usu.exec();
}

void tela_principal::on_actionCadastro_de_Ve_culos_triggered()
{
    cadastro_veiculos c_vei;
    c_vei.exec();
}

void tela_principal::on_actionNova_Venda_triggered()
{
    login conexao;
    QMessageBox msg_sql_error;
    int num_rows_qry = 0;
    if( !conexao.conn_open() ) {
        msg_sql_error.setText("Erro de conexão com a base de dados - Erro #004 - tela_principal.cpp");
        msg_sql_error.exec();
        return;
    }

    // Verifica se existe algum veículo disponivel a venda. Se não existir, fecha a dialog de vendas.
    conexao.conn_open();
    QSqlQuery verifica_veiculos;
    verifica_veiculos.prepare("SELECT * FROM veiculo WHERE quant_estoque > 0");

    if( verifica_veiculos.exec() ) {

        while( verifica_veiculos.next() ) {
            num_rows_qry++;
        }

        if( num_rows_qry == 0 ) {
            msg_sql_error.setText("Não existem veículos cadastrados no sistema para serem vendidos.");
            msg_sql_error.exec();
            return;
        }

    } else {
        msg_sql_error.setText("Erro de conexão com a base de dados - Erro #005 - cadastro_vendas.cpp");
        msg_sql_error.exec();
        return;
    }
    conexao.conn_close();

    cadastro_vendas c_ven;
    c_ven.setPosicao(posicao_usuario_txt);
    c_ven.alternative_constructor();
    c_ven.exec();
}

void tela_principal::on_actionLista_de_Usu_rios_triggered()
{
    action_table_view = 1;
    ui->pushButton_choice_view->hide();

    ui->tableView_Dados->reset(); ui->tableView_Dados->show();

    login conexao;
    QSqlQueryModel *modal = new QSqlQueryModel();

    conexao.conn_open();
    QSqlQuery *qry = new QSqlQuery(conexao.mydb);

    qry->prepare("select idvendedor, nome, login from vendedor");
    qry->exec();

    modal->setQuery(*qry);
    ui->tableView_Dados->setModel(modal);

    for(int c=0; c<ui->tableView_Dados->horizontalHeader()->count(); c++) {
        ui->tableView_Dados->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }

    conexao.conn_close();
}

void tela_principal::on_actionLista_de_Ve_culos_triggered()
{
    action_table_view = 2;

    ui->tableView_Dados->reset(); ui->tableView_Dados->show();

    login conexao;
    QSqlQueryModel *modal = new QSqlQueryModel();

    conexao.conn_open();
    QSqlQuery *qry = new QSqlQuery(conexao.mydb);

    qry->prepare("select idveiculo, placa from veiculo");
    qry->exec();

    // Caso a consulta não tenha sido vazia
    modal->setQuery(*qry);
    ui->tableView_Dados->setModel(modal);

    for(int c=0; c<ui->tableView_Dados->horizontalHeader()->count(); c++) {
        ui->tableView_Dados->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }

    conexao.conn_close();
}

void tela_principal::on_actionLista_de_Vendas_triggered()
{
    action_table_view = 3;

    ui->tableView_Dados->reset(); ui->tableView_Dados->show();

    login conexao;
    QSqlQueryModel *modal = new QSqlQueryModel();

    conexao.conn_open();
    QSqlQuery *qry = new QSqlQuery(conexao.mydb);

    qry->prepare("select vendageral.idvendageral, vendedor.nome from vendageral left join vendedor on vendageral.vendedor_idvendedor = vendedor.idvendedor order by vendageral.idvendageral");
    qry->exec();

    // Caso a consulta não tenha sido vazia
    modal->setQuery(*qry);
    ui->tableView_Dados->setModel(modal);

    for(int c=0; c<ui->tableView_Dados->horizontalHeader()->count(); c++) {
        ui->tableView_Dados->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }

    conexao.conn_close();
}

int tela_principal::getID(int _column, QString _data) {

    int id_return = 0;
    QMessageBox msg_id_error;

    login conexao;
    if( !conexao.conn_open() ) {
        msg_id_error.setText("Erro de conexão com a base de dados - Erro #006 - tela_principal.cpp");
        msg_id_error.exec();
        return id_return;
    }

    conexao.conn_open();
    QSqlQuery qry;

    if( action_table_view == 1 ) {
        switch(_column){
            case 0:
                qry.prepare("select idvendedor from vendedor where idvendedor = ?");
                break;
            case 1:
                qry.prepare("select idvendedor from vendedor where nome = ?");
                break;
            case 2:
                qry.prepare("select idvendedor from vendedor where login = ?");
                break;
        }
    } else {
        if( action_table_view == 2 ) {
            switch(_column){
                case 0:
                    qry.prepare("select idveiculo from veiculo where idveiculo = ?");
                    break;
                case 1:
                    qry.prepare("select idveiculo from veiculo where placa = ?");
                    break;
            }
        } else {
            if( action_table_view == 3 ){
                switch(_column){
                    case 0:
                        qry.prepare("select idvendageral from vendageral where idvendageral = ?");
                        break;
                    case 1:
                        qry.prepare("select vendageral.idvendageral from vendageral left join vendedor on vendageral.vendedor_idvendedor = vendedor.idvendedor and vendedor.nome = ?");
                        break;
                }
            }
        }
    }

    qry.addBindValue(_data);

    if( qry.exec() ) {
        while(qry.next()) {
            id_return = qry.value(0).toInt();
        }
        conexao.conn_close();
    } else {
        msg_id_error.setText("Erro de conexão com a base de dados - Erro #007 - tela_principal.cpp");
        msg_id_error.exec();
        return id_return;
    }

    return id_return;
}

int tela_principal::apaga_vendas_vendedor(int id_vendedor) {

    int success = 0, deleta_venda_id;
    QMessageBox msg_id_error;

    login conexao;
    if( !conexao.conn_open() ) {
        msg_id_error.setText("Erro de conexão com a base de dados - Erro #008 - tela_principal.cpp");
        msg_id_error.exec();
        return 0;
    }

    conexao.conn_open();
    QSqlQuery qry;

    // Busca todas as vendas realizadas pelo vendedor
    qry.prepare("select idvendageral from vendageral where vendedor_idvendedor = ?");
    qry.addBindValue(id_vendedor);

    if( qry.exec() ) {
        while( qry.next() ) {

            deleta_venda_id = qry.value(0).toInt();

            QSqlQuery qry_cliente;
            qry_cliente.prepare("delete from vendacliente where vendageral_idvendageral = ?");
            qry_cliente.addBindValue(deleta_venda_id);

            if( qry_cliente.exec() ) {
                success = 1;
            } else {
                success = 3;
                break;
            }

            QSqlQuery qry_geral;
            qry_geral.prepare("delete from vendageral where idvendageral = ?");
            qry_geral.addBindValue(deleta_venda_id);

            if( qry_geral.exec() ) {
                success = 1;
            } else {
                success = 4;
                break;
            }
        }

    } else {
        success = 2;
    }

    conexao.conn_close();
    return success;
}

void tela_principal::on_tableView_Dados_doubleClicked(const QModelIndex &index)
{
    ui->pushButton_choice_delete->hide(); ui->pushButton_choice_edit->hide();
    ui->label_choice->show(); ui->textBrowser_choice->show();

    // NOTA: index.row() começa a contar as linhas a partir do 0
    // Primeira linha = linha 0, Segunda linha = linha 1, etc.

    row_table_selected = getID(index.column(),index.data().toString());
    QString row_string = QString::number(row_table_selected);

    if( row_table_selected != 0 ) {
        if( action_table_view == 1 ||  action_table_view == 2 ){

            ui->pushButton_choice_delete->show();

            if( action_table_view == 1 ){

                ui->label_choice->setText("<center><b>Dados do vendedor selecionado</b></center>");
                ui->textBrowser_choice->setText("Vendedor - ID = "+row_string);

            } else { //  action_table_view == 2

                ui->pushButton_choice_edit->show();
                ui->label_choice->setText("<center><b>Dados do veículo selecionado</b></center>");
                ui->textBrowser_choice->setText("Veículo - ID = "+row_string);
                ui->pushButton_choice_view->show();

            }
        } else {
            if(action_table_view == 3){

                ui->label_choice->setText("<center><b>Dados da venda selecionado</b></center>");
                ui->textBrowser_choice->setText("Venda Concluída - ID = "+row_string);
                ui->textBrowser_choice->show();
                ui->pushButton_choice_view->show();

            }
        }
    }
}

void tela_principal::on_pushButton_choice_edit_clicked()
{
    if( action_table_view == 2 ) { // Veículos
        EditTableViewVeiculos e_t_v_v;
        e_t_v_v.setRowTableView(row_table_selected);
        e_t_v_v.alternative_constructor();
        e_t_v_v.exec();
    }
}

void tela_principal::on_pushButton_choice_delete_clicked()
{
    login conexao;
    QMessageBox msg_delete_error;
    int flag_confirma = 0, num_vendas = 0;

    if( row_table_selected != 0 ) { // Se existe algo para deletar

        if( action_table_view == 1 ) {

            // Caso o usuário esteja tentando deletar a si mesmo
            if( row_table_selected == posicao_usuario_txt ) {
                msg_delete_error.setText("Você não pode deletar a si mesmo.");
                msg_delete_error.exec();
                return;
            }

            num_vendas = this->getNumVendas(action_table_view,row_table_selected);

            if( num_vendas > 0 ) {

                int ret = QMessageBox::warning(this, tr("Deletar"),
                                            tr("Ao deletar o vendedor, todas as vendas realizadas por ele serão deletadas. Deseja continuar?"),
                                            QMessageBox::Yes |
                                            QMessageBox::No);
                if(ret == QMessageBox::Yes){
                    flag_confirma = 1;
                }
            } else { // Caso não existam vendas cadastradas pelo vendedor correspondente
                flag_confirma = 1;
            }
        } else {
            if( action_table_view == 2 ){

                int num_vendas = this->getNumVendas(action_table_view,row_table_selected);

                if( num_vendas > 0 ) {
                    msg_delete_error.setText("O veículo não pode ser deletado pois existem vendas cadastradas envonvendo ele.");
                    msg_delete_error.exec();
                    return;
                } else {
                    flag_confirma = 1;
                }
            }
        }

        if( flag_confirma == 1 ) { // Caso o usuário tenha confirmado o delete

            if( !conexao.conn_open() ) {
                msg_delete_error.setText("Erro de conexão com a base de dados - Erro #009 - tela_principal.cpp");
                msg_delete_error.exec();
                return;
            }

            conexao.conn_open();
            QSqlQuery qry;

            if( action_table_view == 1 ){ // Deletar da tabela usuario
                qry.prepare("Delete from vendedor where idvendedor = ?");
            } else {
                if( action_table_view == 2 ) { // Deletar da tabela veiculo
                    qry.prepare("Delete from veiculo where idveiculo = ?");
                }
            }

            qry.addBindValue(row_table_selected);

            if( qry.exec() ) {

                if( action_table_view == 1 && num_vendas == 0 ) {
                    msg_delete_error.setText("Vendedor deletado com sucesso.");
                } else {
                    if( action_table_view == 1 && num_vendas > 0 ) {

                        // Deleta todas as vendas relacionadas ao vendedor
                        int deleta = this->apaga_vendas_vendedor(row_table_selected);

                        if(deleta == 1){
                            msg_delete_error.setText("Vendedor deletado com sucesso.");
                        } else {
                            msg_delete_error.setText(QString::number(deleta));
                        }

                    } else {
                        if( action_table_view == 2 ) {
                            msg_delete_error.setText("Veículo deletado com sucesso.");
                        }
                    }
                }

                msg_delete_error.exec();
                ui->label_choice->hide();
                ui->textBrowser_choice->hide();
                ui->pushButton_choice_delete->hide();
                ui->pushButton_choice_edit->hide();
                ui->pushButton_choice_view->hide();
                row_table_selected = 0;
                conexao.conn_close();
                return;
            } else {
                msg_delete_error.setText("Erro de conexão com a base de dados - Erro #011 - tela_principal.cpp");
                msg_delete_error.exec();
                return;
            }
        }
    }
}

void tela_principal::on_actionAlterar_Senha_triggered()
{
    alterar_senha change_password;
    change_password.set_ID_logado(posicao_usuario_txt);
    change_password.exec();
}

void tela_principal::on_pushButton_choice_view_clicked()
{
    if( action_table_view == 2 ) {
        view_veiculos v_vei;
        v_vei.setRowTableView(row_table_selected);
        v_vei.alternative_constructor();
        v_vei.exec();
    } else {
        if( action_table_view == 3 ) {
            view_vendas v_ven;
            v_ven.setRowTableView(row_table_selected);
            v_ven.alternative_constructor();
            v_ven.exec();
        }
    }
}

void tela_principal::on_actionAdicionar_no_estoque_triggered()
{
    veiculo_add_quant v_a_q;
    v_a_q.exec();
}
