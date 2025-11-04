/**
 * @file main.c
 * @author Dánjal Leitisstein Olsen (s255821@dtu.dk)
 * @brief
 * @version 0.1
 * @date 2025-10-28
 *
 * @copyright Copyright (c) 2025
 */

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_COUNT 20'000
#define MAX_SUCCESSOR_COUNT MAX_WORD_COUNT / 2

char book[] = {
#embed "Frankenstein.txt"
    , '\0'};

/// Array of tokens registered so far
/// No duplicates are allowed.
char *tokens[MAX_WORD_COUNT];
/// `tokens`'s current size
size_t tokens_size = 0;

/// Array of successor tokens
/// One token can have many successor tokens. `succs[x]` corresponds to
/// `token[x]`'s successors.
/// We store directly tokens instead of token_ids, because we will directly
/// print them. If we wanted to delete the book, then it would make more sense
/// to store `token_id`s
char *succs[MAX_WORD_COUNT][MAX_SUCCESSOR_COUNT];
/// `succs`'s size
size_t succs_sizes[MAX_WORD_COUNT] = {};

/// Overwrites non-printable characters in `book` with a space.
/// Non-printable characters may lead to duplicates like
/// `"\xefthe" != "the"` even both print `the`.
void replace_non_printable_chars_with_space() {
  size_t len = strlen(book);

  for (size_t i = 0; i < len; i++) {
    // If book[i] is not one of the ASCII printable characters,
    // replace it with a space.
    if (!isprint(book[i])) {
      book[i] = ' ';
    }
  }
}

/// Returns token id if token is known, otherwise creates a new entry and
/// returns the new token id
size_t token_id(char *token) {
  size_t id;
  for (id = 0; id < tokens_size; ++id) {
    if (strcmp(tokens[id], token) == 0) {
      return id;
    }
  }
  tokens[id] = token;
  tokens_size++;
  return id;
}

/// Appends a token to the successors list of a `token`
void append_to_succs(char *token, char *succ) {
  size_t *next_empty_index_ptr = &succs_sizes[token_id(token)];

  if (*next_empty_index_ptr >= MAX_SUCCESSOR_COUNT) {
    fprintf(stderr, "Successor array full.");
    exit(EXIT_FAILURE);
  }

  succs[token_id(token)][(*next_empty_index_ptr)++] = succ;
}

/// Creates tokens from the `book` and fills `tokens` and `succs` arrays using
/// the `token`s.
void tokenize_and_fill_succs(char *delimiters, char *str) {
  // Block for tokenizing Frankenstein.txt and assigning token IDs.
  size_t tokenIDs_in_order[MAX_WORD_COUNT];
  size_t order_len = 0;

  char *token = strtok(str, delimiters);
  while (token) {
    if (order_len >= MAX_WORD_COUNT) {
      fprintf(
          stderr,
          "Too many tokens (>%d). Increase MAX_WORD_COUNT or shorten input.\n",
          (int)MAX_WORD_COUNT);
      exit(EXIT_FAILURE);
    }
    size_t id = token_id(token);
    tokenIDs_in_order[order_len] = id;
    order_len++;
    token = strtok(NULL, delimiters);
  }

  if (order_len < 2) {
    return;
  }

  // Block for populating successor table.
  char *succ;

  for (size_t i = 0; i < tokens_size; i++) {
    token = tokens[i];

    for (size_t j = i + 1; j < order_len; j++) {
      if (strcmp(token, tokens[tokenIDs_in_order[j - 1]]) == 0) {
        succ = tokens[tokenIDs_in_order[j]];

        if (succs_sizes[i] >= MAX_SUCCESSOR_COUNT) {
          continue;
        }

        append_to_succs(token, succ);
      }
    }
  }
}

int main() {
  replace_non_printable_chars_with_space();

  tokenize_and_fill_succs(" \n\r", book);

  if (tokens_size <= 0) {
    printf("No tokens found. Book[0]=0X%02X\n", (unsigned char)book[0]);
  }

  printf("Tokens size: %zu\n", tokens_size);
  for (size_t i = 0; i < 200 && i < tokens_size; i++) {
    printf("Token %zu: '%s'\n", i, tokens[i]);
    printf("Successors for this token: %zu\n", succs_sizes[i]);
    for (size_t j = 0; j < 10 && j < succs_sizes[i]; j++) {
      printf("  Successor %zu: '%s'\n", j, succs[i][j]);
    }
  }
}

/// Returns last character of a string
char last_char(char *str) {
  char lastChar = str[strlen(str) - 1];

  return lastChar;
}

/// Returns whether the token ends with `!`, `?` or `.`.
bool token_ends_a_sentence(char *token) {
  if (!(last_char(token) == '!' || last_char(token) == '?' ||
        last_char(token) == '.')) {
    return false;
  }

  return true;
}

/// Returns a random `token_id` that corresponds to a `token` that starts with a
/// capital letter.
/// Uses `tokens`, `tokens_size.
size_t random_token_id_that_starts_a_sentence() {
  size_t tokenID = 0;
  size_t randomTokenID;
  bool foundID = false;

  while (!foundID) {
    // Generate a random tokenID from 0..tokens_size - 1.
    randomTokenID = rand() % tokens_size;
    if (isupper(tokens[randomTokenID][0])) {
      tokenID = randomTokenID;
      foundID = true;
    }
  }

  return tokenID;
}

/*

/// Generates a random sentence using `tokens`, `succs`, and `succs_sizes`.
/// The sentence array will be filled up to `sentence_size-1` characters using
/// random tokens until:
/// - a token is found where `token_ends_a_sentence()`
/// - or more tokens cannot be concatenated to the `sentence` anymore.
///  Returns the filled sentence array.
char *generate_sentence(char *sentence, size_t sentence_size) {
  size_t current_token_id = random_token_id_that_starts_a_sentence();
  auto token = tokens[current_token_id];

  strcat(sentence, token);
  if (token_ends_a_sentence(token))
    return sentence;

  // Calculated sentence length for the next iteration.
  // Used to stop the loop if the length exceeds sentence size
  size_t sentence_len_next;
  // Concatenates random successors to the sentence as long as
  // `sentence` can hold them.
  do {
    // YOUR CODE HERE
  } while (sentence_len_next < sentence_size - 1);
  return sentence;
}

int main() {
  replace_non_printable_chars_with_space();

  char *delimiters = " \n\r";
  tokenize_and_fill_succs(delimiters, book);

  char sentence[1000];
  srand(time(NULL));

  // Initialize `sentence` and then find a question sentence.
  do {
    // YOUR CODE HERE
    generate_sentence(sentence, sizeof sentence);
  } while (last_char(sentence) != '?');
  puts(sentence);
  puts("");

  // Initialize `sentence` and then find a sentence ending with exclamation
  // mark.
  do {
    // YOUR CODE HERE
    generate_sentence(sentence, sizeof sentence);
  } while (last_char(sentence) != '!');
  puts(sentence);
}
*/