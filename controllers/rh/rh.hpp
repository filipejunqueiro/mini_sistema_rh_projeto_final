#pragma once

#include "models/rh/rh.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct RHController
{
	RH _rh;
	std::string _jsonFilePath;
	json _j;

	RHController() = default;

	RHController(const std::string jsonFilePath);

	virtual void showMessageAndWait(const std::string& msg);

	virtual std::string readLineTrimmed();

	virtual bool readDateFromCin(int& dia, int& mes, int& ano);

	virtual bool readMonthYearFromCin(int& mes, int& ano);

	virtual void relatorioMensal();

	virtual void init();

	virtual void load();

	virtual void save();

	virtual void showMenu();

	virtual void showColaboradoresMenu();

	virtual void showColaboradorMenu();

	virtual void showColaboradorSubMenu(Colaborador &colaborador);

	virtual void displayCalendario(const Colaborador &col, int mes, int ano);

	virtual void clearScreen();

	virtual void addColaborador();

	virtual void delColaborador();

	virtual void listColaboradores();

	virtual void updateColaborador(Colaborador& col);

	virtual bool verificarColaboradorExiste(const std::string &nome, size_t &index);

	virtual bool validarData(int dia, int mes, int ano, bool verificarAnoMinimo = false);

	virtual int getDiasNoMes(int mes, int ano);

	virtual bool validarFimDeSemana(int dia, int mes, int ano);

	virtual bool verificarEventoExistente(const Colaborador &col, const Data &data, const bool showMsg = true);

	virtual void marcarFerias(Colaborador &col);

	virtual void marcarFalta(Colaborador& col);

	virtual void addNota(Colaborador& col);

	virtual void delNota(Colaborador& col);

	virtual void addFormacao(Colaborador& col);

	virtual void delFormacao(Colaborador& col);

	virtual void addDepartamento(Colaborador& col);

	virtual void delDepartamento(Colaborador& col);

	virtual void verCalendario(const Colaborador& col);

	virtual void exportarRelatorioColaborador(const Colaborador& col);

	virtual void exportarRelatoriosColaboradores();

	virtual int diaSemana(int dia, int mes, int ano);
};