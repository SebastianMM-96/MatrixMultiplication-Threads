#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


int main()
{
    
/*Apuntadores de archivos_txt*/
FILE *inputFile1;
FILE *inputFile2;
//FILE *outputFile1;

/*Contadoes para los loop que se haran en este programa*/
int i,j,k;

/*Variables que manejaran el numero de filas y columnas de las dos matrices que se ingresaran en el archivo. */
int rows1,cols1,rows2,cols2;

/*Variable para escribir el nombre del archivo.*/
char fileName[100];

/*Si el archivo no se encontro la variable por default sera falsa. */
bool fileFound=false;

/*Si las matrices se pueden multiplicar el valor por default sera verdadero*/
bool multiplcation = true;

/*Variable para la suma de los elementos multiplicados de las matrices. */
int sum = 0;

/*Le pedimos al usuario que ingrese el nombre del archivo donde
contiene la matriz A con su extension. */
do
{
    printf("Ingrese el nombre del archivo de la matriz A - con su extension - : ");
    scanf("%[^\n]%*c",fileName);
    inputFile1 = fopen(fileName,"r");
    
    /*Si el archivo no esta, enviara mensaje al usuario de que el archivo no se encontro.*/
    if(!inputFile1)
    {
            printf("File Not Found!!\n");
            fileFound=true;
    }
    else
        fileFound=false;

}while(fileFound);

/*NOTA: Estos archivos deben de encontrarse en la carpeta donde se encuentra el archivo con extencion .c*/


/*Le pedimos al usuario que ingrese el nombre del archivo donde
contiene la matriz B con su extension. */

do
{
    printf("Ingrese el nombre del archivo de la matriz B - con su extension - : ");
    scanf("%[^\n]%*c",fileName);
    inputFile2 = fopen(fileName,"r");
    if(!inputFile2)
    {
            printf("File Not Found!!\n");
            fileFound=true;
    }
    else
        fileFound=false;

}while(fileFound);


/*Obtenemos las filas y columnas de los archivos que contienen matrices. */
fscanf(inputFile1,"%i",&rows1);
fscanf(inputFile1,"%i",&cols1);
fscanf(inputFile2,"%i",&rows2);
fscanf(inputFile2,"%i",&cols2);

/*Imprimimos los valores de las filas y columnas de las dos matrices. */
printf("\n\nRows1 = %d",rows1);
printf("\nCols1 = %d",cols1);
printf("\n\nRows2 = %d",rows2);
printf("\nCols2 = %d",cols2);


/*Si las columnas son diferentes, entonces las matrices y la multiplicacion no podran realizarse. */
if(cols1!=rows2)
    multiplcation=false;
else
    multiplcation=true;

if(multiplcation==false)
{
    printf("No se pueden multiplicar las dos matrices, por favor revise sus matrices.\n");
    fclose(inputFile1);
    fclose(inputFile2);
    return 0;
}

else
{
    //Si no sucede eso
    /*Reservamos memoria para realizar la multiplicacion de las matrices*/

    //MAT A
    int **mat1 = (int **)malloc(rows1 * sizeof(int*));
    for(i = 0; i < rows1; i++)
        mat1[i] = (int *)malloc(cols1 * sizeof(int));

    i=0;

    //MAT B
    int **mat2 = (int **)malloc(rows2 * sizeof(int*));
    for(i = 0; i < rows2; i++)
        mat2[i] = (int *)malloc(cols2 * sizeof(int));

    i=0;

    //MAT Resultante
    int **mat3 = (int **)malloc(rows1 * sizeof(int*));
    for(i = 0; i < rows1; i++)
        mat3[i] = (int *)malloc(cols2 * sizeof(int));

    i=0;

    /*Obtenemos los elementos contenidos en el archivo 1*/
    while(!feof(inputFile1))
    {
        for(i=0;i<rows1;i++)
        {
            for(j=0;j<cols1;j++)
                fscanf(inputFile1,"%d",&mat1[i][j]);
        }
    }

    i=0;
    j=0;

    /*Obtenemos los elementos contenidos en el archivo 2*/
    while(!feof(inputFile2))
    {
        for(i=0;i<rows2;i++)
        {
            for(j=0;j<cols2;j++)
                fscanf(inputFile2,"%d",&mat2[i][j]);
        }
    }

    /////////////////////////
    i=0;
    j=0;

    /*Mostramos al usuario los elementos de la matriz A*/

    printf("\n\n");
    //print matrix 1
    printf("Matrix A\n");
    for(i=0;i<rows1;i++)
    {
        for(j=0;j<cols1;j++)
            printf("%d\t",mat1[i][j]);

        printf("\n");
    }
    ////////////////////////////
    i=0;
    k=0;

    /*Mostramos al usuario los elementos de la matriz A*/

    printf("\n\n");
    //print matrix 2
    printf("Matrix B\n");
    for(i=0;i<rows2;i++)
    {
        for(j=0;j<cols2;j++)
            printf("%d\t",mat2[i][j]);

        printf("\n");
    }
    /////////////////////////
    i=0;
    j=0;

    //Realizamos la multiplicacion de las dos matrices, esta multiplicacion se alojara en el arreglo mat3[][]
    for(i=0;i<rows1;i++)
    {
        for(j=0;j<cols2;j++)
        {
            sum=0;
            for(k=0;k<rows2;k++)
                sum+=mat1[i][k]*mat2[k][j];
            
            //printf("Valor: %d\n", sum );
            mat3[i][j] = sum;
        }
        //printf("Valor: %d\n", sum );
        //mat3[i][j]=sum;
    }

    i=0;
    j=0;
    k=0;

    //print multiplication result
    printf("\n\nResult = \n\n");

    for(i=0;i<rows1;i++)
    {
        for(j=0;j<cols2;j++)
            printf("%d\t",mat3[i][j]);

        printf("\n");
    }

    /*Cerramos los archivos donde estaban contenidas las matrices. */
    fclose(inputFile1);
    fclose(inputFile2);
}

}