#pragma once

#include "models/colaborador/colaborador.hpp"
#include <nlohmann/json.hpp>

using nlohmann::json;

struct RH {
	Colaborador* _colaboradores = nullptr;
	size_t _count = 0;

	// Default Constructor
	RH() = default;

	// Parameterized Constructor
	RH(const Colaborador& colaborador);

	// Deep Copy
	RH(const RH& rh);

	// Deep Assignment
	virtual RH& operator=(const RH& rh);

	// Destructor
	~RH();

	virtual void addColaborador(const Colaborador& colaborador);

	virtual void removeColaborador(const size_t& idx);

	virtual void removeColaboradorPorNome(const std::string& nome);
};

// JSON Serialization

inline void to_json(json& j, const RH& rh) {
	j = json::array();

	for (size_t i = 0; i < rh._count; ++i)
		j.push_back(rh._colaboradores[i]);
}

inline void from_json(const json& j, RH& rh) {
	delete[] rh._colaboradores;

	rh._count = j.size();
	rh._colaboradores = new Colaborador[rh._count];

	for (size_t i = 0; i < rh._count; ++i)
		rh._colaboradores[i] = j.at(i).get<Colaborador>();
}