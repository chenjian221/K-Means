
/* created by Mingyi Liu
 * GT account: mliu366
 * GT ID: 903332202
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>


int         num_items;
int         num_attrs;
double**     data; //all data points
double**     centroids;  //the centroids
double**     centroidsCopy;  //the copy of centroids


void        inputFile();
double**     malloc_matrix   (int a, int b);
void        free_matrix     (int n, double **a);
void        init_centroids          (int a);
double      distance        (double a[],double b[],int num_attrs);
void        form_cluster();
void        copy_centroids();
void        update_centroids();
void        outputFile      (int k);


void inputFile(const char * fileName)
{
	FILE *ifp;
	ifp = fopen(fileName, "r");
	if(ifp==NULL)
	{
		printf("This File Cannot Be Open\n");
	}
	if(feof(ifp))
	{
		printf("This File is Empty\n");
		fclose(ifp);
	}
	fscanf(ifp, "%d %d\n", &num_items, &num_attrs);
	data=malloc_matrix(num_items, num_attrs);
	for (int i=0; i<num_items; i++)
	{
		for (int j=0; j<num_attrs; j++)
				{
					fscanf(ifp, "%lf", &data[i][j]);
				}
	}
}

double **malloc_matrix(int a, int b)
{
		    double **data1 = (double**)malloc(a*sizeof(double*));
		    for(int i=0; i<a; i++)
		    	data1[i] = (double*)malloc(b*sizeof(double));

		    return data1;
}

void free_matrix(int n, double **a)
{
	        for(int i=0; i<n; i++)
	        	free(a[i]);
	        free (a);
}


void outputFile(int k)
{
	FILE *ofp1,*ofp2;
	ofp1 = fopen("outputFile_clusters", "w");
	fprintf(ofp1, "%d %d %d\n", num_items, num_attrs, k);
	for (int i=0; i<num_items; i++)
	{
		for (int j=0; j<num_attrs; j++)
				{
					if(j==(num_attrs-1))
						fprintf(ofp1, "%lf\n", data[i][j]);
					else
						fprintf(ofp1, "%lf", data[i][j]);
				}
		}
	fclose(ofp1);

	ofp2 = fopen("outputFile_centers", "w");
	fprintf(ofp2, "%d %d %d\n", num_items, num_attrs, k);
	for (int i=0; i<k; i++)
	{
			for (int j=0; j<num_attrs; j++)
					{
							fprintf(ofp1, "%lf", centroids[i][j]);
					}
			}
		fclose(ofp2);
}


void  init_centroids(int a)
{
	 centroids=malloc_matrix(a, num_attrs);
		for(int i=0; i<a; i++)
		{
			for(int j=0; j<num_attrs; j++)
				centroids[i][j]=data[i][j];
		}
}



double distance(double a*,double b*,int num_attrs)
{
	double sum=0;
	double distance;
	for(int i=0; i<num_attrs; i++)
		sum+=sqr(a[i]-b[i]);
	distance=sqrt(sum);
	return distance;
}

/*
 * assign points to the nearest centroid
 * w refers to which cluster the point belongs
 */
void  form_cluster(int a)
{
   double w;
   double D,minD;
   for (int i=0; i<num_items; i++)
   {
	   w=0;
	   minD=distance(data[i],centroids[0],num_attrs);
	   for(int j=1; j<a; j++)
	   {
		   D=distance(data[i],centroids[j],num_attrs);
		   if (D<minD)
		   {
			   w=j;
			   minD=D;
		   }
	   }
		  data[i][num_attrs]=w;
   }

}

/*
 * update new centroids
 */
void update_centroids(int a)
{
	    int n;
		double *sum;
		sum=malloc(sizeof(double)*(num_attrs));
		for(int i=0;i<a;i++)
		{
			n=0;
			for(int k=0;k<num_attrs;k++)
				sum[k]=0;
			for(int j=0;j<num_items;j++)
			{
				if(data[j][num_attrs]==i)
				{
					n++;
					for(int k=0;k<num_attrs;k++)
					{
						sum[k]+=data[j][k];
					}
				}
			}
			for(int k=0;k<num_attrs;k++)
			{
				centroids[i][k]=sum[k]/n;
			}
		}
		free(sum);
}

void copy_centroids(int K){

}
void restore(int K){

}
int main(int argc, const char * argv[])
{
	int time;
	double sumC=0;//sumC is the sum of euclid distance between previous centroid and current centroid
	if (argc != 4)
	{
		return 1;
	}

	//todo: read file
	inputFile(argv[1]);
	int K = (int) strtol(argv[2], NULL, 10);
	// int maxIteration = (int) strtol(argv[2], NULL, 10);// read from file
	int maxIteration  = 10000000;
	double threshold = strtod(argv[3],NULL); // unsure

	init_centroids(K);

	for(time=0;time<=maxIteration;time++ )
	{
		form_cluster(K);
		copy_centroids(K);
		update_centroids(K);
		for(int i=0;i<K;i++)
		{
			sumC+=distance(centroidsCopy[i],centroids[i],num_attrs);
		}

		if(sumC<threshold) // if the sumC is smaller than a defined threshold, the program ends
		{
			printf("the iteration number is:%d\n",time);
			break;
		}
	}

	restore(K);
	outputFile(K);

}
