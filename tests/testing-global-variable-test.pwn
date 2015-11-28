/* pawn-test
 * A ridiculously simple automated testing framework for PAWN scripts.
 *
 * Copyright (c) 2015 Pék Marcell
 * This project is covered by the MIT License, see the attached LICENSE
 * file for more details.
 *
 * testing-glboal-variable-test.pwn - testing the PawnTest_testing global
 *                                    variable
 *
 */

#include <a_samp>
#include <pawn-test/pawn-test>

forward Test();

public Test()
{
    return PawnTest_Assert( PawnTest_testing == true,
                            "PawnTest_testing is false during testing.");
}

main()
{
    new TestSuite = PawnTest_InitSuite("Testing the 'pawntest_testing' global variable");
    PawnTest_AddCase(TestSuite, "Test");
    PawnTest_Run(TestSuite);
    PawnTest_DestroySuite(TestSuite);
}