#pragma once

#include "models/data/data.hpp"
#include "models/evento/evento.hpp"
#include "models/nota/nota.hpp"
#include "models/formacao/formacao.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <sodium.h>

using nlohmann::json;

struct Colaborador
{
	std::string _departamento = "";
	std::string *_nome = nullptr;
	Evento *_eventos = nullptr;
	size_t _eventos_count = 0;
	Nota* _notas = nullptr;
	size_t _notas_count = 0;
	Formacao* _formacoes = nullptr;
	size_t _formacoes_count = 0;
	
	// Default Constructor
	Colaborador() = default;

	// Parameterized Constructors
	Colaborador(const std::string &nome);

	// Deep Copy
	Colaborador(const Colaborador &colaborador);

	// Deep Assignment
	virtual Colaborador &operator=(const Colaborador &colaborador);

	// Destructor
	~Colaborador();

	virtual bool sameColaborador(const std::string &nome);

	virtual void addEvento(const Evento &evento);

	virtual void removeEvento(const size_t &idx);

	virtual void removeEventoPorData(const Data &data);

	virtual void addNota(const Nota& nota);

	virtual void removeNota(const size_t& idx);

	virtual void addFormacao(const Formacao& formacao);

	virtual void removeFormacao(const size_t& idx);
};

// Sodium

inline char *encodeBase64(const unsigned char *data, size_t len)
{
	size_t outLen = sodium_base64_ENCODED_LEN(
			len,
			sodium_base64_VARIANT_ORIGINAL);

	char *out = (char *)malloc(outLen);
	if (!out)
		return nullptr;

	sodium_bin2base64(
			out,
			outLen,
			data,
			len,
			sodium_base64_VARIANT_ORIGINAL);

	return out;
}

inline unsigned char *decodeBase64(const char *b64, size_t &outSize)
{
	size_t b64Len = strlen(b64);

	unsigned char *out = (unsigned char *)malloc(b64Len);
	if (!out)
		return nullptr;

	if (sodium_base642bin(
					out,
					b64Len,
					b64,
					b64Len,
					nullptr,
					&outSize,
					nullptr,
					sodium_base64_VARIANT_ORIGINAL) != 0)
	{
		free(out);
		outSize = 0;
		return nullptr;
	}

	return out;
}

// JSON Serialization

inline void to_json(json &j, const Colaborador &colaborador)
{
	const std::string &nome = *colaborador._nome;

	const unsigned char *raw = (const unsigned char *)nome.c_str();
	size_t len = nome.size();

	char *encoded = encodeBase64(raw, len);

	j = {
		{"departamento", std::string(colaborador._departamento)},
		{"nome", std::string(encoded)},
		{"eventos", json::array()},
		{"notas", json::array()},
		{"formacoes", json::array()}
	};


	free(encoded);

	for (size_t i = 0; i < colaborador._eventos_count; ++i)
		j["eventos"].push_back(colaborador._eventos[i]);
	
	for (size_t i = 0; i < colaborador._notas_count; ++i)
		j["notas"].push_back(colaborador._notas[i]);

	for (size_t i = 0; i < colaborador._formacoes_count; ++i)
		j["formacoes"].push_back(colaborador._formacoes[i]);
}

inline void from_json(const json &j, Colaborador &colaborador)
{
	if (colaborador._nome)
	{
		delete colaborador._nome;
		colaborador._nome = nullptr;
	}

	if (colaborador._eventos)
	{
		delete[] colaborador._eventos;
		colaborador._eventos = nullptr;
	}

	colaborador._departamento = std::string(j.at("departamento").get<std::string>());

	std::string encoded = j.at("nome").get<std::string>();

	size_t outSize = 0;
	unsigned char *raw = decodeBase64(encoded.c_str(), outSize);

	if (!raw)
	{
		colaborador._nome = new std::string("");
	}
	else
	{
		colaborador._nome = new std::string((char *)raw, outSize);
		free(raw);
	}

	const auto &eventos_arr = j.at("eventos");
	colaborador._eventos_count = eventos_arr.size();
	colaborador._eventos = new Evento[colaborador._eventos_count];

	for (size_t i = 0; i < colaborador._eventos_count; ++i)
		colaborador._eventos[i] = eventos_arr.at(i).get<Evento>();

	const auto &notas_arr = j.at("notas");
	colaborador._notas_count = notas_arr.size();
	colaborador._notas = new Nota[colaborador._notas_count];

	for (size_t i = 0; i < colaborador._notas_count; ++i)
		colaborador._notas[i] = notas_arr.at(i).get<Nota>();

	const auto& formacoes_arr = j.at("formacoes");
	colaborador._formacoes_count = formacoes_arr.size();
	colaborador._formacoes = new Formacao[colaborador._formacoes_count];

	for (size_t i = 0; i < colaborador._formacoes_count; ++i)
		colaborador._formacoes[i] = formacoes_arr.at(i).get<Formacao>();
}
