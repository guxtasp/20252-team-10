#ifndef CLASSE_POSGRADUACAO
#define CLASSE_POSGRADUACAO
#include "../Estudante/estudante.h"
#include <string>
using namespace mysqlx;

class PosGraduacao : public Estudante{
    public:
        PosGraduacao();
        // Construtor compatível com Estudante
        PosGraduacao(std::string nome, std::string email, std::string senha, int nivelAcesso, Schema* db,
                    std::string matricula, std::string curso, std::string nivel);
        ~PosGraduacao();
};
#endif