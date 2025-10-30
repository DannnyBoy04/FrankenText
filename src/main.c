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
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_COUNT 50'000
#define MAX_SUCCESSOR_COUNT MAX_WORD_COUNT / 10
#define RAND_MAX 50'000

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
size_t succs_sizes[MAX_WORD_COUNT];

/// Overwrites non-printable characters in `book` with a space.
/// Non-printable characters may lead to duplicates like
/// `"\xefthe" != "the"` even both print `the`.
void replace_non_printable_chars_with_space() {
  // YOUR CODE HERE

  for (size_t i = 0; i < strlen(book) - 1; i++) {
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
  auto next_empty_index_ptr = &succs_sizes[token_id(token)];

  if (*next_empty_index_ptr >= MAX_SUCCESSOR_COUNT) {
    printf("Successor array full.");
    exit(EXIT_FAILURE);
  }

  succs[token_id(token)][(*next_empty_index_ptr)++] = succ;
}

/// Creates tokens from the `book` and fills `tokens` and `succs` arrays using
/// the `token`s.
void tokenize_and_fill_succs(char *delimiters, char *str) {
  // YOUR CODE HERE

  // Block for tokenizing Frankenstein.txt and assigning token IDs.

  char *token;
  size_t index = 0;

  token = strtok(str, delimiters);

  while (token) {
    tokens[index] = token;

    token = strtok(NULL, delimiters);

    index++;
  }

  // Block for populating successor table.
}

/// Returns last character of a string
char last_char(char *str) {
  // YOUR CODE HERE

  char lastChar = str[strlen(str) - 1];

  return lastChar;
}

/// Returns whether the token ends with `!`, `?` or `.`.
bool token_ends_a_sentence(char *token) {
  // YOUR CODE HERE

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
  // YOUR CODE HERE

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