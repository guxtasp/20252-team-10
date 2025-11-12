#ifndef Reagente_H
#define Reagente_H

#include <string>

class Reagente {
private:
    std::string nome;
    std::string dataValidade;
    int quantidade;
    int quantidadeCritica;
    std::string localArmazenamento;
    int nivelAcesso;
    std::string unidadeMedida;
    std::string marca;
    std::string codigoReferencia;
    int id;

public:
    //Construtor
    Reagente(int id, std::string nome, std::string dataValidade, int quantidade, 
             int quantidadeCritica, std::string localArmazenamento, int nivelAcesso, 
             std::string unidadeMedida, std::string marca, std::string codigoReferencia);
    
    //Destrutor
    virtual ~Reagente(); 

    // Gets
    std::string getNome();
    std::string getDataValidade();
    int getQuantidade();
    int getQuantidadeCritica();
    std::string getLocalArmazenamento();
    int getNivelAcesso();
    std::string getUnidadeMedida();
    std::string getMarca();
    std::string getCodigoReferencia();
    int getId();

    //Sets
    void setNome(std::string nome);
    void setDataValidade(std::string dataValidade);
    void setQuantidade(int quantidade);
    void setQuantidadeCritica(int quantidadeCritica);
    void setLocalArmazenamento(std::string localArmazenamento);
    void setNivelAcesso(int nivelAcesso);
    void setUnidadeMedida(std::string unidadeMedida);
    void setMarca(std::string marca);
    void setCodigoReferencia(std::string codigoReferencia);
};

#endif