/* Un laboratorio de especialidades medicinales posee un archivo en disco cuyos registros
contienen los siguientes campos:

	LOTE: 5 caracteres
	CODIGO DE BARRAS: 21 caracteres
	NOMBRE: 40 caracteres
	COSTO: numero real
	STOCK: numero entero (cantidad de envases almacenados)
	MES,ANIO: numeros enteros: (mes y anio de vencimiento de la medicacion)

El archivo contiene un registro por cada lote, es decir que pueden existir varios registros para un mismo
medicamento si estos pertenecen a lotes distintos. Se solicita desarrollar
uno o mas programas para:
1) Crear el archivo, ingresando los datos por teclado
2) Borrar del archivo los registros de aquellos lotes que se encuentren vencidos,
comparando la fecha de vencimiento de cada uno con una fecha testigo que se ingresa
por teclado. Para borrar los registros es necesario generar un nuevo archivo que contenga
solamente los lotes no vencidos, emitiendo ademas un listado por pantalla
con los registros eliminados.
3) Generar e imprimir un listado con el codigo, el nombre y la cantidad de unidades en stock
para cada medicamento, sin importar el lote al que pertenezca. El archivo no esta ordenado, y
los medicamentos son a lo sumo 130. */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define TAM 130
#pragma warning (disable:4996)


struct sfecha{
	int mes,anio;
};

struct smedicina{
	char lote[5];
	char codigo[21];
	char nombre[40];
	float costo;
	int stock;
	struct sfecha vencimiento;
};

int main(){
	struct smedicina medicinas;
	struct sfecha fechaVencida;
	int contador=0;
	char rta='s';
	int fechaMed, fechaTest;
	FILE *f,*nv,*v;

	f=fopen("medicinas.dat","wb+");
	if(f==NULL)
	{
		puts("No se puede crear el archivo");
		return 1;
	}


	while(rta=='s'&&contador<=TAM)
	{
		printf("Ingrese el lote: ");
		gets(medicinas.lote);
		fflush(stdin);
		printf("Ingrese el codigo: ");
		gets(medicinas.codigo);
		fflush(stdin);
		printf("Ingrese el nombre: ");
		gets(medicinas.nombre);
		fflush(stdin);
		printf("Ingrese el costo: ");
		scanf("%f",&medicinas.costo);
		fflush(stdin);
		printf("Ingrese el stock: ");
		scanf("%d",&medicinas.stock);
		printf("Ingrese el mes de vencimiento: ");
		scanf("%d",&medicinas.vencimiento.mes);
		printf("Ingrese el anio de vencimiento: ");
		scanf("%d",&medicinas.vencimiento.anio);
		fflush(stdin);
		fwrite(&medicinas,sizeof(medicinas),1,f);
		contador++;

		printf("Desea continuar? s/n: ");
		scanf("%c",&rta);
		fflush(stdin);
		while(rta!='s'&&rta!='n')
		{
			printf("Desea continuar? s/n: ");
			scanf("%c",&rta);
			fflush(stdin);
		}
		system("cls");
	}

	if(contador==TAM)
		printf("No hay mas lugar...");

	printf("Ingrese el mes vencido: ");
	scanf("%d",&fechaVencida.mes);
	printf("Ingrese el anio vencido: ");
	scanf("%d",&fechaVencida.anio);

	nv=fopen("novencidos.dat","wb+");
	if(nv==NULL)
	{
		puts("No se puede abrir NV");
		fclose(f);
		return 1;
	}
	v=fopen("vencidos.dat","wb+");
	if(v==NULL)
	{
		puts("No se puede abrir V");
		fclose(f);
		fclose(nv);
		return 1;
	}

	rewind(f);
	
	fread(&medicinas,sizeof(medicinas),1,f);
	while(!feof(f))
	{
		if(strlen(medicinas.lote)!=0)
		{
			fechaMed=(medicinas.vencimiento.anio*100)+medicinas.vencimiento.mes;
			fechaTest=(fechaVencida.anio*100)+fechaVencida.mes;
			if(fechaMed>fechaTest)
			{
				fwrite(&medicinas,sizeof(medicinas),1,nv);
			}
			if(fechaMed<=fechaTest)
			{
				fwrite(&medicinas,sizeof(medicinas),1,v);
			}
		}
		fread(&medicinas,sizeof(medicinas),1,f);
	}

	fclose(f);

	rewind(v);
	
	printf("\n-------------- MEDICAMENTOS VENCIDOS Y ELIMINADOS ---------------------\n");
	printf("CODIGO -------------- DESCRIPCION ------------------------------ STOCK\n");
	fread(&medicinas,sizeof(medicinas),1,v);
	while(!feof(v))
	{
		printf("%-21s %-40s %4d\n",medicinas.codigo,medicinas.nombre,medicinas.stock);
		fread(&medicinas,sizeof(medicinas),1,v);
	}

	fclose(v);

	rewind(nv);
	
	printf("\n---------------- MEDICAMENTOS NO VENCIDOS -----------------------------\n");
	printf("CODIGO -------------- DESCRIPCION ------------------------------ STOCK\n");
	fread(&medicinas,sizeof(medicinas),1,nv);
	while(!feof(nv)){
		printf("%-21s %-40s %4d\n",medicinas.codigo,medicinas.nombre,medicinas.stock);
		fread(&medicinas,sizeof(medicinas),1,nv);
	}

	fclose(nv);

	remove("medicinas.dat");
	rename("novencidos.dat","medicinas.dat");

	return 0;
}
