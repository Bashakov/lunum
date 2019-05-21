

#include <string.h>
#include <stdlib.h>
#include <math.h>
#ifndef LUNUM_API_NOCOMPLEX
#include <complex.h>
typedef double complex Complex;
#endif // LUNUM_API_NOCOMPLEX
#include "numarray.h"

typedef unsigned char Bool;


#define EXPR_ADD(T) {for(int i=0;i<N;++i)((T*)c)[i]=((T*)a)[i]+((T*)b)[i];}
#define EXPR_SUB(T) {for(int i=0;i<N;++i)((T*)c)[i]=((T*)a)[i]-((T*)b)[i];}
#define EXPR_MUL(T) {for(int i=0;i<N;++i)((T*)c)[i]=((T*)a)[i]*((T*)b)[i];}
#define EXPR_DIV(T) {for(int i=0;i<N;++i)((T*)c)[i]=((T*)a)[i]/((T*)b)[i];}
#define EXPR_POW(T) {for(int i=0;i<N;++i)((T*)c)[i]=(T)pow((double)((T*)a)[i],(double)((T*)b)[i]);}
#define EXPR_COW(T) {for(int i=0;i<N;++i)((T*)c)[i]=cpow(((T*)a)[i],((T*)b)[i]);}

#define EXPR_ASSIGN0(T,val) {for(int i=0;i<N;++i)((T*)a)[i]=val;}
#define EXPR_ASSIGN1(T,val) {for(int i=0;i<N;++i)((T*)a)[i]=*((T*)val);}
#define EXPR_ASSIGN2(Ta,Tb) {for(int i=0;i<N;++i)((Ta*)a)[i]=(Ta)((Tb*)b)[i];}



char *array_typename(enum ArrayType T)
{
  switch (T) {
  case ARRAY_TYPE_BOOL    : return "bool";
  case ARRAY_TYPE_CHAR    : return "char";
  case ARRAY_TYPE_SHORT   : return "short";
  case ARRAY_TYPE_INT     : return "int";
  case ARRAY_TYPE_LONG    : return "long";
  case ARRAY_TYPE_FLOAT   : return "float";
  case ARRAY_TYPE_DOUBLE  : return "double";
#ifndef LUNUM_API_NOCOMPLEX
  case ARRAY_TYPE_COMPLEX : return "complex";
#endif // LUNUM_API_COMPLEX
  }
  return NULL; // indicates invalid type
}

enum ArrayType array_typeflag(char c)
{
  switch (c) {
  case 'b': return ARRAY_TYPE_BOOL;
  case 'c': return ARRAY_TYPE_CHAR;
  case 's': return ARRAY_TYPE_SHORT;
  case 'i': return ARRAY_TYPE_INT;
  case 'l': return ARRAY_TYPE_LONG;
  case 'f': return ARRAY_TYPE_FLOAT;
  case 'd': return ARRAY_TYPE_DOUBLE;
#ifndef LUNUM_API_NOCOMPLEX
  case 'z': return ARRAY_TYPE_COMPLEX;
#endif // LUNUM_API_COMPLEX
  }
  return (ArrayType)-1; // indicates invalid type
}


struct Array array_new_zeros(int N, enum ArrayType T)
{
  struct Array A;

  A.data  = malloc(N*array_sizeof(T));
  A.owns  = 1;
  A.size  = N;
  A.dtype = T;
  A.shape = (int*) malloc(sizeof(int));
  A.ndims = 1;

  A.shape[0] = N;
  void *a = A.data;

  switch (T) {
  case ARRAY_TYPE_BOOL    : EXPR_ASSIGN0(Bool   ,0) ; break;
  case ARRAY_TYPE_CHAR    : EXPR_ASSIGN0(char   ,0) ; break;
  case ARRAY_TYPE_SHORT   : EXPR_ASSIGN0(short  ,0) ; break;
  case ARRAY_TYPE_INT     : EXPR_ASSIGN0(int    ,0) ; break;
  case ARRAY_TYPE_LONG    : EXPR_ASSIGN0(long   ,0) ; break;
  case ARRAY_TYPE_FLOAT   : EXPR_ASSIGN0(float  ,0) ; break;
  case ARRAY_TYPE_DOUBLE  : EXPR_ASSIGN0(double ,0) ; break;
#ifndef LUNUM_API_NOCOMPLEX
  case ARRAY_TYPE_COMPLEX : EXPR_ASSIGN0(Complex,0) ; break;
#endif // LUNUM_API_COMPLEX
  }

  return A;
}

struct Array array_new_copy(const struct Array *B, enum ArrayType T)
{
  struct Array A = array_new_zeros(B->size, T);
  array_resize(&A, B->shape, B->ndims);
  array_assign_from_array(&A, B);
  return A;
}

void array_del(struct Array *A)
{
  if (A->data && A->owns) free(A->data);
  if (A->shape) free(A->shape);

  A->size = 0;
  A->data = NULL;
  A->shape = NULL;
}

int array_resize(struct Array *A, const int *N, int Nd)
{
  int ntot = 1;
  for (int d=0; d<Nd; ++d) ntot *= N[d];

  if (A->size != ntot) {
    return 1;
  }
  if (A->shape) free(A->shape);

  A->ndims = Nd;
  A->shape = (int*) malloc(Nd*sizeof(int));
  memcpy(A->shape, N, Nd*sizeof(int));

  return 0;
}

void array_binary_op(const struct Array *A, const struct Array *B,
                     struct Array *C, enum ArrayOperation op)
{
  const int N = A->size;
  const void *a = A->data;
  const void *b = B->data;
  void *c = C->data;

  switch (op) {
  case ARRAY_OP_ADD:
    switch (A->dtype) {
    case ARRAY_TYPE_BOOL    : EXPR_ADD(Bool   ) ; break;
    case ARRAY_TYPE_CHAR    : EXPR_ADD(char   ) ; break;
    case ARRAY_TYPE_SHORT   : EXPR_ADD(short  ) ; break;
    case ARRAY_TYPE_INT     : EXPR_ADD(int    ) ; break;
    case ARRAY_TYPE_LONG    : EXPR_ADD(long   ) ; break;
    case ARRAY_TYPE_FLOAT   : EXPR_ADD(float  ) ; break;
    case ARRAY_TYPE_DOUBLE  : EXPR_ADD(double ) ; break;
#ifndef LUNUM_API_NOCOMPLEX
    case ARRAY_TYPE_COMPLEX : EXPR_ADD(Complex) ; break;
#endif // LUNUM_API_COMPLEX
    }
    break;
  case ARRAY_OP_SUB:
    switch (A->dtype) {
    case ARRAY_TYPE_BOOL    : EXPR_SUB(Bool   ) ; break;
    case ARRAY_TYPE_CHAR    : EXPR_SUB(char   ) ; break;
    case ARRAY_TYPE_SHORT   : EXPR_SUB(short  ) ; break;
    case ARRAY_TYPE_INT     : EXPR_SUB(int    ) ; break;
    case ARRAY_TYPE_LONG    : EXPR_SUB(long   ) ; break;
    case ARRAY_TYPE_FLOAT   : EXPR_SUB(float  ) ; break;
    case ARRAY_TYPE_DOUBLE  : EXPR_SUB(double ) ; break;
#ifndef LUNUM_API_NOCOMPLEX
    case ARRAY_TYPE_COMPLEX : EXPR_SUB(Complex) ; break;
#endif // LUNUM_API_COMPLEX
    }
    break;
  case ARRAY_OP_MUL:
    switch (A->dtype) {
    case ARRAY_TYPE_BOOL    : EXPR_MUL(Bool   ) ; break;
    case ARRAY_TYPE_CHAR    : EXPR_MUL(char   ) ; break;
    case ARRAY_TYPE_SHORT   : EXPR_MUL(short  ) ; break;
    case ARRAY_TYPE_INT     : EXPR_MUL(int    ) ; break;
    case ARRAY_TYPE_LONG    : EXPR_MUL(long   ) ; break;
    case ARRAY_TYPE_FLOAT   : EXPR_MUL(float  ) ; break;
    case ARRAY_TYPE_DOUBLE  : EXPR_MUL(double ) ; break;
#ifndef LUNUM_API_NOCOMPLEX
    case ARRAY_TYPE_COMPLEX : EXPR_MUL(Complex) ; break;
#endif // LUNUM_API_COMPLEX
    }
    break;
  case ARRAY_OP_DIV:
    switch (A->dtype) {
    case ARRAY_TYPE_BOOL    : EXPR_DIV(Bool   ) ; break;
    case ARRAY_TYPE_CHAR    : EXPR_DIV(char   ) ; break;
    case ARRAY_TYPE_SHORT   : EXPR_DIV(short  ) ; break;
    case ARRAY_TYPE_INT     : EXPR_DIV(int    ) ; break;
    case ARRAY_TYPE_LONG    : EXPR_DIV(long   ) ; break;
    case ARRAY_TYPE_FLOAT   : EXPR_DIV(float  ) ; break;
    case ARRAY_TYPE_DOUBLE  : EXPR_DIV(double ) ; break;
#ifndef LUNUM_API_NOCOMPLEX
    case ARRAY_TYPE_COMPLEX : EXPR_DIV(Complex) ; break;
#endif // LUNUM_API_COMPLEX
    }
    break;
  case ARRAY_OP_POW:
    switch (A->dtype) {
    case ARRAY_TYPE_BOOL    : EXPR_POW(Bool   ) ; break;
    case ARRAY_TYPE_CHAR    : EXPR_POW(char   ) ; break;
    case ARRAY_TYPE_SHORT   : EXPR_POW(short  ) ; break;
    case ARRAY_TYPE_INT     : EXPR_POW(int    ) ; break;
    case ARRAY_TYPE_LONG    : EXPR_POW(long   ) ; break;
    case ARRAY_TYPE_FLOAT   : EXPR_POW(float  ) ; break;
    case ARRAY_TYPE_DOUBLE  : EXPR_POW(double ) ; break;
#ifndef LUNUM_API_NOCOMPLEX
    case ARRAY_TYPE_COMPLEX : EXPR_COW(Complex) ; break;
#endif // LUNUM_API_COMPLEX
    }
    break;
  }
}

int array_sizeof(enum ArrayType T)
{
  switch (T) {
  case ARRAY_TYPE_BOOL    : return sizeof(Bool);
  case ARRAY_TYPE_CHAR    : return sizeof(char);
  case ARRAY_TYPE_SHORT   : return sizeof(short);
  case ARRAY_TYPE_INT     : return sizeof(int);
  case ARRAY_TYPE_LONG    : return sizeof(long);
  case ARRAY_TYPE_FLOAT   : return sizeof(float);
  case ARRAY_TYPE_DOUBLE  : return sizeof(double);
#ifndef LUNUM_API_NOCOMPLEX
  case ARRAY_TYPE_COMPLEX : return sizeof(Complex);
#endif // LUNUM_API_COMPLEX
  }
  return sizeof(int);
}

void array_assign_from_scalar(struct Array *A, const void *val)
{
  const int N = A->size;
  void     *a = A->data;

  switch (A->dtype) {
  case ARRAY_TYPE_BOOL    : EXPR_ASSIGN1(Bool   , val) ; break;
  case ARRAY_TYPE_CHAR    : EXPR_ASSIGN1(char   , val) ; break;
  case ARRAY_TYPE_SHORT   : EXPR_ASSIGN1(short  , val) ; break;
  case ARRAY_TYPE_INT     : EXPR_ASSIGN1(int    , val) ; break;
  case ARRAY_TYPE_LONG    : EXPR_ASSIGN1(long   , val) ; break;
  case ARRAY_TYPE_FLOAT   : EXPR_ASSIGN1(float  , val) ; break;
  case ARRAY_TYPE_DOUBLE  : EXPR_ASSIGN1(double , val) ; break;
#ifndef LUNUM_API_NOCOMPLEX
  case ARRAY_TYPE_COMPLEX : EXPR_ASSIGN1(Complex, val) ; break;
#endif // LUNUM_API_COMPLEX
  }
}

void array_assign_from_array(struct Array *A, const struct Array *B)
{
  void        *a = A->data;
  const void  *b = B->data;
  const int    N = B->size;

  switch (A->dtype) {
  case ARRAY_TYPE_BOOL:
    switch (B->dtype) {
      //                               (A->type, B->type)
    case ARRAY_TYPE_BOOL    : EXPR_ASSIGN2(Bool, Bool)    ; break;
    case ARRAY_TYPE_CHAR    : EXPR_ASSIGN2(Bool, char)    ; break;
    case ARRAY_TYPE_SHORT   : EXPR_ASSIGN2(Bool, short)   ; break;
    case ARRAY_TYPE_INT     : EXPR_ASSIGN2(Bool, int)     ; break;
    case ARRAY_TYPE_LONG    : EXPR_ASSIGN2(Bool, long)    ; break;
    case ARRAY_TYPE_FLOAT   : EXPR_ASSIGN2(Bool, float)   ; break;
    case ARRAY_TYPE_DOUBLE  : EXPR_ASSIGN2(Bool, double)  ; break;
#ifndef LUNUM_API_NOCOMPLEX
    case ARRAY_TYPE_COMPLEX : EXPR_ASSIGN2(Bool, Complex) ; break;
#endif // LUNUM_API_COMPLEX
    }
    break;

  case ARRAY_TYPE_CHAR:
    switch (B->dtype) {
    case ARRAY_TYPE_BOOL    : EXPR_ASSIGN2(char, Bool)    ; break;
    case ARRAY_TYPE_CHAR    : EXPR_ASSIGN2(char, char)    ; break;
    case ARRAY_TYPE_SHORT   : EXPR_ASSIGN2(char, short)   ; break;
    case ARRAY_TYPE_INT     : EXPR_ASSIGN2(char, int)     ; break;
    case ARRAY_TYPE_LONG    : EXPR_ASSIGN2(char, long)    ; break;
    case ARRAY_TYPE_FLOAT   : EXPR_ASSIGN2(char, float)   ; break;
    case ARRAY_TYPE_DOUBLE  : EXPR_ASSIGN2(char, double)  ; break;
#ifndef LUNUM_API_NOCOMPLEX
    case ARRAY_TYPE_COMPLEX : EXPR_ASSIGN2(char, Complex) ; break;
#endif // LUNUM_API_COMPLEX
    }
    break;

  case ARRAY_TYPE_SHORT:
    switch (B->dtype) {
    case ARRAY_TYPE_BOOL    : EXPR_ASSIGN2(short, Bool)    ; break;
    case ARRAY_TYPE_CHAR    : EXPR_ASSIGN2(short, char)    ; break;
    case ARRAY_TYPE_SHORT   : EXPR_ASSIGN2(short, short)   ; break;
    case ARRAY_TYPE_INT     : EXPR_ASSIGN2(short, int)     ; break;
    case ARRAY_TYPE_LONG    : EXPR_ASSIGN2(short, long)    ; break;
    case ARRAY_TYPE_FLOAT   : EXPR_ASSIGN2(short, float)   ; break;
    case ARRAY_TYPE_DOUBLE  : EXPR_ASSIGN2(short, double)  ; break;
#ifndef LUNUM_API_NOCOMPLEX
    case ARRAY_TYPE_COMPLEX : EXPR_ASSIGN2(short, Complex) ; break;
#endif // LUNUM_API_COMPLEX
    }
    break;

  case ARRAY_TYPE_INT:
    switch (B->dtype) {
    case ARRAY_TYPE_BOOL    : EXPR_ASSIGN2(int, Bool)    ; break;
    case ARRAY_TYPE_CHAR    : EXPR_ASSIGN2(int, char)    ; break;
    case ARRAY_TYPE_SHORT   : EXPR_ASSIGN2(int, short)   ; break;
    case ARRAY_TYPE_INT     : EXPR_ASSIGN2(int, int)     ; break;
    case ARRAY_TYPE_LONG    : EXPR_ASSIGN2(int, long)    ; break;
    case ARRAY_TYPE_FLOAT   : EXPR_ASSIGN2(int, float)   ; break;
    case ARRAY_TYPE_DOUBLE  : EXPR_ASSIGN2(int, double)  ; break;
#ifndef LUNUM_API_NOCOMPLEX
    case ARRAY_TYPE_COMPLEX : EXPR_ASSIGN2(int, Complex) ; break;
#endif // LUNUM_API_COMPLEX
    }
    break;

  case ARRAY_TYPE_LONG:
    switch (B->dtype) {
    case ARRAY_TYPE_BOOL    : EXPR_ASSIGN2(long, Bool)    ; break;
    case ARRAY_TYPE_CHAR    : EXPR_ASSIGN2(long, char)    ; break;
    case ARRAY_TYPE_SHORT   : EXPR_ASSIGN2(long, short)   ; break;
    case ARRAY_TYPE_INT     : EXPR_ASSIGN2(long, int)     ; break;
    case ARRAY_TYPE_LONG    : EXPR_ASSIGN2(long, long)    ; break;
    case ARRAY_TYPE_FLOAT   : EXPR_ASSIGN2(long, float)   ; break;
    case ARRAY_TYPE_DOUBLE  : EXPR_ASSIGN2(long, double)  ; break;
#ifndef LUNUM_API_NOCOMPLEX
    case ARRAY_TYPE_COMPLEX : EXPR_ASSIGN2(long, Complex) ; break;
#endif // LUNUM_API_COMPLEX
    }
    break;

  case ARRAY_TYPE_FLOAT:
    switch (B->dtype) {
    case ARRAY_TYPE_BOOL    : EXPR_ASSIGN2(float, Bool)    ; break;
    case ARRAY_TYPE_CHAR    : EXPR_ASSIGN2(float, char)    ; break;
    case ARRAY_TYPE_SHORT   : EXPR_ASSIGN2(float, short)   ; break;
    case ARRAY_TYPE_INT     : EXPR_ASSIGN2(float, int)     ; break;
    case ARRAY_TYPE_LONG    : EXPR_ASSIGN2(float, long)    ; break;
    case ARRAY_TYPE_FLOAT   : EXPR_ASSIGN2(float, float)   ; break;
    case ARRAY_TYPE_DOUBLE  : EXPR_ASSIGN2(float, double)  ; break;
#ifndef LUNUM_API_NOCOMPLEX
    case ARRAY_TYPE_COMPLEX : EXPR_ASSIGN2(float, Complex) ; break;
#endif // LUNUM_API_COMPLEX
    }
    break;

  case ARRAY_TYPE_DOUBLE:
    switch (B->dtype) {
    case ARRAY_TYPE_BOOL    : EXPR_ASSIGN2(double, Bool)    ; break;
    case ARRAY_TYPE_CHAR    : EXPR_ASSIGN2(double, char)    ; break;
    case ARRAY_TYPE_SHORT   : EXPR_ASSIGN2(double, short)   ; break;
    case ARRAY_TYPE_INT     : EXPR_ASSIGN2(double, int)     ; break;
    case ARRAY_TYPE_LONG    : EXPR_ASSIGN2(double, long)    ; break;
    case ARRAY_TYPE_FLOAT   : EXPR_ASSIGN2(double, float)   ; break;
    case ARRAY_TYPE_DOUBLE  : EXPR_ASSIGN2(double, double)  ; break;
#ifndef LUNUM_API_NOCOMPLEX
    case ARRAY_TYPE_COMPLEX : EXPR_ASSIGN2(double, Complex) ; break;
#endif // LUNUM_API_COMPLEX
    }
    break;

#ifndef LUNUM_API_NOCOMPLEX
  case ARRAY_TYPE_COMPLEX:
    switch (B->dtype) {
    case ARRAY_TYPE_BOOL    : EXPR_ASSIGN2(Complex, Bool)    ; break;
    case ARRAY_TYPE_CHAR    : EXPR_ASSIGN2(Complex, char)    ; break;
    case ARRAY_TYPE_SHORT   : EXPR_ASSIGN2(Complex, short)   ; break;
    case ARRAY_TYPE_INT     : EXPR_ASSIGN2(Complex, int)     ; break;
    case ARRAY_TYPE_LONG    : EXPR_ASSIGN2(Complex, long)    ; break;
    case ARRAY_TYPE_FLOAT   : EXPR_ASSIGN2(Complex, float)   ; break;
    case ARRAY_TYPE_DOUBLE  : EXPR_ASSIGN2(Complex, double)  ; break;
    case ARRAY_TYPE_COMPLEX : EXPR_ASSIGN2(Complex, Complex) ; break;
    }
    break;
#endif // LUNUM_API_COMPLEX
  }

}

struct Array array_new_from_slice(const struct Array *B1,
				  int *start, int *stop, int *skip, int Nd)
// -----------------------------------------------------------------------------
// Extracts a slice from B1, and returns it as the contiguous array 'B0'
// -----------------------------------------------------------------------------
// @start : starting indices into B1
// @stop  : upper bound on selection (non-inclusive)
// @skip  : distance between entries of B1 along each axis
// @Nd    : the number of axes in each array
// -----------------------------------------------------------------------------
{

  int *J = (int*) malloc(Nd*sizeof(int)); // current indices into B1
  int *N = (int*) malloc(Nd*sizeof(int)); // number of elements to select
  int *S = (int*) malloc(Nd*sizeof(int)); // strides (in memory) along each axis

  int ntot = 1;

  for (int d=0; d<Nd; ++d) {
    J[d] = 0;
    N[d] = 1 + (stop[d] - start[d] - 1) / skip[d];
    ntot *= N[d];
  }

  S[Nd-1] = 1;
  for (int d=Nd-2; d>=0; --d) S[d] = S[d+1] * B1->shape[d+1];


  struct Array B0 = array_new_zeros(ntot, B1->dtype);
  array_resize(&B0, N, Nd);
  int sizeof_T = array_sizeof(B0.dtype);
  int m = 0; // indexes into B0, advanced uniformly


  char *b0 = (char*) B0 .data;
  char *b1 = (char*) B1->data;


  while (J[0] < N[0]) {

    int M = 0;
    for (int d=0; d<Nd; ++d) M += (J[d] * skip[d] + start[d]) * S[d];

    // ----- use the indices m,M -----
    memcpy(b0 + (m++)*sizeof_T, b1 + M*sizeof_T, sizeof_T);
    // -----                 -----

    ++J[Nd-1];
    for (int d=Nd-1; d!=0; --d) {
      if (J[d] == N[d]) {
	J[d] = 0;
	++J[d-1];
      }
    }
  }

  free(J);
  free(N);
  free(S);

  return B0;
}

struct Array array_new_from_mask(const struct Array *B1, struct Array *M)
// -----------------------------------------------------------------------------
// Extracts the indices of B1 for which M is true, and returns a 1d-array
// -----------------------------------------------------------------------------
// @M : Array of bool's, must have the same size as B1
// -----------------------------------------------------------------------------
{
  int sizeof_T = array_sizeof(B1->dtype);

  char *b0 = (char*) malloc(sizeof_T);
  char *b1 = (char*) B1->data;

  int m = 0;

  for (int n=0; n<B1->size; ++n) {
    if (((Bool*)M->data)[n]) {
      b0 = (char*) realloc(b0, (++m)*sizeof(double));
      memcpy(b0 + (m-1)*sizeof_T, b1 + n*sizeof_T, sizeof_T);
    }
  }

  struct Array B0 = array_new_zeros(m, B1->dtype);
  memcpy(B0.data, b0, m*sizeof_T);
  free(b0);

  return B0;
}
