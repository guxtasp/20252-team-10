#include "databaseConnection.h"
// Construtor e faz a conexao com o banco de dados
DatabaseConnection::DatabaseConnection() : session(nullptr), db(nullptr) {
    try {
        // Tenta estabelecer a sessão
        session = new Session(HOST, PORT, USER, PASSWORD);
        // Seleciona o schema (Banco de Dados)
        Schema schema = session->getSchema(SCHEMA);
        db = new Schema(schema);
        std::cout << "Conexão MySQL estabelecida com sucesso!" << std::endl;

    } catch (const Error &err) {
        // Em caso de erro, exibe a mensagem
        std::cerr << "Erro ao conectar ao MySQL: " << err.what() << std::endl;
        //Se a conexão falhar, garante que os ponteiros sejam nulos
        if (session) {
            delete session; //Deleta a sessão se foi alocada
            session = nullptr; //Define o ponteiro como nulo
        }
        if (db) { //Deleta o schema se foi alocado
            delete db; //Libera a memória
            db = nullptr; //Define o ponteiro como nulo
        }
    }
}

// O destrutor garante que a sessão seja fechada quando o objeto for destruído
DatabaseConnection::~DatabaseConnection() {
    // Libera o schema se alocado
    if (db) {
        delete db;
    }  
    if (session) { // Se estiver uma sessão ativa
        session->close();  // Fecha a sessão
        delete session; // Libera a memória
        std::cout << "Conexão MySQL fechada." << std::endl;
    }
}


//Getters
// Retorna a sessão ativa
Session* DatabaseConnection::getSession() {
    return session;
}

// Retorna o schema (Banco de Dados) selecionado
Schema* DatabaseConnection::getSchema() {
    // Verifica se o schema foi inicializado
    if (db) {
        return db; // Retorna o ponteiro para o schema
    }

    throw Error("Nenhum banco de dados disponível"); // Caso não esteja inicializado ou dê algum erro,
    // retorna que o esquema de banco de dados não está disponível, provavelmente por falha na conexão ou não foi criado
}