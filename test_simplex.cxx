#include "SimplexTest.hxx"
#include <cppunit/TestCaller.h>
#include <cppunit/TestResult.h>

int main(int argc, char** argv){
  CppUnit::TestCaller<SimplexTest> test("testTest",&SimplexTest::testTest);

  CppUnit::TestResult result;
  test.run(&result);
  return 0;
}
    
