# Mini Sistema de RH (Continuação) | Projeto Final

## Funcionalidades

- Adicionar / Listar colaboradores
- Marcar Férias / Faltas
- Visualizar calendário mensal com marcações (F = férias, O = Dias Trabalhados X = falta, W = fim de semana)

## Requisitos

- Windows (o projeto configura UTF-8 para a consola)
- CMake >= 3.20
- Um compilador com suporte a C++23 (MSVC/cl.exe recomendado no Windows, foi o que usei)
- vcpkg (recomendado) para instalar dependências

## Dependências

O projeto usa as seguintes bibliotecas via vcpkg (configuradas no `CMakePresets.json`):

- nlohmann-json
- libsodium

## Instalar dependências (vcpkg)

Se ainda não tem o vcpkg instalado, siga as instruções oficiais: https://github.com/microsoft/vcpkg

Exemplo rápido usando PowerShell (executar na raiz do repositório):

```powershell
# 1) Instalar vcpkg

git clone https://github.com/microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.bat

# 2) Adicionar as vars de env

$env:VCPKG_ROOT = "C:/path/to/vcpkg"
$env:PATH = "$env:VCPKG_ROOT;$env:PATH"

# 3) Instalar as bibliotecas necessárias

vcpkg install nlohmann-json
vcpkg install unofficial-sodium
```

## Configuração e Build (CMake presets)

O repositório inclui `CMakePresets.json` com presets para Windows (Ninja + MSVC)-

No PowerShell (repositorio raiz):

```powershell
# Configurar (gera build files em out/build/x64-debug)
cmake --preset x64-debug

# Compilar
cmake --build --preset x64-debug
```

Após o build, o executável e a pasta `files` serão copiados para o diretório de saída (`out/build/x64-debug`). Pode executar a app diretamente:

```powershell
# A partir da raiz do projeto
./out/build/x64-debug/mini_sistema_rh.exe
```

## Execução e uso

Ao correr a aplicação, verá um menu interactivo no terminal:

- 1 Adicionar Colaborador
- 2 Listar Colaboradores
- 3 Marcar Férias
- 4 Marcar Falta
- 5 Ver Calendário
- 6 Salvar
- 0 Sair

O ficheiro de dados padrão é `files/rh.json`. Se estiver vazio ou inexistente, a aplicação tentará inicializá-lo.

![RH JSON EXAMPLE](https://i.imgur.com/h7wSmou.png)

## Notas de plataforma / UTF-8

- Para suportar caracteres acentuados na consola do Windows, o projeto chama `SetConsoleOutputCP(CP_UTF8)` no arranque da parte do UI.
- Se usar um terminal diferente (Windows Terminal, PowerShell), certifique-se de que a fonte e a configuração suportam UTF-8.

## Estrutura do projecto

- `CMakeLists.txt` - build system
- `CMakePresets.json` - presets para desenvolver/compilar no Windows
- `src/` - ficheiro `main.cpp`
- `controllers/` - lógica da UI e controladores (ex: `controllers/rh/rh.cpp`)
- `models/` - modelos de dados (Colaborador, Evento, Data, RH)
- `files/` - ficheiro de persistência JSON (`rh.json`)

## Outros

Este trabalho tem muito a melhorar. Não dei pelo tempo passar e compliquei algumas coisas que não deveria para o tempo que tinhamos.
<br><br>
Eu não aidionei as veiws, porque acho que não teria tempo e também desmotivei um pouco, o trabalho corre na minha opinião com tudo o que foi pedido.
<br><br>
Caso voltemos a tocar neste trabalho, gostaria de explorar um pouco mais a sodium lib e trabalhar um pouco mais com a serialização de jsons.
