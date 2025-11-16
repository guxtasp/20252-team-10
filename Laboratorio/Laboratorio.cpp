#include "../Laboratorio/Laboratorio.h"
#include <sstream>
#include <algorithm>
#include <iostream>

Laboratorio::Laboratorio(int id, const std::string &nome, const std::string &departamento, Schema* db)
    : id(id), nome(nome), departamento(departamento), db(db) 
{
    // Se a conexao com o DB existir, carrega os reagentes
    if (this->db) {
        std::cout << "Laboratorio conectado ao DB. Reagentes sendo carregados" << std::endl;
        carregarReagentesDoDB();
    }
}

Laboratorio::~Laboratorio()
{
    // Limpar memoria
    for (auto retirada : retiradas)
    {
        delete retirada;
    }
//Limpa a memoria dos reagentes carregados
    for (auto reagente : reagentes)
    {
        delete reagente;
    }
}

// Implementacao do metodo que carrega reagentes do DB
void Laboratorio::carregarReagentesDoDB() {
    try {
        Table reagenteTable = db->getTable("Reagente");

        //LEFT JOIN é usado para pegar dados de Liquido ou Solido
        RowResult res = reagenteTable.select(
            "Reagente.id", "Reagente.nome", "Reagente.quantidade", 
            "Reagente.quantidadeCritica", "Reagente.codigoReferencia", "Reagente.marca", 
            "Reagente.localArmazenamento", "Reagente.nivelAcesso", "Reagente.unidadeMedida", 
            "Reagente.dataValidade", // Colunas do Reagente
            "RL.densidade", "RL.volume", // Colunas do Liquido
            "RS.massa", "RS.estadoFisico" // Colunas do Solido
        )
        .leftJoin("ReagenteLiquido as RL", "Reagente.id = RL.id")
        .leftJoin("ReagenteSolido as RS", "Reagente.id = RS.id")
        .execute();

        //Criar objetos
        for (Row row : res.fetchAll()) {
            Reagente* novoReagente = nullptr;
            
            // Pega dados da tabela base
            int id = row[0].get<int>();
            std::string nome = row[1].get<std::string>();
            int qtd = row[2].get<int>();
            int qtdCritica = row[3].get<int>();
            std::string codRef = row[4].get<std::string>();
            std::string marca = row[5].get<std::string>();
            std::string local = row[6].get<std::string>();
            int nivelAcesso = row[7].get<int>();
            std::string unidade = row[8].get<std::string>();
            std::string validade = row[9].get<std::string>();

            // Verifica se e Liquido (checa se a coluna do JOIN nao e nula)
            if (!row[10].isNull()) {
                double densidade = row[10].get<double>();
                double volume = row[11].get<double>();
                novoReagente = new ReagenteLiquido(id, nome, validade, qtd, qtdCritica, local, 
                                                   nivelAcesso, unidade, marca, codRef, 
                                                   densidade, volume); //
            } 
            // Verifica se e Solido
            else if (!row[12].isNull()) {
                double massa = row[12].get<double>();
                std::string estado = row[13].get<std::string>();
                novoReagente = new ReagenteSolido(id, nome, validade, qtd, qtdCritica, local, 
                                                  nivelAcesso, unidade, marca, codRef, 
                                                  massa, estado); //
            }

            if (novoReagente) {
                this->reagentes.push_back(novoReagente); // Adiciona no vector
            }
        }
        std::cout << this->reagentes.size() << " reagentes carregados do DB para a memoria." << std::endl;

    } catch (const mysqlx::Error &err) {
        std::cerr << "Erro ao carregar reagentes do DB: " << err.what() << std::endl;
    }
}

void Laboratorio::cadastrarNovoReagente(
    std::string nome, std::string dataValidade, int quantidade, 
    int quantidadeCritica, std::string local, int nivelAcesso, 
    std::string unidade, std::string marca, std::string codRef,
    int tipo, double densidade, double volume, 
    double massa, std::string estadoFisico)
{
    //insercao no DB
    try {
        Table reagenteTable = db->getTable("Reagente");
        
        Result res = reagenteTable.insert(
            "nome", "dataValidade", "quantidade", "quantidadeCritica", 
            "localArmazenamento", "nivelAcesso", "unidadeMedida", "marca", "codigoReferencia"
        ).values(nome, dataValidade, quantidade, quantidadeCritica, 
                 local, nivelAcesso, unidade, marca, codRef)
         .execute(); //
        
        int reagenteId = res.getAutoIncrementValue(); // Pega o ID criado

        Reagente* novoReagente = nullptr; // Ponteiro para objeto em memoria

        if (tipo == 1) { 
            db->getTable("ReagenteLiquido")
                .insert("id", "densidade", "volume")
                .values(reagenteId, densidade, volume)
                .execute(); //
            
            // Cria o objeto para guardar no vector
            novoReagente = new ReagenteLiquido(reagenteId, nome, dataValidade, quantidade, 
                                               quantidadeCritica, local, nivelAcesso, 
                                               unidade, marca, codRef, densidade, volume); //
            
            std::cout << "Reagente Liquido '" << nome << "' cadastrado com sucesso!\n"; //
        
        } else if (tipo == 2) { 
            db->getTable("ReagenteSolido")
                .insert("id", "massa", "estadoFisico")
                .values(reagenteId, massa, estadoFisico)
                .execute(); //

            // Cria o objeto para guardar no vector
            novoReagente = new ReagenteSolido(reagenteId, nome, dataValidade, quantidade, 
                                              quantidadeCritica, local, nivelAcesso, 
                                              unidade, marca, codRef, massa, estadoFisico); //
            
            std::cout << "Reagente Solido '" << nome << "' cadastrado com sucesso!\n"; //
        }

        //Adiciona o novo objeto criado no vector em memoria
        if (novoReagente) {
            this->reagentes.push_back(novoReagente);
            std::cout << "Reagente adicionado tambem ao vetor em memoria." << std::endl;
        }
        
    } catch (const mysqlx::Error &err) {
        std::cerr << "Erro ao cadastrar reagente: " << err.what() << std::endl; //
    }
}

// Busca reagente por nome
Reagente *Laboratorio::buscarReagente(const std::string &nome)
{
    for (auto reagente : reagentes)
    {
        if (reagente->getNome().find(nome) != std::string::npos)
        {
            return reagente;
        }
    }
    return nullptr;
}
// Se nenhum filtro for fornecido, lista todos os reagentes.
std::vector<Reagente *> Laboratorio::listarReagentes(const std::string &filtroNome)
{
    if (filtroNome.empty())
    {
        return reagentes;
    }
    // Busca reagentes com o nome contido no string
    std::vector<Reagente *> resultado;
    for (auto reagente : reagentes)
    {
        if (reagente->getNome().find(filtroNome) != std::string::npos)
        {
            resultado.push_back(reagente);
        }
    }
    return resultado;
}
// Lista reagentes por local de armazenamento
std::vector<Reagente *> Laboratorio::listarReagentesPorLocal(const std::string &local)
{
    std::vector<Reagente *> resultado;
    for (auto reagente : reagentes)
    {
        if (reagente->getLocalArmazenamento().find(local) != std::string::npos)
        {
            resultado.push_back(reagente);
        }
    }
    return resultado;
}

// Gerenciamento de Retiradas
std::string Laboratorio::registrarRetirada(Usuario *usuario, const std::string &nomeReagente, float quantidade)
{
    Reagente *reagente = buscarReagente(nomeReagente);
    if (!reagente)
    {
        return "Erro: Reagente '" + nomeReagente + "' nao encontrado";
    }

    // Verifica se reagente esta vencido
    if (reagente->estaVencido())
    {
        return "Erro: Reagente '" + reagente->getNome() + "' esta vencido";
    }

    // Cria retirada
    Retirada *novaRetirada = new Retirada(retiradas.size() + 1, usuario, reagente, quantidade);

    // Confirma retirada
    std::string resultado = novaRetirada->confirmarRetirada();

    // Se a retirada na memoria deu certo, atualiza o DB
    if (resultado.find("Erro:") == std::string::npos) 
    {
        try {
            // Pega a nova quantidade do objeto em memoria
            int novaQuantidade = reagente->getQuantidade();
            int reagenteId = reagente->getId();

            // Envia o comando para o banco de dados
            db->getTable("Reagente")
                .modify("id = :id") // Encontra a linha pelo ID
                .set("quantidade", novaQuantidade) // Define a nova quantidade
                .bind("id", reagenteId) // Associa o ID
                .execute();
            
            std::cout << "Banco de dados atualizado para a retirada." << std::endl;

        } catch (const mysqlx::Error &err) {
            std::cerr << "ERRO de DB ao atualizar quantidade: " << err.what() << std::endl;
            // Se o DB falhar, cancela a retirada
            novaRetirada->cancelarRetirada(); //
            delete novaRetirada;
            return "Erro no banco ao registrar retirada. Estoque em memoria revertido.";
        }
        
        // So adiciona a retirada na lista se tudo deu certo
        retiradas.push_back(novaRetirada); //
    }
    else
    {
        // Se a retirada na memoria falhou (ex: sem estoque)
        delete novaRetirada; //
    }

    return resultado; // Retorna a mensagem de sucesso ou erro
}

// Lista todas as retiradas de um usuario especifico
std::vector<Retirada *> Laboratorio::listarRetiradasUsuario(Usuario *usuario)
{
    std::vector<Retirada *> resultado;
    for (auto retirada : retiradas)
    {
        if (retirada->getUsuario()->getId() == usuario->getId())
        {
            resultado.push_back(retirada);
        }
    }
    return resultado;
}
// Retorna as retiradas mais recente
std::vector<Retirada *> Laboratorio::getHistoricoRecente()
{
    int tamanho = retiradas.size();
    int inicio = (tamanho > 10) ? tamanho - 10 : 0;

    std::vector<Retirada *> historico;
    for (int i = inicio; i < tamanho; i++)
    {
        historico.push_back(retiradas[i]);
    }
    return historico;
}

// Gerenciamento de Usuarios
std::string Laboratorio::adicionarUsuario(Usuario *usuario)
{
    // Verifica se usuario ja existe
    for (auto user : usuarios)
    {
        if (user->getId() == usuario->getId())
        {
            return "Usuario '" + usuario->getNome() + "' ja esta no laboratorio";
        }
    }

    usuarios.push_back(usuario);

    // Se for gestor, adiciona a lista de gestores
    if (usuario->getNivelAcesso() == "gestor")
    {
        gestores.push_back(usuario);
    }

    return "Usuario '" + usuario->getNome() + "' adicionado ao laboratorio";
}

std::string Laboratorio::removerUsuario(Usuario *usuario)
{
    // Encontra usuario
    auto it = std::find(usuarios.begin(), usuarios.end(), usuario);
    if (it == usuarios.end())
    {
        return "Usuario '" + usuario->getNome() + "' nao encontrado no laboratorio";
    }

    // Verifica se ha retiradas pendentes
    if (verificarRetiradasPendentes(usuario))
    {
        return "Erro: Usuario '" + usuario->getNome() + "' tem retiradas pendentes";
    }

    usuarios.erase(it);

    // Remove gestores se for o caso
    auto it_gestor = std::find(gestores.begin(), gestores.end(), usuario);
    if (it_gestor != gestores.end())
    {
        gestores.erase(it_gestor);
    }

    return "Usuario '" + usuario->getNome() + "' removido do laboratorio";
}

// Verifica se um usuario tem retiradas pendentes
bool Laboratorio::verificarRetiradasPendentes(Usuario *usuario)
{
    auto retiradasUsuario = listarRetiradasUsuario(usuario);
    for (auto retirada : retiradasUsuario)
    {
        if (!retirada->estaConfirmada())
        {
            return true;
        }
    }
    return false;
}

// Alertas e Monitoramento
// Retorna reagentes em quantidade critica
std::vector<Reagente *> Laboratorio::getReagentesCriticos()
{
    std::vector<Reagente *> criticos;
    for (auto reagente : reagentes)
    {
        if (reagente->getQuantidade() <= reagente->getQuantidadeCritica())
        {
            criticos.push_back(reagente);
        }
    }
    return criticos;
}
// Retorna reagentes vencidos
std::vector<Reagente *> Laboratorio::getReagentesVencidos()
{
    std::vector<Reagente *> vencidos;
    for (auto reagente : reagentes)
    {
        if (reagente->estaVencido())
        {
            vencidos.push_back(reagente);
        }
    }
    return vencidos;
}
// Retorna reagentes proximos do vencimento
std::vector<Reagente *> Laboratorio::getReagentesProximoVencimento(int dias)
{
    std::vector<Reagente *> proximos;
    for (auto reagente : reagentes)
    {
        if (reagente->proximoVencimento(dias))
        {
            proximos.push_back(reagente);
        }
    }
    return proximos;
}
// Gera relatorio de alertas para gestores
std::string Laboratorio::getAlertasGestor()
{
    auto criticos = getReagentesCriticos();
    auto vencidos = getReagentesVencidos();
    auto proximos = getReagentesProximoVencimento();

    std::stringstream alertas;
    alertas << "=== ALERTAS PARA GESTORES ===\n";
    alertas << "Reagentes em quantidade critica: " << criticos.size() << "\n";
    alertas << "Reagentes vencidos: " << vencidos.size() << "\n";
    alertas << "Reagentes proximos do vencimento: " << proximos.size() << "\n";

    return alertas.str();
}

// Informacoes do Laboratorio
std::string Laboratorio::getEstatisticas()
{
    std::stringstream stats;
    stats << "=== ESTATISTICAS DO LABORATORIO ===\n"
          << "Total de reagentes: " << reagentes.size() << "\n"
          << "Total de usuarios: " << usuarios.size() << "\n"
          << "Total de gestores: " << gestores.size() << "\n"
          << "Total de retiradas: " << retiradas.size() << "\n"
          << "Reagentes criticos: " << getReagentesCriticos().size() << "\n"
          << "Reagentes vencidos: " << getReagentesVencidos().size();

    return stats.str();
}

std::string Laboratorio::toString() const
{
    return "Laboratorio " + std::to_string(id) + ": " + nome + " (" + departamento + ")";
}