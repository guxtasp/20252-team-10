#include "estudante.h"
#include <iostream>
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
    nivel = nivel; 
}