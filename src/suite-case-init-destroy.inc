/* pawn-test
 * A ridiculously simple automated testing framework for PAWN scripts.
 *
 * Copyright (c) 2015 Pék Marcell
 * This project is covered by the MIT License, see the attached LICENSE
 * file for more details.
 *
 * suite-case-init-destroy.inc - functions for creating, destroying and managing test suites and cases
 *
 */

/**
    <summary>
        Initializes a test suite.
    </summary>

    <param name="name[]">
        The name of the test suite.
    </param>

    <returns>
        A test case ID if one could be allocated, -1 otherwise.
    </returns>
*/
stock PawnTest_InitSuite(name[], size=sizeof(name))
{
    _PawnTest_dummy(PawnTest_errno);    //get rid of the stupid "variable never used" warning
    
    for( new i = 0; i<PAWNTEST_MAX_SUITES; i++ ) {
        if( !_PawnTest_Suites[i][PAWNTEST_INITIALIZED] ) {
            _PawnTest_Suites[i][PAWNTEST_INITIALIZED] = true;
            memcpy( _PawnTest_Suites[i][PAWNTEST_SUITE_NAME], name, 0, size*4, PAWNTEST_MAX_SUITE_NAME );
            return i;
        }
    }
    
    PawnTest_errno = PAWNTEST_SUITE_OUT_OF_BOUNDS;
    return -1;
}

/**
    <summary>
        Destroys a test suite.
    </summary>

    <param name="suiteid">
        The ID of the test suite returned by PawnTest_InitSuite().
    </param>
    
    <returns>
        True if the operation could be performed, false otherwise.
    </returns>
*/
stock bool:PawnTest_DestroySuite(suiteid)
{
    if( !_PawnTest_CheckSuiteBounds(suiteid) ) return false;

    if( !_PawnTest_Suites[suiteid][PAWNTEST_INITIALIZED] ) {
        PawnTest_errno = PAWNTEST_OBJ_NOT_INITIALIZED;
        return false;
    }

    _PawnTest_ClearSuite(suiteid );
    return true;
}

/**
    <summary>
        Gets the name of the test suite.
    </summary>

    <param name="suiteid">
        The ID of the test suite returned by PawnTest_InitSuite().
    </param>
    
    <param name="name[]">
        The name of the test suite, passed by reference.
    </param>

    <param name="size = sizeof(name)">
        The size of the buffer.
    </param>

    <returns>
        True if the operation could be performed, false otherwise.
    </returns>
*/
stock PawnTest_GetSuiteName(suiteid, name[], size = sizeof(name))
{
    if( !_PawnTest_CheckSuiteBounds(suiteid) ) return false;

    if( !_PawnTest_Suites[suiteid][PAWNTEST_INITIALIZED] ) {
        PawnTest_errno = PAWNTEST_OBJ_NOT_INITIALIZED;
        return false;
    }
    
    memcpy( name, _PawnTest_Suites[suiteid][PAWNTEST_SUITE_NAME],
            0, strlen(_PawnTest_Suites[suiteid][PAWNTEST_SUITE_NAME])*4, size);

    return true;
}

/**
    <summary>
        Sets the name of the test suite.
    </summary>

    <param name="suiteid">
        The ID of the test suite returned by PawnTest_InitSuite().
    </param>
    
    <param name="name[]">
        The new name of the test suite, passed by reference.
    </param>

    <param name="size = sizeof(name)">
        The size of the buffer.
    </param>

    <returns>
        True if the name was set successfully, false otherwise.
    </returns>
*/
stock bool:PawnTest_SetSuiteName(suiteid, name[], size = sizeof(name))
{
    if( !_PawnTest_CheckSuiteBounds(suiteid) ) return false;

    if( !_PawnTest_Suites[suiteid][PAWNTEST_INITIALIZED] ) {
        PawnTest_errno = PAWNTEST_OBJ_NOT_INITIALIZED;
        return false;
    }
    
    if( size >= PAWNTEST_MAX_SUITE_NAME || size == 0 ) {
        PawnTest_errno = PAWNTEST_STR_OUT_OF_BOUNDS;
        return false;
    }

    memcpy( _PawnTest_Suites[suiteid][PAWNTEST_SUITE_NAME], name,
            0, size*4, PAWNTEST_MAX_SUITE_NAME);
    
    return true;
}

/**
    <summary>
        Adds a test case to a test suite.
    </summary>

    <param name="suiteid">
        The ID of the test suite returned by PawnTest_InitSuite().
    </param>

    <param name="func_name[]">
        The name of the test case, passed by reference.
    </param>

    <param name="size = sizeof(func_name)">
        The size of the buffer.
    </param>
    
    <returns>
        The ID of the test case if it was successfully allocated,
        -1 otherwise.
    </returns>
    
    <remarks>
        The test case has to be public and therefore must be forwarded.
    </remarks>
*/
stock PawnTest_AddCase(suiteid, func_name[], size = sizeof(func_name))
{
    if( !_PawnTest_CheckSuiteBounds(suiteid) ) return -1;
    if( func_name[0] == '\0' ) return -1;   // the runner will attempt to call CallLocalFunction, which
                                            // crashes when the string is empty.
                                            
    if( funcidx(func_name) == -1 ) {
        PawnTest_errno = PAWNTEST_OBJ_NOT_FOUND;
        return -1;
    }
    
    if( _PawnTest_Suites[suiteid][PAWNTEST_NUM_OF_CASES]++ >= PAWNTEST_MAX_CASES ) {
        _PawnTest_Suites[suiteid][PAWNTEST_NUM_OF_CASES] = PAWNTEST_MAX_CASES;
        PawnTest_errno = PAWNTEST_CASE_OUT_OF_BOUNDS;
        return -1;
    }

    if( size >= PAWNTEST_MAX_SUITE_NAME ) {
        PawnTest_errno = PAWNTEST_STR_OUT_OF_BOUNDS;
        return -1;
    }
    
    for( new i = 0; i<PAWNTEST_MAX_CASES; i++ ) {
        if( _PawnTest_Cases[suiteid][i][PAWNTEST_CASE_NAME][0] == '\0' ) {
            memcpy( _PawnTest_Cases[suiteid][i][PAWNTEST_CASE_NAME], func_name,
                    0, size*4, PAWNTEST_MAX_SUITE_NAME);
            
            _PawnTest_Cases[suiteid][i][PAWNTEST_RETURN_WITH] = -1;
            _PawnTest_Cases[suiteid][i][PAWNTEST_EXPECTED_TO_FAIL] = false;
            return i;
        }
    }
    
    PawnTest_errno = PAWNTEST_OBJ_NOT_FOUND;
    return -1;
}

/**
    <summary>
        Checks if a given test case is valid.
    </summary>

    <param name="suiteid">
        The ID of the test suite returned by PawnTest_InitSuite().
    </param>

    <param name="caseid">
        The ID of the test case returned by PawnTest_AddCase().
    </param>
    
    <returns>
        True if the test case is valid, false otherwise.
    </returns>
*/
stock bool:PawnTest_IsValidCase(suiteid, caseid)
{
    return (caseid < PAWNTEST_MAX_CASES
            && caseid >= 0
            && _PawnTest_Cases[suiteid][caseid][PAWNTEST_CASE_NAME][0] != '\0');
}

/**
    <summary>
        Gets the name of the given test case.
    </summary>

    <param name="suiteid">
        The ID of the test suite returned by PawnTest_InitSuite().
    </param>

    <param name="caseid">
        The ID of the test case returned by PawnTest_AddCase().
    </param>

    <param name="func_name[]">
        The name of the test case, passed by reference.
    </param>

    <param name="size = sizeof(name)">
        The size of the buffer.
    </param>

    <returns>
        This function does not handle returns.
    </returns>
*/
stock PawnTest_GetCaseName(suiteid, caseid, func_name[], size = sizeof(func_name))
{
    if( !_PawnTest_CheckSuiteBounds(suiteid) ) return false;
    
    if( PawnTest_IsValidCase(suiteid, caseid) ) {
        PawnTest_errno = PAWNTEST_OBJ_NOT_FOUND;
        return;
    }
    
    memcpy( name, _PawnTest_Cases[suiteid][caseid][PAWNTEST_CASE_NAME], 0,
            strlen(_PawnTest_Cases[suiteid][caseid][PAWNTEST_CASE_NAME])*4, size);
    return;
}

/**
    <summary>
        Removes a test case from a test suite.
    </summary>

    <param name="suiteid">
        The ID of the test suite returned by PawnTest_InitSuite().
    </param>

    <param name="caseid">
        The ID of the test case returned by PawnTest_AddCase().
    </param>
    
    <returns>
        True if the operation was successfully performed, false otherwise.
    </returns>
*/
stock bool:PawnTest_RemoveCase(suiteid, caseid)
{
    if( !_PawnTest_CheckSuiteBounds(suiteid) ) return false;
    
    if( PawnTest_IsValidCase(suiteid, caseid) ) {
        PawnTest_errno = PAWNTEST_OBJ_NOT_FOUND;
        return false;
    }

    new currfun = _PawnTest_Suites[suiteid][PAWNTEST_NUM_OF_CASES]--;
        
    if( currfun < 0 ) {
        _PawnTest_Suites[suiteid][PAWNTEST_NUM_OF_CASES] = 0;
        PawnTest_errno = PAWNTEST_CASE_OUT_OF_BOUNDS;
        return false;
    }

    _PawnTest_Cases[suiteid][caseid][PAWNTEST_CASE_NAME][0] = '\0';
    return true;
}

/**
    <summary>
        Sets the setup function of the test suite.
    </summary>

    <param name="suiteid">
        The ID of the test suite returned by PawnTest_InitSuite().
    </param>

    <param name="func_name[]">
        The name of the setup function, passed by reference.
        Pass an empty string ("") to disable a previously added setup function.
    </param>

    <param name="size = sizeof(func_name)">
        The size of the buffer.
    </param>
    
    <returns>
        True if the operation could be performed successfully, false otherwise.
    </returns>
    
    <remarks>
        The setup function has to be public and therefore must be forwarded.
    </remarks>
*/
stock bool:PawnTest_SetSetup(suiteid, func_name[], size = sizeof(func_name))
{
    if( !_PawnTest_CheckSuiteBounds(suiteid) ) return false;
    
    if( func_name[0] != '\0' && funcidx(func_name) == -1 ) {
        PawnTest_errno = PAWNTEST_OBJ_NOT_FOUND;
        return false;
    }
    
    if( size >= PAWNTEST_MAX_SUITE_NAME ) {
        PawnTest_errno = PAWNTEST_STR_OUT_OF_BOUNDS;
        return false;
    }

    memcpy( _PawnTest_Suites[suiteid][PAWNTEST_CONSTRUCTOR_NAME], func_name,
            0, size*4, PAWNTEST_MAX_SUITE_NAME);

    return true;
}

/**
    <summary>
        Sets the teardown function of the test suite.
    </summary>

    <param name="suiteid">
        The ID of the test suite returned by PawnTest_InitSuite().
    </param>

    <param name="func_name[]">
        The name of the teardown function, passed by reference.
        Pass an empty string ("") to disable a previously added teardown function.
    </param>

    <param name="size = sizeof(func_name)">
        The size of the buffer.
    </param>
    
    <returns>
        True if the operation could be performed successfully, false otherwise.
    </returns>
    
    <remarks>
        The teardown function has to be public and therefore must be forwarded.
    </remarks>
*/
stock bool:PawnTest_SetTeardown(suiteid, func_name[], size = sizeof(func_name))
{
    if( !_PawnTest_CheckSuiteBounds(suiteid) ) return false;
    
    if( func_name[0] != '\0' && funcidx(func_name) == -1 ) {
        PawnTest_errno = PAWNTEST_OBJ_NOT_FOUND;
        return false;
    }
    
    if( size >= PAWNTEST_MAX_SUITE_NAME ) {
        PawnTest_errno = PAWNTEST_STR_OUT_OF_BOUNDS;
        return false;
    }
 
    memcpy( _PawnTest_Suites[suiteid][PAWNTEST_DESTRUCTOR_NAME], func_name,
            0, size*4, PAWNTEST_MAX_SUITE_NAME);

    return true;
}