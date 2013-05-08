/*
SURCOS - A program to simulate furrow fertigation.

Copyright 2011-2013, Javier Burguete Tolosa.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 
1. Redistributions of source code must retain the above copyright notice, this
	list of conditions and the following disclaimer.
 
2. Redistributions in binary form must reproduce the above copyright notice,
	this list of conditions and the following disclaimer in the documentation
	and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY Javier Burguete Tolosa ``AS IS'' AND ANY EXPRESS
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL Javier Burguete Tolosa OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
calibrate.c: source file to define the calibration

Copyright 2011-2013, Javier Burguete Tolosa
*/

#include "calibrate.h"

/*
void calibrate_read(Calibrate *c, char *dir)
{
	char buffer[512];
	FILE *file;
	snprintf(buffer, 512, "%s/calibrate.in", dir);
	file = g_fopen(buffer, "r");
	fscanf(file, "%d%d", &c->n, &c->tipe);
	fclose(file);
}

void data_entrada(int i)
{
	char nombre[32];
	FILE *file;
	snprintf(nombre,32,"%dentrada.in",i);
	file=g_fopen(nombre,"r");
	fscanf(file,FRL FRL,&tc,&pe);
	fclose(file);
}

void data_tiempo(int i)
{
	char nombre[32];
	FILE *file;
	snprintf(nombre,32,"%dtiempo.in",i);
	file=g_fopen(nombre,"r");
	fscanf(file,FRL FRL,&tf,&cfl);
	fclose(file);
}

void experimento_leer(Experimento *e,char *nombre)
{
	int i;
	FILE *file;
	file=g_fopen(nombre,"r");
	fscanf(file,"%d",&e->n);
	e->t = (FLOAT*)g_realloc(e->t, 2 * sizeof(FLOAT) * e->n);
	e->p = e->t + e->n;
	for (i=0; i<e->n; ++i) fscanf(file,FRF FRF,e->t+i,e->p+i);
	fclose(file);
}

void data_experimento(int i)
{
	char nombre[32];
	snprintf(nombre,32,"%davance.in",i);
	experimento_leer(avance,nombre);
	switch (tipo_ensayos)
	{
	case 1:
		snprintf(nombre,32,"%darriba.in",i);
		experimento_leer(calado,nombre);
		break;
	case 2:
		snprintf(nombre,32,"%dreceso.in",i);
		experimento_leer(receso,nombre);
	}
}

DOUBLE interpola_matriz(DOUBLE x,FLOAT *xi,FLOAT *yi,int n)
{
	register int i;
	if (x<=xi[0]) return yi[0];
	if (x>=xi[n-1]) return yi[n-1];
	i=jbm_farray_search(x,xi,n-1);
	return jbm_extrapolate(x,xi[i],xi[i+1],yi[i],yi[i+1]);
}

DOUBLE compara(DOUBLE t,DOUBLE xa,DOUBLE xr,Parametros *p)
{
	DOUBLE x,y;
	x = interpola_matriz(t, avance->t, avance->p, avance->n);
	switch (tipo_ensayos)
	{
	case 0:
		return jbm_fsqr(x-xa);
	case 1:
		y = interpola_matriz(t, calado->t, calado->p, calado->n);
		return fabs(p->h - y) * (xa + x) + fabs(xa - x) * (p->h + y);
	}
	y = interpola_matriz(t, receso->t, receso->p, receso->n);
	return jbm_fsqr(x-xa)+FACTOR_RECESO*jbm_fsqr(y-xr);
}

DOUBLE simula(FILE *file)
{
	int i;
	DOUBLE e,xa,xr;
	malla();
	parametros_malla(p,n);
	flujo_parametros(p,n);
	for (t=0, i=0, e=0.; t<tf; ++i)
	{
		dtmax_funcion(p,n);
		if (t+dtmax>tf)
		{
			dt=tf-t;
			t=tf;
		}
		else
		{
			dt=dtmax;
			t=t+dt;
		}
		paso();
		guardar_avance(file,p,n,&xa,&xr);
		e+=compara(t,xa,xr,p);
	}
	return e/i;
}

void minimizar(FLOAT *e,int *nmin,int k,int n)
{
	register int i,j;
	for (i=0; i<n; ++i) if (e[k]<e[nmin[i]])
	{
		for (j=9; j>i; --j) nmin[j]=nmin[j-1];
		nmin[i]=k;
		return;
	}
}

void intervalo(int *p,int *nmin,int *imin,int *imax)
{
	register int i,j;
	for (i=0, *imin=*imax=p[nmin[0]]; ++i<10;)
	{
		j=p[nmin[i]];
		if (j > *imax) *imax = j; else if (j < *imin) *imin = j;
	}
}

#define INTERPOLA(p1,p2,p3,i,n) if (n==0) p1=p2; else p1=p2+i*(p3-p2)/n;

void calibra(int o)
{
	register DOUBLE k,k2;
	int i,j;
	int i1[o],i2[o],i3[o],i4[o];
	int nmin[10]={0,1,2,3,4,5,6,7,8,9};
	FLOAT e[o];
	FILE *file;
	file=g_fopen("calibra.out","r");
	for (i=0; i<o; ++i) fscanf(file,FRF"%d%d%d%d",e+i,i1+i,i2+i,i3+i,i4+i);
	fclose(file);
	for (i=0; ++i<10;) minimizar(e,nmin,i,i);
	for (; i<o; ++i) minimizar(e,nmin,i,10);
	printf("OPTIMO e="FWF"\n",e[nmin[0]]);
	INTERPOLA(k,sb->n,sc->n,i1[nmin[0]],n1);
	printf("n="FWL"\n",k);
	INTERPOLA(k,sb->i1,sc->i1,i2[nmin[0]],n2);
	printf("alfa="FWL"\n",k);
	INTERPOLA(k,sb->i2,sc->i2,i3[nmin[0]],n3);
	printf("beta="FWL"\n",k);
	INTERPOLA(k,sb->i3,sc->i3,i4[nmin[0]],n4);
	printf("gamma="FWL"\n",k);
	printf("INTERVALO emin="FWF" emax="FWF"\n",e[nmin[0]],e[nmin[9]]);
	intervalo(i1,nmin,&i,&j);
	INTERPOLA(k,sb->n,sc->n,i,n1);
	INTERPOLA(k2,sb->n,sc->n,j,n1);
	printf("n="FWL"-"FWL"\n",k,k2);
	intervalo(i2,nmin,&i,&j);
	INTERPOLA(k,sb->i1,sc->i1,i,n2);
	INTERPOLA(k2,sb->i1,sc->i1,j,n2);
	printf("alfa="FWL"-"FWL"\n",k,k2);
	intervalo(i3,nmin,&i,&j);
	INTERPOLA(k,sb->i2,sc->i2,i,n3);
	INTERPOLA(k2,sb->i2,sc->i2,j,n3);
	printf("beta="FWL"-"FWL"\n",k,k2);
	intervalo(i4,nmin,&i,&j);
	INTERPOLA(k,sb->i3,sc->i3,i,n4);
	INTERPOLA(k2,sb->i3,sc->i3,j,n4);
	printf("gamma="FWL"-"FWL"\n",k,k2);
}

int main()
{
	int i,j,k,l,m,o;
	DOUBLE e,e2,ek,el;
	char nombre[32];
	FILE *file,*file2;
	avance->t = calado->t = receso->t = 0;
	data_terreno();
	printf("Terreno\n");
	data_malla();
	printf("Malla\n");
	data_ensayos();
	printf("Ensayos\n");
	file2=g_fopen("calibra.out","w");
	for (i=0, o=0; i<=n1; ++i)
	{
		INTERPOLA(sa->n,sb->n,sc->n,i,n1);
		for (j=0; j<=n2; ++j)
		{
			INTERPOLA(sa->i1,sb->i1,sc->i1,j,n2);
			for (k=0; k<=n3; ++k)
			{
				INTERPOLA(sa->i2,sb->i2,sc->i2,k,n3);
				for (l=0; l<=n4; ++l)
				{
					INTERPOLA(sa->i3,sb->i3,sc->i3,l,n4);
					for (m=0, e=0.; ++m<=n_ensayos; e+=e2*e2)
					{
						data_entrada(m);
						data_tiempo(m);
						data_experimento(m);
						snprintf(nombre,32,"%d-%d-%d-%d-%d.out",i,j,k,l,m);
						file=g_fopen(nombre,"w");
						e2=simula(file);
						fclose(file);
					}
					++o;
					e=sqrt(e);
					fprintf(file2,FWL" %d %d %d %d\n",e,i,j,k,l);
					printf(FWL" %d%d%d%d\n",e,i,j,k,l);
					if (l>0 && e>el) break;
					el=e;
				}
				if (k>0 && e>ek) break;
				ek=e;
			}
		}
	}
	fclose(file2);
	calibra(o);
	return 0;
}
*/
