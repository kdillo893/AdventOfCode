package main

import (
	"fmt"
	"log"
	"os"
	"regexp"
	"strconv"
	"sync"
)

func mulWorker(segment string, res chan int64) {

	thisRes := int64(0)
	rex := regexp.MustCompile("mul\\((\\d{1,3}),(\\d{1,3})\\)")
	// fmt.Println("wkr:", wnum, "val:", string(byteArr))
	subMatches := rex.FindAllStringSubmatch(segment, -1)

	for iters := 0; iters < len(subMatches); iters++ {
		//have a match for first and second "number", multiply the two and add to res.
		var mulArr []string
		mulArr = subMatches[iters]
		if len(mulArr) < 3 {
			continue
		}

		num1, err1 := strconv.ParseInt(mulArr[1], 10, 0)
		num2, err2 := strconv.ParseInt(mulArr[2], 10, 0)
		if err1 != nil || err2 != nil {
			fmt.Println("This shouldn't happen: ", mulArr)
			continue
		}

		// fmt.Println(mulArr)
		thisRes += int64(num1 * num2)
	}
	// fmt.Println("mulCount:", len(subMatches), "mulRes:", thisRes)

	res <- thisRes
}

func main() {

	//import the file to parse:
	var filename string
	if len(os.Args) != 2 {
		//the filename to parse is the index 1 of args. program is 0th
		log.Fatalf("Usage: day3 [filename]")
	}

	filename = os.Args[1]

	//get the file text, parse buffers
	// file, err := os.Open(filename)
	// if err != nil {
	// 	log.Fatalf("error parsing file, filename: "+filename, err)
	// }

	fileText, err := os.ReadFile(filename)
	if err != nil {
		log.Fatalf("error parsing file, filename: "+filename, err)
	}


	doDontRex := regexp.MustCompile("do(n't)?\\(\\)")
	//start at 0, do all until first do/dont
	//  to do this without nonsense inside for loop, prepend with 0:0
	doDontIndexes := [][]int{{0, 0}}
	doDontIndexes = append(doDontIndexes[:], doDontRex.FindAllStringIndex(string(fileText), -1)...)
	// fmt.Println("ddi: ", doDontIndexes)

	var wg sync.WaitGroup
	resChannel := make(chan int64, len(doDontIndexes))

	var res int64 = 0
	doSegment := true
	for i := 0; i < len(doDontIndexes); i++ {
		//ddi is array with start and end, grab that for "do/dont"
		ddi := doDontIndexes[i]
		doString := string(fileText[ddi[0]:ddi[1]])

		//nextIdx is either "end of the text" or next index if it's there.
		var nextIdx int
		if len(doDontIndexes) <= i+1 {
			nextIdx = len(fileText) - 1
		} else {
			nextIdx = doDontIndexes[i+1][0]
		}

		switch doString {
		case "do()":
			doSegment = true
			break
		case "don't()":
			doSegment = false
		}

		if !doSegment {
			// fmt.Println(ddi, doString, "cont between indexes:", ddi[1], nextIdx)
			continue
		}

		//we want to process this stuff
		segment := string(fileText[ddi[1]:nextIdx])
		// fmt.Println(ddi, doString, "between indexes:", ddi[1], nextIdx, segment)

		wg.Add(1)

		go func() {
			defer wg.Done()
			mulWorker(segment, resChannel)
		}()
	}

	go func() {
		wg.Wait()
		close(resChannel)
	}()

  for r := range resChannel {
    res += r

  }

	fmt.Println(res)
}
