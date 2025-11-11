#include "../Usuario/usuario.h"
#include <iostream>

// Construtor
// ID corresponde a -1 quando o usuário ainda não foi salvo no banco de dados, assim que ao salvar,
// o ID será atualizado com o valor do banco de dados
Usuario::Usuario(int id, std::string nome, std::string email, std::string senha, int nivelAcesso)
    : id(-1), nome(nome), email(email), senha(senha), nivelAcesso(nivelAcesso) {}
    

// Destrutor
Usuario::~Usuario() {}

// Gets
std::string Usuario::getNome() const {
    return nome;
}

std::string Usuario::getEmail() const {
    return email;
}

int Usuario::getId() const {
    return id;
}

int Usuario::getNivelAcesso() const {
    return nivelAcesso;
}

// Sets
void Usuario::setNome(std::string nome) {
    this->nome = nome;
}

void Usuario::setEmail(std::string email) {
    this->email = email;
}

void Usuario::setId(int id) {
    this->id = id;
}

void Usuario::setNivelAcesso(int nivelAcesso) {
    this->nivelAcesso = nivelAcesso;
}

// Demais métodos
void Usuario::detalharPerfil() {
    std::cout << "Nome: " << nome << std::endl;
    std::cout << "Email: " << email << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "Nível de Acesso: " << nivelAcesso << std::endl;
}

void Usuario::fazerLogin() {
    std::cout << "Usuário " << nome << " fez login com sucesso!" << std::endl;
}

bool Usuario::verificarSenha(std::string senha) {
    return this->senha == senha;
}

void Usuario::alterarSenha(std::string senhaAntiga, std::string senhaNova) {
    if (verificarSenha(senhaAntiga)) {
        this->senha = senhaNova;
        std::cout << "Senha alterada com sucesso!" << std::endl;
    } else {
        std::cout << "Senha antiga incorreta!" << std::endl;
    }
}
