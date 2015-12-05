#-------------------------------------------------
#
# Project created by QtCreator 2015-10-29T11:56:51
#
#-------------------------------------------------

QT       += core gui sql multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sistema_venda_carros
TEMPLATE = app


SOURCES += main.cpp\
        login.cpp \
    tela_principal.cpp \
    cadastro_usuarios.cpp \
    cadastro_veiculos.cpp \
    cadastro_vendas.cpp \
    edittableviewveiculos.cpp \
    alterar_senha.cpp \
    view_veiculos.cpp \
    view_vendas.cpp \
    veiculo_add_quant.cpp

HEADERS  += login.h \
    tela_principal.h \
    cadastro_usuarios.h \
    cadastro_veiculos.h \
    cadastro_vendas.h \
    edittableviewveiculos.h \
    alterar_senha.h \
    view_veiculos.h \
    view_vendas.h \
    veiculo_add_quant.h

FORMS    += login.ui \
    tela_principal.ui \
    cadastro_usuarios.ui \
    cadastro_veiculos.ui \
    cadastro_vendas.ui \
    edittableviewveiculos.ui \
    alterar_senha.ui \
    view_veiculos.ui \
    view_vendas.ui \
    veiculo_add_quant.ui

RESOURCES += \
    resources/resources.qrc
