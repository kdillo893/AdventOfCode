import argparse


def lineJoltage(line):
    """
    take a line string with numeric characters and return the int value
    of "joltage" from the highest possible value left-to-right
    """

    if len(line) <= 2:
        return int(line)

    left = 0
    leftMax = 0

    # seek the highest left, then right
    for lidx in range(0, len(line) - 1):
        int_l = int(line[lidx])
        if int_l > leftMax:
            left = lidx
            leftMax = int_l
            pass
        pass

    right = left+1
    rightMax = 0
    for ridx in range(right, len(line)):
        int_r = int(line[ridx])
        if int_r > rightMax:
            right = ridx
            rightMax = int_r
            pass
        pass

    result = int(line[left] + line[right])
    print(line, result)
    return result


def main():
    argParser = argparse.ArgumentParser(
        prog="Advent of Code 2025: Day 3",
        usage="python d3.py [filename]",
        description="Find the 'Joltage' total from a text file")

    argParser.add_argument('filename')
    args = argParser.parse_args()

    # parse file in first arg
    file = open(args.filename, 'r')

    lines = file.read().splitlines(False)

    totalJoltage = 0
    for line in lines:
        totalJoltage += lineJoltage(line)

    print("totalJoltage=", totalJoltage)
    return


if __name__ == "__main__":
    main()
