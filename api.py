import subprocess
import os
import platform
import json
from flask import Flask, request, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

# Diretório principal e de build definidos globalmente
cmake_source_directory = os.path.abspath(os.path.dirname(__file__))
build_directory = os.path.abspath(os.path.join(cmake_source_directory, 'build'))

# Função para rodar o CMake e gerar o build
def build_compiler():
    if not os.path.exists(build_directory):
        os.makedirs(build_directory)

    try:
        system = platform.system()
        cmake_command = ["cmake", cmake_source_directory] if system != "Windows" else ["cmake", "-G", "MinGW Makefiles", cmake_source_directory]
        cmake_process = subprocess.run(cmake_command, cwd=build_directory, capture_output=True, text=True)

        if cmake_process.returncode != 0:
            return f"Erro ao rodar o CMake: {cmake_process.stderr}", cmake_process.returncode

        build_command = ["cmake", "--build", ".", "--", "-j"]
        build_process = subprocess.run(build_command, cwd=build_directory, capture_output=True, text=True)

        if build_process.returncode != 0:
            return f"Erro ao rodar o build: {build_process.stderr}", build_process.returncode

        return "Build concluído com sucesso!", 0

    except Exception as e:
        return f"Erro durante o processo de build: {str(e)}", 1

# Função para rodar o compilador
def run_compiler(code):
    if not os.path.exists(build_directory):
        os.makedirs(build_directory)

    try:
        # Salvar o código no arquivo code.txt
        code_file_path = os.path.join(build_directory, "code.txt")
        with open(code_file_path, "w") as file:
            file.write(code)

        # Caminho para o executável
        system = platform.system()
        compiler_executable = os.path.join(build_directory, "compiler.exe") if system == "Windows" else os.path.join(build_directory, "compiler")

        if not os.path.exists(compiler_executable):
            return {"errorLine": 0, "message": f"Executável não encontrado: {compiler_executable}"}

        # Executar o compilador
        compiler_run = subprocess.run([compiler_executable], cwd=build_directory, capture_output=True, text=True)

        if compiler_run.returncode != 0:
            return {"errorLine": 1, "message": compiler_run.stderr}

        # Verifica o arquivo de saída `output.txt`
        output_file_path = os.path.join(build_directory, "output.txt")
        if os.path.exists(output_file_path):
            # Ler o conteúdo de output.txt e retornar
            with open(output_file_path, "r") as output_file:
                output_content = output_file.read().strip()
            return {"message": "Execução concluída com sucesso.", "output": output_content}
        else:
            return {"errorLine": None, "message": "Arquivo output.txt não encontrado."}

    except Exception as e:
        return {"errorLine": 0, "message": f"Erro durante a execução: {str(e)}"}


# Função para converter o conteúdo do arquivo em JSON conforme estrutura esperada
def parse_instruction(line):
    parts = line.strip().split()
    label = None
    instruction = None
    attribute1 = None
    attribute2 = None

    # Verifica se o primeiro item é um rótulo numérico
    if parts[0].isdigit():
        label = parts[0]
        parts = parts[1:]  # Remove o rótulo da lista de partes

    # Define a instrução e os atributos restantes
    if len(parts) > 0:
        instruction = parts[0]
    if len(parts) > 1:
        attribute1 = parts[1]
    if len(parts) > 2:
        attribute2 = parts[2]

    return {
        "label": label,  # Inclui label com valor None se não houver rótulo
        "instruction": instruction,
        "attribute1": attribute1,
        "attribute2": attribute2
    }


def generate_json(file_path):
    instructions = []
    with open(file_path, "r") as file:
        for index, line in enumerate(file):
            if line.strip():  # Ignora linhas vazias
                parsed_instruction = parse_instruction(line)
                # Adiciona a linha com os campos mapeados corretamente
                instructions.append({
                    "line": index ,
                    "label": parsed_instruction["label"],
                    "instruction": parsed_instruction["instruction"],
                    "attribute1": parsed_instruction["attribute1"],
                    "attribute2": parsed_instruction["attribute2"],

                })

    return {"instructions": instructions}

# Rota para compilar o código
@app.route('/compile', methods=['POST'])
def compile_code():
    data = request.json
    code = data.get('code')

    if not code:
        return jsonify({"errorLine": 0, "message": "Nenhum código fornecido"}), 400

    result = run_compiler(code)
    print(code)
    # Se o compilador retornar um erro, ele já será um dicionário com `errorLine` e `message`
    if isinstance(result, dict):
        return jsonify(result)

    # Se o compilador rodou corretamente, `result` conterá o caminho do arquivo de saída
    return jsonify({"message": "Código compilado com sucesso.", "output_file": result})

# Rota para gerar JSON a partir do arquivo de saída
@app.route('/get_obj', methods=['GET'])
def generate_json_from_output():

    output_file_path = os.path.join(build_directory, "assembly.txt")

    if not os.path.exists(output_file_path):
        return jsonify({"error": "Arquivo de saída não encontrado. Execute a compilação primeiro."}), 404

    instructions_json = generate_json(output_file_path)
    return jsonify(instructions_json)

if __name__ == '__main__':
    build_message, build_status = build_compiler()
    if build_status != 0:
        print(build_message)
    else:
        print("Servidor iniciado com sucesso!")
    app.run(debug=True)
