#include "rh.hpp"

// Parameterized Constructor
RH::RH(const Colaborador& colaborador) {
	_count = 1;
	_colaboradores = new Colaborador[1];
	_colaboradores[0] = colaborador;
}

// Deep Copy
RH::RH(const RH& rh) {
	_count = rh._count;
	_colaboradores = new Colaborador[_count];

	for (size_t i = 0; i < _count; ++i)
		_colaboradores[i] = rh._colaboradores[i];
}

// Deep Assignment
RH& RH::operator=(const RH& rh) {
	if (this != &rh) {
		delete[] _colaboradores;

		_count = rh._count;
		_colaboradores = new Colaborador[_count];

		for (size_t i = 0;i < _count;++i)
			_colaboradores[i] = rh._colaboradores[i];
	}

	return *this;
}

// Destructor
RH::~RH() {
	delete[] _colaboradores;
}

void RH::addColaborador(const Colaborador& colaborador) {
	size_t tempCount = _count + 1;
	Colaborador* tempColaboradores = new Colaborador[tempCount];

	for (size_t i = 0; i < _count; ++i)
		tempColaboradores[i] = _colaboradores[i];

	tempColaboradores[tempCount - 1] = colaborador;

	delete[] _colaboradores;
	_colaboradores = tempColaboradores;
	_count = tempCount;
}

void RH::removeColaborador(const size_t& idx) {
	if (idx >= _count)
		return;

	size_t tempCount = _count - 1;
	Colaborador* tempColaboradores = new Colaborador[tempCount];

	for (size_t i = 0, j = 0; i < _count; ++i) {
		if (i != idx) {
			tempColaboradores[j] = _colaboradores[i];
			++j;
		}
	}

	delete[] _colaboradores;
	_colaboradores = tempColaboradores;
	_count = tempCount;
}

void RH::removeColaboradorPorNome(const std::string& nome) {
	for (size_t i = 0; i < _count; ++i) {
		if (_colaboradores[i].sameColaborador(nome)) {
			removeColaborador(i);
			return;
		}
	}
}