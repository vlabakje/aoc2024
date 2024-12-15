import re

def main(filename):
    total = 0
    tolerance = 0.0001
    offset = 10_000_000_000_000
    with open(filename) as fp:	
        for machine in fp.read().split('\n\n'):
            ax, ay, bx, by, px, py = map(int, re.findall(r'(\d+)', machine))
            px += offset
            py += offset
            A = (bx*py - by*px) / (bx*ay - by*ax)
            B = (px-ax*A) / bx
            if abs(A - round(A)) < tolerance and abs(B - round(B)) < tolerance:
                total += 3*A + B
    return int(total)

if __name__ == "__main__":
    print(main("input"))
