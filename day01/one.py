def main(filename):
    left, right = [], []
    with open(filename) as fp:
        for line in fp:
            if line.strip() != "":
                a, b = map(int, line.strip().split())
                left.append(a)
                right.append(b)
    print(sum(abs(a - b) for a, b in zip(sorted(left), sorted(right))))

main("example")
main("input")
