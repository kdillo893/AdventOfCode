package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"sort"
	"strconv"
	"strings"
	"sync"
)

func absInt(a int, b int) int {
	if a > b {
		return a - b
	}
	return b - a
}

func pt1(list1 []int, list2 []int) int {
	//sort the list1 and list2 arrays, make it a goroutine
	//waitgroup for syncing with sorting arrays
	var wg sync.WaitGroup

	wg.Add(1)
	go func() {
		wg.Done()
		sort.Ints(list1)
	}()
	wg.Add(1)
	go func() {
		wg.Done()
		sort.Ints(list2)
	}()

	wg.Wait()
	//length of each should be 1000, just double checking
	// fmt.Println(len(list1), len(list2))

	//segment the summing into pieces; can vary the speed with worker #s
	chunkSize := 50
	numWorkers := int(len(list1) / chunkSize)

	//need worker to round out the end if un-even slicing.
	if numWorkers*chunkSize < len(list1) {
		numWorkers++
	}

	//sum total has a mutex for putting worker results.
	var mut sync.Mutex
	sumTotal := 0

	for work := 0; work < numWorkers; work++ {

		start := work * chunkSize
		end := start + chunkSize

		//truncate if overflowing past length of arrays
		if end > len(list1) {
			end = len(list1)
			if len(list1) == start {
				break
			}
		}

		wg.Add(1)
		go func(start int, end int) {

			defer wg.Done()

			workerSum := 0

			for i := start; i < end; i++ {
				diff := absInt(list1[i], list2[i])
				// fmt.Println(list1[i], list2[i], diff)

				workerSum += diff
			}

			mut.Lock()
			sumTotal += workerSum
			mut.Unlock()
		}(start, end)
	}

	wg.Wait()


	return sumTotal
}

func main() {

	//import the file to parse:
	if len(os.Args) == 2 {
		//the filename to parse is the index 1 of args.
		filename := os.Args[1]

		//read relative file line-by-line
		file, err := os.Open(filename)

		if err != nil {
			log.Fatalf("can't open file: %s", filename)
		}

		defer file.Close()

		var first []int
    var second []int

    //part2, value multiplied by instances right and left.
    firstMap := make(map[int]int)
    secondMap := make(map[int]int)

		//read file line-by-line, first number into left array, second into right.
		reader := bufio.NewReader(file)
		for {
			line, err := reader.ReadString('\n')
			if err != nil {
				fmt.Println("Reader reached end of file")
				break
			}

			//have the line, put segments into array
			parts := strings.Fields(line)

			firstval, err := strconv.Atoi(parts[0])
			secondval, err := strconv.Atoi(parts[1])

			first = append(first, firstval)
			second = append(second, secondval)

      //add the value to map if not there, otherwise increment.
      _,ok1 := firstMap[firstval]
      if ok1 {
        firstMap[firstval]++
      } else {
        firstMap[firstval] = 1
      }

      _,ok2 := secondMap[secondval]
      if ok2 {
        secondMap[secondval]++
      } else {
        secondMap[secondval] = 1
      }
		}
    
    // fmt.Println(first)
    sumDiffs := pt1(first, second)
    fmt.Println(sumDiffs)

    //pt2: looping over keys creating similarity score
    simScore := 0
    //single threaded way is just go through each key in the first map and check for second
    //TODO: how to parse this range in multi-threaded way? would need worker per bucket...
    for key, firstcount := range firstMap {
      secondcount, ok := secondMap[key]
      if ok {
        simScore += key * firstcount * secondcount
      }
    }

    fmt.Println(simScore)
	}
}
