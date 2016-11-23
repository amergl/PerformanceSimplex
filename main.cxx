#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <memory.h>

using namespace std;

void printArray(double* array, int m, int n){
  for(int i = 0 ; i < m; ++i){
    for(int j = 0; j < n; ++j){
      printf("%e ",array[i*n+j]);
    }
    printf("\n");
  }
}

void eliminate(double* array, int m, int n, int p_row, int p_col){
  for(int i = 0; i<m; ++i)
    for(int j = 0 ; j<n;++j){
      if(i==p_row)
	continue;
      if(j!=p_col)
	array[i*n+j]-=(array[i*n+p_col]*array[p_row*n+j])/array[p_row*n+p_col];
    }
  for(int i = 0; i<m;++i)
    if(i!=p_row)
      array[i*n+p_col]=0;
}

bool findPivot(double* array, int m, int n, int& p_row, int& p_col){
  //find pivot column
  int offset = (m-1)*n;
  p_col=offset;//pivot column
  for(int i = 1 ; i<(n-1);++i){
    if(array[offset + i] < array[p_col]){
      p_col = offset+i;
    }
  }
  if(array[p_col] >= 0)
    return 0;
  p_col-=offset;
  printf("pivot column is %d\n",p_col);

  //find pivot row
  p_row=0;
  while(array[p_row*n+p_col]<0 && p_row < m)
    ++p_row;
  
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
  for(int i = 0 ; i < n;++i){
    if(i != p_col)
      array[p_row*n + i]/=array[p_row*n+p_col];
  }
  array[p_row*n+p_col]=1;

  return 1;
}

int dualSimplex(double* array, int m, int n){
  //find pivot transposed
  int p_row=0;
  double b_i=0.0;
  int offset = n-1;
  for(int i = 0 ; i < (m-1);++i){
    if(array[i*n+offset] <= b_i){
      p_row=i;
      b_i=array[p_row*n+offset];
    }
  }

  if(b_i == 0)
      return 0;
  
  int p_col=0;
  double a_i=1.0;
  for(int i = 0 ; i < (n-1);++i){
    if(b_i/array[p_row*n+i] >= b_i/a_i){
      p_col=i;
      a_i=array[p_row*n+p_col];
    }
  }

  //norm line
  for(int i = 0 ; i < n;++i)
    array[p_row*n+i]/=a_i;
  
  eliminate(array,m,n,p_row,p_col);

  return 1;
}

void simplex(double* array,int m,int n){
    printf("Input scheme\n");
    printArray(array,m,n);
    while(dualSimplex(array,m,n));
    printf("Dual simplex result\n");
    printArray(array,m,n);


    int p_row,p_col;
    while(findPivot(array,m,n,p_row,p_col)){
	eliminate(array,m,n,p_row,p_col);
    }
    printf("Primal simplex result\n");
    printArray(array,m,n);

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
      for(int i = 0; i < (int)v.size();++i)
	memcpy(data+i*n,&(*(v[i]))[0],n*sizeof(double));

      simplex(data,v.size(),v[0]->size());

      for(vector<double>* ptr : v)
	delete ptr;
    }

  }

  return 0;
}
