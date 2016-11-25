#include <stdio.h>
#include <cppunit/TestFixture.h>

class SimplexTest : public CppUnit::TestFixture{
public:
  void setUp(){
    printf("Set up\n");
  }

  void tearDown(){
    printf("Tear down\n");
  }

  void testTest(){
    printf("Test testcase\n");
  }
};
