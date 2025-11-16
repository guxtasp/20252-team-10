#include "../Usuario/usuario.h"
#include "../Gestor/gestor.h"
#include <string>
#include <iostream>

// Construtor
Gestor::Gestor(std::string nome, std::string email, std::string senha, int nivelAcesso, Schema* db) : 
    Usuario(nome, email, senha, nivelAcesso, db) {

    //Inicializa o ponteiro como nulo 
    this->_meuLaboratorio = nullptr;
    };

// Destrutor
Gestor::~Gestor() {}

// Demais funções
void Gestor::cadastrarUsuario() {
    // Pega os dados básicos do novo usuário
    std::cout << "Digite o tipo de usuario a cadastrar:\n";
    std::cout << "1. Gestor\n2. Pos-Graduando\n3. Aluno de Graduacao\n";
    int nivelAcesso; // Nivel de acesso aos Reagentes, 1 = Gestor, 2 = Graduacao e 3 = pos Graduacao
    std::cin >> nivelAcesso; 

    std::string nome, email, senha;
    std::string matricula, curso, nivel;
    int nivelInt; // Nivel do estudante (Graducao ou Pos-Graduacao)
    
    std::cout << "Digite o nome do usuário: ";
    std::cin >> nome;
    std::cout << "Digite o email do usuário: ";
    std::cin >> email;
    std::cout << "Digite a senha do usuário: ";
    std::cin >> senha;

    if (nivelAcesso == 2 || nivelAcesso == 3) {
        std::cout << "Digite a matricula do usuário " << nome << " :";
        std::cin >> matricula;
        std::cout << "Digite o curso do usuário " << nome << " :";
        std::cin >> curso;
        std::cout << "Digite o numero correspondente ao nivel(1-Graduacao, 2-Mestrado, 3-Doutorado, 4-PosDoutorado): ";
        std::cin >> nivelInt;
        switch(nivelInt){
            case 1: nivel = "Graduacao"; break;
            case 2: nivel = "Mestrado"; break;
            case 3: nivel = "Doutorado"; break;
            case 4: nivel = "PosDoutorado"; break;
            default: nivel = "Desconhecido"; break;
        }
    }
    try {
        Table usuarioTable = db->getTable("Usuario"); // Insire na tabela Usuario (base)
        Result res = usuarioTable.insert("nome", "email", "senha", "nivelAcesso") //Insere os dados básicos
            .values(nome, email, senha, nivelAcesso).execute(); // Define os valores e executa a inserção

        // Recupera o ID do usuário recém-inserido
        int usuarioId = res.getAutoIncrementValue();

        // Insere nas tabelas especializadas conforme o nivelAcesso
        if (nivelAcesso == 1) { // Gestor
            db->getTable("Gestor") // Insere na tabela Gestor
                .insert("id", "cadastrado_por_gestor_id") // Id do gestor e quem cadastrou
                .values(usuarioId, getId()) // quem cadastrou é o gestor atual
                .execute();
            std::cout << "Gestor " << nome << " cadastrado com sucesso!\n"; // Mensagem de sucesso

        } else if (nivelAcesso == 2 || nivelAcesso == 3) { // Pos-Graduando ou Aluno de Graduacao
            db->getTable("Estudante") // Insere na tabela Estudante
                .insert("id", "matricula", "curso", "nivel", "cadastrado_por_gestor_id") // Dados do estudante
                .values(usuarioId, matricula, curso, nivel, getId()) // quem cadastrou é o gestor atual
                .execute(); // Executa a inserção

            if (nivelAcesso == 2) { // Pos-Graduando
                db->getTable("PosGraduacao") // Insere na tabela PosGraduacao
                    .insert("id") // Apenas o ID do usuário
                    .values(usuarioId) // Define o valor do ID
                    .execute(); // Executa a inserção
                std::cout << "Pós-Graduando " << nome << " cadastrado com sucesso!\n"; // Mensagem de sucesso
            } else {
                std::cout << "Aluno de Graduação " << nome << " cadastrado com sucesso!\n"; // Mensagem de sucesso
            }
        }

    } catch (const mysqlx::Error &err) {
        std::cerr << "Erro ao cadastrar usuário: " << err.what() << std::endl;
    }
}

void Gestor::deletarUsuario() {
    std::cout << "Digite o email do usuário a ser deletado: ";
    std::string email;
    std::cin >> email;

    try{
        //Busca o usuário no banco de dados
        Table usuarioTable = db->getTable("Usuario");
        RowResult result = usuarioTable.select("id", "nivelAcesso")
                                .where("email = :email")
                                .bind("email", email)
                                .execute();
        
        //Caso não encontre o usuário com o email fornecido                      
        if (result.count() == 0) {
            std::cout << "Usuário com email " << email << " não encontrado." << std::endl;
            return;
        }
        Row row = result.fetchOne();//pega a primeira linha do resultado
        int usuarioId = row[0];// ID do usuário a ser deletado
        int nivelAcesso = row[1];// Nível de acesso do usuário a ser deletado
        
        //Deleta das tabelas especializadas conforme o tipo de usuário
        if (nivelAcesso == 1) { // Gestor
            db->getTable("Gestor")
                .remove()
                .where("id = :id")
                .bind("id", usuarioId)
                .execute();
        } else if (nivelAcesso == 2 || nivelAcesso == 3) { // Pos-Graduando ou Aluno de Graduacao
            db->getTable("Estudante")
                .remove()
                .where("id = :id")
                .bind("id", usuarioId)
                .execute();

            if (nivelAcesso == 2) { // Pos-Graduando
                db->getTable("PosGraduacao")
                    .remove()
                    .where("id = :id")
                    .bind("id", usuarioId)
                    .execute();
            }
        }
        //Deleta da tabela base Usuario
        usuarioTable.remove()
                    .where("id = :id")
                    .bind("id", usuarioId)
                    .execute();
        
        std::cout << "Usuário deletado com sucesso!" << std::endl;// Mensagem de sucesso
    } catch (const mysqlx::Error &err) {
        std::cerr << "Erro ao deletar usuário: " << err.what() << std::endl;//mensagem de erro
    }
}


void Gestor::listarUsuarios() {
    std::cout << "\n=== LISTANDO USUÁRIOS (via Gestor) ===" << std::endl;
    std::cout << "Nome do Gestor: " << getNome() << std::endl;
    std::cout << "Email do Gestor: " << getEmail() << std::endl;
    std::cout << "Nível de Acesso: " << getNivelAcesso() << std::endl;
    std::cout << "========================================\n" << std::endl;
}

// Metodo para cadastrar reagente
void Gestor::cadastrarReagente() {
    // Variaveis para guardar os dados da tabela base Reagente
    std::string nome, dataValidade, local, unidade, marca, codRef;
    int quantidade, quantidadeCritica, nivelAcesso;
    
    std::cout << "Cadastro de Novo Reagente \n";
    std::cout << "Nome: ";
    std::cin.ignore(); // Ignora o 'Enter' anterior
    std::getline(std::cin, nome);
    std::cout << "Data de Validade (AAAA-MM-DD): ";
    std::cin >> dataValidade;
    std::cout << "Quantidade: ";
    std::cin >> quantidade;
    std::cout << "Quantidade Critica: ";
    std::cin >> quantidadeCritica;
    std::cout << "Local de Armazenamento: ";
    std::cin.ignore();
    std::getline(std::cin, local);
    std::cout << "Nivel de Acesso (1, 2 ou 3): ";
    std::cin >> nivelAcesso;
    std::cout << "Unidade de Medida (ex: 'ml', 'g'): ";
    std::cin >> unidade;
    std::cout << "Marca: ";
    std::cin.ignore();
    std::getline(std::cin, marca);
    std::cout << "Codigo de Referencia: ";
    std::cin >> codRef;

    std::cout << "Tipo de Reagente:\n";
    std::cout << "Digite 1 se for Liquido \n Digite 2 se for Solido \n";
    int tipo;
    std::cin >> tipo;

    // Declara as variaveis de tipo
    //densidade e volume so serao usadas se tipo == 1
    // massa e estadoFisico so serao usadas se tipo == 2
    double densidade = 0.0;
    double volume = 0.0;
    double massa = 0.0;
    std::string estadoFisico; 

    if (tipo == 1) { // Liquido
        std::cout << "Densidade: ";
        std::cin >> densidade;
        std::cout << "Volume: ";
        std::cin >> volume;
        
    } else if (tipo == 2) { // Solido
        std::cout << "Massa: ";
        std::cin >> massa;
        
        // Pergunta o estado fisico que é um atributo da classe ReagenteSolido
        std::cout << "Estado Fisico (ex: 'po', 'cristal'): ";
        std::cin.ignore(); // Ignora o 'Enter' da leitura da massa
        std::getline(std::cin, estadoFisico);
    }
    // 3. Salva no Banco de Dados
    try {
        // Insere na tabela base "Reagente"
        // O 'db' e herdado de Usuario e esta disponivel aqui
        Table reagenteTable = db->getTable("Reagente");
        
        // Monta a query de insercao com os dados basicos
        Result res = reagenteTable.insert(
            "nome", "dataValidade", "quantidade", "quantidadeCritica", 
            "localArmazenamento", "nivelAcesso", "unidadeMedida", "marca", "codigoReferencia"
        ).values(nome, dataValidade, quantidade, quantidadeCritica, 
                 local, nivelAcesso, unidade, marca, codRef)
         .execute(); // Executa a insercao no DB
        
        // Recupera o ID do reagente que acabou de ser criado
        // (Precisamos desse ID para ligar com a tabela Liquido/Solido)
        int reagenteId = res.getAutoIncrementValue();

        // Insere nas tabelas especializadas (Liquido ou Solido)
        if (tipo == 1) { 
            // Se for liquido, insere na tabela 'ReagenteLiquido'
            db->getTable("ReagenteLiquido")
                .insert("id", "densidade", "volume")
                .values(reagenteId, densidade, volume)
                .execute();
            
            // Imprime a confirmacao para o usuario
            std::cout << "Reagente Liquido '" << nome << "' cadastrado com sucesso!\n";
        
        } else if (tipo == 2) { 
            // Se for solido, insere na tabela 'ReagenteSolido'
            db->getTable("ReagenteSolido")
                .insert("id", "massa", "estadoFisico")
                .values(reagenteId, massa, estadoFisico)
                .execute();
            
            // Imprime a confirmacao para o usuario
            std::cout << "Reagente Solido '" << nome << "' cadastrado com sucesso!\n";
        }
        
    } catch (const mysqlx::Error &err) {
        // Se qualquer operacao do 'try' falhar, captura o erro
        // (Ex: se o banco estiver offline ou a tabela nao existir)
        std::cerr << "Erro ao cadastrar reagente: " << err.what() << std::endl;
    }
}

// Implementação da função virtual. O Gestor ignora a checagem de nível.
void Gestor::acessarReagenteRestrito(int idReagente) {
    
    std::cout << "\n(Gestor) Acessando Reagente ID: " << idReagente << "\n";
    if (db == nullptr) {
        std::cerr << "ERRO: Gestor não está conectado ao banco." << std::endl;
        return;
    }

    try {
        Table reagenteTable = db->getTable("Reagente");
        
        // Busca o reagente pelo ID
        RowResult res = reagenteTable.select(
            "id", "nome", "quantidade", "unidadeMedida", 
            "localArmazenamento", "dataValidade", "nivelAcesso"
        ).where("id = :id").bind("id", idReagente).execute();

        if (res.count() == 0) {
            std::cout << "Reagente com ID " << idReagente << " não encontrado." << std::endl;
            return;
        }

        // Pega os detalhes
        Row row = res.fetchOne();

        // O Gestor imprime tudo (não há checagem de nível)
        std::cout << "Acesso Permitido (Nível Gestor):\n";
        std::cout << "Nome:    " << row[1].get<std::string>() << "\n";
        std::cout << "Qtde:    " << row[2].get<int>() << " " << row[3].get<std::string>() << "\n";
        std::cout << "Local:   " << row[4].get<std::string>() << "\n";
        std::cout << "Validade: " << row[5].get<std::string>() << "\n";
        std::cout << "(Nível do reagente: " << row[6].get<int>() << ")\n";
        std::cout << "-------------------------------------\n";

    } catch (const mysqlx::Error &err) {
        std::cerr << "Erro ao acessar reagente: " << err.what() << std::endl;
    }
}