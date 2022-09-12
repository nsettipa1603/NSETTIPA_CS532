#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct Student
{
	struct Student* Next;
	char* FName , *LName ,* Major , *Degree , * Advisor ;
	double GPA ;
	int Credit_Hours;
};
char * Split(char * String )
{
	int ci = 0;
	char C;
	while(String[ci]!=',' && String[ci]!='\n' && String[ci]!=0)
	{
		C = String[ci];
		ci++;
	}
	if(String[ci]==0) return NULL;
	if(String[ci]==',' || String[ci]=='\n') String[ci]=0;
	return (String+ci+1);
}
void CopyStr(char ** Dest , char * Source)
{
	int size=1+strlen(Source);
	*Dest = (char*) malloc(size* sizeof(char));
	strcpy(*Dest , Source);
}
struct Student * MakeStudent()
{
	struct Student * Sptr =calloc(sizeof(struct Student) ,1);
}
struct Student * SelectMaxGPAExcept(struct Student * First_index ,int *Current ,int First , int Sec )
{
	struct Student* Temp =First_index;
	struct Student* Max =Temp;
	int maxi=0;
	if(First== maxi || Sec==maxi)
	{
		maxi++; Max=Max->Next;
	}
	if(First== maxi || Sec==maxi)
	{
		maxi++; Max=Max->Next;
	}
	int i;
	for ( i= 0;  Temp!=NULL; i++)
	{
		if(i!= First && i!=  Sec)
		{
		if(Max->GPA<= Temp->GPA) {Max=Temp;maxi=i;}
		}
		Temp=Temp->Next;
	}
	*Current=maxi;
	return Max;
}
int main()
{
	FILE* F;
	F=fopen( "students_dataset.csv", "r");
	char Buffer[250];
	fgets(Buffer, 250, F);
	struct Student * Stu_lst_Strt=NULL;
	struct Student * Stu_lst_last=NULL;
	struct Student * Current_Student=NULL;
	Stu_lst_Strt =MakeStudent();
	int Total_Credit=0;
	int N_rows=0;
	double Avg_GPA_CS=0;
	int Cs_Students=0;
	while (0!=fgets(Buffer, 250, F))
	{
		if(Current_Student==NULL) Current_Student= Stu_lst_Strt;
		else
		{
			Current_Student->Next= MakeStudent();
			Current_Student = Current_Student->Next;
		}
		char* Sp=Buffer;
		for(int i=0 ; i<8 ; i++)
		{
			char* Sp2=Split(Sp);
			//printf(" %s ",Sp);
			if(i==0) CopyStr(&Current_Student->FName , Sp);
			else if(i==1) CopyStr(&Current_Student->LName ,Sp) ;
			else if(i==2) CopyStr(&Current_Student->Major ,Sp) ;
			else if(i==3) CopyStr(&Current_Student->Degree,Sp) ;
			else if(i==4) Current_Student->GPA= strtod(Sp ,NULL);
			else if(i==5) Current_Student->Credit_Hours = strtod(Sp,NULL);
			else if(i==7) CopyStr(&Current_Student->Advisor ,Sp);
			Sp=Sp2;
		}
		if(strcmp( Current_Student->Major , "Computer Science") ==0 )
		{Avg_GPA_CS+=Current_Student->GPA;
			Cs_Students++;
		}Total_Credit+= Current_Student->Credit_Hours;
		N_rows++;
	}
	Current_Student->Next=NULL;
	int Fnum=-1 , SNum=-1;
	struct Student * MaxSptr=SelectMaxGPAExcept(Stu_lst_Strt ,&Fnum,Fnum,SNum);
	printf("Highest Grade Student %s %s with Grade %f\n", MaxSptr->FName ,MaxSptr->LName ,MaxSptr->GPA);
	MaxSptr=SelectMaxGPAExcept(Stu_lst_Strt ,&SNum,Fnum,SNum);
	printf("2nd Highest Grade Student %s %s with Grade %f\n", MaxSptr->FName ,MaxSptr->LName ,MaxSptr->GPA);
	MaxSptr=SelectMaxGPAExcept(Stu_lst_Strt ,&SNum,Fnum,SNum);
	printf("3rd Highest Grade Student %s %s with Grade %f\n", MaxSptr->FName ,MaxSptr->LName ,MaxSptr->GPA);
	struct Student * Temp = Stu_lst_Strt;
	char * Degreelist[N_rows];
	int DegreeCount=0;
	for(int i=0 ;i< N_rows ;i++)
	{
		char IsThere=0;
		for (int j = 0; j < DegreeCount; j++)
		{
			if(strcmp(Degreelist[j] ,Temp->Degree)==0) {IsThere=1; break;}
		}
		if(IsThere==0 && strcmp(Temp->Degree,"Undeclared")!=0 ) Degreelist[DegreeCount++]= Temp->Degree;
		Temp=Temp->Next;
	}
	for (int i = 0; i < DegreeCount; i++)
	{
		printf("%s\n",Degreelist[i]);
	}
	printf("Average Credit hours %d\n" , Total_Credit/N_rows);
	printf("Average GPA CS students %f\n" , Avg_GPA_CS / Cs_Students);

	Temp=Stu_lst_Strt;
	struct Student* Departments[N_rows]; int CountDeparts=0;
	for(int i=0 ;i< N_rows ;i++)
	{
		char IsThere=0;
		for (int j = 0; j < CountDeparts; j++)
		{
			if(strcmp(Departments[j]->Major ,Temp->Major)==0) {IsThere=1; break;}
		}
		if(IsThere==0) Departments[CountDeparts++]= Temp;
		Temp=Temp->Next;
	}

	int AdvLstCount[CountDeparts];
	for(int i=0 ;i< CountDeparts ; i++) AdvLstCount[i]=0;
	for (int i = 0; i < CountDeparts; i++)
	{
		char* Advisors[N_rows]; int AdvCount=0;
		Temp= Stu_lst_Strt;
		for (int j = 0; j < N_rows; j++)
		{

			if(strcmp( Departments[i]->Major, Temp->Major)==0  )
			{
				//Temp->Advisor
				char IsThere=0;
				for (int k = 0; k < AdvCount; k++)
				{
				 if(strcmp(Advisors[k] ,Temp->Advisor)==0 ) {IsThere=1 ; break;}
				}
				if(IsThere==0) {Advisors[AdvCount++]= Temp->Advisor; AdvLstCount[i]++;}
			}
			Temp= Temp->Next;
		}

		printf("%s % d\n", Departments[i]->Major , AdvLstCount[i]);

	}


	return 0;
}
