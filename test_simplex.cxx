#include <stdio.h>
#include <memory.h>
#include <vector>
#include <string>
#include <fstream>

#include <cppunit/TestCaller.h>
#include <cppunit/TestResult.h>

#include "SimplexTest.hxx"
#include "simplex.hxx"

bool read(std::string filename,double** data, int& m, int& n){
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

  istream.close();
  return true;
}

void SimplexTest::testPrimalSimplex(){
  std::vector<std::string> files={
    "test/data.txt","test/data2.txt"
  };

  double* array=0;
  int m=0,n=0;
  for(std::string file : files){
    if(!read(file,&array,m,n))
       return;

    simplex(array,m,n);
    
    delete[] array;
  }

}

void SimplexTest::testDualSimplex(){
  std::vector<std::string> files={
    "test/data3.txt"
  };

  double* array=0;
  int m=0,n=0;
  for(std::string file : files){
    if(!read(file,&array,m,n))
      return;

    dualsimplex(array,m,n);

    delete[] array;
  }
}

int main(int argc, char** argv){
  using namespace CppUnit;
  TestCaller<SimplexTest> primalTest("test primal simplex",&SimplexTest::testPrimalSimplex);
  TestCaller<SimplexTest> dualTest("test dual simplex",&SimplexTest::testDualSimplex);

  TestResult result;
  primalTest.run(&result);
  dualTest.run(&result);
  return 0;
}
    
