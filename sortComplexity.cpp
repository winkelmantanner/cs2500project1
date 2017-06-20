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

const long MAX_SIZE = 10000;

const long MAX_DATUM_VALUE = 10000;
/*
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
*/


struct Error
{
  string message;
  Error( const string m ) : message(m) {}
};

ostream& operator<<( ostream & lhs, const Error & rhs )
{
  lhs << rhs.message;
  return lhs;
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
  // invariant: at each execution of the gaurd, A[start...(start + k)] is sorted.
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
  // invariant: at each execution of the gaurd, A[min...(i-1)] is sorted
  for( long i = min + 1; i <= max; i++ )
  {
    long j = i;
    // invariant: at each execution of the gaurd,
    //   A[j] is the minimum element of A[j...i]
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




void generateData( long A[], const long size, const DataState state = randomData )
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

template<typename T>
void datacopy( T * const source, T * const destinantion, const long size )
{
  for( long k = 0; k < size; k++ )
    destinantion[k] = source[k];
  return;
}


template<typename T>
bool isSorted( T * const data, const long n, DataState direction = presorted )
{
  for( long k = 1; k < n; k++ )
  {
    if( ( direction == presorted && data[k] < data[k-1] ) || ( direction == reversesorted && data[k-1] < data[k] ) )
    {
      return false;
    }
  }
  
  return true;
}


int main()
{
  long data[MAX_SIZE];
  long original_data[MAX_SIZE];
  
  
  srand( time(NULL) );
  
  try
  {
    
    for( short dsIndex = 0; dsIndex < 3; dsIndex++ )
    {
      long n = 1;
      for( long k = 1; n * sqrt(2) < MAX_SIZE; k++ )
      {
        n = pow( 2, (k/2) );
        if( k % 2 == 1 )
          n = floor( n * sqrt(2) );
      
      
      
        if( dsIndex == 0 )
        {
          generateData( original_data, n, randomData );
        }
        else if( dsIndex == 1 )
        {
          generateData( original_data, n, presorted );
          if( !isSorted( original_data, n ) )
            throw Error( "Error: generate data failed to generate sorted data." );
        }
        else
        {
          generateData( original_data, n, reversesorted );
          if( !isSorted( original_data, n, reversesorted ) )
            throw Error( "Error: generate data failed to generate sorted data." );
        }
        
        
        datacopy( original_data, data, n );
        
        clock_t start;
        double duration;
        start = clock();
        mergesort( data, 0, n - 1 );
        duration = clock() - start;
        
        if( !isSorted( data, n ) )
        {
          throw Error( "Error: mergesort failed to sort the data" );
        }
        
        cout << (duration / CLOCKS_PER_SEC) << " " << flush;
        
        
        datacopy( original_data, data, n );
        
        start = clock();
        insertionsort( data, 0, n - 1 );
        duration = clock() - start;
        
        if( !isSorted( data, n ) )
        {
          throw Error( "Error: insertionsort failed to sort the data" );
        }
        
        cout << (duration / CLOCKS_PER_SEC) << " " << flush;
        
        
        datacopy( original_data, data, n );
        
        start = clock();
        bubblesort( data, 0, n - 1 );
        duration = clock() - start;
        
        if( !isSorted( data, n ) )
        {
          throw Error( "Error: bubblesort failed to sort the data" );
        }
        
        cout << (duration / CLOCKS_PER_SEC) << " ";
        cout << k << " " << n << endl;
      }
      
      
    }
  
  }
  catch( const Error e )
  {
    cout << e << endl;
  }
  
  return 0;
}
