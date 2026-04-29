import speech_recognition as sr
from voicelang import convert_to_voicelang

r = sr.Recognizer()
r.pause_threshold = 2.0

with sr.Microphone() as source:
    print("Speak your command:")
    audio = r.listen(source, phrase_time_limit=10)

try:
    raw_text = r.recognize_google(audio)
    print("Recognized:", raw_text)

    voicelang_text = convert_to_voicelang(raw_text)
    print("VoiceLang:", voicelang_text)

    # ✅ Save to input.vl
    with open("input.vl", "w") as f:
        f.write(voicelang_text)

    print("Saved to input.vl")

except:
    print("Could not understand audio")