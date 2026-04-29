import re

keyword_patterns = [
    (r"\botherwise if\b", "ELSEIF"),
    (r"\belse if\b", "ELSEIF"),
    (r"\bend if\b", "ENDIF"),
    (r"\bendif\b", "ENDIF"),
    (r"\bstop\b", "STOP"),
    (r"\bdone\b", "DONE"),
    (r"\bdeclare\b", "DECLARE"),
    (r"\bset\b", "SET"),
    (r"\bprint\b", "PRINT"),
    (r"\binput\b", "INPUT"),
    (r"\bfrom user\b", "FROM USER"),
    (r"\bvalue\b", "VALUE"),
    (r"\bif\b", "IF"),
    (r"\bwhile\b", "WHILE"),
    (r"\bfor\b", "FOR"),
    (r"\bstep\b", "STEP"),
    (r"\bthen\b", "THEN"),
    (r"\belse\b", "ELSE"),
    (r"\botherwise\b", "ELSE"),
]

comparison_patterns = [
    (r"\bis not equal to\b", "!="),
    (r"\bis greater than\b", ">"),
    (r"\bis less than\b", "<"),
    (r"\bnot equal to\b", "!="),
    (r"\bis equal to\b", "=="),
    (r"\bequal to\b", "=="),
    (r"\bgreater than\b", ">"),
    (r"\bless than\b", "<"),
]

operator_patterns = [
    (r"\bmodulo\b", "%"),
    (r"\bmod\b", "%"),
    (r"\bremainder\b", "%"),
    (r"\bmultiplied by\b", "*"),
    (r"\bmultiply by\b", "*"),
    (r"\btimes\b", "*"),
    (r"\binto\b", "*"),
    (r"\bdivide by\b", "/"),
    (r"\bplus\b", "+"),
    (r"\bminus\b", "-"),
]

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
    "nineteen": "19",
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
    for word, digit in teens_map.items():
        text = re.sub(rf"\b{word}\b", digit, text)

    for word, digit in tens_map.items():
        for one_word, one_digit in number_map.items():
            text = re.sub(rf"\b{word}\s+{one_word}\b", str(int(digit) + int(one_digit)), text)
        text = re.sub(rf"\b{word}\b", digit, text)

    for word, digit in number_map.items():
        text = re.sub(rf"\b{word}\b", digit, text)

    return text


def convert_to_voicelang(text):
    text = text.lower()
    text = text.replace(",", " ")
    text = text.replace(".", " ")
    text = normalize_number_words(text)

    text = re.sub(r"\bset\s+([a-zA-Z_][a-zA-Z0-9_]*)\s+to\b", r"SET \1 =", text)
    text = re.sub(r"\bset\s+([a-zA-Z_][a-zA-Z0-9_]*)\s+as\b", r"SET \1 =", text)

    for pattern, replacement in comparison_patterns:
        text = re.sub(pattern, replacement, text)

    for pattern, replacement in operator_patterns:
        text = re.sub(pattern, replacement, text)

    for pattern, replacement in keyword_patterns:
        text = re.sub(pattern, replacement, text)

    text = re.sub(r"\s+", " ", text).strip()

    for keyword in ["DECLARE", "SET", "PRINT", "INPUT", "IF", "WHILE", "FOR", "STEP", "ELSEIF", "ELSE", "ENDIF", "STOP", "DONE"]:
        text = re.sub(rf"\s+({keyword})\b", r"\n\1", text)

    text = re.sub(r"\bTHEN\s+", "THEN\n", text)
    text = re.sub(r"\n+", "\n", text)

    return text.strip().upper()
