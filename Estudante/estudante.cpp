#include "estudante.h"
#include <iostream>
// Construtor
Estudante::Estudante(std::string nome, std::string email, std::string senha, int nivelAcesso,
                        std::string matricula, std::string curso, std::string nivel) :
    Usuario(nome, email, senha, nivelAcesso), matricula(matricula), curso(curso), nivel(nivel) {}