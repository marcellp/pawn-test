pawn-test
=========

pawn-test is a simple automated testing framework for PAWN code. The framework
provides a number of functions for setting up and managing test suites and test
cases. It was designed to be compatible with the PAWN compiler that comes
with San Andreas: Multiplayer (Pawn 3.2.3664), but it should work with other
PAWN projects as well.

Even though there exists a working testing environment for SA:MP (that would be
`y_testing`), it is part of the YSI library, which many scripters do not want
to deal with. It also creates a lot of syntactic sugar which may not appeal
to everyone. This is a standalone, lightweight library that can be used without
additional dependencies.

License
-------

pawn-test is an open source project covered by the MIT License. See
[LICENSE](COPYING) for more information or visit
 http://opensource.org/licenses/MIT.

Including pawn-test
-------------------

To use pawn-test in a PAWN project, copy the `src` directory into your project's
`pawno/include` directory. Rename the directory to `pawn-test`.

To run pawn-test with the default configuration, simply include the bootstrapper in
your project:

    #include <pawn_test/pawn_test>
  
If you want to provide your own configuration, either edit the configuration file
(`config.inc`) globally or define your custom configuration options after including
the bootstrapper:

    #include <pawn_test/pawn_test>
    #define PAWNTEST_MAX_SUITES 10
    #define PAWNTEST_MAX_CASES 50

Using pawn-test
---------------

pawn-test is an [XUnit](https://en.wikipedia.org/wiki/XUnit)-like test framework.

Testing code with pawn-test is very easy. Follow the API documentation and you should be ready
in a matter of minutes. Don't forget that every function pawn-test uses has to be `public`.

    #include <pawn_test/pawn_test>
    
    forward Test1_Setup();
    forward Test1_Teardown();
    forward Test1_footest();
    forward Test1_bartest();
    
    stock Test1()
    {
        new TestSuite = PawnTest_InitSuite("Sample test");
        
        PawnTest_SetSetup(TestSuite, "Test1_Setup");
        PawnTest_SetTeardown(TestSuite, "Test1_Teardown");
        PawnTest_AddCase(TestSuite, "Test1_footest");
        PawnTest_AddCase(TestSuite, "Test1_bartest");
        PawnTest_Run(TestSuite);
        
        PawnTest_DestroySuite(TestSuite);
    }

Test suites can be run multiple times if necessary.

Examples
--------

The codebase of pawn-test is tested using pawn-test itself. There are some fairly straightforward
unit tests in the `tests/` directory.

Output
------

pawn-test prints its status messages to `stdout`. If you're using pawn-test with SA:MP,
that's *server_log.txt*. The Windows version prints everything to the server window too.

pawn-test produces the following output if a test suite passes:

    [==========]    running test suite "Test suite API test" w/ 8 test case(s)...
    [----------]    the test environment was successfully set up.
    [ RUN      ]    test "CreateSuite" (12%)
    [       OK ]    test "CreateSuite" (0 ms, 0.001 s in total).
    [ RUN      ]    test "InvalidIdDestroySuite" (25%)
    [       OK ]    test "InvalidIdDestroySuite" (0 ms, 0.002 s in total).
    [ RUN      ]    test "InvalidIdGetSuiteName" (37%)
    [       OK ]    test "InvalidIdGetSuiteName" (0 ms, 0.003 s in total).
    [ RUN      ]    test "InvalidIdSetSuiteName" (50%)
    [       OK ]    test "InvalidIdSetSuiteName" (0 ms, 0.005 s in total).
    [ RUN      ]    test "CheckSuiteName" (62%)
    [       OK ]    test "CheckSuiteName" (0 ms, 0.007 s in total).
    [ RUN      ]    test "ChangeSuiteName" (75%)
    [       OK ]    test "ChangeSuiteName" (0 ms, 0.009 s in total).
    [ RUN      ]    test "ChangeSuiteName_BufferTest" (87%)
    [       OK ]    test "ChangeSuiteName_BufferTest" (0 ms, 0.010 s in total).
    [ RUN      ]    test "DestroySuite" (100%)
    [       OK ]    test "DestroySuite" (0 ms, 0.012 s in total).
    [----------]    the test environment was successfully torn down.
    [==========]    test suite "Test suite API test" (#0) finished, 8 test(s) ran.
    [  PASSED  ]    8 test(s).
    [==========]    testing stopped.
