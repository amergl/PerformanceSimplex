#ifndef PERFORMANCE_SIMPLEX
#define PERFORMANCE_SIMPLEX

#include <stdio.h>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cmath>
#include <omp.h>

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

void printArray(double *array, int m, int n) {
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      printf("%e ", array[i * n + j]);
    }
    printf("\n");
  }
}

//creates the simplex scheme from a condensed format
void inflate(double *array, int in_m, int in_n, double **output, int &m, int &n, int &n_vars) {
  const double EPS = 1e-16;
  n_vars = in_n-2;
  n = n_vars+1;

  double type = 0;
  //get constraint type
  for (int i = 0; i < (in_m - 1); ++i) {
    type = array[i * in_n + n_vars];
    if (fabs(type - (-1)) < EPS) { //case less than
      n += 1;
    } else if (fabs(type - (0)) < EPS) { // case equal to
      //ignore
    } else if (fabs(type - (1)) < EPS) { // case greater than
      n += 1;
      #pragma omp parallel for
      for (int j = 0; j < in_n; ++j)
        (array[i * in_n + j]) *= -1;
    } else
      return;
  }

  //compute simplex tableau
  m = in_m;
  (*output) = (double *) calloc(m * n, sizeof(double));
  double *A = *output;
  int count = 0;

  for (int row = 0; row < (m-1); ++row) {
    //insert constraints into tableau
    #pragma omp parallel for
    for (int col = 0; col < n_vars; ++col) {
      A[row * n + col] = array[row * in_n + col];
    }
    //add slack variable
    const bool isInequality = fabs(array[row * in_n + n_vars ]) > 0;
    if (isInequality) {
          A[row * n + n_vars + count] = 1;
          count++;
    }
    //insert constraint right hand side
    A[row * n + n - 1] = array[row * in_n + n_vars + 1];
  }

  for(int col = 0; col < n_vars;++col)
    A[(m-1)*n + col] = array[(in_m-1)*in_n+col];

}

void eliminate(double *array, int m, int n, int p_row, int p_col) {
  #pragma omp parallel for
  for (int i = 0; i < m; ++i) {
    if (i != p_row)
      #pragma omp parallel for
      for (int j = 0; j < n; ++j) {
        if (j != p_col)
          array[i * n + j] -= (array[i * n + p_col] * array[p_row * n + j]) / array[p_row * n + p_col];
        
      } //for j
  } // for i
  #pragma omp parallel for
  for (int i = 0; i < m; ++i) {
    if (i != p_row)
      array[i * n + p_col] = 0;
  }
}

bool findPivot(double *array, int m, int n, int &p_row, int &p_col) {
  //find pivot column
  int offset = (m - 1) * n;
  p_col = offset;//pivot column
  for (int i = 1; i < (n - 1); ++i) {
    if (array[offset + i] < array[p_col]) {
      p_col = offset + i;
    }
  }
  if (array[p_col] >= 0)
    return 0;
  p_col -= offset;

  //find pivot row
  p_row = 0;

  //used for init value of p_val
  while (array[p_row * n + p_col] < 0 && p_row < (m-1))
    ++p_row;

  double v1=0,v2=0,v2byv1,p_val = array[p_row * n + (n - 1)] / array[p_row * n + p_col];
  //pragma omp parallel for
  for (int i = p_row+1; i < (m - 1); ++i) {
    v1 = array[i * n + p_col];
    v2 = array[i * n + (n - 1)];
    if (v1 > 0) {
      v2byv1=v2/v1;
      if (v2byv1 < p_val) {
        p_row = i;
        p_val = v2byv1;
      }
    }
  }

  //norm p_row
  double v=array[p_row * n + p_col];
  #pragma omp parallel for
  for (int i = 0; i < n; ++i) {
    if (i != p_col)
      array[p_row * n + i] /= v;
  }
  array[p_row * n + p_col] = 1;
  return 1;
}

void dualsimplex(double *condensed, int condensed_m, int condensed_n, double **solution, int &solution_size) {
  //find pivot transposed
  int m,n,n_vars;
  double* array;
  inflate(condensed,condensed_m,condensed_n,&array,m,n,n_vars);
  solution_size = n - 1;

  int* permutation = (int*)calloc(solution_size,sizeof(int));
  #pragma omp parallel for
  for(int i = 0 ; i < (solution_size-n_vars);++i)
    permutation[i]=n_vars+i;


  int p_row = 0,p_col = 0;
  double b_i,a_i=0;
  int offset = n - 1;

  int it=solution_size;
  while(it-- > 0){
    b_i=0;
    #pragma omp parallel for
    for (int i = 0; i < (m - 1); ++i) {
      if (array[i * n + offset] <= b_i) {
	p_row = i;
	b_i = array[p_row * n + offset];
      }
    }

    if (fabs(b_i) <= 1e-16 )
      break;

    p_col = 0;
    a_i = 1.0;
    
    for (int i = 0; i < (n - 1); ++i) {
      if (b_i / array[p_row * n + i] >= b_i / a_i) {
	p_col = i;
	a_i = array[p_row * n + p_col];
      }
    }

    //norm line
    #pragma omp parallel for
    for (int i = 0; i < n; ++i)
      array[p_row * n + i] /= a_i;

    permutation[p_row]=p_col;
    eliminate(array, m, n, p_row, p_col);
  }


  (*solution) = (double*)calloc(solution_size,sizeof(double));
  for(int i = 0; i < (solution_size-n_vars);++i)
    (*solution)[permutation[i]]=array[i*n+n-1];
}

void simplex(double *condensed, int condensed_m, int condensed_n, double **solution, int &solution_size) {
  int p_row, p_col,m,n,n_vars;
  double* array;
  inflate(condensed,condensed_m,condensed_n,&array,m,n,n_vars);
  solution_size = n - 1;
  int* permutation = (int*)calloc(solution_size,sizeof(int));
  #pragma omp parallel for
  for(int i = 0 ; i < (solution_size-n_vars);++i)
    permutation[i]=n_vars+i;


  while (findPivot(array, m, n, p_row, p_col)) {
    permutation[p_row] = p_col;
    eliminate(array, m, n, p_row, p_col);
  }

  (*solution) = (double*)calloc(solution_size,sizeof(double));

  for(int i = 0; i < (solution_size-n_vars);++i)
    (*solution)[permutation[i]]=array[i*n+n-1];

  delete[] permutation;
}

#endif
