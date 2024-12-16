import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
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

    try {
      BufferedReader bReader = new BufferedReader(new FileReader(inputFile));
      // read lines, process them with linestream? lets try that.
      Stream<String> lineStream = bReader.lines();
      int safeLines = lineStream.mapToInt(line -> part1LineParse(line)).sum();
      System.out.printf("Purely SafeLines: %d\n", safeLines);
      lineStream.close();
      bReader.close();

      // pt2 filter out safe lines, feed new lines into part2filter, add together.

      bReader = new BufferedReader(new FileReader(inputFile));
      lineStream = bReader.lines();
      System.out.println(lineStream);
      int actuallySafe = safeLines
          + lineStream.filter(line -> part1LineParse(line) == 0)
                      .mapToInt(line -> part2LineParse(line))
                      .sum();
      System.out.printf("Actually SafeLines: %d\n", actuallySafe);

    } catch (IOException ex) {
      System.err.println("Unable to read file");
      throw ex;
    }
  }

  /**
   * Input line string "unsafe"; test with a string where one value is deleted
   * @returns 1 if is a safe line, 0 if is not.
   */
  private static int part2LineParse(String line) {

    //can split then reform... which is probably slow.
    //alternative is to remove the n'th instance of number and trim whitespace,
    //  then do pt1 check

    String[] nums = line.split(" ");

    for (int i = 0; i < nums.length; i++) {
      StringBuilder sb = new StringBuilder();
      for (int j = 0; j < nums.length; j++) {
        if (j == i) continue;

        sb.append(nums[j] + " ");
      }
      String num = nums[i];
      String newLine = sb.toString().trim();

      //System.out.printf("newTest:%s, line:%s\n", newLine, line);
      int testLine = part1LineParse(newLine);
      if (testLine == 1) {
        System.out.printf("remove %s for line: %s\n", num, line);
        return 1;
      }
    }

    //fallback is 0

    return 0;
  }

  /**
   * Parses each line for day2 part 1 to see if the line is "safe"
   * 
   * @returns 1 if safe line, 0 if unsafe line.
   */
  private static int part1LineParse(String line) {

    // safe if sequence decreases by 2 or less each time. split by spaces.
    OptionalInt last = OptionalInt.empty();
    Boolean increasing = null;
    for (String strVal : line.split(" ")) {
      int val = Integer.valueOf(strVal);

      if (last.isPresent()) {
        int diff = val - last.getAsInt();
        if (diff > 0) {
          // if this is decreasing and we are increasing, break.
          if (increasing != null && !increasing)
            return 0;

          increasing = true;
        } else if (diff < 0) {
          // if this is increasing and we are decreasing, break.
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
