

file = "dict.txt"
output = "dict.h"

varName = "words"

lines = []

with open(file, "r") as f:
    lines = [line.strip() for line in f.readlines()]

l = str(len(lines))

with open(output, "w") as o:
    o.writ("#include <pgmspace.h>\n")
    o.writ("#define NUM_LINES\t\t" + l + "\n")
     o.write("const String " + varName + "[NUM_LINES] PROGMEM = {")
    for line in lines:
            o.write('"' + line + '", ')
    o.write("};")
