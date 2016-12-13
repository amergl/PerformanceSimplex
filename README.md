# Performance Simplex

## Functionality Tests

The functional tests are implemented using cppunit. The cmake target
test_simplex produces the test executable.

They can either be run by executing `test_simplex` or `ctest -R simplex`. 

## Performance Tests

The performance tests are implemented using google/benchmark. 
The library is included as a submodule and must be downloaded by running

```
git submodule update --init --recursive
```

Afterwards the target `benchmark_simplex` produces an executable for running
the tests.

This can be done by executing `benchmark_simplex` directly or running
`ctest -V -R simplex_performance`.



