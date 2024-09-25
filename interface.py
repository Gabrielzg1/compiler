import tkinter as tk
from tkinter import scrolledtext
import subprocess
import os
import platform

# Função para rodar o CMake e gerar o build
def build_compiler():
    cmake_source_directory = os.path.abspath(os.path.dirname(__file__))
    build_directory = os.path.abspath(os.path.join(cmake_source_directory, 'build'))

    # Criar o diretório de build se ele não existir
    if not os.path.exists(build_directory):
        os.makedirs(build_directory)

    try:
        # Executar o CMake (diferencia entre Windows e macOS)
        system = platform.system()
        if system == "Windows":
            cmake_command = ["cmake", "-G", "MinGW Makefiles", cmake_source_directory]
        else:  # macOS ou Linux
            cmake_command = ["cmake", cmake_source_directory]

        cmake_process = subprocess.run(cmake_command, cwd=build_directory, capture_output=True, text=True)

        # Checar a saída do CMake
        if cmake_process.returncode != 0:
            raise RuntimeError(f"Erro ao rodar o CMake: {cmake_process.stderr}")

        # Executar o make ou build (diferenciar entre Windows e macOS)
        if system == "Windows":
            build_command = ["cmake", "--build", ".", "--", "-j"]
        else:  # macOS ou Linux
            build_command = ["cmake", "--build", ".", "--", "-j"]

        build_process = subprocess.run(build_command, cwd=build_directory, capture_output=True, text=True)

        # Checar a saída do build
        if build_process.returncode != 0:
            raise RuntimeError(f"Erro ao rodar o build: {build_process.stderr}")

        print("Build concluído com sucesso!")

    except Exception as e:
        print(f"Erro durante o processo de build: {str(e)}")


# Função para rodar o compilador e exibir o resultado
def run_compiler():
    code = code_input.get("1.0", tk.END)  # Obtém o código do campo de texto
    cmake_source_directory = os.path.abspath(os.path.dirname(__file__))
    result = ""
    build_directory = os.path.abspath(os.path.join(cmake_source_directory, 'build'))

    # Criar o diretório de build se ele não existir
    if not os.path.exists(build_directory):
        os.makedirs(build_directory)

    # Salvar o código em um arquivo 'code.txt'
    try:
        with open(os.path.join(build_directory, "code.txt"), "w") as file:
            file.write(code)

        # Caminho para o executável (diferenciar entre Windows e macOS)
        system = platform.system()
        if system == "Windows":
            compiler_executable = os.path.join(build_directory, "compiler.exe")
        else:  # macOS ou Linux
            compiler_executable = os.path.join(build_directory, "compiler")

        # Verificar se o arquivo executável existe antes de tentar executá-lo
        if not os.path.exists(compiler_executable):
            raise FileNotFoundError(f"Executável não encontrado: {compiler_executable}")

        # Executar o compilador e capturar a saída
        compiler_run = subprocess.run([compiler_executable], cwd=build_directory, capture_output=True, text=True)

        # Adicionar a saída padrão (stdout) ao resultado
        if compiler_run.stdout:
            result += f"Saída do compilador:\n{compiler_run.stdout}\n"

        # Adicionar a saída de erro (stderr) ao resultado
        if compiler_run.stderr:
            result += f"Erros do compilador:\n{compiler_run.stderr}\n"

        # Verificar se o retorno foi um erro
        if compiler_run.returncode != 0:
            result += f"Erro durante a execução do compilador. Código de retorno: {compiler_run.returncode}\n"

        # Ler o conteúdo do arquivo output.txt, se ele existir
        output_file_path = os.path.join(build_directory, "output.txt")
        if os.path.exists(output_file_path):
            with open(output_file_path, "r") as output_file:
                output_content = output_file.read()
                result += f"\nConteúdo do output.txt:\n{output_content}"
        else:
            result += "\nO arquivo output.txt não foi encontrado."

    except Exception as e:
        result = f"Erro durante a execução: {str(e)}"

    # Exibir o resultado no campo de saída
    output_field.config(state=tk.NORMAL)  # Permitir edição temporária
    output_field.delete("1.0", tk.END)  # Limpar o campo de saída
    output_field.insert(tk.END, result)  # Inserir o resultado
    output_field.config(state=tk.DISABLED)  # Bloquear o campo de saída

# Função para atualizar os números de linha
def update_line_numbers(event=None):
    code = code_input.get("1.0", tk.END)
    lines = code.count("\n") + 1  # Contar o número de linhas
    line_numbers.config(state=tk.NORMAL)  # Permitir edição temporária
    line_numbers.delete("1.0", tk.END)

    # Inserir os números de linha
    line_num_str = "\n".join(str(i) for i in range(1, lines + 1))
    line_numbers.insert(tk.END, line_num_str)
    line_numbers.config(state=tk.DISABLED)  # Bloquear edição

# Criar a janela principal
root = tk.Tk()
root.title("Compilador com Números de Linha")

# Frame para alinhar o campo de texto e os números de linha
frame = tk.Frame(root)
frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

# Campo de texto para os números de linha
line_numbers = tk.Text(frame, width=4, padx=3, takefocus=0, border=0, background='lightgray', state=tk.DISABLED)
line_numbers.pack(side=tk.LEFT, fill=tk.Y)

# Campo de texto para o código com rolagem
code_input = scrolledtext.ScrolledText(frame, height=10, width=50, undo=True)
code_input.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

# Atualizar os números de linha conforme o texto é alterado
code_input.bind('<KeyRelease>', update_line_numbers)

# Botão para rodar o compilador
run_button = tk.Button(root, text="Executar", command=run_compiler)
run_button.pack(pady=5)

# Campo de saída para o resultado da execução
output_field = scrolledtext.ScrolledText(root, height=50, width=100, state=tk.DISABLED)
output_field.pack(padx=10, pady=10)

# Inicializar os números de linha
update_line_numbers()

# Chamar a função para fazer o build do compilador
build_compiler()

# Iniciar o loop da interface gráfica
root.mainloop()
