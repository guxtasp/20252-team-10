#include "Alerta.h"
#include <iostream>
#include <stdexcept>

Alerta::Alerta(unsigned int tipo)
{
    this->_tipo = tipo;
    try
    {
        this->situacaoPorTipo(); // preenche a situacao baseado no tipo de alerta
    }
    catch (std::runtime_error &e)
    {
        throw;
    }

    this->setDataEmissao();
}

unsigned int Alerta::getId()
{
    return this->_id;
}
void Alerta::setId(int id)
{
    this->_id = id;
}
std::string Alerta::getDataEmissao()
{
    return this->_dataEmissao;
}
void Alerta::setDataEmissao()
{
    this->_tempoBruto = std::time(nullptr);
    this->_tempoInfo = std::localtime(&_tempoBruto);
    this->_dataEmissao = std::asctime(_tempoInfo);
}

void Alerta::setTipo(int tipo)
{
    this->_tipo = tipo;
}
unsigned int Alerta::getTipo()
{
    return this->_tipo;
}

void Alerta::setSituacao(std::string situacao)
{
    this->_situacao = situacao;
}

std::string Alerta::getSituacao()
{
    return this->_situacao;
}

void Alerta::situacaoPorTipo()
{
    std::string situacao;
    switch (_tipo)
    {
    case 1: // tipo 1 = reagente em quantidade critica
        situacao = "[ALERTA]: O reagente [PLACEHOLDER] atingiu quantidade crítica!\n";
        break;
    case 2:
        situacao = "[ALERTA]: o reagente [PLACEHOLDER] está próximo do vencimento!\n";
        break;
    default:
        throw std::runtime_error("Alerta criado com tipo inválido\n");
        break;
    }
    this->_situacao = situacao;
}

void Alerta::notificar()
{
    if (!(this->_situacao.empty()))
    {
        std::cout << this->_dataEmissao << " : " << this->_situacao;
    }
    else
    {
        throw std::runtime_error("Alerta com string situação vazia\n");
    }
}