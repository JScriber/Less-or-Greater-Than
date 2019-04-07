#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct {
  unsigned int min;
  unsigned int max;
} Interval;

/**
 * Checks if the given value is within the interval boundaries.
 * @param interval
 * @param value
 */
int is_within_boundaries(const Interval* INTERVAL, int value)
{
  return value >= (*INTERVAL).min && value <= (*INTERVAL).max;
}

/**
 * Get a random value in the given interval.
 * @param interval
 * @return a random value.
 */
int random_between(const Interval* interval_ptr)
{
  const Interval interval = *interval_ptr;
  srand(time(NULL));

  return (int) (rand() % (interval.max - interval.min) + interval.min);
}

/**
 * Asks the user to pick a value.
 * @param interval
 */
int select_value(const Interval* interval_ptr)
{
  const Interval interval = *interval_ptr;
  int selected;
  const int BUFFER_LIMIT = 100;
  char buffer[BUFFER_LIMIT], *end;

  do {
    printf("> ");

    fgets(buffer, BUFFER_LIMIT * sizeof(char), stdin);
    buffer[strlen(buffer) - 1] = 0;

    selected = strtol(buffer, &end, 10);

    if (end == buffer) {
      printf("Please enter an integer!\n");
    } else {
      if (selected < interval.min) {
        printf("The value musn't be lesser than %d!\n", interval.min);
      } else {
        if (selected > interval.max) {
          printf("The value musn't be greater than %d!\n", interval.max);
        }
      }
    }

  } while(end == buffer || !is_within_boundaries(interval_ptr, selected));

  return selected;
}

/**
 * Starts the game.
 * @param interval
 * @param limit of trials.
 */
int init_game(const Interval* interval_ptr, const unsigned int MAX_TRIALS) {
  const Interval interval = *interval_ptr;
  const int random_value = random_between(interval_ptr);

  int selected, trials = 0, leave = 0;

  printf("You must choose something in between %d and %d.\n", interval.min, interval.max);

  do {
    // Ask the user to select a value.
    int selected = select_value(interval_ptr);

    if (selected == random_value) {
      printf("Congratulations! You guessed it!\n");
    } else {
      if (selected > random_value) {
        printf("Is lesser than %d.", selected);
      } else {
        printf("Is greater than %d.", selected);
      }

      // Calculate trial left.
      const int TRIALS_LEFT = MAX_TRIALS - (++ trials);

      printf(" %d %s left.\n", TRIALS_LEFT, (TRIALS_LEFT > 1 ? "trials" : "trial"));
    }

    leave = selected == random_value || trials == MAX_TRIALS;
  } while (!leave);

  // Check if has left because of defeat.
  if (trials == MAX_TRIALS) {
    printf("You lost the game! The number to guess was %d.\n", random_value);
  }
}

/** Says how the program msut be used. */
void doc_program() {
  printf("Expected parameters:\nP1: min interval\nP2: max interval\nP3: max trials\n");
}

/** Extracts the value out of the parameter. */
unsigned int extract_param(const char * param) {
  char * end;
  int value = strtol(param, &end, 10);

  if (param == end || value < 0) {
    doc_program();

    exit(EXIT_FAILURE);
  }

  return (unsigned int) value;
}

int main(int argc, char const ** argv)
{
  // Expects 4 arguments (3 + the initial string).
  const unsigned int EXPECTED_ARGS = 4;

  // Default interval.
  Interval interval = {
    min: 1,
    max: 20
  };

  // Default number of trials.
  unsigned int max_trials = 5;

  if (argc == EXPECTED_ARGS) {
    interval.min = extract_param(argv[1]);
    interval.max = extract_param(argv[2]);
    max_trials   = extract_param(argv[3]);
  }

  // Start the game with the parameters.
  init_game(&interval, max_trials);

  return 0;
}
