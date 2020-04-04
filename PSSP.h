
#include "math.h"
#include "3SAT.h"

int *matrix; // -1 = false 0 = packed 1= true

inline void PSSPInit()
{
	matrix=(int*)malloc(sizeof(int)*(numVars+1));
}
void PSSPFree()
{
	free(matrix);
}

inline void Flip(int lit)
{
	if(lit>0)
	{
		matrix[abs(lit)]++;
	}
	else
	{
		matrix[abs(lit)]--;
	}
}

inline bool PackedCheck() {
	register int i, j;
	int sat, lit;
	for (i = 1; i <= numClauses; i++) {
		sat = 0;
		j = 0;
		while ((lit = clause[i][j])) {
			if (matrix[abs(lit)] * lit > 0)
				sat = 1;
			j++;
		}
		if (sat == 0)
			return 0;
	}
	return 1;
}

int PSSP_RepeatConst=36;

long int numSelectOperation;

int PSSP1()
{
	PSSPInit();
    int packed[3];
	int numPacked;
	int num;
	numSelectOperation=0;
	while(true)
	//for(int restart=0;restart<1024*4;restart++)
	{
		for(int i=1;i<=numVars;i++)
			matrix[i]=0;
		for(int i=0;i<PSSP_RepeatConst*numVars*numVars;i++)
		{
			int c=rand()%numClauses+1;
			numSelectOperation++;
			bool SAT=false;
			int lit;
			int j=0;
			while(lit=clause[c][j])
			{
				if(lit*matrix[abs(lit)]>0)
					SAT=true;
				j++;
			}
			if(SAT==false)
			{
				numPacked=0;
				num=0;
				j=0;
				while(lit=clause[c][j])
				{
					num++;
					if(matrix[abs(lit)]==0)
						 packed[numPacked++]=lit;
					j++;
				}
				if(numPacked==0 || numPacked==num)
				{
					Flip(clause[c][rand()%num]);
				}
				else
				{
					Flip(packed[rand()%numPacked]);
				}
			}
			if(i%(numVars*4)==0)
				if(PackedCheck())
					return 10;
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////
////                New Version                        ///
inline void _PSSPInit()
{
	matrix=(int*)malloc(sizeof(int)*(numVars+1));
	numOccurrence = (int*) malloc(sizeof(int) * (numLiterals + 1));
	occurrence = (int**) malloc(sizeof(int*) * (numLiterals + 1));
	falseClause = (int*) malloc(sizeof(int) * (numClauses + 1));
	whereFalse = (int*) malloc(sizeof(int) * (numClauses + 1));
	numTrueLit = (unsigned short*) malloc(sizeof(unsigned short) * (numClauses + 1));
	
	register int i, j;
	int lit, r;
	int clauseSize;
	int tatom;
	char c;

    allocateMemory();
	int *numOccurrenceT = (int*) malloc(sizeof(int) * (numLiterals + 1));

	for(i=0;i<=numLiterals;i++)
	{
		numOccurrence[i]=0;
	    numOccurrenceT[i] = 0;
	}

	for (i = 1; i <= numClauses; i++) {
		j = 0;
		while ((lit = clause[i][j])) {
			numOccurrenceT[lit + numVars]++;
			j++;
		}
	}

    for (i = 0; i < numLiterals + 1; i++) {
		occurrence[i] = (int*) malloc(sizeof(int) * (numOccurrenceT[i] + 1));
		occurrence[i][numOccurrenceT[i]]=0;
		if (numOccurrenceT[i] > maxNumOccurences)
			maxNumOccurences = numOccurrenceT[i];
	}
	for (i = 1; i <= numClauses; i++) {
		j = 0;
		while ((lit = clause[i][j])) {
			occurrence[lit + numVars][numOccurrence[lit + numVars]++] = i;
			j++;
		}
		///////////////////// It was a bug in adrian's main code! ///////////////////////////
		//occurrence[lit + numVars][numOccurrence[lit + numVars]] = 0; //sentinel at the end!
	}
	free(numOccurrenceT);
}
void _RestartInit()
{
	for (int i = 1; i <= numClauses; i++) {
		numTrueLit[i] = 0;
		falseClause[i-1]=i;
		whereFalse[i]=i-1;
	}
	numFalse=numClauses;
}
void _PSSPFree()
{
	free(matrix);
	free(occurrence);
	free(numTrueLit);
	free(falseClause);
	free(whereFalse);
}

inline void _Flip(int lit)
{
	int i;
    int tClause;
	if(matrix[abs(lit)]!=0)
	{
		i = 0;
		while ((tClause = occurrence[numVars - lit][i])) {
			if (numTrueLit[tClause] == 1) {
				falseClause[numFalse] = tClause;
				whereFalse[tClause] = numFalse;
				numFalse++;
			}
			numTrueLit[tClause]--;
			i++;
		}
	}

	if(lit>0)
	{
		matrix[abs(lit)]++;
	}
	else
	{
		matrix[abs(lit)]--;
	}

    if(matrix[abs(lit)]!=0)
	{
		i = 0;
		while ((tClause = occurrence[lit + numVars][i])) {
			if (numTrueLit[tClause] == 0) {
				falseClause[whereFalse[tClause]] = falseClause[--numFalse];
				whereFalse[falseClause[numFalse]] = whereFalse[tClause];
				whereFalse[tClause] = -1;
			}
			numTrueLit[tClause]++;
			i++;
		}
	}
}

double _PSSP_RepeatConst=3;

int _PSSP1()
{
	_PSSPInit();
    int packed[3];
	int numPacked;
	int num;
	numSelectOperation=0;
	while(true)
	//for(int restart=0;restart<1024*4;restart++)
	{
		for(int i=1;i<=numVars;i++)
			matrix[i]=0;
		_RestartInit();
		for(int i=0;i<_PSSP_RepeatConst*numVars*numVars;i++)
		//for(;;)
		{
			//int c=falseClause[rand() % numFalse];
			int c=falseClause[rand() % numFalse];
			numSelectOperation++;
		    numPacked=0;
			num=0;
			int j=0;
			int lit;
			while(lit=clause[c][j])
			{
				num++;
				if(matrix[abs(lit)]==0)
					 packed[numPacked++]=lit;
				j++;
			}
			if(numPacked==0 || numPacked==num)
			{
				_Flip(clause[c][rand()%num]);
			}
			else
			{
				_Flip(packed[rand()%numPacked]);
			}
		    if (numFalse == 0) 
			{
				if(PackedCheck())
					return 10;
				else
					return 0;
			}
			if(ChkTimeOut())return 0;
		}
	}
	return 0;
}
	
