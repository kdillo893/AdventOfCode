const fs = require('node:fs/promises');

//Basic idea is "rotating dial, 0 - 99"

function main() {

  console.log("processing", process.argv[2]);

  (async () => {
    try {
      const file = await fs.open(process.argv[2]);

      //position starts at 50
      let position = 50;
      let pw = 0;

      for await (const line of file.readLines()) {

        //dial spin: L = -1, R = +1
        const lr = 1 * (((line.charAt(0) === 'R') * 2) - 1)
        let val = Number(line.substring(1));

        //part2 method:
        //  a. if number is greater than 100, mod it 100 and
        //      add 100s place to pw
        const fulls = Math.floor(val / 100);
        pw += fulls;
        val = val % 100;

        const lastPos = position;
        position = position + (lr * val);
        //  b. if going from positive to negative, give extra point
        //      ensure not from 0
        const leftFlip = position < 0 && lastPos != 0;
        if (leftFlip) {
          pw++;
        }

        //  c. if exceeding 100, extra point
        const rFlip = position > 100;
        if (rFlip) {
          pw++;
        }

        //position fixing: add 100 mod 100, lets negatives go in
        //  the normal wheel
        position = (position + 100) % 100;

        //1. "end on 0"
        if (!position) {
          pw++;
        }

        console.log({ line, position, l: Number(leftFlip), r: Number(rFlip), fulls, pw });
      }

      console.log("pw=", pw);

    } catch (er) {
      console.error(er);
    }
  })();

}

main()
