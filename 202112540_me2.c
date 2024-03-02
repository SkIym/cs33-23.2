#include <stdio.h>
#include <stdlib.h>

void EQUIVALENCE (void);
int NUM_EQUI_CLASSES(void);
void UNION (int, int);
int FIND (int);

// Declare global variables here
int *FATHER; //the global FATHER array
int SIZE; //the global FATHER array size
// Declare global variables here

int main()
{   
    EQUIVALENCE ();
    printf("%d", NUM_EQUI_CLASSES());
    free (FATHER);
    return 0;
}

void EQUIVALENCE ()
{
    int i, j, n;
    
    // get size of S
    scanf("%d", &n);
    SIZE = n;

    //Initialize FATHER array
    FATHER = (int *) malloc (n * sizeof(int));
    for(i=1; i<=n; i++) {
      FATHER[i] = -1;
    }

    scanf("%d %d", &i, &j);
    while (i != 0 && j != 0)
    {   
        i = FIND(i);
        j = FIND(j);
        if (i != j) UNION(i, j);
        scanf("%d %d", &i, &j);
    }
    
    return;
}

int FIND(int i) {

  // find root
  int r = i;
  while(FATHER[r] > 0) {
    r = FATHER[r];
  }

  int j = i;
  int k;
  while(j != r) {
    k = FATHER[j];
    FATHER[j] = r;
    j = k;
  }

  return r;

}

void UNION(int i, int j) {
  int count = FATHER[i] + FATHER[j];
  if (abs(FATHER[i]) > abs(FATHER[j])) {
    FATHER[j] = i;
    FATHER[i] = count;
  } else {
    FATHER[i] = j;
    FATHER[j] = count;
  }
}


int NUM_EQUI_CLASSES()
{
	int numEquiClasses=0;
	
	for(int i=1; i<=SIZE; i++) {
    if(FATHER[i] < 0) {
      numEquiClasses++;
    }
  }

	return numEquiClasses;
}