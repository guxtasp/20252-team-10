#include "reagente.h"
#include <stdexcept>
#include <iostream> 

//Construtor
Reagente::Reagente(int id, std::string nome, std::string dataValidade, int quantidade, 
                   int quantidadeCritica, std::string localArmazenamento, int nivelAcesso, 
                   std::string unidadeMedida, std::string marca, std::string codigoReferencia)
{
    
    this->id = id;
    this->nome = nome;
    this->dataValidade = dataValidade;
    this->quantidade = quantidade;
    this->quantidadeCritica = quantidadeCritica;
    this->localArmazenamento = localArmazenamento;
    this->nivelAcesso = nivelAcesso;
    this->unidadeMedida = unidadeMedida;
    this->marca = marca;
    this->codigoReferencia = codigoReferencia;
}

//Destrutor
Reagente::~Reagente() 
{
   
}

// Gets
std::string Reagente::getNome() { return this->nome; }
std::string Reagente::getDataValidade() { return this->dataValidade; }
int Reagente::getQuantidade() { return this->quantidade; }
int Reagente::getQuantidadeCritica() { return this->quantidadeCritica; }
std::string Reagente::getLocalArmazenamento() { return this->localArmazenamento; }
int Reagente::getNivelAcesso() { return this->nivelAcesso; }
std::string Reagente::getUnidadeMedida() { return this->unidadeMedida; }
std::string Reagente::getMarca() { return this->marca; }
std::string Reagente::getCodigoReferencia() { return this->codigoReferencia; }
int Reagente::getId() { return this->id; }

// Sets
void Reagente::setNome(std::string nome) { this->nome = nome; }
void Reagente::setDataValidade(std::string dataValidade) { this->dataValidade = dataValidade; }
void Reagente::setQuantidade(int quantidade) { this->quantidade = quantidade; }
void Reagente::setQuantidadeCritica(int quantidadeCritica) { this->quantidadeCritica = quantidadeCritica; }
void Reagente::setLocalArmazenamento(std::string localArmazenamento) { this->localArmazenamento = localArmazenamento; }
void Reagente::setNivelAcesso(int nivelAcesso) { this->nivelAcesso = nivelAcesso; }
void Reagente::setUnidadeMedida(std::string unidadeMedida) { this->unidadeMedida = unidadeMedida; }
void Reagente::setMarca(std::string marca) { this->marca = marca; }
void Reagente::setCodigoReferencia(std::string codigoReferencia) { this->codigoReferencia = codigoReferencia; }
