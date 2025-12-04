import argparse
import time
import numpy as np


def joltageXDig(line, resultLength=12):
    """
    take a line string with numeric characters and return the int value
    of "joltage" from the highest possible 12-digit value
    @line = string input with all the lines
    @resultLength = joltage length of result for the processing

    Optimizations:
        1. (not worth) if the resultLength of line (n) is less than 24, could instead 
            find the smallest n-12 digits
        2. numpy partition for the largest "n" values
    """

    # Learning numpy on fly, argsort creates a sorted array of indeces from
    #   the original array
    lineChars = list(line)

    sorted_indices = np.argsort(lineChars)
    # print(sorted_indices)

    # simple case: if the highest 12 are all equal, take a string of the same
    #   character 12 times converted to int
    if lineChars[sorted_indices[-resultLength]] == lineChars[sorted_indices[-1]]:
        # print("full ", lineChars[sorted_indices[-1]])
        return int(lineChars[sorted_indices[-1]] * resultLength)

    # complex case: get the leftmost "high value" index, filter out the indeces
    #   that are before that, then get the next leftmost "high value" until
    #   we have 12 characters
    outStr = ""
    while len(outStr) < resultLength:
        # get the minimum index of the highest value, given that
        #   index + (remaining character count) doesn't meet/exceed the length
        # Filter order:
        # 1. "index + remaining is not exceeding length of line"
        # 2. "index is the leftmost highest value character"
        #       easy solution is do a min enumerating with valid indexes
        #       ... could try a numpy min, but I would need to convert the
        #       char array into int for this...

        NotTooLate = sorted_indices + (resultLength - len(outStr)) <= len(line)
        validIndexes = sorted_indices[NotTooLate]
        # print(line, outStr, validIndexes)
        highChar = lineChars[validIndexes[-1]]

        leftmostIdx = min([idx for idx, x in enumerate(lineChars)
                           if idx in validIndexes and x == highChar])

        # safe to put it in the output
        outStr += highChar

        # filter out indices that are less than the one we just used
        filterOrdered = sorted_indices > leftmostIdx
        sorted_indices = sorted_indices[filterOrdered]
        # print(sorted_indices, highChar, leftmostIdx, len(outStr) - 1)

    # print(line, outStr)

    return int(outStr)


def joltage2dig(line):
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
        totalJoltage += joltageXDig(line)
        # totalJoltage += joltage2dig(line)
    joltCalcTime = time.process_time_ns() - joltCalcTime

    print("totalJoltage=", totalJoltage)
    print(f"apt={argParseTime}ns, fst={fileSplitTime}ns, jct={joltCalcTime}ns")
    print(f"apt={argParseTime/1000000000}s, fst={fileSplitTime /
          1000000000}s, jct={joltCalcTime/1000000000}s")
    return


if __name__ == "__main__":
    main()
