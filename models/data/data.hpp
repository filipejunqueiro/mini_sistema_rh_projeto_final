#pragma once

#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct Data {
	int* _dia = nullptr;
	int* _mes = nullptr;
	int* _ano = nullptr;

	// Default Constructor
	Data() = default;

	// Parameterized Constructor
	Data(const int& dia, const int& mes, const int& ano);

	// Deep Copy
	Data(const Data& data);

	// Deep Assignment
	Data& operator=(const Data& data);

	// Destructor
	~Data();

	virtual bool sameData(const Data& data);

	virtual std::string toString();
};

// JSON Serialization

inline void to_json(json& j, const Data& data) {
	j = {
		{"dia", *data._dia},
		{"mes", *data._mes},
		{"ano", *data._ano}
	};
}

inline void from_json(const json& j, Data& data) {
	delete data._dia;
	delete data._mes;
	delete data._ano;

	data._dia = new int(j.at("dia").get<int>());
	data._mes = new int(j.at("mes").get<int>());
	data._ano = new int(j.at("ano").get<int>());
}