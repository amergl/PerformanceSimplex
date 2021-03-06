#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>

#include <cppunit/TestCaller.h>
#include <cppunit/TestAssert.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>

#include "SimplexTest.hxx"
#include "simplex.hxx"

void equals(double* calculated, double* solution, int m, int n, double accuracy){
  for(int i = 0; i < m; ++i)
    for(int j = 0; j < n; ++j)
      CPPUNIT_ASSERT_DOUBLES_EQUAL(solution[i*n+j],calculated[i*n+j],accuracy);
}

void equals(double* calculated,double* solution,int n,double accuracy){
  for(int i = 0; i < n; ++i)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(solution[i],calculated[i],accuracy);
}

bool readCondensed(std::string filename, double** data, int& m, int& n){
  using namespace std;

  ifstream istream(filename);
  if(!istream.good())
    return false;

  vector<double> v;
  string line,item;

  getline(istream,line);
  stringstream sstream;
  sstream << line;
  while(getline(sstream,item,' '))
    v.push_back(stod(item));

  n=v.size()+2;

  vector<vector<double>> equations;
  while(getline(istream,line)){

    equations.push_back(vector<double>());
    sstream = stringstream();
    sstream << line;

    while(getline(sstream,item,' '))
      (equations.back()).push_back(stod(item));
  }

  m=equations.size();

  (*data)=new double[(m+1)*n];

  for(int i = 0; i < (int)m;++i)
    memcpy((*data)+i*n,equations[i].data(),n*sizeof(double));
  memcpy((*data)+m*n,v.data(),v.size()*sizeof(double));
  ++m;

  return true;
}

bool readInflated(std::string filename,double** data, int& m, int& n){
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
    memcpy((*data)+i*n,&((*(v[i]))[0]),n*sizeof(double));

  istream.close();
  return true;
}

bool readSolution(std::string filename, double** data, int& size){
  using namespace std;

  ifstream istream(filename);
  if(!istream.good())
    return false;

  vector<double> v;
  stringstream stream;
  string line,item;

  getline(istream,line);
  stream << line;
  while(getline(stream,item,' '))
    v.push_back(stod(item));

  size = v.size();
  (*data) = new double[size];
  memcpy(*data,v.data(),size*sizeof(double));

  return true;
}

void SimplexTest::testPrimalSimplex(){
  std::vector<std::string> files={
    "test/data","test/data2"
  };

  double* array=0;
  double* solution=0;
  double* calculated=0;
  int m=0,n=0,solution_size=0;
  for(std::string file : files){
    CPPUNIT_ASSERT(readCondensed(file+".raw",&array,m,n));
    CPPUNIT_ASSERT(readSolution(file+".sol",&solution,solution_size));

    simplex(array,m,n,&calculated,solution_size);

    equals(calculated,solution,solution_size,EPS);

    delete[] array;
    delete[] solution;
  }

}

void SimplexTest::testDualSimplex(){
  std::vector<std::string> files={
    "test/data3"
  };

  double* array=0;
  double* solution=0;
  double* calculated=0;
  int m=0,n=0,solution_size=0,calculated_size=0;
  for(std::string file : files){
    CPPUNIT_ASSERT(readCondensed(file+".raw",&array,m,n));
    CPPUNIT_ASSERT(readSolution(file+".sol",&solution,solution_size));

    dualsimplex(array,m,n,&calculated,calculated_size);

    CPPUNIT_ASSERT_EQUAL(solution_size,calculated_size);

    equals(calculated,solution,solution_size,EPS);
    
    delete[] array;
    delete[] solution;
  }

}

void SimplexTest::testInflate(){
  using namespace std;
  vector<string> files={
    "test/data","test/data2","test/data3"
  };
  
  double *condensed=0, *inflated=0, *solution=0;
  int condensed_m=0,condensed_n=0, inflated_m=0, inflated_n=0, solution_m=0, solution_n=0, n_vars=0;
  for(string file : files){
    CPPUNIT_ASSERT(readCondensed(file+".raw",&condensed,condensed_m,condensed_n));

    inflate(condensed,condensed_m,condensed_n,&inflated,inflated_m,inflated_n,n_vars);

    CPPUNIT_ASSERT(readInflated(file+".txt",&solution,solution_m,solution_n));
    CPPUNIT_ASSERT(solution_m==inflated_m);
    CPPUNIT_ASSERT(solution_n==inflated_n);
    
    equals(inflated,solution,inflated_m,inflated_n,EPS);

    delete[] condensed;
    delete[] inflated;
    delete[] solution;
  }

}

void SimplexTest::testPerformance(){
  std::vector<std::string> files={
    "test/dataX"
  };

  double* array=0;
  double* calculated=0;
  int m=0,n=0,calculated_size=0;
  for(std::string file : files){
    CPPUNIT_ASSERT(readCondensed(file+".raw",&array,m,n));

    clock_t t0 = clock();
    simplex(array,m,n,&calculated,calculated_size);
    printf("\n[\"%s\"]: Time used %.4es\n",file.c_str(),((double)(clock()-t0))/(CLOCKS_PER_SEC));
    
    delete[] array;
  }

}

int main(int argc, char** argv){
  using namespace CppUnit;
  
  TextUi::TestRunner runner;
  runner.setOutputter(new CompilerOutputter(&(runner.result()),std::cerr));

  Test* inflateTest = new TestCaller<SimplexTest>("inflate",&SimplexTest::testInflate);
  runner.addTest(inflateTest);
  Test* primalTest = new TestCaller<SimplexTest>("primal simplex",&SimplexTest::testPrimalSimplex);
  runner.addTest(primalTest);
  Test* dualTest = new TestCaller<SimplexTest>("dual simplex",&SimplexTest::testDualSimplex);
  runner.addTest(dualTest);
  Test* performanceTest = new TestCaller<SimplexTest>("performance test",&SimplexTest::testPerformance);
  runner.addTest(performanceTest);

  runner.run();
  return 0;
}
    
