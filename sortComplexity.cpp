// Programmer: Tanner Winkelman
// Instructor: Michael Gosnell
// Class: cs2500 Section: A
// Summer Semester 2017
// Purpose: Compare the complexity of mergesort, bubblesort, and insertionsort.

#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
using namespace std;


const string WEIGHT_STRING = "heavyheavyheavyheavyheavyheavyheavyheavyheavy";

const long INF = 2147483647;

const long MAX_SIZE = 100000;

const long MAX_DATUM_VALUE = 1000000;

struct dataType
{
  long key;
  string weight;
  dataType()
  {
    weight = WEIGHT_STRING;
    key = 0;
  }
};

bool operator<( const dataType & lhs, const dataType & rhs )
{
  return lhs.key < rhs.key;
}


// Pre: start is the index of the first element in the left division; m is the index of the first element in the right division; end is the index of the last element in the right division; each division is sorted
template<typename T>
void merge(
  T A[],
  const long start,
  const long m,
  const long end
)
{
  long size = end - start + 1;
  T * L = new T[m - start + 1];  // with space for sentinel
  for( long i = start; i < m; i++ )
  {
    L[i-start] = A[i];
  }
  L[m - start] = INF; // sentinel
  long i = 0;
  long j = 0;
  for( long k = 0; k < size; k++ )
  {
    if( L[i] < A[j + m] || j > end - m )
    {
      A[k + start] = L[i];
      i++;
    }
    else
    {
      A[k + start] = A[j + m];
      j++;
    }
  }
  return;
}

// Pre: min is the index of the first element in the sort, max is the index of the last element in the sort.
template<typename T>
void mergesort( T A[], const long min, const long max )
{
  if( max != min )
  {
    long center = (max + min + 1) / 2;
    mergesort( A, min, center - 1 );
    mergesort( A, center, max );
    merge( A, min, center, max );
  }
  
  return;
}


template<typename T>
void my_swap( T & i1, T & i2 )
{
  T swapSpace = i1;
  i1 = i2;
  i2 = swapSpace;
  return;
}

template<typename T>
void insertionsort( T A[], const long min, const long max )
{
  for( long i = min + 1; i <= max; i++ )
  {
    long j = i;
    while( j > min && A[j] < A[j-1] )
    {
      my_swap( A[j-1], A[j] );
      j--;
    }
  }
  
  return;
}


template<typename T>
void bubblesort( T A[], const long min, const long max )
{
  // invariant: at each execution of the gaurd, the last i elements are the largest i elements in sorted order
  for( long i = 0; i < max - min + 1; i++ )
  {
    // invariant: at each execution of the gaurd, the largest element of A[min...j] is A[j]
    for( long j = min; j < max - i; j++ )
    {
      if( A[j+1] < A[j] )
      {
        my_swap( A[j+1], A[j] );
      }
    }
  }
  
  return;
}

enum DataState
{
  presorted,
  reversesorted,
  randomData
};




void generateData( long A[], const long size, const DataState state )
{
  for( long k = 0; k < size; k++ )
  {
    A[k] = rand() % MAX_DATUM_VALUE;
  }
  if( state == presorted )
  {
    mergesort( A, 0, size - 1 );
  }
  else if( state == reversesorted )
  {
    mergesort( A, 0, size - 1 );
    for( long j = 0; j < size / 2; j++ )
      my_swap( A[j], A[size - j - 1] );
  }
  return;
}



int main()
{
  long data[MAX_SIZE];
  
  srand( time(NULL) );
  
  long n = 1;
  for( long k = 1; n * sqrt(2) < MAX_SIZE; k++ )
  {
    n = pow( 2, (k/2) );
    if( k % 2 == 1 )
      n = floor( n * sqrt(2) );
    generateData( data, n, randomData );
    clock_t start;
    double duration;
    start = clock();
    mergesort( data, 0, n - 1 );
    duration = clock() - start;
    
    cout << duration << " " << flush;
    
  }
  
  
  
  return 0;
}
