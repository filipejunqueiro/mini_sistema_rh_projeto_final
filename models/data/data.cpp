#include "data.hpp"
#include <stdexcept>

// Parameterized Constructor
Data::Data(const int& dia, const int& mes, const int& ano) {
	if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || ano < 2025)
		throw std::invalid_argument("Data invalida");

	_dia = new int(dia);
	_mes = new int(mes);
	_ano = new int(ano);
}

// Deep Copy
Data::Data(const Data& data) {
	_dia = new int(*data._dia);
	_mes = new int(*data._mes);
	_ano = new int(*data._ano);
}

// Deep Assignment
Data& Data::operator=(const Data& data) {
	if (this != &data) {
		delete _dia;
		delete _mes;
		delete _ano;

		_dia = new int(*data._dia);
		_mes = new int(*data._mes);
		_ano = new int(*data._ano);
	}

	return *this;
}

// Destructor
Data::~Data() {
	delete _dia;
	delete _mes;
	delete _ano;
}

bool Data::sameData(const Data& data) {
	return (*_dia == *data._dia) && (*_mes == *data._mes) && (*_ano == *data._ano);
}

std::string Data::toString() {
	return std::to_string(*_dia) + "/" + std::to_string(*_mes) + "/" + std::to_string(*_ano);
}