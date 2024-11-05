import json

def parse_instruction(line):
    # Divide a linha nos elementos separados por espaços
    parts = line.strip().split()
    instruction = parts[0]
    # Atribui os atributos ou `None` se não houver
    attribute1 = parts[1] if len(parts) > 1 else None
    attribute2 = parts[2] if len(parts) > 2 else None
    return {
        "instruction": instruction,
        "attribute1": attribute1,
        "attribute2": attribute2
    }

def generate_json(file_path):
    instructions = []
    # Lê o arquivo linha por linha
    with open(file_path, "r") as file:
        for index, line in enumerate(file):
            # Ignora linhas vazias
            if line.strip():
                parsed_instruction = parse_instruction(line)
                # Adiciona o número da linha ao objeto de instrução
                instructions.append({"line": index + 1, **parsed_instruction})

    # Converte a lista de instruções em JSON
    instructions_json = json.dumps({"instructions": instructions}, indent=4)
    return instructions_json

# Caminho do arquivo
file_path = "test.txt"  # Altere para o nome do seu arquivo
instructions_json = generate_json(file_path)

# Exibe ou salva o JSON em um arquivo
print(instructions_json)
# Para salvar em um arquivo:
with open("instructions.json", "w") as json_file:
    json_file.write(instructions_json)
