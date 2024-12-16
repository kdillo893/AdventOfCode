import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.OptionalInt;
import java.util.stream.Stream;

public class day2 {

  public static void main(String[] args) throws IOException {

    // get the file from args
    if (args.length != 1) {
      System.out.println("Usage: day2 [inputFile]");
      for (String str : args) {
        System.out.printf("arg=%s", str);
      }
      return;
    }

    File inputFile = new File(args[0]);

    try (BufferedReader bReader = new BufferedReader(new FileReader(inputFile))) {
      // read lines, process them with linestream? lets try that.
      Stream<String> lineStream = bReader.lines();
      int safeLines = lineStream.mapToInt(line -> part1LineParse(line)).sum();

      System.out.printf("SafeLines: %d\n", safeLines);

    } catch (IOException ex) {
      System.err.println("Unable to read file");
      throw ex;
    }
  }


  /**
   * Parses each line for day2 part 1 to see if the line is "safe"
   * @returns 1 if safe line, 0 if unsafe line.
   */
  private static int part1LineParse(String line) {

    System.out.printf("line=%s\n", line);

    //safe if sequence decreases by 2 or less each time. split by spaces.
    OptionalInt last = OptionalInt.empty();
    Boolean increasing = null;
    for(String strVal : line.split(" ") ) {
      int val = Integer.valueOf(strVal);

      if (last.isPresent()) {
        int diff = val - last.getAsInt();
        if (diff > 0) {
          //if this is decreasing and we are increasing, break.
          if (increasing != null && !increasing)
            return 0;

          increasing = true;
        } else if (diff < 0) {
          //if this is increasing and we are decreasing, break.
          if (increasing != null && increasing)
            return 0;

          increasing = false;
        }

        diff = Math.abs(diff);
        if (diff > 3 || diff <= 0)
        return 0;
      }

      last = OptionalInt.of(val);
    }
    

    return 1;
  }
}
