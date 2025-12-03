const fs = require('node:fs/promises');


/**
 * Check if the number ID passed is repeated sets of characters
 */
function isTwiceDigits(id) {
  id = String(id);

  //check half the length of string,
  //  if it's odd quit;
  //  if it's even,
  //    are they the same strings?
  if (id.length & 1) return false;

  const left = id.substring(0, id.length >> 1);
  const right = id.substring(id.length >> 1);

  if (left == right) {
    console.log({ id, left, right });
    return true;
  }

  return false;
}


function main() {
  console.log("processing", process.argv[2]);

  fs.readFile(process.argv[2], { encoding: "ascii" })
    .then((fbuffer) => {

      //Ideally I wouldn't have split by comma, instead chunk
      //  the file into equal parts per thread and seeking nearest comma
      let totalSum = fbuffer.split(',').map((idRange) => {
        //iterate portions inclusively
        const idRangeParts = idRange.split('-');
        const start = Number(idRangeParts[0]);
        const end = Number(idRangeParts[1]);

        //making range array following mozilla docs for Array.from()
        //  "+1 for inclusive end"
        const badIds = Array.from(
          {
            length: Math.ceil(end - start + 1)
          },
          (_, i) => start + i)
          .filter((id) => isTwiceDigits(id));

        //console.log(badIds);

        return badIds;
      }).flat()
        .reduce((badId, runningTotal) => runningTotal + badId, 0);

      console.log("total sum:", totalSum);

    }).catch((err) => console.error(err));
}

main();
