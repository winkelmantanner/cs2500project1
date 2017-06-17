#include <iostream>
using namespace std;


const string WEIGHT_STRING = "heavy";

const long INFINITY = 123456;

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
  L[m - start] = INFINITY; // sentinel
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


int main()
{
  long D[] = { 5,1,6,3,7,4,7 };
  
  mergesort( D, 0, 6 );
  
  for( long k = 0; k < 7; k++ )
    cout << D[k] << " " << flush;
  cout << endl;
  
  
  
  return 0;
}
