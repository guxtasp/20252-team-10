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

        static Usuario** usuariosCarregados;
        static int quantidadeUsuarios;
        static int capacidadeUsuarios;

        static Gestor** gestores;
        static int quantidadeGestores;
        static int capacidadeGestores;

        static Estudante** estudantes;
        static int quantidadeEstudantes;
        static int capacidadeEstudantes;

        static PosGraduando** posGraduandos;
        static int quantidadePosGraduandos;
        static int capacidadePosGraduandos;

        // Destrutor
        ~Gestor();

        //Demais funções
        void cadastrarUsuario();
        void deletarUsuario();
        void listarUsuarios();
        static void carregarUsuarios();

        //sets
        void setLaboratorio(Laboratorio* lab);

        //cadastra um novo reagente no laboratorio gerenciado por este gestor
        void cadastrarReagente();

        //O Gestor tem acesso total
        void acessarReagenteRestrito(int idReagente) override;
};

#endif
