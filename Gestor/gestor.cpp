#include "../Usuario/usuario.h"
#include "../Gestor/gestor.h"
#include <string>
#include <iostream>

// Construtor
Gestor::Gestor(std::string nome, std::string email, std::string senha, int nivelAcesso, Schema* db) : 
    Usuario(nome, email, senha, nivelAcesso, db) {};

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
