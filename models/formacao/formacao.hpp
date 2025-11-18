#pragma once

#include "models/data/data.hpp"
#include <string>

struct Formacao {
	std::string *_nome = nullptr;
	Data *_data_conclusao = nullptr;

	// Default Constructor
	Formacao() = default;

	// Parameterized Constructor
	Formacao(const std::string& nome, const Data& data_conclusao);

	// Deep Copy
	Formacao(const Formacao& formacao);

	// Deep Assignment
	Formacao& operator=(const Formacao& formacao);

	// Destructor
	~Formacao();
};

// JSON Serialization

inline void to_json(nlohmann::json& j, const Formacao& formacao)
{
	j = {
		{"nome", *formacao._nome},
		{"data_conclusao", *formacao._data_conclusao}
	};
}

inline void from_json(const nlohmann::json& j, Formacao& formacao)
{
	delete formacao._nome;
	delete formacao._data_conclusao;

	formacao._nome = new std::string(j.at("nome").get<std::string>());
	formacao._data_conclusao = new Data(j.at("data_conclusao").get<Data>());
}