#pragma once

#include "models/data/data.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

enum Tipo {
	FERIAS,
	FIM_DE_SEMANA,
	FALTA
};

struct Evento {
	Data* _data = nullptr;
	Tipo* _tipo = nullptr;

	// Default Constructor
	Evento() = default;

	// Parameterized Constructor
	Evento(const Data& data, const Tipo& tipo);

	// Deep Copy
	Evento(const Evento& evento);

	// Deep Assignment
	virtual Evento& operator=(const Evento& evento);

	// Destructor
	~Evento();
};

inline void from_json(const json& j, Evento& evento) {
	delete evento._data;
	delete evento._tipo;

	const auto& data_json = j.at("data");
	const auto& tipo_json = j.at("tipo");

	evento._data = new Data(
		data_json.at("dia").get<int>(),
		data_json.at("mes").get<int>(),
		data_json.at("ano").get<int>()
	);

	evento._tipo = new Tipo(static_cast<Tipo>(tipo_json.get<int>()));
}

inline void to_json(json& j, const Evento& evento) {
	j = {
		{"data", {
			{"dia", *evento._data->_dia},
			{"mes", *evento._data->_mes},
			{"ano", *evento._data->_ano}
		}},
		{"tipo", static_cast<int>(*evento._tipo)}
	};
}