clock_t ___begin;
double ___TimeOut;

void BeginTime();
void BeginTime()
{
	___begin=clock();
}
double EndTime();
double EndTime()
{
	return ((double)(clock()-___begin)*1000)/CLOCKS_PER_SEC;
}
bool TimeOut();
bool ChkTimeOut()
{
	if(((double)(clock()-___begin)*1000)/CLOCKS_PER_SEC>___TimeOut)
	{
		return true;
	}
	return false;
}
