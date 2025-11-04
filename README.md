# Overview

This is a basic C program that uses a Markov chain–like approach to generate random sentences from the book *Frankenstein; Or, The Modern Prometheus*. It tokenizes the text, tracks which words follow which, and then produces random sentences by linking together these successor relationships. The program continues generating until it finds a question sentence and then an exclamation sentence.

# Files

## main.c

This file contains all of the program logic and data structures. It embeds the full text of ``Frankenstein.txt``, cleans it of non-printable characters, tokenizes it, and builds a table of successors to the tokens.. It then randomly generates sentences based on the tokens and their successors.

### replace_non_printable_chars_with_space()

Loops through every character in the embedded text and replaces non-printable characters with spaces to prevent duplicate or malformed tokens.

### token_id()

Checks whether a word (token) has already been registered.  
If it exists, returns its ID; otherwise adds it to the token list.

### append_to_succs()

Adds a successor word to the list of words that can follow a given token.

### tokenize_and_fill_succs()

Splits the text into tokens using spaces, newlines and carriage returns as delimiters.  
It assigns each unique token an ID and then builds successor lists showing which words tend to follow which in the text.

### last_char()

Returns the last character of a given string.

### token_ends_a_sentence()

Checks if a token ends with punctuation that closes a sentence (`.`, `!`, or `?`).

### random_token_id_that_starts_a_sentence()

Selects a random token that begins with a capital letter. This is used to start a new generated sentence.

### generate_sentence()

Constructs a random sentence by repeatedly picking successors for the current token.  
The loop continues until:
- a token is found that ends a sentence (`.`, `!`, or `?`), or  
- the maximum sentence length is reached.

### main()

The entry point of the program. It performs the following steps:
1. Replaces all non-printable characters in the text.
2. Tokenizes the text and builds the successor lists.
3. Randomly generates and prints:
   - one question sentence (ending with `?`)
   - one exclamation sentence (ending with `!`)

The result is two random sentences that resemble the writing style of *Frankenstein; Or, The Modern Prometheus*.