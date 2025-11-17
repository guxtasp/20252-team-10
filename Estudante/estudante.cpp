#include "estudante.h"
#include <iostream>
using namespace mysqlx;

// Construtor
Estudante::Estudante(std::string nome, std::string email, std::string senha, int nivelAcesso,
                    mysqlx::abi2::r0::Schema* db, std::string matricula, std::string curso, std::string nivel):
    Usuario(nome, email, senha, nivelAcesso, db), matricula(matricula), curso(curso), nivel(nivel) {}

// Destrutor
Estudante::~Estudante() {}

// Gets
std::string Estudante::getMatricula(){
    return matricula;
}
std::string Estudante::getCurso(){
    return curso;
}
std::string Estudante::getNivel(){
    return nivel;
}

// Sets
void Estudante::setMatricula(std::string matricula){
    this->matricula = matricula;
}
void Estudante::setCurso(std::string curso){
    this->curso = curso;
}
void Estudante::setNivel(std::string nivel){
    this->nivel = nivel;
}

// Esta função checa o nivel de acesso antes de exibir.
void Estudante::acessarReagenteRestrito(int idReagente) {

    std::cout << "\n(Estudante) Tentando acessar Reagente ID: " << idReagente << "\n";
    if (db == nullptr) {
        std::cerr << "ERRO: Estudante não está conectado ao banco." << std::endl;
        return;
    }

    try {
        Table reagenteTable = db->getTable("Reagente");

        // Busca o reagente específico pelo ID
        RowResult res = reagenteTable.select(
            "id", "nome", "quantidade", "unidadeMedida",
            "localArmazenamento", "dataValidade", "nivelAcesso"
        ).where("id = :id").bind("id", idReagente).execute();

        if (res.count() == 0) {
            std::cout << "Reagente com ID " << idReagente << " não encontrado." << std::endl;
            return;
        }

        Row row = res.fetchOne();
        int nivelDoReagente = row[6].get<int>(); // Nivel de segurança do reagente

        // Checa a permissão: Nivel do Estudante (this->nivelAcesso) deve ser menor ou igual ao nivel do reagente
        if (this->getNivelAcesso() <= nivelDoReagente) {
            // Se tem permissão, exibe os dados
            std::cout << "Acesso Permitido (Nível " << this->getNivelAcesso() << "):\n";
            std::cout << "Nome:    " << row[1].get<std::string>() << "\n";
            std::cout << "Qtde:    " << row[2].get<int>() << " " << row[3].get<std::string>() << "\n";
            std::cout << "Local:   " << row[4].get<std::string>() << "\n";
            std::cout << "Validade: " << row[5].get<std::string>() << "\n";
        } else {
            // Se não tem permissao, nega o acesso
            std::cout << "ACESSO NEGADO: Este reagente requer Nível "
                      << nivelDoReagente << ".\n";
            std::cout << "(Seu nível é " << this->getNivelAcesso() << ").\n";
        }
        std::cout << std::endl;

    } catch (const mysqlx::Error &err) {
        std::cerr << "Erro ao acessar reagente: " << err.what() << std::endl;
    }
}
