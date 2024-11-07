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
        "label": label,
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
                    "line": index + 1,
                    "label": parsed_instruction["label"],
                    "instruction": parsed_instruction["instruction"],
                    "attribute1": parsed_instruction["attribute1"],
                    "attribute2": parsed_instruction["attribute2"],
                })

    return {"instructions": instructions}

# Exemplo de uso
output = generate_json("test.txt")
print(output)
