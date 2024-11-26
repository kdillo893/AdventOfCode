// cube bag with different number of cubes, looks like it at least.
//  process a "Game" and see if that game is possible with count of cubes known
//  -- comma separation in the "Game" listing is one pull of cubes.
//  -- semicolon separation is "attempts" for pulling cubes from the set,
//  resetting between.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Game {
  int gameId;
  int red;
  int green;
  int blue;
  struct Game *nextGame;
} game_t;

void printGame(game_t *game) {
  fprintf(stdout, "theGame: id=%d, red=%d, green=%d, blue=%d\n", game->gameId,
          game->red, game->green, game->blue);
}

int cubePower(game_t *game) {
  return game->red * game->green * game->blue;
}

/**
 * parse the Game line for separate pulls, return the max of cubes
 * Sample game line below:
 * Game 1: 3 green, 1 blue, 3 red; 3 blue, 1 green, 3 red; 2 red, 12 green, 7
 * blue; 1 red, 4 blue, 5 green; 7 green, 2 blue, 2 red
 */
game_t *parseGame(game_t *cubeSet, char *gameLine, int linelen) {

  cubeSet->red = 0;
  cubeSet->green = 0;
  cubeSet->blue = 0;

  // game ID will be the int before the ':' and after the first ' '
  int colon_idx = -1;
  int space_idx = -1;
  for (int i = 0; i < (int)linelen; i++) {
    if (gameLine[i] == ' ') {
      space_idx = i;
    }
    if (gameLine[i] == ':') {
      colon_idx = i;
      break;
    }
  }

  int idLen = colon_idx - space_idx;
  char gameIdStr[idLen];
  strncpy(gameIdStr, gameLine + space_idx + 1, idLen);
  gameIdStr[idLen - 1] = '\0';

  cubeSet->gameId = atoi(gameIdStr);

  // now check the games starting at the char after the colon.
  // can separate by "pull" by tokenizing by ; then by "cube" tokenizing those
  // by ,

  // indicate the start of the "games" for this line.
  int gamesStartIdx = colon_idx + 2;

  // initializes the tokenizing.
  char *pulls;
  char *pull = strtok_r(gameLine + gamesStartIdx, ";", &pulls);

  while (pull != NULL) {

    char *dice_segs;
    char *dice = strtok_r(pull, ",", &dice_segs);
    // for each of the dice thing, evaluate and new max.
    while (dice != NULL) {

      // tokenize by space,
      char *parts;
      char *countStr = strtok_r(dice, " \n", &parts);
      char *color = strtok_r(NULL, " \n", &parts);

      int count = atoi(countStr);

      if (strcmp(color, "green") == 0 && count >= cubeSet->green) {
        cubeSet->green = count;
      }

      if (strcmp(color, "red") == 0 && count >= cubeSet->red) {
        cubeSet->red = count;
      }

      if (strcmp(color, "blue") == 0 && count >= cubeSet->blue) {
        cubeSet->blue = count;
      }

      //gotonext dice count
      dice = strtok_r(NULL, ",", &dice_segs);
    }

    // to continue to next token, use NULL pointer
    pull = strtok_r(NULL, ";", &pulls);
  }

  return cubeSet;

}

int main(int argc, char *argv[]) {

  // inputs are "the input file" and "red/green/blue" cube count for querying
  // games.

  if (argc != 5) {
    fprintf(
        stderr,
        "Search a file for games that can occur with the provided known cube "
        "counts. \nUsage: \"%s [file] [red cubes] [green cubes] [blue "
        "cubes]\"\n",
        argv[0]);
    return -1;
  }

  FILE *inputFile = fopen(argv[1], "r");
  if (inputFile == NULL) {
    fprintf(stderr, "no file found for %s", argv[1]);
    return 2;
  }

  // grab the max cube counts allowed, aka "what we know is in the bag"
  const int maxReds = atoi(argv[2]);
  const int maxGreens = atoi(argv[3]);
  const int maxBlues = atoi(argv[4]);

  // fprintf(stdout, "parms: red=%d, green=%d, blue=%d\n", maxReds, maxGreens,
  //         maxBlues);

  // have file, start parsing lines.
  char *lineptr;
  size_t bufflen = 0;
  ssize_t linelen = -1;

  int idSum = 0;
  int powerSum = 0;

  game_t *currentGame = malloc(sizeof(game_t));

  while ((linelen = getline(&lineptr, &bufflen, inputFile)) != -1) {

    // parse the game, returns the "cubes" count
    currentGame = parseGame(currentGame, lineptr, linelen);

    //calculate powersum for EVERY game, not just the good games.
    powerSum += cubePower(currentGame);

    int greenGood = currentGame->green <= maxGreens;
    int redGood = currentGame->red <= maxReds;
    int blueGood = currentGame->blue <= maxBlues;

    if (redGood && greenGood && blueGood) {
      idSum += currentGame->gameId;
      // fprintf(stdout, "ID=%d, Red=%d(%s), Green=%d(%s), Blue=%d(%s)\n",
      //         currentGame->gameId,
      //         currentGame->red, redGood ? "true" : "false",
      //         currentGame->green, greenGood ? "true" : "false",
      //         currentGame->blue, blueGood ? "true" : "false");
      // fprintf(stdout, "currentIdSum=%d\n", idSum);

    }

    // doing a "getchar" to halt between each.
    // getchar();
  }

  free(currentGame);

  fprintf(stdout, "id sum is: %d\n", idSum);
  fprintf(stdout, "power sum is: %d\n", powerSum);
  return 0;
}
