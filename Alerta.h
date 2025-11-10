#ifndef CLASSE_ALERTA
#define CLASSE_ALERTA
#include <ctime>
#include <string>

class Alerta
{
private:
    unsigned int _id;
    time_t _tempoBruto;       // tempo em unix time stamp
    struct tm *_tempoInfo;    // struct que guarda informacoes do tempo atual
    std::string _dataEmissao; // tempo formatado para leitura
    unsigned int _tipo;
    std::string _situacao; // conteúdo do alerta

public:
    Alerta(unsigned int tipo);
    unsigned int getId();
    void setId(int id);
    std::string getDataEmissao();
    void setDataEmissao();
    void setTipo(int tipo);
    unsigned int getTipo();
    void setSituacao(std::string situacao);
    std::string getSituacao();
    void situacaoPorTipo();
    void notificar();
};

#endif