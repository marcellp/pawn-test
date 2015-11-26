/* pawn-test
 * A ridiculously simple automated testing framework for PAWN scripts.
 *
 * Copyright (c) 2015 Pék Marcell
 * This project is covered by the MIT License, see the attached LICENSE
 * file for more details.
 *
 * test-functions.inc - functions that aid unit testing
 *
 */
 
#define PAWNTEST_ASSERT_EQ(%1,%2,%3) PawnTest_AssertEx(%1 == %2, %3 " (==)")
#define PAWNTEST_ASSERT_NQ(%1,%2,%3) PawnTest_AssertEx(%1 != %2, %3 " (!=)")
#define PAWNTEST_ASSERT_LT(%1,%2,%3) PawnTest_AssertEx(%1 < %2, %3 " (<)")
#define PAWNTEST_ASSERT_LE(%1,%2,%3) PawnTest_AssertEx(%1 <= %2, %3 " (<=)")
#define PAWNTEST_ASSERT_GT(%1,%2,%3) PawnTest_AssertEx(%1 > %2, %3 " (>)")
#define PAWNTEST_ASSERT_GE(%1,%2,%3) PawnTest_AssertEx(%1 >= %2, %3 " (>=)")

#define PAWNTEST_ASSERT_STR_EQ(%1,%2,%3) PawnTest_AssertEx( !strcmp(%1, %2), %3 " (string eq)")

#define PAWNTEST_ASSERT_TRUE(%1,%2) PawnTest_AssertEx(%1 == true, %2 " (true)")
#define PAWNTEST_ASSERT_FALSE(%1,%2) PawnTest_AssertEx(%1 == false, %2 " (false)")

/**
    <summary>
        Tests that the supplied arguments are approximately equal.
        It calculates the difference between the two arguments and checks
        if that difference is less than or equal to 2^-11.
    </summary>
*/
#define PAWNTEST_ASSERT_AEQ(%1,%2,%3) PawnTest_AssertEx( floatabs(%1-%2) < floatpower(2,-11), %3 " (almost eq)" )

/**
    <summary>
        Tests that the supplied arguments are approximately equal.
        It calculates the difference between the two arguments and checks if
        that difference is less than or equal to the delta value supplied by the user.
    </summary>
*/
#define PAWNTEST_ASSERT_AEQ_D(%1,%2,%3,%4) PawnTest_AssertEx( floatabs(%1-%2) < %3, %4 " (almost eq, custom d)" )

/**
    <summary>
        Evaluates a boolean condition that should be true
        when the function is called.
    </summary>

    <param name="condition">
        The condition to be evaulated.
    </param>
    
    <param name="message[]">
        The message to be displayed if the assertion fails.
    </param>

    <param name="return_if_false">
        If the condition evaluates to false, this value will be returned.
    </param>
    
    <param name="return_if_true">
        If the condition evaluates to true, this value will be returned.
    </param>
    
    <returns>
        return_if_true if the passed boolean value is true, return_if_false otherwise.
    </returns>
*/
stock PawnTest_Assert(condition, message[], return_if_false = PAWNTEST_CASE_FAILED, return_if_true = PAWNTEST_CASE_PASSED)
{
    // Convert integer to boolean in case the function returns an int instead of a bool.
    if( !(!!condition) ) {
        printf("\t\tassertion \"%s\" failed.", message );
        return return_if_false;
    }
    
    return return_if_true;
}

/**
    <summary>
        Evaluates a boolean condition that should be true
        when the function is called.
    </summary>

    <param name="condition">
        The condition to be evaulated.
    </param>
    
    <param name="message[]">
        The message to be displayed if the assertion fails.
    </param>

    <returns>
        True if the passed boolean value is true, false otherwise.
    </returns>
*/
stock bool:PawnTest_AssertEx(condition, message[])
{    
    return !!(PawnTest_Assert(condition, message, false, true));
}

#define PAWNTEST_MAKE_FAIL(%1,%2) PawnTest_MakeReturnWith(%1 ,%2, )

/**
    <summary>
        Force the test case to return a given value. This value
        will override the actual return value of the test function.
    </summary>

    <param name="suiteid">
        The ID of the test suite returned by PawnTest_InitSuite().
    </param>

    <param name="caseid">
        The ID of the test case returned by PawnTest_AddCase().
    </param>

    <param name="result">
        The result code the test case will return.
    </param>    
    
    <returns>
        This function does not handle returns.
    </returns>

    <remarks>
        This takes effect when the test case is next run.
    </remarks>
*/
stock PawnTest_MakeReturnWith(suiteid, caseid, result)
{
    if( !_PawnTest_CheckSuiteBounds(suiteid) ) return;
   
    if( !PawnTest_IsValidCase(suiteid, caseid) ) {
        PawnTest_errno = PAWNTEST_OBJ_NOT_FOUND;
        return;
    }
    
    _PawnTest_Cases[suiteid][caseid][PAWNTEST_RETURN_WITH] = result;
    return;
}

/**
    <summary>
        Marks a test case as an expected failure. This means that it will
        not be counted as a failure if it fails when run.
    </summary>

    <param name="suiteid">
        The ID of the test suite returned by PawnTest_InitSuite().
    </param>

    <param name="caseid">
        The ID of the test case returned by PawnTest_AddCase().
    </param>

    <param name="result">
        The result code the test case will return.
    </param>    
    
    <remarks>
        This takes effect when the test case is next run.
    </remarks>
*/
stock PawnTest_ExpectFailure(suiteid, caseid)
{
    if( !_PawnTest_CheckSuiteBounds(suiteid) ) return;
   
    if( !PawnTest_IsValidCase(suiteid, caseid) ) {
        PawnTest_errno = PAWNTEST_OBJ_NOT_FOUND;
        return;
    }
    
    _PawnTest_Cases[suiteid][caseid][PAWNTEST_EXPECTED_TO_FAIL] = true;
    return;
}

/**
    <summary>
        Force the test suite to terminate.
    </summary>

    <param name="suiteid">
        The ID of the test suite returned by PawnTest_InitSuite().
    </param>
    
    <returns>
        This function does not handle returns.
    </returns>

    <remarks>
        This takes effect /after/ a test case from the test suite
        is next run.
    </remarks>
*/
stock PawnTest_Stop(suiteid)
{
    if( !_PawnTest_CheckSuiteBounds(suiteid) ) return;

    _PawnTest_Suites[suiteid][PAWNTEST_STOP] = true;
    return;
}