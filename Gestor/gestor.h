#ifndef CLASSE_GESTOR
#define CLASSE_GESTOR

#include "../Usuario/usuario.h"
#include <string>
#include <mysql-cppconn-8/mysqlx/xdevapi.h>
using namespace mysqlx;

class Gestor : public Usuario{
    private:
        Schema* db; // Ponteiro para o esquema do banco de dados    
    public:
        //Construtor
        Gestor(std::string nome, std::string email, std::string senha, int nivelAcesso, Schema* database);

        // Destrutor
        ~Gestor();

        //Demais funções
        void cadastrarUsuario();
        void deletarUsuario();
        void listarUsuarios();
};

#endif
