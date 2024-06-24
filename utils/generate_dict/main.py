
df = "dict.txt"

exc = ["cities.txt", "countries.txt", "days_and_months.txt",
              "manual_entries.txt", "names.txt", "states_and_provinces.txt"]
out = "clean_dict.txt"

minLength = 5
maxLength = 10


outputWords = []



def hasVowels(word):
    return len([l for l in word if l in ["a", "e", "i", "o", "u", "y"]]) > 0


with open(dictfile) as f:
    outputWords = [word.lower() for word in [line.strip("\n") for line in f.readlines()] if (
        len(word) >= minLength and len(word) <= maxLength and hasVowels(word))]

for e in exclusions:
    with open("exclusions/" + e, "r") as exf:
        exWords = []
        for exword in exf:
            exWords.append(exword.lower().strip("\n"))
        outputWords = [word for word in outputWords if word not in exWords]

print(len(outputWords))
with open(output, "w") as o:
    for word in outputWords:
        o.write(word + "\n")
