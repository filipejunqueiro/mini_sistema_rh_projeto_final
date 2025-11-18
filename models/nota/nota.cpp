#include "nota.hpp"

// Parameterized Constructor
Nota::Nota(const std::string &titulo, const std::string &conteudo)
{
	_titulo = new std::string(titulo);
	_conteudo = new std::string(conteudo);
}

// Deep Copy
Nota::Nota(const Nota &nota)
{
	_titulo = new std::string(*nota._titulo);
	_conteudo = new std::string(*nota._conteudo);
}

// Deep Assignment
Nota &Nota::operator=(const Nota &nota)
{
	if (this != &nota)
	{
		delete _titulo;
		delete _conteudo;
		_titulo = new std::string(*nota._titulo);
		_conteudo = new std::string(*nota._conteudo);
	}
	return *this;
}

// Destructor

Nota::~Nota()
{
	delete _titulo;
	delete _conteudo;
}