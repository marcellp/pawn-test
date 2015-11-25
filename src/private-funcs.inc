/* pawn-test
 * A ridiculously simple automated testing framework for PAWN scripts.
 *
 * Copyright (c) 2015 Pék Marcell
 * This project is covered by the MIT License, see the attached LICENSES
 * file for more details.
 *
 * private-funcs.inc - a few functions that pawn-test uses internally
 *
 */
 
/**
    <summary>
        Clears a given test suite.
    </summary>

    <param name="suiteid">
        The ID of the test suite returned by PawnTest_InitSuite()
    </param>
    
    <returns>
        This function does not handle returns.
    </returns>
*/
stock _PawnTest_ClearSuite(suiteid)
{
    if( !_PawnTest_CheckSuiteBounds(suiteid) ) return;

    for(new i = 0; _E_PAWNTEST_SUITES:i < _E_PAWNTEST_SUITES; i++) {
        _PawnTest_Suites[suiteid][_E_PAWNTEST_SUITES:i] = 0;
    }
    
    for( new i = 0; i<PAWNTEST_MAX_CASES; i++ ) {
        _PawnTest_Cases[suiteid][i][PAWNTEST_CASE_NAME][0] = '\0';
        _PawnTest_Cases[suiteid][i][PAWNTEST_RETURN_WITH] = -1;
        _PawnTest_Cases[suiteid][i][PAWNTEST_EXPECTED_TO_FAIL] = false;
    }
    
    return;
}

/**
    <summary>
        Checks if a given test suite ID is valid.
    </summary>

    <param name="suiteid">
        The ID of the test suite returned by PawnTest_InitSuite()
    </param>
    
    <returns>
        False if the array index is out of bounds, true otherwise.
    </returns>
*/
stock _PawnTest_CheckSuiteBounds(suiteid)
{
    if( suiteid >= PAWNTEST_MAX_CASES || suiteid < 0 ) {
        PawnTest_errno = PAWNTEST_SUITE_OUT_OF_BOUNDS;
        return false;
    }
    return true;
}

/**
    <summary>
        Prints the result of a given test case.
    </summary>

    <param name="tag[]">
        The string that shows up in brackets (has to be 10 characters long).
    </param>

    <param name="casename[]">
        The name of the test case.
    </param>

    <param name="timer_this">
        An integer storing the time at which this test case was called.
    </param>

    <param name="timer_suite">
        An integer storing the time at which the test run was started.
    </param>

    <returns>
        This function does not handle returns.
    </returns>
*/
stock _PawnTest_PrintTestResult(tag[], casename[], timer_this, timer_suite)
{
    if( strlen(tag) > 8 ) return;
    
    // This is a pretty hacky fix but PAWN doesn't support C-style paddings (%10s, %-10s).
    new padding[9] = "        ";
    strdel(padding, 0, strlen(tag));
    
    printf( "[ %s%s ]\ttest \"%s\" (%d ms, %.3f s in total).",
            padding, tag, casename, GetTickCount() - timer_this,
            (GetTickCount() - timer_suite)*0.001);
    return;
}

stock _PawnTest_dummy( val ) {
    return val;
}