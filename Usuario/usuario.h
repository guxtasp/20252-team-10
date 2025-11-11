#ifndef USUARIO_H
#define USUARIO_H

#include <string>

class Usuario{
    private:
        std::string senha; 
        std::string nome;
        std::string email;
        int id;
        int nivelAcesso;
    public:
        //Construtor
        Usuario(int id, std::string nome, std::string email, std::string senha, int nivelAcesso);
        //Destrutor
        ~Usuario();

        // Gets
        virtual std::string getNome() const;
        virtual std::string getEmail() const;
        virtual int getId() const;
        virtual int getNivelAcesso() const;

        //Sets
        virtual void setNome(std::string nome);
        virtual void setEmail(std::string email);
        virtual void setId(int id);
        virtual void setNivelAcesso(int nivelAcesso);

        // Demais 
        virtual void detalharPerfil();
        virtual void fazerLogin();
        bool verificarSenha(std::string senha);
        void alterarSenha(std::string senhaAntiga, std::string senhaNova);
};

#endif
