#ifndef CLASSE_GESTOR
#define CLASSE_GESTOR

#include "../Usuario/usuario.h"
#include "../Laboratorio/Laboratorio.h"
#include <string>
#include <mysql-cppconn/mysqlx/xdevapi.h>
#include "../Reagente/Reagente.h"
#include "../Laboratorio/Laboratorio.h"


using namespace mysqlx;

class Gestor : public Usuario{
    private:
    Laboratorio * laboratorio; // O laboratorio que este Gestor gerencia
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
        //sets
        void setLaboratorio(Laboratorio* lab);
        //cadastra um novo reagente no laboratorio gerenciado por este gestor
        void cadastrarReagente();
        //O Gestor tem acesso total
        void acessarReagenteRestrito(int idReagente) override;
};

#endif
