import argparse
import time


def joltage12(line):
    """
    take a line string with numeric characters and return the int value
    of "joltage" from the highest possible 12-digit value
    """
    pass


def lineJoltage(line):
    """
    take a line string with numeric characters and return the int value
    of "joltage" from the highest possible 2-digit value left-to-right
    Optimizations:
        1. ord is faster than int translation
        2. ... I think there's a way to seek quicker
    """

    length = len(line)

    if length <= 2:
        return int(line)

    left = 0
    leftMax = 0

    # seek the highest left, then right
    for lidx in range(0, length - 1):
        int_l = ord(line[lidx])
        if int_l > leftMax:
            left = lidx
            leftMax = int_l

            if leftMax == 9:
                break
        pass

    right = left+1
    rightMax = 0
    for ridx in range(right, length):
        int_r = ord(line[ridx])
        if int_r > rightMax:
            right = ridx
            rightMax = int_r

            if rightMax == 9:
                break
        pass

    result = int(line[left] + line[right])
    # print(line, result)
    return result


def main():
    argParseTime = time.process_time_ns()
    argParser = argparse.ArgumentParser(
        prog="Advent of Code 2025: Day 3",
        usage="python d3.py [filename]",
        description="Find the 'Joltage' total from a text file")

    argParser.add_argument('filename')
    args = argParser.parse_args()

    argParseTime = time.process_time_ns() - argParseTime

    fileSplitTime = time.process_time_ns()
    # parse file in first arg
    file = open(args.filename, 'r')
    lines = file.read().splitlines(False)
    fileSplitTime = time.process_time_ns() - fileSplitTime

    joltCalcTime = time.process_time_ns()
    totalJoltage = 0
    for line in lines:
        totalJoltage += lineJoltage(line)
    joltCalcTime = time.process_time_ns() - joltCalcTime

    print("totalJoltage=", totalJoltage)
    print(f"apt={argParseTime}ns, fst={fileSplitTime}ns, jct={joltCalcTime}ns")
    return


if __name__ == "__main__":
    main()
