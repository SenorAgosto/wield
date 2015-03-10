# wield 

A small toolkit for creating stage-based applications in c++.

### Dependencies 

- c++11
- [UsingIntrusivePtrIn](https://github.com/paxos1977/UsingIntrusivePtrIn)

Used for unit testing on all platforms:

- [UnitTest++](https://github.com/unittest-cpp/unittest-cpp). Unit test framework.
- [boost](http://boost.org). Uses filesystem, bind, and asio in example code.

Non-windows Platforms:

- Intel Thread Building Blocks. Needed on non-windows platforms for unit tests.

Windows Platforms:

- Visual Leak Detector (VLD). Used on windows to detect memory leaks in unit tests)

### Contributors 

Austin Gilbert <ceretullis@gmail.com>

### License

4-Clause BSD license, see [LICENSE.md](LICENSE.md) for details. Other licensing available upon request. 
