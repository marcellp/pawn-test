/* pawn-test
 * A ridiculously simple automated testing framework for PAWN scripts.
 *
 * Copyright (c) 2015 Pék Marcell
 * This project is covered by the MIT License, see the attached LICENSE
 * file for more details.
 *
 * test-suite-test.pwn - the test suite API unit test
 *
 */

#include <a_samp>
#include <pawn-test/pawn-test>

new ThisTestSuite;

forward Setup(suiteid);
forward Tear(suiteid);
forward CreateSuite(suiteid, caseid);
forward InvalidIdDestroySuite(suiteid, caseid);
forward InvalidIdGetSuiteName(suiteid, caseid);
forward InvalidIdSetSuiteName(suiteid, caseid);
forward CheckSuiteName(suiteid, caseid);
forward ChangeSuiteName(suiteid, caseid);
forward ChangeSuiteName_BufferTest(suiteid, caseid);
forward DestroySuite(suiteid, caseid);

main()
{
    new TestSuite = PawnTest_InitSuite("Test suite API test");

    PawnTest_SetSetup(TestSuite, "Setup");
    PawnTest_SetTeardown(TestSuite, "Tear");
    PawnTest_AddCase(TestSuite, "CreateSuite");
    PawnTest_AddCase(TestSuite, "InvalidIdDestroySuite");
    PawnTest_AddCase(TestSuite, "InvalidIdGetSuiteName");
    PawnTest_AddCase(TestSuite, "InvalidIdSetSuiteName");    
    PawnTest_AddCase(TestSuite, "CheckSuiteName");
    PawnTest_AddCase(TestSuite, "ChangeSuiteName");
    PawnTest_AddCase(TestSuite, "ChangeSuiteName_BufferTest");
    PawnTest_AddCase(TestSuite, "DestroySuite");    
    PawnTest_Run(TestSuite);

    PawnTest_DestroySuite(TestSuite);
}

public Setup(suiteid)
{
    ThisTestSuite = -1;
    return PAWNTEST_SETUP_SUCCESSFUL;
}

public Tear(suiteid)
{
    ThisTestSuite = -1;
    return PAWNTEST_TEARDOWN_SUCCESSFUL;
}

public CreateSuite(suiteid, caseid)
{
    PawnTest_errno = PAWNTEST_NOERROR;
    
    ThisTestSuite = PawnTest_InitSuite("SampleTestSuite");
    
    return  PawnTest_AssertEx(PawnTest_errno == PAWNTEST_NOERROR, "Error while initializing")
            ? PAWNTEST_CASE_PASSED
            : PAWNTEST_SUITE_FAILED;
}

public InvalidIdDestroySuite(suiteid, caseid)
{
    new bool:ok = true;
    
    ok &= PawnTest_AssertEx( !PawnTest_DestroySuite(-1), "Did not fail when suiteid = -1" );
    ok &= PawnTest_AssertEx( !PawnTest_DestroySuite(PAWNTEST_MAX_SUITES), "Did not fail when suiteid = max" );
    ok &= PawnTest_AssertEx( !PawnTest_DestroySuite(PAWNTEST_MAX_SUITES+1), "Did not fail when suiteid = max+1" );

    return (ok) ? PAWNTEST_CASE_PASSED : PAWNTEST_CASE_FAILED;
}

public InvalidIdGetSuiteName(suiteid, caseid)
{
    new bool:ok = true;
    new name[10] = "foo";
    
    ok &= PawnTest_AssertEx( !PawnTest_GetSuiteName(-1, name), "Did not fail when suiteid = -1" );
    ok &= PawnTest_AssertEx( !PawnTest_GetSuiteName(PAWNTEST_MAX_SUITES, name), "Did not fail when suiteid = max" );
    ok &= PawnTest_AssertEx( !PawnTest_GetSuiteName(PAWNTEST_MAX_SUITES+1, name), "Did not fail when suiteid = max+1" );

    return (ok) ? PAWNTEST_CASE_PASSED : PAWNTEST_CASE_FAILED;
}

public InvalidIdSetSuiteName(suiteid, caseid)
{
    new bool:ok = true;
    new name[10] = "foo";
    
    ok &= PawnTest_AssertEx( !PawnTest_SetSuiteName(-1, name), "Did not fail when suiteid = -1" );
    ok &= PawnTest_AssertEx( !PawnTest_SetSuiteName(PAWNTEST_MAX_SUITES, name), "Did not fail when suiteid = max" );
    ok &= PawnTest_AssertEx( !PawnTest_SetSuiteName(PAWNTEST_MAX_SUITES+1, name), "Did not fail when suiteid = max+1" );

    return (ok) ? PAWNTEST_CASE_PASSED : PAWNTEST_CASE_FAILED;
}

public CheckSuiteName(suiteid, caseid)
{
    new bool:ok = true;
    new buf[PAWNTEST_MAX_SUITE_NAME];
    
    PawnTest_GetSuiteName(ThisTestSuite, buf, PAWNTEST_MAX_SUITE_NAME);
        
    ok = PawnTest_AssertEx( !strcmp(buf, "SampleTestSuite"), "suite name != initialized name" );

    return (ok) ? PAWNTEST_CASE_PASSED : PAWNTEST_CASE_FAILED;
}

public ChangeSuiteName(suiteid, caseid)
{
    new bool:ok = true;
    new buf[18] = "foofoobarbar";
    
    PawnTest_SetSuiteName(ThisTestSuite, buf);
    buf[0] = '\0';
    PawnTest_GetSuiteName(ThisTestSuite, buf);
    
    ok = PawnTest_AssertEx( !strcmp(buf, "foofoobarbar"), "suite name != foofoobarbar" );

    return (ok) ? PAWNTEST_CASE_PASSED : PAWNTEST_CASE_FAILED;
}

public ChangeSuiteName_BufferTest(suiteid, caseid)
{
    new bool:ok = true;
    new buf1[3], buf2[PAWNTEST_MAX_SUITE_NAME+3];
    
    PawnTest_SetSuiteName(ThisTestSuite, "barfoobarfoo");
    PawnTest_GetSuiteName(ThisTestSuite, buf1);
    PawnTest_GetSuiteName(ThisTestSuite, buf2);
        
    ok &= PawnTest_AssertEx( buf1[0] == '\0', "buf2[0] != '\\0' (small buffer)" );
    ok &= PawnTest_AssertEx( !strcmp(buf2, "barfoobarfoo"), "buf3 != barfoobarfoo" );

    return (ok) ? PAWNTEST_CASE_PASSED : PAWNTEST_CASE_FAILED;
}

public DestroySuite(suiteid, caseid)
{
    new bool:ok = true;
    new bool:ret = false;
    new name[PAWNTEST_MAX_SUITE_NAME];
    
    ret = PawnTest_DestroySuite(ThisTestSuite); // destroy the test suite
    ok &= PawnTest_AssertEx( ret == true, "Error after destroying suite" );
    
    PawnTest_errno = PAWNTEST_NOERROR;
    PawnTest_GetSuiteName(ThisTestSuite, name);
    ok &= PawnTest_AssertEx(  PawnTest_errno == PAWNTEST_OBJ_NOT_INITIALIZED,
                            "No error after getting the name of destroyed suite" );

    PawnTest_errno = PAWNTEST_NOERROR;
    PawnTest_SetSuiteName(ThisTestSuite, "foobarfoo");
    ok &= PawnTest_AssertEx(  PawnTest_errno == PAWNTEST_OBJ_NOT_INITIALIZED,
                            "No error after setting the name of destroyed suite" );

    PawnTest_errno = PAWNTEST_NOERROR;
    PawnTest_DestroySuite(ThisTestSuite);
    ok &= PawnTest_AssertEx(  PawnTest_errno == PAWNTEST_OBJ_NOT_INITIALIZED,
                            "No error after re-destroying a destroyed suite" );

    return (ok) ? PAWNTEST_CASE_PASSED : PAWNTEST_CASE_FAILED;
}