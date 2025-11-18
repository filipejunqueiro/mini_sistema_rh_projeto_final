#include "evento.hpp"
#include "models/data/data.hpp"

// Parameterized Constructor
Evento::Evento(const Data& data, const Tipo& tipo) {
	_data = new Data(data);
	_tipo = new Tipo(tipo);
}

// Deep Copy
Evento::Evento(const Evento& evento) {
	_data = new Data(*evento._data);
	_tipo = new Tipo(*evento._tipo);
}

// Deep Assignment
Evento& Evento::operator=(const Evento& evento) {
	if (this != &evento) {
		delete _data;
		delete _tipo;

		_data = new Data(*evento._data);
		_tipo = new Tipo(*evento._tipo);
	}

	return *this;
}

// Destructor
Evento::~Evento() {
	delete _data;
	delete _tipo;
}