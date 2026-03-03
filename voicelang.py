# voicelang.py

import re

keyword_map = {
    "declare": "DECLARE",
    "create": "DECLARE",
    "set": "SET",
    "print": "PRINT",
    "as": "AS",
    "int": "INT",
    "float": "FLOAT",
    "value": "VALUE",
    "to": "=",
}

operator_map = {
    "plus": "+",
    "minus": "-",
    "multiply": "*",
    "divide": "/"
}

number_map = {
    "zero": "0",
    "one": "1",
    "two": "2",
    "three": "3",
    "four": "4",
    "five": "5",
    "six": "6",
    "seven": "7",
    "eight": "8",
    "nine": "9",
    "ten": "10"
}


def convert_to_voicelang(text):
    text = text.lower()

    # Replace number words first
    for word, digit in number_map.items():
        text = re.sub(rf"\b{word}\b", digit, text)

    # Replace operators
    for word, symbol in operator_map.items():
        text = re.sub(rf"\b{word}\b", symbol, text)

    # Replace keywords
    for word, token in keyword_map.items():
        text = re.sub(rf"\b{word}\b", token, text)

    # Handle declaration without type
    # Example: DECLARE x VALUE 5 → DECLARE x AS INT VALUE 5
    text = re.sub(
        r"DECLARE\s+(\w+)\s+VALUE",
        r"DECLARE \1 AS INT VALUE",
        text
    )

    # Clean extra spaces
    text = re.sub(r"\s+", " ", text)

    return text.strip().upper()