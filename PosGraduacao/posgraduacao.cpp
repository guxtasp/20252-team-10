#include "posgraduacao.h"

// Construtor sem parâmetros
PosGraduacao::PosGraduacao() : Estudante("", "", "", 0, nullptr, "", "", "") {}
// Construtor
PosGraduacao::PosGraduacao(std::string nome, std::string email, std::string senha, int nivelAcesso, 
    Schema* db,std::string matricula, std::string curso, std::string nivel)
    : Estudante(nome, email, senha, nivelAcesso, db, matricula, curso, nivel) {}

PosGraduacao::~PosGraduacao() {}
