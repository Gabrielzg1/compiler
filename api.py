
import subprocess
import os
import platform
from flask import Flask, request, jsonify
from flask_cors import CORS  # Importando a extensão

app = Flask(__name__)
CORS(app)


# Função para rodar o CMake e gerar o build
def build_compiler():
    cmake_source_directory = os.path.abspath(os.path.dirname(__file__))
    build_directory = os.path.abspath(os.path.join(cmake_source_directory, 'build'))

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
    cmake_source_directory = os.path.abspath(os.path.dirname(__file__))
    build_directory = os.path.abspath(os.path.join(cmake_source_directory, 'build'))

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

        # Ler o output.txt
        output_file_path = os.path.join(build_directory, "output.txt")
        if os.path.exists(output_file_path):
            with open(output_file_path, "r") as output_file:

                output_content = output_file.readlines()

                # Verifica se a primeira linha do arquivo está vazia ou contém apenas espaços
                error_line_str = output_content[0].strip()
                error_line = None  # Define como None por padrão
                if error_line_str.isdigit():  # Somente converte se for um número válido
                    error_line = int(error_line_str)

                message = output_content[1].strip() if len(output_content) > 1 else "Nenhuma mensagem de erro."

                return {"errorLine": error_line, "message": message}
        else:
            return {"errorLine": None, "message": "Arquivo output.txt não encontrado."}

    except Exception as e:
        return {"errorLine": 0, "message": f"Erro durante a execução: {str(e)}"}



@app.route('/compile', methods=['POST'])
def compile_code():
    data = request.json
    code = data.get('code')

    if not code:
        return jsonify({"errorLine": 0, "message": "Nenhum código fornecido"}), 400

    result = run_compiler(code)
    return jsonify(result)


if __name__ == '__main__':
    # Build do compilador na inicialização
    build_message, build_status = build_compiler()
    if build_status != 0:
        print(build_message)
    else:
        print("Servidor iniciado com sucesso!")
    app.run(debug=True)
