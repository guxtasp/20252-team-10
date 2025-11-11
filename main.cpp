#include "DatabaseConnection/databaseConnection.h"
#include "Gestor/gestor.h"
#include <iostream>

int main() {
    try{
    // ===================== Configuração do Banco de Dados =====================
    //Estabelece a conexão com o banco de dados
    DatabaseConnection conexaoDB; 
    Session* session = conexaoDB.getSession(); // Obtém a sessão de conexão
    Schema* db = conexaoDB.getSchema(); // Obtém o esquema do banco de dados
    // Verifica se a conexão e o esquema foram inicializados corretamente
    if (!session || !db) {
            throw std::runtime_error("Falha ao inicializar a conexão com o banco de dados.");
    }

    // ===================== Inicializando o Gestor Admin =====================
    std::cout << "Instanciando Gestor Admin..." << std::endl;
    Gestor gestorAdmin("Admin", "admin@example.com", "senha123", 1, db); // Gestor administrador inicial
    Table usuariosTabela = db->getTable("Usuario"); // Consulta a tabela de usuários
    RowResult resultado = usuariosTabela // Busca por um usuário com o email do gestorAdmin por consulta (SELECT)
            .select("id") // Seleciona o campo id
            .where("email = :email AND nivelAcesso = 1") // Filtra para consultar por meio do email e nivel de acesso (gestores)
            .bind("email", gestorAdmin.getEmail())// Substitui o parâmetro :email ao email do gestorAdmin
            .execute(); // Executa a consulta
    if (resultado.count() == 0) { // Se nenhum gestor foi encontrado
        std::cout << "Nenhum gestor encontrado no banco. Cadastrando gestor padrão..." << std::endl; // Informa que vai cadastrar o gestor padrão
        try{
            //cadastra o gestorAdmin no banco de dados
            Table usuarioTable = db->getTable("Usuario"); // Insere na tabela Usuario (base)
            Result res = usuarioTable.insert("nome", "email", "senha", "nivelAcesso") //Insere os dados básicos
                .values(gestorAdmin.getNome(), gestorAdmin.getEmail(), "senha123", 1).execute(); // Define os valores e executa a inserção   
            int usuarioId = res.getAutoIncrementValue(); // Recupera o ID do usuário recém-inserido
            db->getTable("Gestor") // Insere na tabela Gestor
                .insert("id", "cadastrado_por_gestor_id") // Id do gestor e quem cadastrou
                .values(usuarioId, usuarioId) // quem cadastrou é ele mesmo
                .execute();
            std::cout << "Gestor padrão cadastrado com sucesso! Email: " << gestorAdmin.getEmail() << " Senha: senha123\n"; // Mensagem de sucesso
        } catch (const Error &err) {
            std::cerr << "Erro ao cadastrar gestor: " << err.what() << std::endl;
        }
    } else {
        Row row = resultado.fetchOne(); // Obtém a primeira linha do resultado
        gestorAdmin.setId(row[0]);  // atualiza o ID do objeto gestorAdmin com o ID do banco de dados
        std::cout << "Gestor já existe no banco. ID: " 
                      << gestorAdmin.getId() << std::endl; // Informa que o gestor já existe
    }
    } catch (const mysqlx::Error &err) {
        std::cerr << "Erro MySQL: " << err.what() << std::endl;
    } catch (const std::exception &ex) {
        std::cerr << "Erro genérico: " << ex.what() << std::endl;
    } catch (...) {
        std::cerr << "Erro desconhecido ao executar o programa." << std::endl;
    }
    // ===================== Sistema =====================











    return 0;
}