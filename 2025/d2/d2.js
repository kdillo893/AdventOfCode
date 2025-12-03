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

/**
 * check if the id number has repeated digits anywhere
 */
function isRepeatedDigits(id) {
  id = String(id);

  //growing portions of checks from "1" up to "len/2"
  //starting at the "len/2" sub-size parts is likely the easiest.

  for (let subSize = Math.floor(id.length / 2); subSize > 0; subSize--) {
    //if not divisible by the substring length, go next
    if (id.length % subSize != 0) continue;

    //split the id into parts of size subSize, check if all are equal
    //I would prefer a faster way than looping for slices.
    const parts = [];
    for (let i = 0; i < id.length; i += subSize) {
      parts.push(id.slice(i, i + subSize));
    }

    const isRepeated = parts.every(part => part == parts[0]);

    if (isRepeated) {
      console.log({ parts, id, subSize })
      return isRepeated;
    }
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
          //.filter((id) => isTwiceDigits(id));
          .filter((id) => isRepeatedDigits(id));

        //console.log(badIds);

        return badIds;
      }).flat()
        .reduce((badId, runningTotal) => runningTotal + badId, 0);

      console.log("total sum:", totalSum);

    }).catch((err) => console.error(err));
}

main();
