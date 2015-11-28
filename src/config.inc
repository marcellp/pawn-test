/* pawn-test
 * A ridiculously simple automated testing framework for PAWN scripts.
 *
 * Copyright (c) 2015 Pék Marcell
 * This project is covered by the MIT License, see the attached LICENSE
 * file for more details.
 *
 * config.inc - the pawn-test configuration file
 *
 */

// The current version of pawn-test
#define PAWNTEST_VERSION "1.1"

// This is the maximum # of test suites that can be stored in memory.
#define PAWNTEST_MAX_SUITES 50

// This is the maximum # of test cases in a test suite that can be stored in memory.
#define PAWNTEST_MAX_CASES 50

// This defines the maximum length of a number of strings, including suite names and
// test case function names.
#define PAWNTEST_MAX_SUITE_NAME 75