#include "colaborador.hpp"

// Parameterized Constructors
Colaborador::Colaborador(const std::string& nome) {
	_nome = new std::string(nome);
}

// Deep Copy
Colaborador::Colaborador(const Colaborador& colaborador) {
	_departamento = std::string(colaborador._departamento);
	_nome = new std::string(*colaborador._nome);

	// Eventos

	_eventos_count = colaborador._eventos_count;
	_eventos = new Evento[_eventos_count];

	for (size_t i = 0;i < _eventos_count;++i)
		_eventos[i] = colaborador._eventos[i];

	// Notas

	_notas_count = colaborador._notas_count;
	_notas = new Nota[_notas_count];

	for (size_t i = 0; i < _notas_count; ++i)
		_notas[i] = colaborador._notas[i];

	// Formações

	_formacoes_count = colaborador._formacoes_count;
	_formacoes = new Formacao[_formacoes_count];

	for (size_t i = 0; i < _formacoes_count; ++i)
		_formacoes[i] = colaborador._formacoes[i];
}

// Deep Assignment
Colaborador& Colaborador::operator=(const Colaborador& colaborador) {
	if (this != &colaborador) {
		delete _nome;
		delete[] _eventos;
		delete[] _notas;
		delete[] _formacoes;

		_departamento = std::string(colaborador._departamento);
		_nome = new std::string(*colaborador._nome);

		// Eventos

		_eventos_count = colaborador._eventos_count;
		_eventos = new Evento[_eventos_count];

		for (size_t i = 0; i < _eventos_count; ++i)
			_eventos[i] = colaborador._eventos[i];

		// Notas

		_notas_count = colaborador._notas_count;
		_notas = new Nota[_notas_count];

		for (size_t i = 0; i < _notas_count; ++i)
			_notas[i] = colaborador._notas[i];

		// Formações

		_formacoes_count = colaborador._formacoes_count;
		_formacoes = new Formacao[_formacoes_count];

		for (size_t i = 0; i < _formacoes_count; ++i)
			_formacoes[i] = colaborador._formacoes[i];
	}

	return *this;
}

// Destructor
Colaborador::~Colaborador() {
	delete _nome;
	delete[] _eventos;
	delete[] _notas;
	delete[] _formacoes;
}

bool Colaborador::sameColaborador(const std::string& nome) {
	return (nome == *_nome);
}

void Colaborador::addEvento(const Evento& evento) {
	size_t tempCount = _eventos_count + 1;
	Evento* tempEventos = new Evento[tempCount];

	for (size_t i = 0; i < _eventos_count; ++i)
		tempEventos[i] = _eventos[i];

	tempEventos[tempCount - 1] = evento;

	delete[] _eventos;
	_eventos = tempEventos;
	_eventos_count = tempCount;
}

void Colaborador::removeEvento(const size_t& idx) {
	if (idx >= _eventos_count)
		return;

	size_t tempCount = _eventos_count - 1;
	Evento* tempEventos = new Evento[tempCount];

	for (size_t i = 0, j = 0; i < _eventos_count; ++i) {
		if (i != idx) {
			tempEventos[j] = _eventos[i];
			++j;
		}
	}

	delete[] _eventos;
	_eventos = tempEventos;
	_eventos_count = tempCount;
}

void Colaborador::removeEventoPorData(const Data& data) {
	for (size_t i = 0;i < _eventos_count;++i) {
		if (_eventos[i]._data->sameData(data)) {
			removeEvento(i);
			return;
		}
	}
}

void Colaborador::addNota(const Nota& nota) {
	size_t tempCount = _notas_count + 1;

	Nota* tempNotas = new Nota[tempCount];

	for (size_t i = 0; i < _notas_count; ++i)
		tempNotas[i] = _notas[i];

	tempNotas[tempCount - 1] = nota;

	delete[] _notas;
	_notas = tempNotas;
	_notas_count = tempCount;
}

void Colaborador::removeNota(const size_t& idx) {
	if (idx >= _notas_count)
		return;

	size_t tempCount = _notas_count - 1;
	Nota* tempNotas = new Nota[tempCount];

	for (size_t i = 0, j = 0; i < _notas_count; ++i) {
		if (i != idx) {
			tempNotas[j] = _notas[i];
			++j;
		}
	}

	delete[] _notas;

	_notas = tempNotas;
	_notas_count = tempCount;
}

void Colaborador::addFormacao(const Formacao& formacao) {
	size_t tempCount = _formacoes_count + 1;

	Formacao* tempFormacoes = new Formacao[tempCount];

	for (size_t i = 0; i < _formacoes_count; ++i)
		tempFormacoes[i] = _formacoes[i];

	tempFormacoes[tempCount - 1] = formacao;

	delete[] _formacoes;
	_formacoes = tempFormacoes;
	_formacoes_count = tempCount;
}

void Colaborador::removeFormacao(const size_t& idx) {
	if (idx >= _formacoes_count)
		return;
	size_t tempCount = _formacoes_count - 1;
	Formacao* tempFormacoes = new Formacao[tempCount];
	for (size_t i = 0, j = 0; i < _formacoes_count; ++i) {
		if (i != idx) {
			tempFormacoes[j] = _formacoes[i];
			++j;
		}
	}
	delete[] _formacoes;
	_formacoes = tempFormacoes;
	_formacoes_count = tempCount;
}