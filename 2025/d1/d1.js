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
          //first character is L/R, this determines add/sub
          // L = -1, R = +1
          const lr = 1 * (((line.charAt(0) === 'R') * 2) - 1)

          //the value is int at index 1 of the line
          const val = Number(line.substring(1));

          position = position + (lr * val);

          //position fixing: add 100 mod 100, lets negatives go in
          //  the normal wheel
          position = (position + 100) % 100;

          if (!position) {
            pw++;
          }

        }

        console.log("pw=", pw);

      } catch (er) {
        console.error(er);
      }
    })();

}

main()
