#include "rh.hpp"
#define NOMINMAX
#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sodium.h>
using json = nlohmann::json;

// Parameterized Constructor
RHController::RHController(const std::string jsonFilePath)
{
	_jsonFilePath = jsonFilePath;
}

// IO Operations //

// Carrega o ficheiro JSON para a estrutura interna RH.
void RHController::load()
{
	try
	{
		std::ifstream inFile(_jsonFilePath);
		if (!inFile)
		{
			_j = json();
			std::cout << "O ficheiro " << _jsonFilePath << " não foi encontrado." << std::endl;
			return;
		}

		inFile.seekg(0, std::ios::end);
		if (inFile.tellg() == 0)
		{
			_j = json();
			std::cout << "O ficheiro " << _jsonFilePath << " está vazio." << std::endl;
			return;
		}
		inFile.seekg(0, std::ios::beg);

		json j;
		try
		{
			inFile >> j;
		}
		catch (const json::parse_error &e)
		{
			_j = json();
			std::cout << "Erro ao ler JSON: " << e.what() << std::endl;
			return;
		}

		_rh = j.get<RH>();
	}
	catch (const std::exception &e)
	{
		_j = json();
		std::cout << "Erro ao carregar dados: " << e.what() << std::endl;
		return;
	}
}

// Persiste a estrutura RH para o ficheiro JSON.
void RHController::save()
{
	std::ofstream outFile(_jsonFilePath);
	if (!outFile)
		return;

	json j = _rh;
	outFile << j.dump(4);
}

void RHController::exportarRelatorioColaborador(const Colaborador& col) {
	auto j = json::array();

	for (size_t i = 0;i < col._eventos_count; i++) {
		const Evento& ev = col._eventos[i];

		json evJson;

		evJson["data"] = {
			{"dia", *ev._data->_dia},
			{"mes", *ev._data->_mes},
			{"ano", *ev._data->_ano}
		};

		evJson["tipo"] = (*ev._tipo == FERIAS) ? "Férias" : "Falta";

		j.push_back(evJson);
	}

	std::ofstream outFile("files/relatorios/" + *col._nome + "_relatorio.json");

	outFile << j.dump(4);
	outFile.close();
}

void RHController::exportarRelatoriosColaboradores() {
	for (size_t i = 0; i < _rh._count; i++) {
		const Colaborador &col = _rh._colaboradores[i];
		exportarRelatorioColaborador(col);
	}
}

// CLI Operations //

// Apenas mostra mensagem de confirmação e espera tecla.
void RHController::showMessageAndWait(const std::string& msg)
{
	std::cout << std::endl << msg << std::endl
		<< "Pressiona qualquer tecla para continuares...";
	_getch();
}

// Lê uma linha da entrada padrão (ignora whitespaces iniciais).
std::string RHController::readLineTrimmed()
{
	std::string s;

	if (!std::cin.good()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	std::getline(std::cin, s);

	auto start = s.find_first_not_of(" \t\r\n");
	auto end = s.find_last_not_of(" \t\r\n");

	if (start == std::string::npos)
		return "";

	return s.substr(start, end - start + 1);
}

// Lê uma data no formato "dia mes ano" a partir de std::cin.
// Em caso de falha limpa o estado da entrada, avisa o utilizador e retorna false.
bool RHController::readDateFromCin(int& dia, int& mes, int& ano)
{
	std::string line;
	std::getline(std::cin, line);

	if (line.empty())
		return false;

	for (char& c : line) {
		if (!std::isdigit(c))
			c = ' ';
	}

	std::stringstream ss(line);
	int d, m, a;

	if (!(ss >> d >> m >> a)) {
		showMessageAndWait("Data inválida!");
		return false;
	}

	dia = d;
	mes = m;
	ano = a;

	return true;
}

// Lê mês e ano (usado no calendário)
bool RHController::readMonthYearFromCin(int& mes, int& ano)
{
	std::string line;
	std::getline(std::cin, line);

	if (line.empty())
		return false;

	for (char& c : line)
		if (!std::isdigit(c))
			c = ' ';

	std::stringstream ss(line);
	int m, a;

	if (!(ss >> m >> a)) {
		showMessageAndWait("Data invalida!");
		return false;
	}

	mes = m;
	ano = a;

	return true;
}

void RHController::clearScreen()
{
	std::cout << "\033[2J\033[H";	
	std::cout.flush();
}

void RHController::showMenu()
{
	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	bool show = true;
	while (show)
	{
		clearScreen();

		std::cout << "=== Mini Sistema de RH ===" << std::endl;
		std::cout << "1. Menu Colaboradores" << std::endl;
		std::cout << "2. Menu Colaborador" << std::endl;
		std::cout << "3. Salvar" << std::endl;
		std::cout << "0. Sair" << std::endl;
		std::cout << std::endl << "Pressiona uma tecla para escolher: ";

		switch (_getch())
		{
		case '1':
			showColaboradoresMenu();
			break;
		case '2':
			showColaboradorMenu();
			break;
		case '3':
			save();
			break;
		case '0':
			show = false;
			break;
		}
	}
}

void RHController::showColaboradoresMenu()
{
	bool show = true;
	while (show)
	{
		clearScreen();
		std::cout << "=== Colaboradores ===" << std::endl;
		std::cout << "1. Adicionar" << std::endl;
		std::cout << "2. Remover" << std::endl;
		std::cout << "3. Listar" << std::endl;
		std::cout << "4. Relatório" << std::endl;
		std::cout << "5. Exportar Relatórios" << std::endl;
		std::cout << "0. Sair" << std::endl;
		std::cout << std::endl << "Pressiona uma tecla para escolher: ";

		switch (_getch())
		{
		case '1':
			addColaborador();
			break;
		case '2':
			delColaborador();
			break;
		case '3':
			listColaboradores();
			break;
		case '4':
			relatorioMensal();
			break;
		case '5':
			exportarRelatoriosColaboradores();
			break;
		case '0':
			show = false;
			break;
		}
	}
}

void RHController::showColaboradorMenu() {
	clearScreen();

	std::cout << "Nome do colaborador: ";
	std::string nome = readLineTrimmed();

	if (nome.empty()) {
		showMessageAndWait("Nome inválido!");
		return;
	}

	size_t colaboradorIndex = 0;

	if (!verificarColaboradorExiste(nome, colaboradorIndex))
	{
		showMessageAndWait("Colaborador não encontrado!");
		return;
	}

	auto &colaborador = _rh._colaboradores[colaboradorIndex];

	bool show = true;
	while (show)
	{
		clearScreen();
		std::cout << "=== Colaborador: " << *colaborador._nome << " ===" << std::endl;
		std::cout << "1. Atualizar" << std::endl;
		std::cout << "2. Marcar Ferias" << std::endl;
		std::cout << "3. Marcar Falta" << std::endl;
		std::cout << "4. Ver Calendario" << std::endl;
		std::cout << "5. Exportar Relatório" << std::endl;
		std::cout << "6. Submenu" << std::endl;
		std::cout << "0. Sair" << std::endl;
		std::cout << std::endl << "Pressiona uma tecla para escolher: ";

		switch (_getch())
		{
		case '1':
			updateColaborador(colaborador);
			break;
		case '2':
			marcarFerias(colaborador);
			break;
		case '3':
			marcarFalta(colaborador);
			break;
		case '4':
			verCalendario(colaborador);
			break;
		case '5':
			exportarRelatorioColaborador(colaborador);
			break;
		case '6':
			showColaboradorSubMenu(colaborador);
			break;
		case '0':
			show = false;
			break;
		}
	}
}

void RHController::showColaboradorSubMenu(Colaborador &colaborador)
{
	bool show = true;
	while (show)
	{
		clearScreen();
		std::cout << "=== Colaborador: " << *colaborador._nome << " ===" << std::endl;
		std::cout << "1. Adicionar Formação" << std::endl;
		std::cout << "2. Remover Formação" << std::endl;
		std::cout << "3. Adicionar Nota" << std::endl;
		std::cout << "4. Remover Nota" << std::endl;
		std::cout << "5. Adicionar Departamento" << std::endl;
		std::cout << "6. Remover Departamento" << std::endl;
		std::cout << "0. Sair" << std::endl;
		std::cout << std::endl << "Pressiona uma tecla para escolher: ";
		switch (_getch())
		{
		case '1':
			addFormacao(colaborador);
			break;
		case '2':
			delFormacao(colaborador);
			break;
		case '3':
			addNota(colaborador);
			break;
		case '4':
			delNota(colaborador);
			break;
		case '5':
			addDepartamento(colaborador);
			break;
		case '6':
			delDepartamento(colaborador);
			break;
		case '0':
			show = false;
			break;
		}
	}
}

// Colaboradores Operations //

void RHController::addColaborador()
{
	clearScreen();
	std::cout << "Nome do colaborador: ";
	std::string nome = readLineTrimmed();

	if (nome.empty())
	{
		showMessageAndWait("Nome inválido!");
		return;
	}

	// Verificar se já existe um colaborador com este nome
	for (size_t i = 0; i < _rh._count; i++)
	{
		if (_rh._colaboradores[i].sameColaborador(nome))
		{
			showMessageAndWait("Já existe um colaborador com este nome!");
			return;
		}
	}

	Colaborador c(nome);
	_rh.addColaborador(c);
	showMessageAndWait("Colaborador adicionado!");
}

void RHController::delColaborador()
{
	clearScreen();
	std::cout << "Nome do colaborador: ";
	std::string nome = readLineTrimmed();

	if (nome.empty())
	{
		showMessageAndWait("Nome inválido!");
		return;
	}

	for (size_t i = 0; i < _rh._count; i++)
	{
		if (_rh._colaboradores[i].sameColaborador(nome))
		{
			_rh.removeColaboradorPorNome(nome);
			showMessageAndWait("Colaborador removido!");
			return;
		}
	}

	showMessageAndWait("O Colaborador não existe!");
}

void RHController::listColaboradores()
{
	clearScreen();
	std::cout << "Lista de Colaboradores:\n\n";

	for (size_t i = 0; i < _rh._count; i++)
	{
		const Colaborador& col = _rh._colaboradores[i];
		const std::string departamento = col._departamento.empty() ? "N/A" : col._departamento;

		std::cout << "\nColaborador: " << *col._nome << " | Departamento: " << departamento << " | " << "\n";
	}

	showMessageAndWait("");
}

void RHController::relatorioMensal() {
	clearScreen();

	auto now = std::chrono::system_clock::now();
	auto ymd = std::chrono::floor<std::chrono::days>(now);

	std::chrono::year_month_day today{ ymd };
	std::chrono::month currentMonth = today.month();

	std::cout << "Relatório Mensal dos Colaboradores | Mês: " << currentMonth << "\n";

	for (size_t i = 0; i < _rh._count; i++) {
		const Colaborador &col = _rh._colaboradores[i];

		const std::string departamento = col._departamento.empty() ? "N/A" : col._departamento;
		std::cout << "\nColaborador: " << *col._nome << " | Departamento: "<< departamento << " | " << "\n";

		size_t feriasCount = 0;
		size_t faltasCount = 0;

		for (size_t e = 0; e < col._eventos_count; e++) {
			const Evento &ev = col._eventos[e];

			if (*ev._data->_mes == static_cast<unsigned>(currentMonth)) {
				if (*ev._tipo == FERIAS) {
					feriasCount++;
				}
				else if (*ev._tipo == FALTA) {
					faltasCount++;
				}
			}
		}

		std::cout << "| Férias: " << feriasCount << " | Faltas: " << faltasCount << " | Dias Restantes: " << (22 - (static_cast<int>(feriasCount))) << " |\n";
	}

	showMessageAndWait("");
}

// Colaborador Operations //

void RHController::updateColaborador(Colaborador &col) {
	clearScreen();

	std::cout << "Novo nome colaborador: ";
	std::string nome = readLineTrimmed();

	if (nome.empty())
	{
		showMessageAndWait("Nome inválido!");
		return;
	}

	size_t index = 0;

	if (verificarColaboradorExiste(nome, index))
	{
		showMessageAndWait("O nome já existe, nada foi alterado!");
		return;
	}

	// Substitui o ponteiro para o novo nome (mantendo model original).
	col._nome = new std::string(nome);
	showMessageAndWait("Colaborador atualizado!");
}

// Verifica se um colaborador com `nome` existe e retorna o índice via reference.
bool RHController::verificarColaboradorExiste(const std::string &nome, size_t &index)
{
	for (size_t i = 0; i < _rh._count; i++)
	{
		if (_rh._colaboradores[i].sameColaborador(nome))
		{
			index = i;
			return true;
		}
	}
	return false;
}

// Retorna o número de dias num mês/ano (considera anos bissextos).
int RHController::getDiasNoMes(int mes, int ano)
{
	switch (mes)
	{
	case 2: // Fevereiro
		return ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) ? 29 : 28;
	case 4:
	case 6:
	case 9:
	case 11: // Meses com 30 dias
		return 30;
	default: // Meses com 31 dias
		return 31;
	}
}

// Valida uma data simples: mês entre 1-12, dia válido para o mês e ano mínimo opcional.
bool RHController::validarData(int dia, int mes, int ano, bool verificarAnoMinimo)
{
	if (mes < 1 || mes > 12)
	{
		showMessageAndWait("Mês inválido (1-12)!");
		return false;
	}

	int diasNoMes = getDiasNoMes(mes, ano);
	if (dia < 1 || dia > diasNoMes)
	{
		showMessageAndWait("Dia inválido para o mês " + std::to_string(mes) + " (1-" + std::to_string(diasNoMes) + ")!");
		return false;
	}

	if (verificarAnoMinimo && ano < 2025)
	{
		showMessageAndWait("Não é possível marcar eventos em anos anteriores a 2025!");
		return false;
	}

	return true;
}

// Verifica se a data cai num fim de semana (Domingo=0, Sábado=6).
bool RHController::validarFimDeSemana(int dia, int mes, int ano)
{
	int weekDay = diaSemana(dia, mes, ano);
	if (weekDay == 0 || weekDay == 6)
	{
		showMessageAndWait("Não é possível marcar eventos no fim de semana!");
		return false;
	}
	return true;
}

// Verifica se já existe um evento no mesmo dia para o colaborador.
bool RHController::verificarEventoExistente(const Colaborador &col, const Data &data, const bool showMsg)
{
	for (size_t e = 0; e < col._eventos_count; e++)
	{
		const Evento &ev = col._eventos[e];
		if (ev._data->sameData(data))
		{
			if (showMsg)
				showMessageAndWait("Já existe um evento registrado nesta data!");
			return true;
		}
	}
	return false;
}

// Marca férias: lê data, valida e adiciona evento FERIAS.
void RHController::marcarFerias(Colaborador &col)
{
	std::cout << "Data (dia mes ano): ";
	int dia, mes, ano;

	if (!readDateFromCin(dia, mes, ano))
		return;

	if (!validarData(dia, mes, ano, true) || !validarFimDeSemana(dia, mes, ano))
		return;

	try
	{
		Data data(dia, mes, ano);

		for (size_t i = 0; i < _rh._count; i++)
		{
			Colaborador &otherCol = _rh._colaboradores[i];

			if (otherCol.sameColaborador(*col._nome))
				continue;

			if (verificarEventoExistente(otherCol, data, false) && otherCol._departamento == col._departamento) {
				std::cout << "Deseja marcar férias para este colaborador para o mesmo dia que @ " << *otherCol._nome << "? (s/n) | (y/n)" << std::flush;
				char resp = _getch();

				if (resp == 's' || resp == 'S' || resp == 'y' || resp == 'Y')
					continue;
				else
					return;
			}
		}

		if (!verificarEventoExistente(col, data))
		{
			col.addEvento(Evento(data, FERIAS));
			showMessageAndWait("Férias marcadas!");
		}
		else return;
	}
	catch (const std::exception &e)
	{
		showMessageAndWait(std::string("Erro: ") + e.what());
	}
}

// Marca falta: lê data, valida e adiciona evento FALTA.
void RHController::marcarFalta(Colaborador& col)
{
	std::cout << "Data (dia mes ano): ";
	int dia, mes, ano;
	if (!readDateFromCin(dia, mes, ano))
		return;

	if (!validarData(dia, mes, ano, true) || !validarFimDeSemana(dia, mes, ano))
	{
		return;
	}

	try
	{
		Data data(dia, mes, ano);

		if (!verificarEventoExistente(col, data))
		{
			col.addEvento(Evento(data, FALTA));
			showMessageAndWait("Falta registrada!");
		}
	}
	catch (const std::exception &e)
	{
		showMessageAndWait(std::string("Erro: ") + e.what());
	}
}

void RHController::addNota(Colaborador& col)
{
	clearScreen();

	std::cout << "Titulo da nota: ";
	std::string titulo = readLineTrimmed();

	std::cout << "Conteúdo da nota: ";
	std::string conteudo = readLineTrimmed();

	if (titulo.empty() || conteudo.empty())
	{
		showMessageAndWait("Titulo ou Conteúdo inválido!");
		return;
	}

	if (titulo.size() > 1024 || conteudo.size() > 8192)
	{
		showMessageAndWait("Titulo ou Conteúdo demasiado longo!");
		return;
	}

	Nota nota(titulo, conteudo);

	try
	{
		col.addNota(nota);
		showMessageAndWait("Nota adicionada!");
	}
	catch (const std::exception &e)
	{
		showMessageAndWait(std::string("Erro ao adicionar nota: ") + e.what());
	}
}

void RHController::delNota(Colaborador& col)
{
	clearScreen();

	if (col._notas_count == 0)
	{
		showMessageAndWait("Não existem notas para remover!");
		return;
	}

	std::cout << "Notas:\n";
	for (size_t i = 0; i < col._notas_count; i++)
	{
		std::cout << i + 1 << ". " << *col._notas[i]._titulo << "\n";
	}

	std::cout << "Número da nota a remover: ";
	size_t idx;
	std::cin >> idx;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	if (idx < 1 || idx > col._notas_count)
	{
		showMessageAndWait("Índice inválido!");
		return;
	}

	col.removeNota(idx - 1);

	showMessageAndWait("Nota removida!");
}

void RHController::addFormacao(Colaborador& col)
{
	clearScreen();

	std::cout << "Nome da formação: ";
	std::string nome = readLineTrimmed();

	if (nome.empty())
	{
		showMessageAndWait("Nome inválido!");
		return;
	}

	std::cout << "Data (dia mes ano): ";
	int dia, mes, ano;

	if (!readDateFromCin(dia, mes, ano))
		return;

	if (!validarData(dia, mes, ano, true) || !validarFimDeSemana(dia, mes, ano))
		return;

	Data data_conclusao(dia, mes, ano);

	Formacao formacao(nome, data_conclusao);

	col.addFormacao(formacao);
	showMessageAndWait("Formação adicionada!");
}

void RHController::delFormacao(Colaborador& col)
{
	clearScreen();

	if (col._formacoes_count == 0)
	{
		showMessageAndWait("Não existem formações para remover!");
		return;
	}

	std::cout << "Formações:\n";
	for (size_t i = 0; i < col._formacoes_count; i++)
	{
		std::cout << i + 1 << ". " << *col._formacoes[i]._nome << "\n";
	}

	std::cout << "Número da formação a remover: ";
	size_t idx;
	std::cin >> idx;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	if (idx < 1 || idx > col._formacoes_count)
	{
		showMessageAndWait("Índice inválido!");
		return;
	}

	col.removeFormacao(idx - 1);
	showMessageAndWait("Formação removida!");
}

void RHController::addDepartamento(Colaborador& col)
{
	clearScreen();
	std::cout << "Nome do departamento: ";
	std::string nome = readLineTrimmed();
	if (nome.empty())
	{
		showMessageAndWait("Nome inválido!");
		return;
	}
	col._departamento = nome;
	showMessageAndWait("Departamento adicionado!");
}

void RHController::delDepartamento(Colaborador& col)
{
	clearScreen();
	if (col._departamento.empty())
	{
		showMessageAndWait("O colaborador não tem departamento atribuído!");
		return;
	}
	col._departamento = "";
	showMessageAndWait("Departamento removido!");
}

// Mostra calendário para um colaborador num mês/ano dados.
void RHController::verCalendario(const Colaborador& col)
{
	std::cout << "Mes e ano (mes ano): ";
	int mes, ano;
	if (!readMonthYearFromCin(mes, ano))
		return;

	clearScreen();
	displayCalendario(col, mes, ano);
	showMessageAndWait("");
}

// Calcula dia da semana usando fórmula de Zeller adaptada.
// Retorna 0=Domingo, ..., 6=Sábado.
int RHController::diaSemana(int dia, int mes, int ano)
{
	if (mes < 3)
	{
		mes += 12;
		ano--;
	}
	int k = ano % 100;
	int j = ano / 100;
	int h = (dia + 13 * (mes + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
	int d = (h + 6) % 7; // 0 = Domingo, 6 = Sábado
	return d;
}

// Renderiza um calendário mensal com marcações: F=Férias, X=Falta, W=Fim de Semana, O=Trabalhou.
void RHController::displayCalendario(const Colaborador &col, int mes, int ano)
{
	if (ano < 2000 || ano > 2100)
	{
		std::cout << "Ano inválido (" << ano << "). Use um valor entre 2000 e 2100.\n";
		return;
	}

	std::cout << "Calendário de " << *col._nome << " - " << mes << "/" << ano << "\n\n";
	std::cout << "DOM   SEG   TER   QUA   QUI   SEX   SAB\n";
	std::cout << "----------------------------------------\n";

	const auto days = getDiasNoMes(mes, ano);
	const auto firstDay = diaSemana(1, mes, ano);

	// Imprimir espaços para alinhar com o dia da semana correto
	for (int i = 0; i < firstDay; i++)
	{
		std::cout << "      ";
	}

	// Imprimir os dias
	for (int d = 1; d <= days; d++)
	{
		char mark = 'O';

		// Verificar eventos do colaborador e marcar conforme tipo
		for (size_t e = 0; e < col._eventos_count; e++)
		{
			const Evento &ev = col._eventos[e];
			if (*ev._data->_mes == mes && *ev._data->_ano == ano && *ev._data->_dia == d)
			{
				mark = (*ev._tipo == FERIAS) ? 'F' : (*ev._tipo == FALTA ? 'X' : 'W');
				break;
			}
		}

		// Marcar fins de semana (sobrescreve marca anterior)
		const auto weekDay = diaSemana(d, mes, ano);
		if (weekDay == 0 || weekDay == 6)
		{
			mark = 'W';
		}

		std::cout << std::setw(2) << d << "(" << mark << ") " << std::flush;

		// Nova linha após sábado ou último dia
		if ((firstDay + d) % 7 == 0 || d == days)
		{
			std::cout << "\n";
		}
	}
	std::cout << "\n";
	std::cout << "F=Férias, O=Trabalhou, X=Falta, W=Fim de Semana\n";
}

// Initialization
void RHController::init()
{
	sodium_init();
	load();
	showMenu();
	save();
}