// Programmer: Tanner Winkelman
// Instructor: Michael Gosnell
// Class: cs2500 Section: A
// Summer Semester 2017
// Purpose: Compare the run time complexity of
//   Timsort, Merge sort, Bubble sort, and Insertion sort.

#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <stdint.h>
#include <unistd.h>
using namespace std;

// the maximum value of a 4-Byte integer
const long INF = 2147483647;

// the maximum size of data used to test the algorithms
const long MAX_SIZE = 10000;

// one more than the maximum value of data in the test
//   (the minimum is 0)
const long MAX_DATUM_VALUE = 10000;

// the lowest size at which Timsort recursively calls itself and merge
const long TIM_SORT_THRESHOLD = 25;


// source:
// https://stackoverflow.com/questions/13772567/get-cpu-cycle-count

//  Windows
#ifdef _WIN32
#include <intrin.h>


uint64_t rdtsc(){
    return __rdtsc();
}

//  Linux/GCC
#else

uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

#endif

// the type of object to be thrown if something goes wrong
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


// Description: merge() supposes that A[start...(m-1)] is sorted and 
//   A[m...end] is sorted and merges the two into one sorted segment 
//   A[start...end].
// Precondition: operator< is defined for type T;
//   start is the index of the first element in the left division;
//   m is the index of the first element in the right division;
//   end is the index of the last element in the right division;
//   each division is sorted
// Postcondition: A is sorted from start to end.
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

// Description: mergesort() rearranges the elements of A from position min
//   to position max, inclusive, to bring them into nondecreasing order.
// Pre: operator< is defined for type T;
//   min is the index of the first element in the sort; 
//   max is the index of the last element in the sort.
// Postcondition: The elements of A from position min to position max are
//   in nondecreasing order.
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

// Description: my_swap() exchanges the data between i1 and i2.
// Precondition: operator= is defined for type T.
// Postcondition: The previous value of i1 is in i2, and the previous value
//   of i2 is in i1.
template<typename T>
void my_swap( T & i1, T & i2 )
{
  T swapSpace = i1;
  i1 = i2;
  i2 = swapSpace;
  return;
}

// Description: insertionsort() rearranges the elements of A from position min
//   to position max, bringing them into nondecreasing order.
// Precondition: operator< is defined for type T, min is at least 0, and
//   max is less than the size of A.
// Postcondition: The elements of A from position min to position max 
//   are in nondecreasing order.
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

// Description: bubblesort() rearranges the elements of an array A to bring
//   them into nondecreasing order.
// Precondition: operator< is defined for type T.
// Postcondition: The element of A are in nondecreasing order.
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


// Description: timsort() rearranges the elements of A from position min
//   to position max, inclusive, to bring them into nondecreasing order.
// Pre: operator< is defined for type T;
//   min is the index of the first element in the sort;
//   max is the index of the last element in the sort.
// Postcondition: The elements of A from position min to position max are
//   in nondecreasing order.
template<typename T>
void timsort( T A[], const long min, const long max )
{
  if( max - min < TIM_SORT_THRESHOLD )
  {
    insertionsort( A, min, max );
  }
  else
  {
    long center = (max + min + 1) / 2;
    timsort( A, min, center - 1 );
    timsort( A, center, max );
    merge( A, min, center, max );
  }
  return;
}


enum DataState
{
  presorted,
  reversesorted,
  randomData
};



// Desciption: generateData() fills A with data of the specified state.
// Precondition: The size of A is at least size.
// Postcondition: A is filled with integers that are either nondecreasing,
//   nonincreasing, or random, depending on the value of state.
void generateData( 
  long A[], 
  const long size, 
  const DataState state = randomData
)
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

// Description: datacopy() copies the data from source to destination.
// Precondition: The sizes of both source and destination are at least size;
//   operator= is defined for type T.
// Postcondition: The values of destination from position 0 to position (size-1)
//   are the same as the values at the same indices in source.
template<typename T>
void datacopy( const T * const source, T * const destinantion, const long size )
{
  for( long k = 0; k < size; k++ )
    destinantion[k] = source[k];
  return;
}

// Description: isSorted() returns whether or not the data in the array data
//   from position 0 to position n satisfy the specified direction.
// Precondition: direction should not be randomData;
//   operator< must be defined for type T.
// Postcondition: If the data satisfies the specified direction,
//   then true is returned.  Otherwise false is returned.
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

// Description: isEqual() returns whether or not the two passed
//   arrays have eqivalent elements in positions 0 through (n-1)
//   inclusive.
// Precondition: array1 and array2 must be of at least size n.
// Postcondition: The following boolean value is returned:
//   (array1[0] == array2[0]) and (array1[1] == array2[1])
//   and ... and (array1[n-1] == array2[n-1])
template<typename T>
bool isEqual( T * const array1, T * const array2, const long n )
{
  bool eq = true;
  long k = 0;
  while( eq && k < n )
  {
    if( array1[k] != array2[k] )
      eq = false;
    k++;
  }
  return eq;
}


int main()
{
  long data[MAX_SIZE];
  long original_data[MAX_SIZE];
  
  
  srand( time(NULL) );
  
  try
  {
    
    usleep(1000000);
    
    
    for( short dsIndex = 0; dsIndex < 3; dsIndex++ )
    {
      long n = 1;
      for( long k = 1; n * sqrt(2) < MAX_SIZE; k++ )
      {
        n = pow( 2, (k/2) );
        if( k % 2 == 1 )
          n = floor( (n * sqrt(2)) + 0.5 );
      
      
      
        if( dsIndex == 0 )
        {
          generateData( original_data, n, randomData );
        }
        else if( dsIndex == 1 )
        {
          generateData( original_data, n, presorted );
          if( !isSorted( original_data, n ) )
            throw Error( "Error: generateData failed to generate sorted data" );
        }
        else
        {
          generateData( original_data, n, reversesorted );
          if( !isSorted( original_data, n, reversesorted ) )
            throw Error( 
              "Error: generateData failed to generate reverse sorted data" );
        }
        
        
        datacopy( original_data, data, n );
        if( !isEqual( original_data, data, n ) )
        {
          throw Error( "Error: datacopy() failed" );
        }
        
        long/*clock_t*/ start = rdtsc();
        double duration = 0.0;
        
        usleep(10000);
        start = rdtsc();
        timsort( data, 0, n - 1 );
        duration = rdtsc() - start;
        
        if( !isSorted( data, n ) )
        {
          throw Error( "Error: timsort failed to sort the data" );
        }
        if( n >= 2 && isSorted( data, n, reversesorted ) )
        {
          throw Error( "Error: isSorted() malfunction" );
        }
        
        cout << (duration /*/ CLOCKS_PER_SEC*/) << " " << flush;
        
        
        datacopy( original_data, data, n );
        if( !isEqual( original_data, data, n ) )
        {
          throw Error( "Error: datacopy() failed" );
        }
        
        usleep(10000);
        start = rdtsc();
        mergesort( data, 0, n - 1 );
        duration = rdtsc() - start;
        
        if( !isSorted( data, n ) )
        {
          throw Error( "Error: mergesort failed to sort the data" );
        }
        
        cout << (duration /*/ CLOCKS_PER_SEC*/) << " " << flush;
        
        
        datacopy( original_data, data, n );
        if( !isEqual( original_data, data, n ) )
        {
          throw Error( "Error: datacopy() failed" );
        }
        
        usleep(10000);
        start = rdtsc();
        insertionsort( data, 0, n - 1 );
        duration = rdtsc() - start;
        
        if( !isSorted( data, n ) )
        {
          throw Error( "Error: insertionsort failed to sort the data" );
        }
        
        cout << (duration /*/ CLOCKS_PER_SEC*/) << " " << flush;
        
        
        datacopy( original_data, data, n );
        if( !isEqual( original_data, data, n ) )
        {
          throw Error( "Error: datacopy() failed" );
        }
        
        usleep(10000);
        start = rdtsc();
        bubblesort( data, 0, n - 1 );
        duration = rdtsc() - start;
        
        if( !isSorted( data, n ) )
        {
          throw Error( "Error: bubblesort failed to sort the data" );
        }
        
        cout << (duration /*/ CLOCKS_PER_SEC*/) << " ";
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
