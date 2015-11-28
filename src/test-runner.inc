/* pawn-test
 * A ridiculously simple automated testing framework for PAWN scripts.
 *
 * Copyright (c) 2015 Pék Marcell
 * This project is covered by the MIT License, see the attached LICENSE
 * file for more details.
 *
 * test-runner.inc - the pawn-test test runner
 *
 */

/**
    <summary>
        Executes a test suite.
    </summary>

    <param name="suiteid">
        The ID of the test suite returned by PawnTest_InitSuite().
    </param>

    <param name="fail_after = 0">
        The test run will stop after this many failures.
    </param>    
   
    <returns>
        Returns true if the test suit passed, false otherwise.
    </returns>
*/
stock bool:PawnTest_Run(suiteid, fail_after = 0)
{
    new bool:ok = true;
    new bool:test_failed = false;
    
    new counter[2];
    new ret = 0;
    new failed = 0;
    new tests_run = 0;
    
    // Do some preliminary checks.
    if( !_PawnTest_CheckSuiteBounds(suiteid) ) return false;
    
    if( !_PawnTest_Suites[suiteid][PAWNTEST_INITIALIZED] ) {
        PawnTest_errno = PAWNTEST_OBJ_NOT_INITIALIZED;
        return false;
    }
    
    // Don't run the test suit if there are no tests in it.
    if( _PawnTest_Suites[suiteid][PAWNTEST_NUM_OF_CASES] == 0 ) {
        printf("[==========]\ttest suite \"%s\" has no tests, terminating.\n",
            _PawnTest_Suites[suiteid][PAWNTEST_SUITE_NAME]);
        return false;
    }

    PawnTest_testing = true;
    
    printf("[==========]\trunning test suite \"%s\" w/ %d test case(s)...",
            _PawnTest_Suites[suiteid][PAWNTEST_SUITE_NAME],
            _PawnTest_Suites[suiteid][PAWNTEST_NUM_OF_CASES]);

    // Call the setup function if there is one.
    if( _PawnTest_Suites[suiteid][PAWNTEST_CONSTRUCTOR_NAME][0] != '\0' ) {
        ret = CallLocalFunction(_PawnTest_Suites[suiteid][PAWNTEST_CONSTRUCTOR_NAME],
                                "i", suiteid);

        switch (ret) {
            case PAWNTEST_SETUP_SUCCESSFUL: {
                printf("[----------]\tthe test environment was successfully set up.");
            }
            case PAWNTEST_SETUP_FAILED: {
                printf("[  FAILED  ]\tthe test environment could not be set up.");
                printf("[  FAILED  ]\tyou might have to clean up after yourself.");
                printf("[==========]\ttesting stopped.");

                PawnTest_testing = false;
                return false;
            }
            default: {
                printf("[  FAILED  ]\tthe setup function's return value isn't meaningful.");
                printf("[  FAILED  ]\tyou might have to clean up after yourself.");
                printf("[==========]\ttesting stopped.");

                PawnTest_testing = false;
                return false;            
            }
        }
    }
    
    // Store the time when the test suit started.
    counter[1] = GetTickCount();
    
    for( new i = 0; i<PAWNTEST_MAX_CASES; i++ ) {
        test_failed = false;
        if( !PawnTest_IsValidCase(suiteid, i) ) continue;
                
        printf("[ RUN      ]\ttest \"%s\" (%.0f%%)",
                _PawnTest_Cases[suiteid][i][PAWNTEST_CASE_NAME],
                floatdiv(i+1,_PawnTest_Suites[suiteid][PAWNTEST_NUM_OF_CASES])*100);

        // Store the time when the particular test case was called.
        counter[0] = GetTickCount();
        ret = CallLocalFunction(_PawnTest_Cases[suiteid][i][PAWNTEST_CASE_NAME], "ii", suiteid, i);

        // If MakeReturnWith was called, make this fail regardless of the return value.
        if( _PawnTest_Cases[suiteid][i][PAWNTEST_RETURN_WITH] != -1 )
            ret = _PawnTest_Cases[suiteid][i][PAWNTEST_RETURN_WITH];

        // If ExpectFailure was called, change the return value to an internal expected value.
        if( _PawnTest_Cases[suiteid][i][PAWNTEST_EXPECTED_TO_FAIL]
            && ( ret == PAWNTEST_CASE_FAILED || PAWNTEST_SUITE_FAILED) )
            ret = _PAWNTEST_EXPECTED_FAILURE;

        // Handle the function based on its return value.
        switch( ret ) {
            case PAWNTEST_CASE_PASSED: {
                ok &= true;

                _PawnTest_PrintTestResult(  "OK",
                                            _PawnTest_Cases[suiteid][i][PAWNTEST_CASE_NAME],
                                            counter[0], counter[1]);
                }
            case PAWNTEST_CASE_FAILED: {
                ok &= false;
                test_failed = true;

                _PawnTest_PrintTestResult(  "FAIL",
                                            _PawnTest_Cases[suiteid][i][PAWNTEST_CASE_NAME],
                                            counter[0], counter[1]);
            }
            case _PAWNTEST_EXPECTED_FAILURE: {
                ok &= true;

                _PawnTest_PrintTestResult(  "EFAIL",
                                            _PawnTest_Cases[suiteid][i][PAWNTEST_CASE_NAME],
                                            counter[0], counter[1]);
            }            
            case PAWNTEST_SUITE_FAILED: {
                ok &= false;
                test_failed = true;

                printf("[ CASEFAIL ]\ttest \"%s\" (%d ms, %.3f s in total).",
                       _PawnTest_Cases[suiteid][i][PAWNTEST_CASE_NAME],
                       GetTickCount() - counter[0],
                       (GetTickCount() - counter[1])*0.001);
                printf("[ CASEFAIL ]\ttest suite \"%s\" (#%d) FAILED, terminating...",
                        _PawnTest_Suites[suiteid][PAWNTEST_SUITE_NAME],
                        suiteid);
                break;
            }
            case PAWNTEST_CASE_SKIPPED: {
                ok &= true;

                _PawnTest_PrintTestResult(  "SKIP",
                                            _PawnTest_Cases[suiteid][i][PAWNTEST_CASE_NAME],
                                            counter[0], counter[1]);
            }
            default: {
                ok &= true;

                _PawnTest_PrintTestResult(  "???",
                                            _PawnTest_Cases[suiteid][i][PAWNTEST_CASE_NAME],
                                            counter[0], counter[1]);
            }
        }
        
        // Increment the counter if it was a failure (and the failure wasn't expected)
        if( test_failed )
            failed++;

        tests_run++;
        
        // Reset the "expect to fail" property when the test is run.
        _PawnTest_Cases[suiteid][i][PAWNTEST_EXPECTED_TO_FAIL] = false;
        
        // Reset the "make case fail" property as well.
        _PawnTest_Cases[suiteid][i][PAWNTEST_RETURN_WITH] = -1;
                
        // If failfast is enabled, stop testing.
        if( fail_after > 0 && failed >= fail_after  ) {
            printf( "[----------]\ttest run halted due to %i failure(s).",
                    fail_after);
            break;
        }
        
        // If PawnTest_Stop was called, stop testing.
        if( _PawnTest_Suites[suiteid][PAWNTEST_STOP] ) {
            printf("[----------]\ttest run halted by test suite.");
            break;
        }
    }

    // Call the teardown function if there is one.
    if( _PawnTest_Suites[suiteid][PAWNTEST_DESTRUCTOR_NAME][0] != '\0' ) {
        ret = CallLocalFunction(_PawnTest_Suites[suiteid][PAWNTEST_DESTRUCTOR_NAME],
                                "i", suiteid);
        switch (ret) {
            case PAWNTEST_TEARDOWN_SUCCESSFUL: {
                printf("[----------]\tthe test environment was successfully torn down.");
            }
            case PAWNTEST_TEARDOWN_FAILED: {
                printf("[----------]\tthe test environment could not be torn down.");
                printf("[----------]\tyou might have to clean up after yourself.");
                printf("[==========]\ttesting stopped.");

                PawnTest_testing = false;
                return false;
            }
            default: {
                printf("[----------]\tthe setup function's return value isn't meaningful.");
                printf("[----------]\tyou might have to clean up after yourself.");
                printf("[==========]\ttesting stopped.");

                PawnTest_testing = false;
                return false;            
            }
        }
    }
    
    // Print a status message.
    printf("[==========]\ttest suite \"%s\" (#%d) finished, %d test(s) ran.",
        _PawnTest_Suites[suiteid][PAWNTEST_SUITE_NAME],
        suiteid,
        tests_run);
    
    // Print the result of the test.
    if( ok ) {
        printf("[  PASSED  ]\t%d test(s).", tests_run );
    } else {
        printf("[  FAILED  ]\t%d/%d test(s) failed.", failed, tests_run );
    }

    printf("[==========]\ttesting stopped.\n");
    
    PawnTest_testing = false;
    return ok;
}