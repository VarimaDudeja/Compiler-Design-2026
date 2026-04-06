# voicelang.py

import re

keyword_map = {
    "declare": "DECLARE",
    "declared": "DECLARE",
    "create": "DECLARE",
    "set": "SET",
    "print": "PRINT",
    "as": "AS",
    "int": "INT",
    "float": "FLOAT",
    "value": "VALUE",
    "with": "AS INT",
    "to": "=",
}

operator_map = {
    "plus": "+",
    "minus": "-",
    "multiply": "*",
    "into": "*",
    "by": "/",
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
}

teens_map = {
    "ten": "10",
    "eleven": "11",
    "twelve": "12",
    "thirteen": "13",
    "fourteen": "14",
    "fifteen": "15",
    "sixteen": "16",
    "seventeen": "17",
    "eighteen": "18",
    "nineteen": "19"
}

tens_map = {
    "twenty": "20",
    "thirty": "30",
    "forty": "40",
    "fifty": "50",
    "sixty": "60",
    "seventy": "70",
    "eighty": "80",
    "ninety": "90",
}


def normalize_number_words(text):
    # Handle teens first (e.g., "thirteen")
    for word, digit in teens_map.items():
        text = re.sub(rf"\b{word}\b", digit, text)

    # Handle tens (e.g., "twenty", "thirty")
    for word, digit in tens_map.items():
        # twenty one -> 21
        for one_word, one_digit in number_map.items():
            text = re.sub(rf"\b{word}\s+{one_word}\b", str(int(digit) + int(one_digit)), text)
        # just "twenty" alone
        text = re.sub(rf"\b{word}\b", digit, text)

    # Handle single-digit numbers
    for word, digit in number_map.items():
        text = re.sub(rf"\b{word}\b", digit, text)

    return text


def convert_to_voicelang(text):
    text = text.lower()

    # Replace number words first (supports twenty, forty, thirteen, etc.)
    text = normalize_number_words(text)

    # Handle x as multiplication operator in numeric expressions, while preserving variable names
    text = re.sub(r"(\d+)\s+x\s+(\d+)", r"\1 * \2", text, flags=re.IGNORECASE)

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