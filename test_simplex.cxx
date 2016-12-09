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

  vector<vector<double>*> equations;
  while(getline(istream,line)){

    equations.push_back(new vector<double>());
    sstream = stringstream();
    sstream << line;

    while(getline(sstream,item,' '))
      (equations.back())->push_back(stod(item));
  }

  m=equations.size();
  
  (*data)=new double[(m+1)*n];
  for(int i = 0; i < (int)m;++i)
    memcpy((*data)+i*n,equations[i]->data(),n*sizeof(double));
  memcpy((*data)+m*n,v.data(),v.size()*sizeof(double));
  ++m;
	
  return true;
}

bool read(std::string filename,double** data, int& m, int& n, double** solution, int& solution_size){
  using namespace std;

  ifstream istream(filename);
  if(!istream.good())
    return false;

  vector<vector<double>*> v;
  string line,item;

  vector<double> v2;
  getline(istream,line);
  stringstream sstream;
  sstream << line;
  while(getline(sstream,item,' '))
    v2.push_back(stod(item));
  solution_size=v2.size();
  (*solution)=new double[solution_size];
  memcpy(*solution,&(v2[0]),solution_size*sizeof(double));
  
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
    delete[] solution;
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
    delete[] solution;
  }
}

void SimplexTest::testInflate(){
  using namespace std;
  vector<string> files={
    "test/data","test/data2","test/data3"
  };
  
  string inp_suffix=".raw";
  string sol_suffix=".txt";

  double *condensed=0, *inflated=0, *solution=0, *garbage=0;
  int c_n=0,c_m=0, i_m=0, i_n=0, s_m=0, s_n=0, n_vars=0, garbage_size=0;
  for(string file : files){
    string inp_path=file+inp_suffix;

    if(!readCondensed(inp_path,&condensed,c_n,c_m))
       continue;


    inflate(condensed,c_n,c_m,&inflated,i_m,i_n,n_vars);

    string sol_path=file+sol_suffix;
    read(sol_path,&solution,s_m,s_n,&garbage,garbage_size);

    CPPUNIT_ASSERT(s_m==i_m);
    CPPUNIT_ASSERT(s_n==i_n);
    equals(inflated,solution,i_m,i_n,EPS);

    delete[] condensed;
    delete[] inflated;
    delete[] solution;
    delete[] garbage;
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
  Test* inflateTest = new TestCaller<SimplexTest>("inflate",&SimplexTest::testInflate);
  runner.addTest(inflateTest);

  runner.run();
  return 0;
}
    
