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
  //while last row has negative entries
  
  int offset = (m-1)*n;
  int p_col=offset;//pivot column
  for(int i = 1 ; i<(n-1);++i){
    if(array[offset + i] < array[p_col]){
      p_col = offset+i;
    }
  }
  p_col-=offset;
  printf("pivot column is %d\n",p_col);
  
  int p_row=0;//pivot row
  double p_val=array[p_row*n + (n-1)]/array[p_row*n + p_col];
  for(int i = 0; i < (m-1);++i){
    double v1 = array[i*n + p_col];
    double v2 = array[i*n + (n-1)];
    if(v1 > 0){
      if(v2/v1 < p_val){
	p_row = i;
	p_val = v2/v1;
      }
    }
  }

  printf("pivot row is %d\n",p_row);

  //norm p_row
  //elim all other non-zero entries

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
