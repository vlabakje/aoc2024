import re

def result(line):
    assert line.startswith("mul("), line
    assert line.endswith(")")
    a, b = line[4:-1].split(",")
    return int(a) * int(b)

def main(filename):
    total = 0
    with open(filename) as fp:
        line = fp.read().strip()
        for mul in re.findall("(mul\\([0-9]{1,3},[0-9]{1,3}\\))", line):
            total += result(mul)
    return total

if __name__ == "__main__":
    print(main("example"))
    print(main("input"))
