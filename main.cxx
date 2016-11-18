#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <memory.h>

using namespace std;

void printArray(double* array, int m, int n){
  printf("%dx%d\n",m,n);
  for(int i = 0 ; i < m; ++i){
    for(int j = 0; j < n; ++j){
      printf("%e ",array[i*n+j]);
    }
    printf("\n");
  }
}

void simplex(double* array,int m,int n){

}

int main(int argc, char** argv){
  if(argc == 2){
    ifstream istream(argv[1]);
    if(istream.good()){
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

      int m = v.size();
      int n = v[0]->size();
      double* data = new double[m*n];
      for(int i = 0; i < v.size();++i)
	memcpy(data+i*n,&(*(v[i]))[0],n*sizeof(double));

      simplex(data,v.size(),v[0]->size());

      for(vector<double>* ptr : v)
	delete ptr;
    }

  }

  return 0;
}
