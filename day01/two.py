def main(filename):
    left, right = [], {}
    with open(filename) as fp:
        for line in fp:
            if line.strip() != "":
                a, b = map(int, line.strip().split())
                left.append(a)
                if b not in right:
                    right[b] = 0
                right[b] += 1
    print(sum(a * right.get(a, 0) for a in left))

main("example")
main("input")
