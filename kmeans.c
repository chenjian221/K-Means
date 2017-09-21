
/* created by Mingyi Liu
 * GT account: mliu366
 * GT ID: 903332202
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

// 2X2 Matrix

double**     data; //all data points
double**     centroids;  //the centroids
double**     centroidsCopy;  //the copy of centroids

int numItems;
int numAttrs;

void input_file(const char *fileName);
void free_matrix(int n, double **a);
void init_centroids(int k);
void form_cluster(int a);
void copy_centroids(int k);
void update_centroids(int a);
void output_file(int k);
void restore(int k);

double distance (double a[],double b[],int num_attrs);
double** malloc_matrix   (int a, int b);

int main(int argc, const char * argv[])
{

	double euclidSum=0;//euclidSum is the sum of euclid distance between previous centroid and current centroid
	if (argc != 4)
	{
        printf("You must have enter 3 input arguments\n");
		return 1;
	}
    input_file(argv[1]);
	int K = (int) strtol(argv[2], NULL, 10);
    if(K > numItems){
        printf("You cannot have K values that are greater than number of items\n");
        return 2;
    }
	double threshold = strtod(argv[3],NULL);
	int maxIteration  = 10;
	init_centroids(K);
	for(int time=0;time<=maxIteration;time++ )
	{
		form_cluster(K);
		copy_centroids(K);
		update_centroids(K);
		for(int i=0;i<K;i++)
		{
			euclidSum+=distance(centroidsCopy[i],centroids[i],numAttrs);
		}
		if(euclidSum<threshold) // if the euclidSum is smaller than a defined threshold, the program ends
		{
			printf("the iteration number is:%d\n",time);
			break;
		}
	}

	restore(K);
    output_file(K);

	free_matrix(K,centroids);
	free_matrix(K,centroidsCopy);
	free_matrix(numItems,data);
}
void input_file(const char *fileName)
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
	fscanf(ifp, "%d %d\n", &numItems, &numAttrs);

	data=malloc_matrix(numItems, numAttrs);
	for (int i=0; i<numItems; i++)
	{
		for (int j=0; j<numAttrs; j++)
		{
			fscanf(ifp, "%lf", &data[i][j]);
		}
	}
}

void  form_cluster(int a)
{
    double w;
    double D,minD;
    for (int i=0; i<numItems; i++)
    {
        w=0;
        minD=distance(data[i],centroids[0],numAttrs);
        for(int j=1; j<a; j++)
        {
            D=distance(data[i],centroids[j],numAttrs);
            if (D<minD)
            {
                w=j;
                minD=D;
            }
        }
        data[i][numAttrs-1]=w; // problem
    }
}

void free_matrix(int n, double **a)
{
	for(int i=0; i<n; i++)
		free(a[i]);
	free (a);
}

void  init_centroids(int k)
{
    centroids = malloc_matrix(k, numAttrs);
    centroidsCopy = malloc_matrix(k,numAttrs);

    for(int i=0; i<k; i++)
        for(int j=0; j<numAttrs; j++)
            centroids[i][j]=data[i][j];

}

void copy_centroids(int k){
    for(int i=0; i<k; i++)
        for(int j=0; j<numAttrs; j++)
            centroidsCopy[i][j] = centroids[i][j];

}


/*
 * update new centroids
 */
void update_centroids(int a)
{
	double *sum;
	sum=malloc(sizeof(double)*(numAttrs));
	for(int i=0;i<a;i++)
	{
        int n = 0;
        memset(sum,0, sizeof(sum[0])*numAttrs);
		for(int j=0;j<numItems;j++)
		{
			if(data[j][numAttrs-1]==i)
			{
				n++;
				for(int k=0;k<numAttrs;k++)
				{
					sum[k]+=data[j][k];
				}
			}
		}
		for(int k=0;k<numAttrs;k++)
		{
			centroids[i][k]=sum[k]/n;
		}
	}
	free(sum);
}

double distance(double  a[], double  b[], int num_attrs)
{
	double sum=0;
	double distance;
	for(int i=0; i<num_attrs; i++)
		sum +=pow(a[i]-b[i],2);
	distance=sqrt(sum);
	return distance;
}
void restore(int k){

}
void output_file(int k)
{
	FILE *ofp1,*ofp2;
	ofp1 = fopen("outputFile_clusters", "w");
	fprintf(ofp1, "%d %d %d\n", numItems, numAttrs, k);
	for (int i=0; i<numItems; i++)
	{
		for (int j=0; j<numAttrs; j++)
		{
            fprintf(ofp1, "%lf ", data[i][j]);
		}
        fprintf(ofp1, "\n");
	}
	fclose(ofp1);

	ofp2 = fopen("outputFile_centers", "w");
	fprintf(ofp2, "%d %d %d\n", numItems, numAttrs, k);
	for (int i=0; i<k; i++)
	{
		for (int j=0; j<numAttrs; j++)
		{
			fprintf(ofp1, "%lf ", centroids[i][j]);
		}
        fprintf(ofp1, "\n");
	}
	fclose(ofp2);
}



double **malloc_matrix(int a, int b)
{
	double **data1 = (double**)malloc(a*sizeof(double*));
	for(int i=0; i<a; i++)
		data1[i] = (double*)malloc(b*sizeof(double));

	return data1;
}
/*
 * assign points to the nearest centroid
 * w refers to which cluster the point belongs
 */

