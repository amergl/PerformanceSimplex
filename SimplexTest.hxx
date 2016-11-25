#include <stdio.h>
#include <cppunit/TestFixture.h>

class SimplexTest : public CppUnit::TestFixture{
private:
  const int EPS=1e-8;
  
public:
  void setUp(){
  }

  void tearDown(){
  }

  void testPrimalSimplex();

  void testDualSimplex();
};
