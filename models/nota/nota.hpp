#pragma once

#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct Nota
{
	std::string *_titulo = nullptr;
	std::string *_conteudo = nullptr;

	// Default Constructor
	Nota() = default;
	// Parameterized Constructor
	Nota(const std::string &titulo, const std::string &conteudo);

	// Deep Copy
	Nota(const Nota &nota);

	// Deep Assignment
	virtual Nota &operator=(const Nota &nota);

	// Destructor
	~Nota();
};

// JSON Serialization

inline void to_json(json &j, const Nota &nota)
{
	j = {
			{"titulo", *nota._titulo},
			{"conteudo", *nota._conteudo}
	};
}

inline void from_json(const json &j, Nota &nota)
{
	delete nota._titulo;
	delete nota._conteudo;

	nota._titulo = new std::string(j.at("titulo").get<std::string>());
	nota._conteudo = new std::string(j.at("conteudo").get<std::string>());
}