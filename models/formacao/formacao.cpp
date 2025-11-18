#include "formacao.hpp"
#include "models/data/data.hpp"

// Parameterized Constructor
Formacao::Formacao(const std::string& nome, const Data& data_conclusao) {
	_nome = new std::string(nome);
	_data_conclusao = new Data(data_conclusao);
}

// Deep Copy
Formacao::Formacao(const Formacao& formacao) {
	_nome = new std::string(*formacao._nome);
	_data_conclusao = new Data(*formacao._data_conclusao);
}

// Deep Assignment
Formacao& Formacao::operator=(const Formacao& formacao) {
	if (this != &formacao) {
		delete _nome;
		delete _data_conclusao;

		_nome = new std::string(*formacao._nome);
		_data_conclusao = new Data(*formacao._data_conclusao);
	}

	return *this;
}

// Destructor
Formacao::~Formacao() {
	delete _nome;
	delete _data_conclusao;
}