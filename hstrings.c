/*
 * hstrings is a reimagination of the strings program, where each
 * found string is scored higher if the characters resemble the
 * distribution of "human" words, and scores lower if not.
 *
 * Only ascii, and the scoring is based on english character frequencies.
 *
 * ./hstrings < some.bin | sort
 *
 * First version: 2025-01-25
 * Updated      : 2025-01-25
 * Xabier Saez de Camara
 */

#include <stdio.h>
#include <ctype.h>

#define MAX_CANDIDATE_BUFFSIZE 2 * 1024
#define MIN_LEN 4

static int total_chars = 0;

/*
 * array from 'a' to 'z', the value of each item is log(freq), where
 * freq is the frequency of the correspondig character in english
 * from: https://en.wikipedia.org/wiki/Letter_frequency
 */
static double logprob[] = {
  2.1001016443761387,    0.40011750178156913,  1.023170093501251,
  1.4476246162714965,    2.5417594613807832,   0.8011043220650377,
  0.7006191953986464,    1.8073046805635173,   1.9410411719438094,
  -1.8773173575897015,   -0.25877072895736086, 1.3925249108705269,
  0.8779656175524871,    1.9093943457612694,   2.015835918590865,
  0.6570017339235921,    -2.353878387381596,   1.789590451943215,
  1.8448261901647471,    2.203427521460827,    1.01450577937111,
  -0.022245608947319737, 0.8586616190375187,   -1.8971199848858813,
  0.6800619410112898,    -2.6036901857779675};

int get_candidate(char *candidate, int limit);
int is_candidate_char(int c);
double score_candidate(char *candidate);

int main(void)
{
  int retval = 0;
  char candidate[MAX_CANDIDATE_BUFFSIZE];
  int candidate_len = 0;
  double score = 0;

  while ((candidate_len = get_candidate(candidate, MAX_CANDIDATE_BUFFSIZE)) != EOF) {
    if (candidate_len >= MIN_LEN) {
      score = score_candidate(candidate);
      printf("(%.3g): %s\n", score, candidate);
    }
    if (candidate_len == MAX_CANDIDATE_BUFFSIZE) {
      retval = 1;
    }
  }

  printf("hstrings: Read %d\n", total_chars);
  if (retval != 0) {
    printf("hstrings: Warning. Some lines were truncated\n");
  }
  return retval;
}

int get_candidate(char *candidate, int limit)
{
  int c;
  int i = 0;

  while ((c = getchar()) != EOF && limit > 0) {
    total_chars++;
    if (is_candidate_char(c)) {
      candidate[i++] = (char) c;
      limit--;
    } else {
      break;
    }
  }

  candidate[i] = '\0';
  return c == EOF? EOF : i;
}

int is_candidate_char(int c)
{
  return isprint(c);
}

double score_candidate(char *candidate)
{
  double score = 0;
  int i;

  for (i = 0; candidate[i] != '\0'; i++) {
    if (isalpha(candidate[i])) {
      score += logprob[tolower(candidate[i]) - 'a'];
    }
  }

  return i == 0? 0.0 : score/i;
}
