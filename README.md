pawn-test
=========

pawn-test is a simple automated testing framework for PAWN code. The framework
provides a number of functions for setting up and managing test suites and test
cases. It was designed to be compatible with the PAWN compiler that comes
with San Andreas: Multiplayer (Pawn 3.2.3664), but with a bit of tinkering
it should work with other PAWN projects as well.

Even though there exists a working testing environment for SA:MP (that would be
`y_testing`), it is part of the YSI library, which many scripters do not want
to deal with. It also creates a lot of syntactic sugar which may not appeal
to everyone. This is a standalone, lightweight library that can be used without
additional dependencies.

License
-------

pawn-test is an open source project covered by the MIT License. See
[LICENSE](LICENSE) for more information or visit
 http://opensource.org/licenses/MIT.

Using pawn-test
---------------

To use pawn-test in a PAWN project, copy the `src` directory into your project's
`pawno/include` directory. Rename the directory to `pawn-test`.

To run pawn-test with the default configuration, simply include the bootstrapper in
your project:

    #include <pawn_test/pawn_test>
  
If you want to provide your own configuration, edit the configuration file
(`config.inc`) and change your settings there.

Documentation
-------------

The full API documentation, including tutorials on how to get started, is
available in the [wiki](https://github.com/marcellp/pawn-test/wiki) of
this repository.
