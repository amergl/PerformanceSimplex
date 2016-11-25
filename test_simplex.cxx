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
    CPPUNIT_ASSERT_DOUBLES_EQUAL(calculated[i],solution[i],accuracy);
  return true;
}

bool read(std::string filename,double** data, int& m, int& n, double** solution, int& solution_size){
  using namespace std;

  ifstream istream(filename);
  if(!istream.good())
    return false;

  vector<vector<double>*> v;
  string line,item;
  while(getline(istream,line)){
    v.push_back(new vector<double>());
    stringstream stream;
    stream << line;
    while(getline(stream,item,' ')){
      v.back()->push_back(stod(item));
    }
  }

  m = v.size();
  n = v[0]->size();
  (*data) = new double[m*n];
  for(int i = 0; i < (int)v.size();++i)
    memcpy((*data)+i*n,&(*(v[i]))[0],n*sizeof(double));

  (*solution)=(double*)calloc(m,sizeof(double));
  solution_size=m;
  (*solution)[0]=1;

  istream.close();
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
  Test* primalTest = new TestCaller<SimplexTest>("primal simplex",&SimplexTest::testPrimalSimplex);
  runner.addTest(primalTest);
  Test* dualTest = new TestCaller<SimplexTest>("dual simplex",&SimplexTest::testDualSimplex);
  runner.addTest(dualTest);
  runner.setOutputter(new CompilerOutputter(&(runner.result()),std::cerr));
  runner.run();
    
  return 0;
}
    
