#ifndef CLASSE_GESTOR
#define CLASSE_GESTOR

#include "../Usuario/usuario.h"
#include <string>
#include <mysql-cppconn-8/mysqlx/xdevapi.h>
#include "../Reagente/Reagente.h"
#include "../Laboratorio/Laboratorio.h"


using namespace mysqlx;

class Gestor : public Usuario{
    private:
    // O laboratorio que este Gestor gerencia
    Laboratorio* _meuLaboratorio;
    public:
        //Construtor
        Gestor(std::string nome, std::string email, std::string senha, int nivelAcesso, Schema* db);

        // Destrutor
        ~Gestor();

        //Demais funções
        void cadastrarUsuario();
        void deletarUsuario();
        void listarUsuarios();

        //sets
        void setLaboratorio(Laboratorio* lab);

        //cadastra um novo reagente no laboratorio gerenciado por este gestor
        void cadastrarReagente(Reagente* novoReagente);
};

#endif
