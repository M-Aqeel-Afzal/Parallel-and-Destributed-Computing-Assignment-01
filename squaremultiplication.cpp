#include<iostream>
#include <stdlib.h>
#include <stdlib.h>  
#include <unistd.h>
#include <ctime>
using namespace std;
int order =32,rows=0,rows1=0;
double **m1;
double **m2;
double **m3;
double **m4;
double **m5;
int *index;
int **index1;


pthread_mutex_t lock,lock1;


bool checking_Correctness()    //checking function to compare matrixes
{
	for (int i =0; i < order;i++) 
	{
		for (int j =0; j < order;j++)
		{
			if(m5[i][j] != m3[i][j]&&m4[i][j] != m3[i][j])
			{}
			else
				return false;
		}
	}
return true;

}



void* BlockWise(void* arg)      //Blockwise multithreaded function for multiplication.
{
int total_threads = (*(int*)arg);
  pthread_mutex_lock(&lock);
	for(int i=0;i<order/total_threads;i++)
	{
cout<<"thread block wise: "<<pthread_self()<<"     "<<rows<<endl;
	
	
	for (int j = 0; j < order; j++)
	for (int k = 0; k < order; k++)
		{
		m4[rows][j] += m1[rows][k] * m2[k][j];
		
		}
	
rows=rows+1;

	}
cout<<endl;
pthread_mutex_unlock(&lock);

}



int r1 = 0;
void* Cyclic(void* arg)   //Cyclic multithreaded function for multiplication.
{
int total_threads = (*(int*)arg);
  pthread_mutex_lock(&lock1);
	for(int i=0;i<order/total_threads;i++)
	{
rows1=index1[r1][i];
cout<<"thread cyclic: "<<pthread_self()<<"     "<<rows1<<endl;
	
	
	for (int j = 0; j < order; j++)
	for (int k = 0; k < order; k++)
		{
		m5[rows1][j] += m1[rows1][k] * m2[k][j];
		
		}
	


	}
cout<<endl;
r1++;
pthread_mutex_unlock(&lock1);

}














 
int main(int argc, char *argv[])     // Main driver function
{
	time_t start0, end0;                //timer variables for serial
	time_t start1, end1;                //timer variables for blockwise
	time_t start2, end2;                //timer variables for cyclic
	m1 = new double*[order];
	 m2 = new double*[order];
	 m3 = new double*[order];
	 m4 = new double*[order];
	 m5 = new double*[order];
	 index1 = new int*[order];

	for (int i =0; i<order ;i++)      //assigning dynamic memory
	{	
		m1[i] = new double[order];
		m2[i] = new double[order];
		m3[i] = new double[order];
		m4[i] = new double[order];
		m5[i] = new double[order];
	}


	int tn =atoi(argv[1]);      //getting cammadline arguments and converting into int              
	index = new int[tn];
	int size = 0; 


	for(int i=0 ;i<tn;i++)    //Assigning Blocks to threads
	{
		index1[i] = new int[order/tn];
		index[i]=size;
		size +=order/tn;
		
	
	}

	int temp = 0;
	for (int i =0; i < tn;i++)  //assigning cyclic indexes
	{
		temp =i;
		for (int j =0; j < order/tn;j++)
		{
			index1[i][j]=  temp;
			temp +=tn;
			
		}
	}
	


	for (int i =0; i < order;i++)  //genrating rundom float values and storing into matrix 1 and matrix 2.
	{
		for (int j =0; j < order;j++)
		{
			m1[i][j]= static_cast <double>(rand() % 10)/5 ;
			m2[i][j]= static_cast <double>(rand() % 10)/5 ;
			
			
		}
		
	}

cout<<"\nMatrix 1:"<<endl;
for (int i =0; i <  order;i++)  //matrix 1.
	{
		for (int j =0; j < order;j++)
		{
			
			cout<<m1[i][j]<<" ";
			
		}
		cout<<endl;
	}

cout<<"\n\n\nMatrix 2:"<<endl;
for (int i =0; i <  order;i++)  //matrix 2.
	{
		for (int j =0; j < order;j++)
		{
			
			cout<<m2[i][j]<<" ";
			
		}
		cout<<endl;
	}




	time(&start0);    //starting time calculation

	for (int i =0; i < order;i++) // serial multiplication of m1 and m2 and storing into m3
	{
		for (int j =0; j < order;j++)
		{
			for ( int k = 0; k < order;k++)
			{
				m3[i][j] = m3[i][j] + m1[i][k] * m2[k][j];
			}
		}

	}

	cout<<"\n\nMatrix3\n"<<endl;
	for (int i =0; i < order;i++) //printing materix3
	{
		for (int j =0; j < order;j++)
		{
			cout<<m3[i][j]<<" ";
		}

	}
	cout<<endl<<endl;
	time(&end0); 

	if (pthread_mutex_init(&lock, NULL) != 0)    //initialization of mutex lock
		{
			cout<<"Fail"<<endl;
			return 1;
	    	}

	if (pthread_mutex_init(&lock1, NULL) != 0)    //initialization of mutex lock1
		{
			cout<<"Fail"<<endl;
			return 1;
	    	}





	pthread_t threads[tn];     //declearing threads array for blockwise
	pthread_t threads1[tn];     //declearing threads array for cyclic





	time(&start1);    //starting time calculation

		

		for (int i = 0; i < tn; i++)       //creating threads
			 {
			
			pthread_create(&threads[i], NULL, BlockWise, (void*)&(tn));   //blockwise calling
			
			}
		for (int i =0; i<tn;i++)     //joining block wise threads
		pthread_join(threads[i], NULL);


	pthread_mutex_destroy(&lock);    //destroying mutex
	              //stoping time calculation

	cout<<"\n\nMatrix4\n"<<endl;
	for (int i =0; i < order;i++) //printing materix4
	{
		for (int j =0; j < order;j++)
		{
			cout<<m4[i][j]<<" ";
		}

	}
	time(&end1); 


		time(&start2);    //starting time calculation

		for (int i = 0; i < tn; i++)       //creating threads for cyclic
			 {
			
			pthread_create(&threads[i], NULL, Cyclic, (void*)&(tn));   //cyclic calling
			
			}
		for (int i =0; i<tn;i++)     //joining cyclic threads
		pthread_join(threads[i], NULL);


	pthread_mutex_destroy(&lock1);    //destroying mutex
	              

	cout<<"\n\nMatrix5\n"<<endl;
	for (int i =0; i < order;i++) //printing materix5
	{
		for (int j =0; j < order;j++)
		{
			cout<<m5[i][j]<<" ";
		}

	}
	time(&end2); 



	double time_taken0 = double(end0 - start0);
	    cout << "\nTime taken by program in serial : " << fixed                           //printing time take by calcultion
		 << time_taken0<<endl;

double time_taken1 = double(end1 - start1);
	    cout << "\nTime taken by program in block wise : " << fixed                           //printing time take by calcultion
		 << time_taken1<<endl;

double time_taken2 = double(end2 - start2);
	    cout << "\nTime taken by program in cyclic : " << fixed                           //printing time take by calcultion
		 << time_taken2<<endl;

if(checking_Correctness())
	cout<<"Matrixes are equal!"<<endl;
else
cout<<"Matrixes are not equal!"<<endl;


	return 0;
}




