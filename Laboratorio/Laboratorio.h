#ifndef LABORATORIO_H
#define LABORATORIO_H

#include <vector>
#include <string>
#include "../Usuario/usuario.h"
#include "../Reagente/reagente.h"
#include "../Retirada/Retirada.h"
#include "../Reagente/reagenteliquido.h"
#include "../Reagente/reagentesolido.h"

using namespace mysqlx;

class Laboratorio
{
private:
    int id;
    Schema* db;
    std::string nome;
    std::string departamento;
    std::vector<Reagente *> reagentes;
    std::vector<Usuario *> usuarios;
    std::vector<Retirada *> retiradas;
    std::vector<Usuario *> gestores;


    // Metodos privados
    bool verificarRetiradasPendentes(Usuario *usuario);
    void carregarReagentesDoDB(); // Metodo para carregar dados do DB

public:
    // Constructor e Destructor
    Laboratorio(int id, const std::string &nome, const std::string &departamento, Schema* db);
    ~Laboratorio();

    //Metodo para criar e salvar o reagente
    void cadastrarNovoReagente(
        std::string nome, std::string dataValidade, int quantidade, 
        int quantidadeCritica, std::string local, int nivelAcesso, 
        std::string unidade, std::string marca, std::string codRef,
        int tipo, double densidade, double volume, 
        double massa, std::string estadoFisico
    );

    // Metodo para Buscar Reagentes
    Reagente *buscarReagente(const std::string &nome); 
    std::vector<Reagente *> listarReagentes(const std::string &filtroNome = "");
    std::vector<Reagente *> listarReagentesPorLocal(const std::string &local); 

    // Gerenciamento de Retiradas
    std::string registrarRetirada(Usuario *usuario, const std::string &nomeReagente, float quantidade);
    std::vector<Retirada *> listarRetiradasUsuario(Usuario *usuario);
    std::vector<Retirada *> getHistoricoRecente();

    // Gerenciamento de Usuarios
    std::string adicionarUsuario(Usuario *usuario);
    std::string removerUsuario(Usuario *usuario);

    // Alertas e Monitoramento
    std::vector<Reagente *> getReagentesCriticos();
    std::vector<Reagente *> getReagentesVencidos();
    std::vector<Reagente *> getReagentesProximoVencimento(int dias = 15);
    std::string getAlertasGestor();

    // Informacoes do Laboratorio
    std::string getEstatisticas();

    // Getters
    int getId() const { return id; }
    std::string getNome() const { return nome; }
    std::string getDepartamento() const { return departamento; }
    int getTotalReagentes() const { return reagentes.size(); }
    int getTotalUsuarios() const { return usuarios.size(); }
    int getTotalRetiradas() const { return retiradas.size(); }

    // toString
    std::string toString() const;
};

#endif