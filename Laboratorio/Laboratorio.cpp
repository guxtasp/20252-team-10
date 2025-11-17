#include "Laboratorio.h"
#include "../Estudante/estudante.h"
#include "../PosGraduacao/posgraduacao.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <mysql-cppconn/mysqlx/xdevapi.h>
using namespace mysqlx;


//Variavel estatica de laboratorios
std::vector<Laboratorio*> Laboratorio::laboratorios;

Laboratorio::Laboratorio(int id, const std::string &nome, const std::string &departamento)
    : id(id), nome(nome), departamento(departamento) {}

Laboratorio::~Laboratorio()
{
    // Limpar memoria
    for (auto retirada : retiradas)
    {
        delete retirada;
    }
}

// // Gerenciamento de Reagentes
// std::string Laboratorio::adicionarReagente(Reagente *reagente)
// {
//     // Verifica se reagente ja existe
//     for (auto reag_existente : reagentes)
//     {
//         if (reag_existente->getNome() == reagente->getNome())
//         {
//             return "Reagente '" + reagente->getNome() + "' ja existe. Use atualizar estoque.";
//         }
//     }

//     reagentes.push_back(reagente);
//     return "Reagente '" + reagente->getNome() + "' adicionado com sucesso!";
// }
// // Busca reagente por nome
// Reagente *Laboratorio::buscarReagente(const std::string &nome)
// {
//     for (auto reagente : reagentes)
//     {
//         if (reagente->getNome().find(nome) != std::string::npos)
//         {
//             return reagente;
//         }
//     }
//     return nullptr;
// }
// // Se nenhum filtro for fornecido, lista todos os reagentes.
// std::vector<Reagente *> Laboratorio::listarReagentes(const std::string &filtroNome)
// {
//     if (filtroNome.empty())
//     {
//         return reagentes;
//     }
//     // Busca reagentes com o nome contido no string
//     std::vector<Reagente *> resultado;
//     for (auto reagente : reagentes)
//     {
//         if (reagente->getNome().find(filtroNome) != std::string::npos)
//         {
//             resultado.push_back(reagente);
//         }
//     }
//     return resultado;
// }
// // Lista reagentes por local de armazenamento
// std::vector<Reagente *> Laboratorio::listarReagentesPorLocal(const std::string &local)
// {
//     std::vector<Reagente *> resultado;
//     for (auto reagente : reagentes)
//     {
//         if (reagente->getLocalArmazenamento().find(local) != std::string::npos)
//         {
//             resultado.push_back(reagente);
//         }
//     }
//     return resultado;
// }

// // Gerenciamento de Retiradas
// std::string Laboratorio::registrarRetirada(Usuario *usuario, const std::string &nomeReagente, float quantidade)
// {
//     Reagente *reagente = buscarReagente(nomeReagente);
//     if (!reagente)
//     {
//         return "Erro: Reagente '" + nomeReagente + "' nao encontrado";
//     }

//     // Verifica se reagente esta vencido
//     if (reagente->estaVencido())
//     {
//         return "Erro: Reagente '" + reagente->getNome() + "' esta vencido";
//     }

//     // Cria retirada
//     Retirada *novaRetirada = new Retirada(retiradas.size() + 1, usuario, reagente, quantidade);

//     // Confirma retirada
//     std::string resultado = novaRetirada->confirmarRetirada();

//     if (resultado.find("confirmada") != std::string::npos)
//     {
//         retiradas.push_back(novaRetirada);
//     }
//     else
//     {
//         delete novaRetirada;
//     }

//     return resultado;
// }

// // Lista todas as retiradas de um usuario especifico
// std::vector<Retirada *> Laboratorio::listarRetiradasUsuario(Usuario *usuario)
// {
//     std::vector<Retirada *> resultado;
//     for (auto retirada : retiradas)
//     {
//         if (retirada->getUsuario()->getId() == usuario->getId())
//         {
//             resultado.push_back(retirada);
//         }
//     }
//     return resultado;
// }
// // Retorna as retiradas mais recente
// std::vector<Retirada *> Laboratorio::getHistoricoRecente()
// {
//     int tamanho = retiradas.size();
//     int inicio = (tamanho > 10) ? tamanho - 10 : 0;

//     std::vector<Retirada *> historico;
//     for (int i = inicio; i < tamanho; i++)
//     {
//         historico.push_back(retiradas[i]);
//     }
//     return historico;
// }

// // Gerenciamento de Usuarios
// std::string Laboratorio::adicionarUsuario(Usuario *usuario)
// {
//     // Verifica se usuario ja existe
//     for (auto user : usuarios)
//     {
//         if (user->getId() == usuario->getId())
//         {
//             return "Usuario '" + usuario->getNome() + "' ja esta no laboratorio";
//         }
//     }

//     usuarios.push_back(usuario);

//     // Se for gestor, adiciona a lista de gestores
//     if (usuario->getNivelAcesso() == "gestor")
//     {
//         gestores.push_back(usuario);
//     }

//     return "Usuario '" + usuario->getNome() + "' adicionado ao laboratorio";
// }

// std::string Laboratorio::removerUsuario(Usuario *usuario)
// {
//     // Encontra usuario
//     auto it = std::find(usuarios.begin(), usuarios.end(), usuario);
//     if (it == usuarios.end())
//     {
//         return "Usuario '" + usuario->getNome() + "' nao encontrado no laboratorio";
//     }

//     // Verifica se ha retiradas pendentes
//     if (verificarRetiradasPendentes(usuario))
//     {
//         return "Erro: Usuario '" + usuario->getNome() + "' tem retiradas pendentes";
//     }

//     usuarios.erase(it);

//     // Remove gestores se for o caso
//     auto it_gestor = std::find(gestores.begin(), gestores.end(), usuario);
//     if (it_gestor != gestores.end())
//     {
//         gestores.erase(it_gestor);
//     }

//     return "Usuario '" + usuario->getNome() + "' removido do laboratorio";
// }

// // Verifica se um usuario tem retiradas pendentes
// bool Laboratorio::verificarRetiradasPendentes(Usuario *usuario)
// {
//     auto retiradasUsuario = listarRetiradasUsuario(usuario);
//     for (auto retirada : retiradasUsuario)
//     {
//         if (!retirada->estaConfirmada())
//         {
//             return true;
//         }
//     }
//     return false;
// }

// // Alertas e Monitoramento
// // Retorna reagentes em quantidade critica
// std::vector<Reagente *> Laboratorio::getReagentesCriticos()
// {
//     std::vector<Reagente *> criticos;
//     for (auto reagente : reagentes)
//     {
//         if (reagente->getQuantidade() <= reagente->getQuantidadeCritica())
//         {
//             criticos.push_back(reagente);
//         }
//     }
//     return criticos;
// }
// // Retorna reagentes vencidos
// std::vector<Reagente *> Laboratorio::getReagentesVencidos()
// {
//     std::vector<Reagente *> vencidos;
//     for (auto reagente : reagentes)
//     {
//         if (reagente->estaVencido())
//         {
//             vencidos.push_back(reagente);
//         }
//     }
//     return vencidos;
// }
// // Retorna reagentes proximos do vencimento
// std::vector<Reagente *> Laboratorio::getReagentesProximoVencimento(int dias)
// {
//     std::vector<Reagente *> proximos;
//     for (auto reagente : reagentes)
//     {
//         if (reagente->proximoVencimento(dias))
//         {
//             proximos.push_back(reagente);
//         }
//     }
//     return proximos;
// }
// // Gera relatorio de alertas para gestores
// std::string Laboratorio::getAlertasGestor()
// {
//     auto criticos = getReagentesCriticos();
//     auto vencidos = getReagentesVencidos();
//     auto proximos = getReagentesProximoVencimento();

//     std::stringstream alertas;
//     alertas << "=== ALERTAS PARA GESTORES ===\n";
//     alertas << "Reagentes em quantidade critica: " << criticos.size() << "\n";
//     alertas << "Reagentes vencidos: " << vencidos.size() << "\n";
//     alertas << "Reagentes proximos do vencimento: " << proximos.size() << "\n";

//     return alertas.str();
// }

// // Informacoes do Laboratorio
// std::string Laboratorio::getEstatisticas()
// {
//     std::stringstream stats;
//     stats << "=== ESTATISTICAS DO LABORATORIO ===\n"
//           << "Total de reagentes: " << reagentes.size() << "\n"
//           << "Total de usuarios: " << getTotalUsuarios() << "\n"
//           << "Total de gestores: " << gestores.size() << "\n"
//           << "Total de retiradas: " << retiradas.size() << "\n"
//           << "Reagentes criticos: " << getReagentesCriticos().size() << "\n"
//           << "Reagentes vencidos: " << getReagentesVencidos().size();

//     return stats.str();
// }

std::string Laboratorio::toString() const
{
    return "Laboratorio " + std::to_string(id) + ": " + nome + " (" + departamento + ")";
}

//Lista todos os laboratorios instanciados
std::vector<Laboratorio*> Laboratorio::listarLaboratorios(Schema* db)
{
    Laboratorio::laboratorios.clear(); // Limpa o vector de laboratorios
    Table table = db->getTable("Laboratorio"); // Acessa a tabela do laboratorio no db
    RowResult result = table.select("id", "nome", "departamento").execute(); //Pega os atributos do laboratorio e consulta
    Row row; // Intancia uma linha que retornada pelo select da consulta
    //A cada chamada ele retorna a próxima linha do resultado 
    // Quando não houver mais linhas, ele retorna um Row vazio (que vale como false), 
    //e o while é encerrado.
    while((row = result.fetchOne())){
        int id = row[0].get<int>(); // Armazena o id da posicao da linha com cast do tipo do c++
        std::string nome = row[1].get<std::string>(); // Armazena o nome da posicao da linha com cast do tipo do c++
        std::string departamento = row[2].get<std::string>(); // Armazena o departamento da posicao da linha com cast do tipo do c++
        Laboratorio* laboratorio = new Laboratorio(id, nome, departamento); // Instancia dinamicamenete o objeto Laboratorio 
        Laboratorio::laboratorios.push_back(laboratorio); // Armazena os ponteiros do laboratorio
    }
    return Laboratorio::laboratorios;
};
//Imprime os laboratorios disponiveis
void Laboratorio::imprimirLaboratorios()
{
    std::cout << "\n-----------------------------------------------\n";
    std::cout << "| ID  | Nome                 | Departamento    |\n";
    std::cout << "-----------------------------------------------\n";

        for (int i = 0; i < Laboratorio::laboratorios.size(); i++) { // Itera até chegar no numero de laboratorios
            Laboratorio* laboratorio = Laboratorio::laboratorios[i]; // recebe o laboratorio na posicao i
            std::cout << "| "
                        << std::setw(3)  << laboratorio->getId()        << " | "  //Pega o ID
                        << std::setw(20) << laboratorio->getNome()      << " | " //Pega o nome
                        << std::setw(15) << laboratorio->getDepartamento() // Pega o nome do departamento
                        << " |\n";
                        //Imprime
        }
        std::cout << "-----------------------------------------------\n";
}

//Adicionar gestor
void Laboratorio::adicionarGestor(Gestor* gestor){
    this->gestores.push_back(gestor);
};

//Limpa o vector de laboratorio
void Laboratorio::limparLaboratorios() {
    for (int i = 0; i < Laboratorio::laboratorios.size(); i++) {
        delete Laboratorio::laboratorios[i]; // deleta cada Laboratorio*
    }
    Laboratorio::laboratorios.clear(); // limpa o vetor
}
void Laboratorio::adicionarEstudante(Estudante* estudante){
    //Verifica se o estudante esta no array de estudantes associado no vetor de graduação
    for (int i = 0; i < estudantesGraduacao.size(); i++) {
        if (estudantesGraduacao[i] == estudante) {
            std::cout << "Aviso: Estudante " << estudante->getNome()
                        << " já está associado ao laboratório " << nome
                        << std::endl;
                return;
        }
    }
    //Verifica se o estudante esta no array de estudantes associado no vetor de graduação
    for (int i = 0; i < estudantesPosGraduacao.size(); i++) {
        if (estudantesPosGraduacao[i] == estudante) {
            std::cout << "Aviso: Estudante " << estudante->getNome()
                        << " já está associado ao laboratório " << nome
                        << std::endl;
            return;
        }
    }
    // se for estudante de graduação aloca no seu vetor
    if (estudante->getNivelAcesso() == 3) {
        estudantesGraduacao.push_back(estudante);
        std::cout << "Estudante de Graduação " << estudante->getNome()
                    << " adicionado ao " << nome
                    << std::endl;
        return;
    }
    if (estudante->getNivelAcesso() == 2) {
        PosGraduacao* posGrad = dynamic_cast<PosGraduacao*>(estudante); // se for posgraduação faz um casting 
        if (posGrad != nullptr) { // verifica se nao é nulo, se nao for, adiciona no evtor de pos graduação
            estudantesPosGraduacao.push_back(posGrad);
            std::cout << "Estudante de Pós-Graduação " << estudante->getNome()
                        << " adicionado ao " << nome
                        <<std::endl;
        } else {
            std::cerr << "Erro: Tentativa de adicionar Estudante de Pós-Graduação" << std::endl;
        }
        return;
    }
    std::cerr << "Aviso: Nível de estudante desconhecido: "
                << estudante->getNivel()
                << ". Estudante não adicionado lista de Estudantes do laboratório."
                << std::endl;

}
// Remove estudante do vetor designado 
void Laboratorio::removerEstudante(Estudante* estudante) {
    bool removido = false; // flag se foi removido

    // [GRADUAÇÃO]
    std::vector<Estudante*> novoVetorGraduacao; // Cria um novo vetor
    for (int i = 0; i < estudantesGraduacao.size(); i++) {
        Estudante* atual = estudantesGraduacao[i];
        if (atual != estudante) {// se for diferente 
            novoVetorGraduacao.push_back(atual); // adiciona no novo vetor
        } else { // se for igual, informa a flag
            removido = true; 
        }
    }
    if (removido) { // se for verdadeiro
        estudantesGraduacao.swap(novoVetorGraduacao); // atualiza o novo vetor
    }

    //[POS GRADUAÇÃO]
    // casting para PosGraduacao
    // Se for um estudante de graduação, o resultado será nullptr
    PosGraduacao* posGrad = dynamic_cast<PosGraduacao*>(estudante);
    if (posGrad) {
        std::vector<PosGraduacao*> novoVetorPosGraduacao;
        bool removidoPos = false;
            for (int i = 0; i < estudantesPosGraduacao.size(); i++) {
            PosGraduacao* atual = estudantesPosGraduacao[i];
            if (atual != posGrad) {
                // Mantém o estudante caso nao sej ao que tem que remover
                novoVetorPosGraduacao.push_back(atual);
            } else {
                // Encontrou o estudante que tem que remover
                removidoPos = true;
            }
        }
        if (removidoPos) {
            // Troca o vetor original pela versão sem o estudante removido
            estudantesPosGraduacao.swap(novoVetorPosGraduacao);
            // Marca que houve remoção 
            removido = true;
        }
    }
    if (removido) {
        std::cout << "Estudante " << estudante->getNome() << " removido." << std::endl;
    } else {
        std::cout << "Aviso: Estudante " << estudante->getNome() << " não encontrado." << std::endl;
    }
}
