
#include "Adrian.h"
#include <time.h>

void Produce(int n,double d)
{
	seed=1000;
	numVars = n;
	numClauses = 1;
	int MaxPossible=(n*(n-1)*(n-2)*8)/6;
	clause = (int**)malloc(sizeof(int*)*(MaxPossible+1));
	int* Assumed=(int*)malloc(sizeof(int)*(n+1));
	for(int i=1;i<=n;i++)
	{
		Assumed[i]=-1+2*rand()%2;
	}
	for(int i=1;i<=n;i++)
	{
		for(int j=i+1;j<=n;j++)
		{
			for(int k=j+1;k<=n;k++)
			{
				for(int s=-1;s<=1;s+=2)
				{
				    for(int t=-1;t<=1;t+=2)
				    {
				        for(int u=-1;u<=1;u+=2)
						{
							int l1=i*s;
							int l2=j*t;
							int l3=k*u;
							bool B=true;
							if(Assumed[i]*i==l1)
								if(Assumed[j]*j==l2)
									if(Assumed[k]*k==l3)
										B=false;
							if(B)
							{
								if(rand()%1001<1000*d)
								{
									clause[numClauses]=(int*)malloc(sizeof(int)*4);
									clause[numClauses][0]=l1;
									clause[numClauses][1]=l2;
									clause[numClauses][2]=l3;
									clause[numClauses++][3]=0;
								}
							}
						}
				    }
				}
			}
		}
	}
	numClauses--;
}
void SATFree()
{
	for(int i=1;i<=numClauses;i++)
		free(clause[i]);
	free(clause);
}
