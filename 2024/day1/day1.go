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

func main()  {
  fmt.Println("Hello World!")

  //import the file to parse:
  if len(os.Args) == 2 {
    //the filename to parse is the index 1 of args.
    filename := os.Args[1]

    //read relative file line-by-line
    file,err := os.Open(filename)

    if err != nil {
      log.Fatalf("can't open file: %s", filename)
    }

    defer file.Close()

    first:= []int{}
    second:= []int{}

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
    }

    //sort the first and second arrays, make it a goroutine
    //waitgroup for syncing with sorting arrays
    var wg sync.WaitGroup

    wg.Add(1)
    go func() {
      wg.Done()
      sort.Ints(first)
    }()
    wg.Add(1)
    go func() {
      wg.Done()
      sort.Ints(second)
    }()

    wg.Wait()
    //length of each should be 1000, just double checking
    fmt.Println(len(first), len(second))

    //segment the summing into pieces; can vary the speed...
    //For now lets just do the simple single-thread
    sum := 0
    for i:=0; i < len(first); i++ {
      diff:= absInt(first[i], second[i])
      fmt.Println(first[i], second[i], diff)

      sum+= diff
    }

    fmt.Println(sum)


  }
}
