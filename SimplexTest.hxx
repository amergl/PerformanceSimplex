#include <stdio.h>
#include <cppunit/TestFixture.h>

class SimplexTest : public CppUnit::TestFixture{
public:
  void setUp(){
  }

  void tearDown(){
  }

  void testPrimalSimplex();

  void testDualSimplex();
};
