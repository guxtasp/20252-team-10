#ifndef CLASSE_GESTOR
#define CLASSE_GESTOR

#include "../Usuario/usuario.h"
#include "../Laboratorio/Laboratorio.h"
#include <string>
#include <mysql-cppconn/mysqlx/xdevapi.h>
using namespace mysqlx;

class Gestor : public Usuario{
    private:
        Laboratorio * laboratorio = nullptr;
    public:
        //Construtor
        Gestor(std::string nome, std::string email, std::string senha, int nivelAcesso, Schema* db);

        // Destrutor
        ~Gestor();

        //Demais funções
        void cadastrarUsuario();
        void deletarUsuario();
        void listarUsuarios();
        void associarLaboratorio();
        void associarEstudanteAoLaboratorio(Estudante* estudante, int idLaboratorio, const std::string& papel);
};

#endif
