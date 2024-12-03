import re

def result(line):
    assert line.startswith("mul("), line
    assert line.endswith(")")
    a, b = line[4:-1].split(",")
    return int(a) * int(b)

def main(filename):
    total = 0
    do = True
    with open(filename) as fp:
        line = fp.read().strip()
        for instr in re.finditer("(mul\\([0-9]{1,3},[0-9]{1,3}\\))|(do\\(\\))|(don\'t\\(\\))", line):
            match instr[0].split("(")[0]:
                case "do":
                    do = True
                case "don\'t":
                    do = False
                case "mul":
                    if do:
                        total += result(instr[0])
    return total

if __name__ == "__main__":
    print(main("example2"))
    print(main("input"))
