def main(filename):
    with open(filename) as fp:
        n = 0
        for line in fp:
            if safe(line.strip()):
                n += 1
        return n


def safe(line):
    reports = list(map(int, line.split()))
    c = reports[0]
    up = reports[1] - reports[0] > 0
    for n in reports[1:]:
        if (up and n < c) or (not up and n > c):
            return False
        if abs(n - c) > 3 or n == c:
            return False
        c = n
    return True

if __name__ == "__main__":
    print(main("example"))
    print(main("input"))
