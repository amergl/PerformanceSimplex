#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <vector>
#include <string>
#include <fstream>

#include <cppunit/TestCaller.h>
#include <cppunit/TestAssert.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>

#include "SimplexTest.hxx"
#include "simplex.hxx"

bool equals(double* calculated,double* solution,int n,double accuracy){
  for(int i = 0; i < n; ++i)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(solution[i],calculated[i],accuracy);
  return true;
}


void SimplexTest::testPrimalSimplex(){
  std::vector<std::string> files={
    "test/data.txt","test/data2.txt"
  };

  double* array=0;
  double* solution=0;
  double* calculated=0;
  int m=0,n=0,solution_size=0;
  for(std::string file : files){
    if(!read(file,&array,m,n,&solution,solution_size))
       return;

    simplex(array,m,n,&calculated,solution_size);
    equals(calculated,solution,solution_size,EPS);
    
    delete[] array;
  }

}

void SimplexTest::testDualSimplex(){
  std::vector<std::string> files={
    "test/data3.txt"
  };

  double* array=0;
  double* solution=0;
  double* calculated=0;
  int m=0,n=0,solution_size=0;
  for(std::string file : files){
    if(!read(file,&array,m,n,&solution,solution_size))
       return;

    dualsimplex(array,m,n,&calculated,solution_size);
    equals(calculated,solution,solution_size,EPS);
    
    delete[] array;
  }
}

int main(int argc, char** argv){
  using namespace CppUnit;
  
  TextUi::TestRunner runner;
  runner.setOutputter(new CompilerOutputter(&(runner.result()),std::cerr));

  Test* primalTest = new TestCaller<SimplexTest>("primal simplex",&SimplexTest::testPrimalSimplex);
  runner.addTest(primalTest);
  Test* dualTest = new TestCaller<SimplexTest>("dual simplex",&SimplexTest::testDualSimplex);
  runner.addTest(dualTest);

  bool success = runner.run();
  return !success;
}
    
