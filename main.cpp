
#define IMPRIME        1
#define QNT_VARIAVEIS  10
#define QNT_RESTRICOES 10
#define RAND           1
#define VALUE          2 // if RAND 0

#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>             /* GNU GLPK linear/mixed integer solver */
#include <math.h>
#include <iostream>
#include <string.h>
#include <time.h>

using namespace std;

void exibir(glp_prob* lp);
void montarRestricoes(glp_prob *lp,int *ia, int *ja, double *ar,int limit_i,int limit_s);

int main(void)
{
    clock_t inicio;

    glp_prob *lp;
    int* ia = new int[1000000];
    int* ja = new int[1000000];
    double* ar = new double[1000000];

    /* create problem */
    lp = glp_create_prob();
    glp_set_obj_dir(lp, GLP_MAX);

    if(IMPRIME) cout << "Max Z = ";
    glp_add_cols(lp, QNT_VARIAVEIS);        // cada coluna é uma variavel
    for(int i=1; i<=QNT_VARIAVEIS; i++)     // percorre as variaveis
    {
        glp_set_col_bnds(lp, i, GLP_LO, 0.0, 0.0); // na variavel i restrição de >= à 0
        glp_set_obj_coef(lp, i, i);                // definindo o coeficiente(constante) da variavel i
        if(IMPRIME) cout << glp_get_obj_coef(lp,i) << "(x" << i << ") ";
    }
    if(IMPRIME) cout << "\n";
    /* fill problem */
    glp_add_rows(lp, QNT_RESTRICOES); //adiciona a quantidade de restrições

    srand((unsigned)time(0));

    montarRestricoes(lp,ia,ja,ar,2,4);

    inicio = clock();

    glp_load_matrix(lp, QNT_VARIAVEIS*QNT_RESTRICOES, ia, ja, ar);

    /* solve problem */
    glp_simplex(lp, NULL);

    clock_t total = clock()-inicio;

    exibir(lp);

    cout << "tempo: " << double(total)/CLOCKS_PER_SEC << endl;

    return 0;
}

void exibir(glp_prob* lp)
{
    double *x = new double[QNT_VARIAVEIS];
    int z=0;
    z = glp_get_obj_val(lp);
    for(int i=1; i<=QNT_VARIAVEIS; i++)
    {
        x[i] = glp_get_col_prim(lp, i);
        if(x[i] != 0) cout << "x" << i << ": " << x[i] << endl;
    }
    printf("z: %d\n",z);
}

void montarRestricoes(glp_prob *lp,int *ia, int *ja, double *ar,int limit_i,int limit_s)
{
    int range=(limit_s-limit_i)+1;
    int cont=1;
    for(int i=1; i<=QNT_RESTRICOES; i++ )
    {
        glp_set_row_bnds(lp, i, GLP_UP, 0.0, 4000.0); //restrição na linha i
        for(int j=1; j<=QNT_VARIAVEIS; j++)
        {
            ia[cont] = i;
            ja[cont] = j;
            if(RAND) ar[cont] = limit_i+(rand()%range);//randomizar
            else ar[cont] = VALUE;
            if(IMPRIME) cout << " " << ar[cont] << "(x"<< j << ") ";
//            cout << "ia: " << ia[cont] << endl;
            cont++;
        }
        if(IMPRIME) cout << "<= 4000" << endl;
    }
}
