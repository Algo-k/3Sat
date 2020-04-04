/**
 * probSAT version SC13_v2
 * uses only break!
 * in case of 3-SAT implements pick and flip method without caching
 * Author: Adrian Balint
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "TimeMeasure.h"
//#include <unistd.h>
//#include <time.h>
//#include <sys\times.h>
#include <string.h>
#include <limits.h>
#include <float.h>
//#include <getopt.h>
#include <signal.h>

#define MAXCLAUSELENGTH 10000 //maximum number of literals per clause //TODO: eliminate this limit
#define STOREBLOCK  20000
# undef LLONG_MAX
#define LLONG_MAX  9223372036854775807
#define BIGINT long long int

void (*initLookUpTable)() = NULL;
void (*pickAndFlipVar)() = NULL;

/*--------*/

/*----Instance data (independent from assignment)----*/
/** The numbers of variables. */
int numVars;
/** The number of clauses. */
int numClauses;
/** The number of literals. */
int numLiterals;
/** The value of the variables. The numbering starts at 1 and the possible values are 0 or 1. */
char *atom;
/** The clauses of the formula represented as: clause[clause_number][literal_number].
 * The clause and literal numbering start both at 1. literal and clause 0 0 is sentinel*/
int **clause;
/**min and max clause length*/
int maxClauseSize;
int minClauseSize;
/** The number of occurrence of each literal.*/
int *numOccurrence;
/** The clauses where each literal occurs. For literal i : occurrence[i+MAXATOMS][j] gives the clause =
 * the j'th occurrence of literal i.  */
int **occurrence;
int maxNumOccurences = 0; //maximum number of occurences for a literal
/*--------*/

/**----Assignment dependent data----*/
/** The number of false clauses.*/
int numFalse;
/** Array containing all clauses that are false. Managed as a list.*/
int *falseClause;
/** whereFalse[i]=j tells that clause i is listed in falseClause at position j.  */
int *whereFalse;
/** The number of true literals in each clause. */
unsigned short *numTrueLit;
/*the number of clauses the variable i will make unsat if flipped*/
int *breaks;
/** critVar[i]=j tells that for clause i the variable j is critically responsible for satisfying i.*/
int *critVar;
int bestVar;

/*----probSAT variables----*/
/** Look-up table for the functions. The values are computed in the initProbSAT method.*/
double *probsBreak;
/** contains the probabilities of the variables from an unsatisfied clause*/
double *probs;
double cb; //for break
double eps = 1.0;
int fct = 0; //function to use 0= poly 1=exp
int caching = 0;
/*--------*/

/*----Input file variables----*/
FILE *fp;
char *fileName;
/*---------*/

/** Run time variables variables*/BIGINT seed;
BIGINT maxTries = LLONG_MAX;
BIGINT maxFlips = LLONG_MAX;
BIGINT flip;
float timeOut = FLT_MAX;
int run = 1;
int printSol = 0;
double TryTime;
long ticks_per_second;
int bestNumFalse;
//parameters flags - indicates if the parameters were set on the command line
int cm_spec = 0, cb_spec = 0, fct_spec = 0, caching_spec = 0;

inline void allocateMemoryM() {
	// Allocating memory for the instance data (independent from the assignment).
	numLiterals = numVars * 2;
	atom = (char*) malloc(sizeof(char) * (numVars + 1));
	//clause = (int**) malloc(sizeof(int*) * (numClauses + 1));
	numOccurrence = (int*) malloc(sizeof(int) * (numLiterals + 1));
	occurrence = (int**) malloc(sizeof(int*) * (numLiterals + 1));
	critVar = (int*) malloc(sizeof(int) * (numClauses + 1));

	// Allocating memory for the assignment dependent data.
	falseClause = (int*) malloc(sizeof(int) * (numClauses + 1));
	whereFalse = (int*) malloc(sizeof(int) * (numClauses + 1));
	numTrueLit = (unsigned short*) malloc(sizeof(unsigned short) * (numClauses + 1));
}

inline void initM() {
	//ticks_per_second = sysconf(_SC_CLK_TCK);
	register int i, j;
	int critLit = 0, lit;
	numFalse = 0;
	for (i = 1; i <= numClauses; i++) {
		numTrueLit[i] = 0;
		whereFalse[i] = -1;
	}

	for (i = 1; i <= numVars; i++) {
		atom[i] = rand() % 2;
		breaks[i] = 0;
	}
	//pass trough all clauses and apply the assignment previously generated
	for (i = 1; i <= numClauses; i++) {
		j = 0;
		while ((lit = clause[i][j])) {
			if (atom[abs(lit)] == (lit > 0)) {
				numTrueLit[i]++;
				critLit = lit;
			}
			j++;
		}
		if (numTrueLit[i] == 1) {
			//if the clause has only one literal that causes it to be sat,
			//then this var. will break the sat of the clause if flipped.
			critVar[i] = abs(critLit);
			breaks[abs(critLit)]++;
		} else if (numTrueLit[i] == 0) {
			//add this clause to the list of unsat caluses.
			falseClause[numFalse] = i;
			whereFalse[i] = numFalse;
			numFalse++;
		}
	}
}

/** Checks whether the assignment from atom is a satisfying assignment.*/
inline int checkAssignmentM() {
	register int i, j;
	int sat, lit;
	for (i = 1; i <= numClauses; i++) {
		sat = 0;
		j = 0;
		while ((lit = clause[i][j])) {
			if (atom[abs(lit)] == (lit > 0))
				sat = 1;
			j++;
		}
		if (sat == 0)
			return 0;
	}
	return 1;
}

//go trough the unsat clauses with the flip counter and DO NOT pick RANDOM unsat clause!!
// do not cache the break values but compute them on the fly (this is also the default implementation of WalkSAT in UBCSAT)
inline void pickAndFlipNCM() {
	register int i, j;
	int bestVar;
	int rClause, tClause;
	rClause = falseClause[flip % numFalse]; //random unsat clause
	bestVar = abs(clause[rClause][0]);
	double randPosition;
	int lit;
	double sumProb = 0;
	int xMakesSat = 0;
	i = 0;
	while ((lit = clause[rClause][i])) {
		breaks[i] = 0;
		//lit = clause[rClause][i];
		//numOccurenceX = numOccurrence[numVars - lit]; //only the negated occurrence of lit will count for break
		j=0;
		while ((tClause = occurrence[numVars - lit][j])){
			if (numTrueLit[tClause] == 1)
				breaks[i]++;
			j++;
		}
		probs[i] = probsBreak[breaks[i]];
		sumProb += probs[i];
		i++;
	}
	randPosition = (double) (rand()) / RAND_MAX * sumProb;
	for (i = i - 1; i != 0; i--) {
		sumProb -= probs[i];
		if (sumProb <= randPosition)
			break;
	}
	bestVar = abs(clause[rClause][i]);

	//flip bestvar
	if (atom[bestVar])
		xMakesSat = -bestVar; //if x=1 then all clauses containing -x will be made sat after fliping x
	else
		xMakesSat = bestVar; //if x=0 then all clauses containing x will be made sat after fliping x
	atom[bestVar] = 1 - atom[bestVar];
	//1. Clauses that contain xMakeSAT will get SAT if not already SAT
	//numOccurenceX = numOccurrence[numVars + xMakesSat];
	i = 0;
	while ((tClause = occurrence[xMakesSat + numVars][i])) {
		//if the clause is unsat it will become SAT so it has to be removed from the list of unsat-clauses.
		if (numTrueLit[tClause] == 0) {
			//remove from unsat-list
			falseClause[whereFalse[tClause]] = falseClause[--numFalse]; //overwrite this clause with the last clause in the list.
			whereFalse[falseClause[numFalse]] = whereFalse[tClause];
			whereFalse[tClause] = -1;
		}
		numTrueLit[tClause]++; //the number of true Lit is increased.
		i++;
	}
	//2. all clauses that contain the literal -xMakesSat=0 will not be longer satisfied by variable x.
	//all this clauses contained x as a satisfying literal
	//numOccurenceX = numOccurrence[numVars - xMakesSat];
	i = 0;
	while ((tClause = occurrence[numVars - xMakesSat][i])) {
		if (numTrueLit[tClause] == 1) { //then xMakesSat=1 was the satisfying literal.
			falseClause[numFalse] = tClause;
			whereFalse[tClause] = numFalse;
			numFalse++;
		}
		numTrueLit[tClause]--;
		i++;
	}
	//fliping done!
}
inline void pickAndFlipM() {
	int var;
	int rClause = falseClause[flip % numFalse];
	double sumProb = 0.0;
	double randPosition;
	register int i, j;
	int tClause; //temporary clause variable
	int xMakesSat; //tells which literal of x will make the clauses where it appears sat.
	i = 0;
	while ((var = abs(clause[rClause][i]))) {
		probs[i] = probsBreak[breaks[var]];
		sumProb += probs[i];
		i++;
	}
	randPosition = (double) (rand()) / RAND_MAX * sumProb;
	for (i = i - 1; i != 0; i--) {
		sumProb -= probs[i];
		if (sumProb <= randPosition)
			break;
	}
	bestVar = abs(clause[rClause][i]);

	if (atom[bestVar] == 1)
		xMakesSat = -bestVar; //if x=1 then all clauses containing -x will be made sat after fliping x
	else
		xMakesSat = bestVar; //if x=0 then all clauses containing x will be made sat after fliping x

	atom[bestVar] = 1 - atom[bestVar];

	//1. all clauses that contain the literal xMakesSat will become SAT, if they where not already sat.
	i = 0;
	while ((tClause = occurrence[xMakesSat + numVars][i])) {
		//if the clause is unsat it will become SAT so it has to be removed from the list of unsat-clauses.
		if (numTrueLit[tClause] == 0) {
			//remove from unsat-list
			falseClause[whereFalse[tClause]] = falseClause[--numFalse]; //overwrite this clause with the last clause in the list.
			whereFalse[falseClause[numFalse]] = whereFalse[tClause];
			whereFalse[tClause] = -1;
			critVar[tClause] = abs(xMakesSat); //this variable is now critically responsible for satisfying tClause
			//adapt the scores of the variables
			//the score of x has to be decreased by one because x is critical and will break this clause if fliped.
			breaks[bestVar]++;
		} else {
			//if the clause is satisfied by only one literal then the score has to be increased by one for this var.
			//because fliping this variable will no longer break the clause
			if (numTrueLit[tClause] == 1) {
				breaks[critVar[tClause]]--;
			}
		}
		//if the number of numTrueLit[tClause]>=2 then nothing will change in the scores
		numTrueLit[tClause]++; //the number of true Lit is increased.
		i++;
	}
	//2. all clauses that contain the literal -xMakesSat=0 will not be longer satisfied by variable x.
	//all this clauses contained x as a satisfying literal
	i = 0;
	while ((tClause = occurrence[numVars - xMakesSat][i])) {
		if (numTrueLit[tClause] == 1) { //then xMakesSat=1 was the satisfying literal.
			//this clause gets unsat.
			falseClause[numFalse] = tClause;
			whereFalse[tClause] = numFalse;
			numFalse++;
			//the score of x has to be increased by one because it is not breaking any more for this clause.
			breaks[bestVar]--;
			//the scores of all variables have to be increased by one ; inclusive x because flipping them will make the clause again sat
		} else if (numTrueLit[tClause] == 2) { //find which literal is true and make it critical and decrease its score
			j = 0;
			while ((var = abs(clause[tClause][j]))) {
				if (((clause[tClause][j] > 0) == atom[abs(var)])) { //x can not be the var anymore because it was flipped //&&(xMakesSat!=var)
					critVar[tClause] = var;
					breaks[var]++;
					break;
				}
				j++;
			}
		}
		numTrueLit[tClause]--;
		i++;
	}

}

void initPolyM() {
	int i;
	probsBreak = (double*) malloc(sizeof(double) * (maxNumOccurences + 1));
	for (i = 0; i <= maxNumOccurences; i++) {
		probsBreak[i] = pow((eps + i), -cb);
	}
}

void initExpM() {
	int i;
	probsBreak = (double*) malloc(sizeof(double) * (maxNumOccurences + 1));
	for (i = 0; i <= maxNumOccurences; i++) {
		probsBreak[i] = pow(cb, -i);
	}
}


void setupParametersM() {
	if (!caching_spec) {
		if (maxClauseSize <= 3){
			pickAndFlipVar = pickAndFlipNCM; //no caching of the break values in case of 3SAT
			caching =0;
		}
		else{
			pickAndFlipVar = pickAndFlipM; //cache the break values for other k-SAT
			caching = 1;
		}
	}
	else{
		if (caching)
			pickAndFlipVar = pickAndFlipM; //cache the break values for other k-SAT
		else
			pickAndFlipVar = pickAndFlipNCM; //no caching of the break values in case of 3SAT
	}
	if (!cb_spec) {
		if (maxClauseSize <= 3) {
			cb = 2.06;
			eps = 0.9;

		} else if (maxClauseSize <= 4)
			cb = 2.85;
		else if (maxClauseSize <= 5)
			cb = 3.7;
		else if (maxClauseSize <= 6)
			cb = 5.1;
		else
			cb = 5.4;
	}
	if (!fct_spec) {
		if (maxClauseSize < 4)
			fct = 0;
		else
			fct = 1;
	}
	initLookUpTable = initPolyM;
}

int BreakOnlyModified() {
	int Try = 0;
	TryTime = 0.;
	double totalTime = 0.;
	//parseParameters(argc, argv);
	//parseFile();
	//printFormulaProperties();
	setupParametersM(); //call only after parsing file!!!
	initLookUpTable(); //Initialize the look up table
	//setupSignalHandler();
	//printSolverParameters();
	srand(seed);

	for (Try = 0; /*Try < maxTries*/; Try++) {
		initM();
		bestNumFalse = numClauses;
		for (flip = 0; flip < maxFlips; flip++) {
			if (numFalse == 0)
				break;
			pickAndFlipVar();
			//printStatsEndFlip(); //update bestNumFalse
		}
		//TryTime = elapsed_seconds();
		totalTime += TryTime;
		if (numFalse == 0) {
			if (!checkAssignmentM()) {
				//fprintf(stderr, "c ERROR the assignment is not valid!");
				//printf("c UNKNOWN");
				return 0;
			} else {
				//printEndStatistics();
				//printf("s SATISFIABLE\n");
				//if (printSol == 1)
				//	printSolution();
				return 10;
			}
		}
		//else
			//printf("c UNKNOWN best(%4d) current(%4d) (%-15.5fsec)\n", bestNumFalse, numFalse, TryTime);
	    // This code Added by me
	    if(ChkTimeOut())return 0;
	}
	//printEndStatistics();
	//if (maxTries > 1)
	//	printf("c %-30s: %-8.3fsec\n", "Mean time per Try", totalTime / (double) Try);
	return 0; 
}

