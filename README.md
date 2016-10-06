# wield 

A small toolkit for creating stage-based applications in c++.

### Dependencies 

- c++11
- [UsingIntrusivePtrIn](https://github.com/paxos1977/UsingIntrusivePtrIn)
- [boost](http://boost.org). Uses intrusive_ptr in production code by default. Uses filesystem, bind, and asio in example code.

Used for unit testing on all platforms:

- [UnitTest++](https://github.com/unittest-cpp/unittest-cpp). Unit test framework.
- [arbiter](https://github.com/paxos1977/arbiter). Used in acceptance testing.

Non-windows Platforms:

- Intel Thread Building Blocks. Needed on non-windows platforms for unit tests.

Windows Platforms:

- Need VS Concurrent libs.
- Visual Leak Detector (VLD). Used on windows to detect memory leaks in unit tests.

### Contributors 

Austin Gilbert <ceretullis@gmail.com>

### Acknowledgements 

A warm "thank you" to [minorg](https://github.com/minorg) for his work in stage-based scheduling and the `yield` project (which appears to be no longer publically available).

### License

4-Clause BSD license, see [LICENSE.md](LICENSE.md) for details. Other licensing available upon request. 
