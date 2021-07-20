/*
 * Light Matrix: C code implementation for basic matrix operation
 *
 * Copyright (C) 2017 Jiachi Zou
 *
 * This code is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with code.  If not, see <http:#www.gnu.org/licenses/>.
 */
#include "matrix.h"
#include "stdio.h"
#include "stdlib.h"


#define MAT_LEGAL_CHECKING
#define MAT_INIT_FLAG   0x5C
#define Min(x,y) ((x>y)?(x):(y))
/************************************************************************/
/*                          Private Function                            */
/************************************************************************/

void matrix_swap(int *a, int *b)
{
  int m;
  m = *a;
  *a = *b;
  *b = m;
}

void matrix_perm(int list[], int k, int m, int* p, Mat* mat, float* det)
{
  int i;

  if(k > m)
  {
    float res = mat->element[0][list[0]];

    for(i = 1; i < mat->row ; i++){
        res *= mat->element[i][list[i]];
    }
    if(*p%2){
        //odd is negative
        *det -= res;
    }else{
        //even is positive
        *det += res;
    }
  }
  else
  {
    matrix_perm(list, k + 1, m, p, mat, det);
    for(i = k+1; i <= m; i++)
    {
      matrix_swap(&list[k], &list[i]);
      *p += 1;
      matrix_perm(list, k + 1, m, p, mat, det);
      matrix_swap(&list[k], &list[i]);
      *p -= 1; 
    }
  }
}

/************************************************************************/
/*                           Public Function                            */
/************************************************************************/

Mat* MatCreate(Mat* mat, int row, int col)
{
    int i;

#ifdef MAT_LEGAL_CHECKING
    if(mat->init == MAT_INIT_FLAG){
        if(mat->row == row && mat->col == col)
            return mat;
        else
            MatDelete(mat);
    }
#endif

    mat->element = (float**)malloc(row * sizeof(float*));
    for(i = 0 ; i < row ; i++){
        mat->element[i] = (float*)malloc(col * sizeof(float));  
    }

    if(mat->element == NULL){
        return NULL;
    }
    mat->row = row;
    mat->col = col;
    mat->init = MAT_INIT_FLAG;

    return mat;
}

void MatDelete(Mat* mat)
{
    int i;

#ifdef MAT_LEGAL_CHECKING
    if(mat->init != MAT_INIT_FLAG){
        return ;
    }
#endif

    for(i = 0 ; i<mat->row ; i++)
        free(mat->element[i]);
    free(mat->element);

    mat->init = 0;
}

Mat* MatSetVal(Mat* mat, float* val)
{
    int row,col;

    for(row = 0 ; row < mat->row ; row++){
        for(col = 0 ; col < mat->col ; col++){
            mat->element[row][col] = val[col + row * mat->col];
        }
    }

    return mat;
}

void MatDump(const Mat* mat)
{
    int row,col;

#ifdef MAT_LEGAL_CHECKING
    if(mat == NULL){
        return ;
    }
#endif

    printf("Mat %dx%d:\n", mat->row, mat->col);
    for(row = 0 ; row < mat->row ; row++){
        for(col = 0 ; col < mat->col ; col++){
            printf("%.4f\t", mat->element[row][col]);
        }
        printf("\n");
    }
}

Mat* MatZeros(Mat* mat)
{
    int row,col;

#ifdef MAT_LEGAL_CHECKING
    if(mat->init != MAT_INIT_FLAG){
        printf("err check, none init matrix for MatZeros\n");
        return NULL;
    }
#endif

    for(row = 0 ; row < mat->row ; row++){
        for(col = 0 ; col < mat->col ; col++){
            mat->element[row][col] = 0.0f;
        }
    }

    return mat;
}

Mat* MatEye(Mat* mat)
{
    int i;

#ifdef MAT_LEGAL_CHECKING
    if(mat->init != MAT_INIT_FLAG){
        printf("err check, none init matrix for MatEye\n");
        return NULL;
    }
#endif

    for(i = 0 ; i < Min(mat->row, mat->col) ; i++){
        mat->element[i][i] = 1.0f;
    }

    return mat;
}

/* dst = src1 + src2 */
Mat* MatAdd(Mat* src1, Mat* src2, Mat* dst)
{
    int row, col;

#ifdef MAT_LEGAL_CHECKING
    if( !(src1->row == src2->row && src2->row == dst->row && src1->col == src2->col && src2->col == dst->col) ){
        printf("err check, unmatch matrix for MatAdd\n");
        MatDump(src1);
        MatDump(src2);
        MatDump(dst);
        return NULL;
    }
#endif

    for(row = 0 ; row < src1->row ; row++){
        for(col = 0 ; col < src1->col ; col++){
            dst->element[row][col] = src1->element[row][col] + src2->element[row][col];
        }
    }

    return dst;
}

/* dst = src1 - src2 */
Mat* MatSub(Mat* src1, Mat* src2, Mat* dst)
{
    int row, col;

#ifdef MAT_LEGAL_CHECKING
    if( !(src1->row == src2->row && src2->row == dst->row && src1->col == src2->col && src2->col == dst->col) ){
        printf("err check, unmatch matrix for MatSub\n");
        MatDump(src1);
        MatDump(src2);
        MatDump(dst);
        return NULL;
    }
#endif

    for(row = 0 ; row < src1->row ; row++){
        for(col = 0 ; col < src1->col ; col++){
            dst->element[row][col] = src1->element[row][col] - src2->element[row][col];
        }
    }

    return dst;
}

/* dst = src1 * src2 */
Mat* MatMul(Mat* src1, Mat* src2, Mat* dst)
{
    int row, col;
    int i;
    float temp;

#ifdef MAT_LEGAL_CHECKING
    if( src1->col != src2->row || src1->row != dst->row || src2->col != dst->col ){
        printf("err check, unmatch matrix for MatMul\n");
        MatDump(src1);
        MatDump(src2);
        MatDump(dst);
        return NULL;
    }
#endif

    for(row = 0 ; row < dst->row ; row++){
        for(col = 0 ; col < dst->col ; col++){
            temp = 0.0f;
            for(i = 0 ; i < src1->col ; i++){
                temp += src1->element[row][i] * src2->element[i][col];
            }
            dst->element[row][col] = temp;
        }
    }

    return dst;
}

/* dst = src' */
Mat* MatTrans(Mat* src, Mat* dst)
{
    int row, col;

#ifdef MAT_LEGAL_CHECKING
    if( src->row != dst->col || src->col != dst->row ){
        printf("err check, unmatch matrix for MatTranspose\n");
        MatDump(src);
        MatDump(dst);
        return NULL;
    }
#endif

    for(row = 0 ; row < dst->row ; row++){
        for(col = 0 ; col < dst->col ; col++){
            dst->element[row][col] = src->element[col][row];
        }
    }

    return dst;
}

// return det(mat)
float MatDet(Mat* mat)
{
    float det = 0.0f;
    int plarity = 0;
    int *list;
    int i;

#ifdef MAT_LEGAL_CHECKING
    if( mat->row != mat->col){
        printf("err check, not a square matrix for MatDetermine\n");
        MatDump(mat);
        return 0.0f;
    }
#endif

    list = (int*)malloc(sizeof(int)*mat->col);
    for(i = 0 ; i < mat->col ; i++)
        list[i] = i;

    matrix_perm(list, 0, mat->row-1, &plarity, mat, &det);
    free(list);

    return det;
}

// dst = adj(src)
Mat* MatAdj(Mat* src, Mat* dst)
{
    Mat smat;
    int row, col;
    int i,j,r,c;
    float det;

#ifdef MAT_LEGAL_CHECKING
    if( src->row != src->col || src->row != dst->row || src->col != dst->col){
        printf("err check, not a square matrix for MatAdj\n");
        MatDump(src);
        MatDump(dst);
        return NULL;
    }
#endif

    MatCreate(&smat, src->row-1, src->col-1);

    for(row = 0 ; row < src->row ; row++){
        for(col = 0 ; col < src->col ; col++){
            r = 0;
            for(i = 0 ; i < src->row ; i++){
                if(i == row)
                    continue;
                c = 0;
                for(j = 0; j < src->col ; j++){
                    if(j == col)
                        continue;
                    smat.element[r][c] = src->element[i][j];
                    c++;
                }
                r++;
            }
            det = MatDet(&smat);
            if((row+col)%2)
                det = -det;
            dst->element[col][row] = det;
        }
    }

    MatDelete(&smat);

    return dst;
}

// dst = src^(-1)
Mat* MatInv(Mat* src, Mat* dst)
{
    Mat adj_mat;
    float det;
    int row, col;

#ifdef MAT_LEGAL_CHECKING
    if( src->row != src->col || src->row != dst->row || src->col != dst->col){
        printf("err check, not a square matrix for MatInv\n");
        MatDump(src);
        MatDump(dst);
        return NULL;
    }
#endif

    MatCreate(&adj_mat, src->row, src->col);
    MatAdj(src, &adj_mat);
    det = MatDet(src);

    for(row = 0 ; row < src->row ; row++){
        for(col = 0 ; col < src->col ; col++)
            dst->element[row][col] = adj_mat.element[row][col]/det;
    }

    MatDelete(&adj_mat);

    return dst;
}

/*

[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[O@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@OO@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@OOO@@@/[\OOOOOO@@@@@@@@/[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
............................................................................................       .......]@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@OooOOOOO@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@OOOOOOOOOOOOOOO@@@@@@@OOOOOOooOOOOO@@@@@@@]`.......      ..............................................................................................................................
   ........                                                                                  .........,/@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@OOooOOooooOOOO@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@OO@@@@@@@@@OOOOOOOOOOOOOO@@@@@@@@OOOO@@@@@@@@@@@@@`......                                                                                                                                  
  ........                                                                                   ......]@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@OOoOOO@@@OOoooooooOOOOOooOO@@@@@@@@@@@@@@@@@@@@@@@@@OOOO@@@@@@OOOO@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@OOOO@@@@@@@@@@@@@@@@@@@@OOOOOOOOOOOOO@@@@OOO@@@@@@@@@@@@@\.......                                                                                                                              
  ......                                                                             ...........]@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@OOOO@@@@@@@@@OooooooooooOO@@@@@@@@@@@@@@@@@@@OOOOOOOO@@@OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@@@@@@@@@@@@@@@OO@@@@@@@@@@@@@@@@@@@@@@@@@@@@@OOOOOOOOOOO@@@@O@@@@@@@@@@@@@@\`...........                                                                                                                      
.........                                                                           .........,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@OO@@@@@@@@@@@@@OooooooOO@@@@@@@@@@@@@@@@OOOOOOOOO@@OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@@@@@@OOOOOO@@@@@@@@@O@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@OOOOOOOOOO@@@@@@@@@@@@@@@@@@@`... .....                                                                                                                     
. .......                                                                           ......]@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@Ooo\oOO@@@@@@@@@@@@@OOOOOOOOOOO@OOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@@@@@@@@@@@@@@@@OOOO@@@@@O@@@@@@OOOOO@@@@@@@@@@@@@@@@@@@@@@@@@@@@@OOOOOOOOOOO@@@@@@@@@@@@@@@@@\`......                                                                                                                     
.........                                                                       .......,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@OOOOOOOO`/O@@@@@@@@@@@@OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@OOOOO@@@@@@@@@@@@@@@@@@@@@@@OOOOOOO@@@OOOOOOOOOOOOOOO@@@@OoOO@@@@@@@@@@OoooOO@OOOOOOOOOOO@@@@@@@@@@@@@@@@@\.....                                                                                                                    
.......                                                                 .............@@@@@@@@@@@@O@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@OOOOOOOOOOOOOOO@@@@@@@@@OOOOOOOOOO@OOoooooOOOOOOOOOOOOOOOOOOOOO@@OOOOO@@@@@@@@@@@@@@OOOOOOOOOOOOOOOOOOOOOOOOOOOO@OOOOOOOOOOO@@OOooOO@@@@@OoooooO@@@@OOOOOOOOOOO@@@@@@@@@@@@O@@@@`.......... .                                                                                                          
.......                                                                     ....../@@@@@@@@OOOO@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@OOOOOOOOOOOOOOOOOOO@@@@@@@OOOoOOOOOOOOOoooooOOOooOOOOOOOOOOOOOOOO@@OOOOOO@@@@@@@@@@@@@OOOOOOOOOOOOO@@OOOOOOOOOOOOOO@@@O@@@@@@OOOOOOOOOOOoooooooooooO@@@@@@@OOOOOOOOOOOO@@@@@@@@@@@@O@@@`........ .                                                                                                          
                                                                     .. .......,@@@@@@OOOoooOOOO@@@@@@@@@@@@@@OO@@@@@@@@@OOOOOOOOOOOOOOOOOOO@OOO@@@@@OOO`*oOOOOOOOoooooOOooooooooooOOOOOOoOO@@@OOOOOOOOO@@@@@@@@@OOOOOOOOOOOOO@@@@OOOOOOOOOOOOOOOO@@@@@@O@@@@@@OOOOOOO@OOooooooooO@@@@@@@@@@OOOOOOOOOOO@@@@@@@@@@@@@@@@@.......                                                                                                           
                                                                     ..  ..../@@@@@OOoOoOOoOOOOOOOOOOOOOOO@@@@@@@@@@@OOOOOOOOOOOOOOOOOOOO@@@OO@@@@@OO.*,oOOOOOOoooooOOOoooooooooooooooooO@@@@OOOOOOOOO@@@@@@@@@@OOOOO`,`,OOO@@@@@OOOOOOOOOOOOOOO@@@@@@@@@@@@@@@@@@OOOOOOO@OOooooOO@@@@@@@@@@@@OOOOOOOOOOOO@@@@@@@@@@@@@@@@`.....                                                                                                          
                                                                   ... ...,/@@@@OooOOooOoOOOOOOOOOooOOooOOOOOOO@OOOOOOOOOOOOOOOOOOOOOOO@@@OO@@@@@OO`*,oOOOOOOOOOOOO@@@OOoooooooooooooOO@@@OOOOOOOOOOO@@@@@@@@@OOOOOOo*O/..*O@@@@@@@@@@@@OOOO@@@@@@@@@@@@@@@@@@@@@@@@@OOOOOO@@OoooOO@@@@@@@@@@@@@@OOOOOOOOOOOO@@@@@@@@@@@@@@@`.........                                                                                                    
                                                              ..........]@@@@OoOOoooooOOOOOOOOOoOoooooooooOOOo[*[\oOOOOOOOOOOOOOOOOOO@@@@OO@@@@OO`**oOOOOOOO@O@@@@@@@@@@@OoooooooooO@@@@OOOOOOOOOooO@@@@@@@@@OOOOo.****.=OOOO@@@@@@@@@@@@@@@@@@@@@@@@@@OO@@@@@@@@@@@@@@@OOOOOO@@OooOO@@@@@@@@@@@@@@@OOOOOOOOOOO@@@@@@@@@@O`[@@`.........                                                                                                  
                                                            ........../@@@OoooooOooooOOOOOOOOooooooooooooo.************[oOOOOOOOOOO@@@@OO@@@@@O^*.*OOOOOOO@@@@@@@@@@@@@@@@OOooooOO@@@@OOOOOOOOOOoooOOOoO@@@OOOOOOO..***ooO@@OO@@O@@@@@@@@@@@@@@@@@@@@@@OOOOO@@@@@@@@@@@@@@OOOOOO@@OOoOO@@@@@@@@@@@@@@@OOOOOOOOOOO@@@@@O[.*O@@@@@`........                                                                                                 
                                                             ......./@@OOoooOooooooOOOOOOOOoOoooooooooo[********************[[oOO@@@@@O@@@@@O.**,OOO@OO@@@@@@@@@@@@@@@@@@@@@OoOOO@@@OOOOOOOOOOOoooooooO@@OOOOOOOOOOOOOO@OOOOO@@OOO@@@@@@@@@@@@@@@@@OO@OOOOOOOOO@@@@@@@@@@@@@@OOOOOO@@OoOO@@@@@@@@@@@@@@@OOOOOOOOOOOO[.*O@@@@@@@OO\`......                                                                                                 
                                                    .... ........,@@@OoooOoOOooooOOOOOOOoooooooooooo[***********************,/OOOOOOOO@@@@Oo***oO@@@O@@@@@@@@@@@@@@@@@@@@@@@OOO@@@OOOOOOOOOOOoOOOOOooOOOOOOOOOOoOOOO@OOOO@@@O@@OOO@@@@@@OOOOOOOoooOOOOOOOOOOOOOOO@@@@@@@@@@@@@@OOOOOO@@OOO@@@@@@@@OO@@@@@@OOOOOOO*.*OO@@@@@@@@@@O*O@^...........                                                                                          
                                                     ..  ......,@@@ooooooooooooOOOOOOOooooooooooo`************************.*OOOOOOOO@O\OO/`**=OO@@@@@@@@@@@@@@@@@@@@@@@@@@@@O@@@OOOOOOOOOOOOOO@@@@@@@@OOOooOOOOOOOO@@@@@@@@OOO@OOOOOOOoOOOooOOOOoOOOOoOOOOOOOOOOOOOO@@@@@@@@@@@@@OOOOOO@@OO@@@@@@@@OOOO@@@@@OOOooOOOOO@@@@@@@@@@O\*=@@\....... .                                                                            .........     
                                                      .......,@@..*OOoooooOooOOOOOOOoooooooooo[*************************.*OOOOOOOOO@`.=[*.**,OOO@@@@@@@@@@@@@@@@@@@@@@@@@@OO@@OOOOOOOOOOOOOOO@@@@@@@@OOOOooooOOOO@@@@@@@@@OOOO@OOoOOoOoOoOOoOoOOoOOOOOOOOOOOOOOOOOOOO@@@OO@@@@@@@@@OOOOOOO@@@@@@@@@@@OOOO@@@@@OOOOOOOOOO@@@@@@@@@O*=O@@@\..... .                                                                            .........     
                                                     ......,@@/.,oooooooo.*oOOOOOOoOooooooo.***********..**************oOOOOOOOOO@^..****..*OOOOOOOOO@@@@@@@@@@@@@@@@@@@OO@@OOOOOOOOOOOOOoOO@@@@@@@OOOOOOoooOOOO@@@@@@@@@OOOOO@@OOOOOOOOOOOoOOOOOOOOOOOOOOOOOOOOOOO@OOOO@@OOOOO@@@@@@OOOOOOO@@@@@@@@@@OOOOOO@@@@OOOOOOOOOO@@@@@@@O*.O@@@@@\......                                                                         ............    
                                                 ........,@@/../oOoooo/`*=OOOOOOooooooooo[*********.*.****.**********=OOOOOOOOO@@`...**..,OOOooOOOOOOOOOOOO@@@@@@@@@@@OO@@OOOOOOOOOOOO@OOOO@@@@@@@OoOOOOOOOOOOO@@O@@@@@@@OOOOO@@OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@OOOO@OOOOOO@@@@@@OOOOOOO@@@@@@@@@OOOOOOO@@@@OOOOOOOOOO@@@@@O^.=@@@@@@@\.........                                                      .   .............. . .....    
                                                ........@@/`..oooooo[*.*OOOOOoOooooooo/`*****...............********oOOOOOOOOO@O....*.*.*OOo`*=oOOOOOOOOOOOOOOOO@@@@OOO@@OOOOOOOOOOOO@OoO@O@@@@@OOoOOOOOOOOOO@@@OOOO@@@@OOOOOOOO[,OOOOOOOOOOO[OOOOOOOOOOOOOOOOOOOOOOOO@@OOOOOOOOOOOO@@@@@OOOOOOO@@@@@@@@@OOOOOOO@@@@OOOOOOOOO@@@@O^..O@@@@@@@@^......                                                         ...............    .....    
                                                 ...../@@...,oooOo****=OOOOOooooooooo*******...............*******=OOOOOOOOO@@^.......*oOo*****oooooOOOOOOOOOOOOOOOOOO@OOOOOOOOOOOO@@OOO@@@@@@@OOooOoOOOOOOO@@@OOOOO@@@OOOOOOOOOOOOOOOOOOOOO`./OOOOOOOOOOOOOOOOOOOOOOOOO@@OOOOOOOOOOOO@@@@@OOOOOOO@@@@@@@@OOOOOOOO@@@@OOOOOOOOO@@@O..=@@@@@@@@@@`..... .                                                  . ............       .......    
                                            ......../@@`...oooo[*****oOOOOOooooooo.*..****.................*****,oOOOOOOOOO@@^....**.*o.**.*****\oooooooOOooOOOOo**oOO[oOOOOOOOOOO@@@OO@@@@@@@OOooOooOOOOOO@@OOOOOOO@@@OOOOOOOO@@OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@OOOOOOOOOOOO@@@@OOOOOOOO@@@@@@@OOOOOOOOO@@@OOOOOOOOOOOO^.=O@@@@@@@@@@@`.. ..                                                ............           .......    
                                     ..... .......,@@^...*ooo`*.*..,OOOOoooooooo.*..*.....................*****,OOOOOOOOOO@@.....****.**..*******,[oooooooOOOOo`*,oO.*****,\oOOOO@@@OO@@@@@@@OO/=oOooOOOOO@@OOOOOOO@@@OOOOOOOOO@@OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@OOOOOOOOOOOOO@@@@OOOOOOOO@@@@@@OOOOOOOOOOO@@OOOOOOOOOO^..O@@@@@@@@@@@@\....... .                               .......... ....                 ........  .  .
                                       .   . ...,@@/..../o.**.**..*OOOooooooooo`...**....................****.*OOOOOOOOOO@@.......******..***************oOOo`**,Oo`***********OOOOOO@@@@@@@OOo=oooOOOOOO@@OOOOOOO@@@OOOOOOOOOOO@OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@OOOOOOOOOOOO@@@@@OOOOOOOO@@@@@OOOOOOOOOOOO@@OOOOOOOO^..O@@@@@@@@@@@@@@\.......                               ..........   ..               ........  .   ..
                                         ....../@@`..../[*.**.**,OOOooooooooo[*.........................****,OOOOOOOoOO@@/.....*..****...***************=O/`**.*Oo***********.=OOoOOOOOO@@@@OO\ooooOOOOO@@OOOOOOOO@@/oOOOOOOOOOOOOOOOOO\oOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@OOOOOOOOOOOOO@@@@OOOOOOOO@@@@@OOOOOOOOOOOO@@OOOOOOOo..=@@@@O@@@@@@@@@@@`..... ..                 . .....  ..........                    ..................
                                     ..  ....,@@`.....*****..*.*OOOOOoooooo.*..........................****=OOOOOOOoOO@@^......**.***..*.***************,*****=o[**********..=OOOOOOooOOOO@OO^oooooOOOO@@OOOOOOOOO@/=OOOOOOOOOOOOOOOOOOOooOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@OOOOOOOOOOOOO@@@@OOOOOOOO@@@@OOOOOOOOOOOOO@@OOOOOO*..O@@@@@@@@@@@@@@@@@`... ..                 . ..............               ......  .... ..  .........
                                      .  ..,@@/..............*OOOOOooooooo*..........................*****oOOOOOOoOOO@@^... ..**..**...*..*******************=o`*********....OOoooooooooOOO^..,\ooOOOOO@OOOOOOOOO@O./OOOOOOOOOOOOOOOOOOOOoooOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@OOOOOOOOOOOOO@@@@OOOOOOOOO@@@@OOOOOOOOOOOOO@OOOOO^..O@@@@@@@@@@@@@@@@@@\...........  ..    .... ....... .                . ...........   ..   ....*,]]]
                                    ....../@@`. ........**.*,OOOOOoooooo`*...........................****OOOOOOOOOoo@@`........*..**...**********************o`*********....OoOooooooooOOO/......*oOOOOOOOOOOOOOO@.,OOOOOOOOOOOOOOOOOOOOOOo[\OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@OOOOOOOOOOOOOO@@@@OOOOOOOO@@@@OOOOOOOOOOOOOOOOOOO..=O@@@@@@@@@@@@@@@@@@@`........ ....    ....      ..                  .  ....         .. .....*oOOOO
                                    ....,@@/............**.=OOOOOooooo^**..........................****,OOOOOOOOOoO@@`.........*...**..**************..*****`****.****.....ooooooooooOOOOO`.......**[oOOOOOOOOOOO/.OOOOOOOOOOOOOOOOOOOOOOOOo*=OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@OOOOOOOOOOOOO@@@@OOOOOOOOO@@@OOOOOOOOOOOOOO@OOO\.=OOO@@@@@@@O@@@@@@@@@@@.... .. ...                                  ......           .... ...=OOO@@
                                ......./@@`...   .. ....../OOOOoooooo`**..........................****=OOOOOOOOOoO@@`..............**..**************..*********...**...../ooooooooooOOOO^........*****[\OOOOOOOO.=OOOOOOOOOOOOOOOOOOOOOOOOOoo\O@OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@OOOOOOOOOOOOOO@@@@OOOOOOOOO@@@OOOOOOOOOOOOOOOOOOOOOOO@@@@@@@OO@@@@@@@@@@\.......                                  ........            .......=OO@@@
                            .........,@@/..............**oOOOOOoooo\***.....*.....................**.*OOOOOOOOooO@@^.............**...**********.*.....********...**...../ooOooooooooooOO........**********[OOO@^.OOOOOOOOOOOOOOOOOOOOOOOOOOO\ooO@OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@OOOOOOOOOOOOOO@@@@OooOOOOOO@@@OOOOOOOOOOOOOOOOoOOOOOOO@@@@@@@OO@@@@@@@@@@..... .   ..                   ............                 .......=OO@@@
                              ......=@@`....... ......**oOOOOOoooo`***....**.....................**.*OOOOOOOOooo@@^..... . ............********.........*****.........../oooooooooooooOO^........***********OOOO.=OOOOOOOOOOOOOOOOOOOOOOOOOOOo/\oO@OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@OOOOOOOOOOOOOO@@@@Oo\OOOOOO@@@OOOOOOOOOOOOOOOOooOOOOOO@@@@@@@OO@@@@@@@O@@^......   .               ................                 .......=OO@@@
                             ......@@/.... .    .....*=ooOOOoooo/`****.****.....................***oOOOOOOOOOooO@^...............**..*....................***..........oOoooooooooooooOO. .......******.***oOOO^.=oooOOOOOOOOOOOOOOOOOOOOOOOOOo/^oO@OOOOOOOOOOOOOOOOOOOOOOOOOOoOOOOOOOOO@@OOOOOOOOOOOOOOO@@@OooOOOOOOO@@OOOOOOOOOOOOOOOOOooOOOOO@@@@@@@OOO@@O@@@@@@@`...... ..    .............  .......                     .......=OO@@@
                             ....=@@`.....  ..... ..*=ooOOoooooo****..****.....................**,oOOOOOOoooooO@/...... ............**................................ooOOoooooooooooOO... .......****...*,OOO/..*...,oOOOOOOOOOOOOOOOOOOOOOOOOoo*oO@OOOOOOOOOOOOOOOOOOOOOOOOOOoOOOOOOOOO@@OOOOOOOOOOOOOOO@@@O\ooOOOOOO@O@OOOOOOOOOOOOOOOOOoOOOOOO@@@@@@@OO@@O@@@@O@@^..............................                         .......=OOO@@
                     ......  .../@@.... ...  .......=ooOOooooo^***....****....................**,oOOOOOOoOoooO@@. ..    .............*...............................=OOoooOooooOoooOO/. .......******..**oOOO^..*.....*\OOOOOOOOOOOOOOOOOOOOOOoooooOOO/oOOOOOOOOOOOOOOOOOOOOOOOooOOOOOOOO@@OOOOOOOOOOOOOOOO@@Oo[oOOOOOOO@@OOOOOOOOOOOOOOOOOooOOOOO@@@@@@@OOO@O@@@@O@@\........ .............                                .......*OO@@@
                      .. ....../@/.....   .  ......*,oOooooo.****...****......................*,oOOOOOOOooooO@@... ...  ................................*...........=ooOOoooooOOoOOoOO.........******...*=OOOO............,oOOOOOOOOOOOOOOOOOOOoooooOOO\=oOOOOOOOOOOOOOOOOOOOOOOOooOOOOOOOO@@OOOOOOOOOOOOOOOO@@Oo*oOOOOOOO@@OOOOOOOOOOOOOOOOOooOOOOOO@@@@@@OOO@@@@@@@@@@`....................                               ........*OOO@@
                     ... ....,@@^. ...     .......**oOoooo.*****....***......................*=oOOOoooOoooOO@@^....................................................=OoooooooooooooOOO/...........*.**...*oOOO^..*............,oOOOOOOOOOOOOOOOOOooo=/O@Oo*oOOOOOOOOOOOOOOOOOOOoooo[oOOOOOOOO@@OOOOOOOOOOOOOOOOO@Oo^\OOOOOOOO@OOOOOOOOOOOOOOOOOOooOOOOO@@@@@@OOOO@@@@@@O@@\... .    ..                                       ........*OO@@@
                    .... .../@@`.. ...      .....**oooooo.*****....****......................=oooooooooooooO@^.......*............................................=OoooooooooooooooOO..... ......***...*=OOOO...................[ooOOOOOOOOOOOO@Oo/\^oO@OoooOOoooOOOOOOOOOoooooooo^=oOOOOOOOO@@OOOOOOooOOOOOOOO@@Ooo\oOOOOOOO@OOOOOOOOOOOOOOOOOOo\OOOOO@@@@@O@OOO@@@@@@O@@@`.....  ..                                       ........*oO@@@
                     ....../@/....  ..... ......**ooooo/`******...****......................=ooOoooooooOoOO@/.......o*.... ......................................*oooooooooooooooooO^.............*....*oOOO^......................,\oooooooooOOOo^oooOOOoooOOo\oooooooooooooooooooo\oOOOOOOOO@OOOOOOo^ooOOOOOOO@@OooooOOOOOOO@OOOOOOOOOOOOOOOOOOo=oOOOOO@@@@O@OO@@@@@@@O@@@`........                                       ........*oOO@@
                     .....@@/.....   .. .... ..*.*oooo.****.**....***......................=oooooooooooooo@@`......=`............................................oOOoooooooooooOoooo`....  .......*....=OOOO..........................*\oo/o\*^=OOoo/\oO@OoooOOoooooooooooooooooooooo[oOOOOOOO@@OOOOOOooooOoOOOOOO@OoooooOOOOOOOOOOOOOOOOOOOOOoOOOo[oOOOOO@@@@@@OO@@@@@@@OO@@^.......                                        .......*oOO@@
                    . ..,@@^...     .  . .....**=oooo^*******....***......................=ooooooooooooooO@/......=O......  .  .... ............................oOoooooooooooooooOo^....   .......*...*oOoO/.............................*,``*o]oOo*ooooO@Oo/oOOooooooooooooooooooooooooOOOOoOO@@OOOOOO/oooooooOOOO@Oooo/oOOOOOOOOOOOOOOOOOOOOooOOOoooOOOOO@@@@@@O@@@OO@@@@O@@\......                                          .....*oOO@@
                    ...,@@`....  .. .........**=ooo.*****..*....***......................,ooooooooooooooo@@......=O^....  .. .. .... ..........................=ooooooooooooooooooo... .. ............,OOO@^................................*=oo/oO\oo\oO@@OoooOo=oooooooooooooooooooooooOOOooOO@@OOOOOOooooooooooOO@Oo]/ooOOOOOOOOooooooooOOOOo\oOOoooOOOOO@@@@@O@@@OOO@@OOO@@@......                                         .....*oOO@@
             .........,@@`.....      .......**,oooo*****.*.*....**.......................ooooooooooooOooO@^.....,O^.....          ....................... ....,oOoooooooooooooooOo^..     ............oOOOO..........   ................. .....*`\Oooo[ooO@@o=oOO^o=ooooooooooooooo\/o[oooOOO`=OO@@OOOOOOo[ooooooooOO@OooooooOOOooooooooooooooOOo\OOOo^=oOOOOO@@@O@@@@OOO@@@@OO@@`......                                    ..   ....oOO@@
                 ....,@@`.. .        ......**,ooo,`****..**....***....................../oooo\oooooooooO@O......OO.....              .........................ooooooooooooooooOOo**..       .........*OOoO^......       ..          .    ........*\o*=o*,oO@Oo\oOo/ooooooooooooooooooooooooOO\*\O@@@OO@@@O^*o`\oooooOO@OooooooOOOoooooooooooooooooooOo^*ooOOOOO@@@O@@@OoOO@@@@OO@@^.....                                    ..  .....oOO@@
              ......=@@.... ..      ......***oo*..*****.**.....**......................=ooo\o,oooooooooO@`.....=O^.........  .      .. ....................../oooooooooooooooooO/`....       ........=OOO@^.......                          .......=o`/]=`O@@Oo\oo^=oooooooOOOOOOOOOOOOoo]/oOO^*oO@@@OOOO@@@@o,/o/oooOO@Oo\]oooOOoooooooo*oooooo]=oooOo^*,oOOOOO@@@@@@@OoOO@@@OOO@@^...... .                                ..   ....oOO@@
              .. ..=@@........       . ...*.*o`..*****...*.....*.......................*oo^*\/ooooooooo@/......OO.............      .................... ...,ooOoooooooooooooooo**.... .     .......*=OoOO.........                          . .....o..,,^oO@@o/ooo/oooooOOOOOOOOOOOOOOOOooooOo**oO@@@OOOOOO@@@@\/o[ooOOOOo\oo=oOOooooo[oooooooo[\o*\oooo**oOOOOO@@@@@@@OoO@@O@OOO@@\.....                                  ..  .....=OO@@
            . .....@@`............. .....**,.*...******.*.............................=ooo**,oooOOoooo=@^...../O`.............       .......................ooooooooooooooooooo.*....       ........*oOoO^...... ....                       . . ....o`...*=O@@O/,oo^ooooOOOOOOO@@@@@@@@OOOO/\OO^*,oO@@@OOOOOO/O@@@@OoooOOOOo\o,ooOo\o/`*ooooooooooooo/oooo^*\OOOOO@@@@@@@OoO@@@@@OO@@\..........                             .  .....=OO@@
            .  ...@@`....       ........**.*^...***..***......*......................*ooo**,=oooooooo=@/.....=OO.............        ......................=ooooooooooooooooooo^*....        .......=OOoO^............                       . .....=^.....,O@@O^,]/oooOOOOOOOOOOO@@@@@@@@@O/\Oo**=O@@@@OOOOOO\]/O@@@@OOO@OOo=ooooOo]]**oooo=ooooooo\oooooo]*=OOOOO@@@@@@@OOO@@@@@@O@@@.......                               .  .....=OO@@
            . ...@@`....        . .......**=....******.*.............................=oo***.,ooooooOo=@^....,OO*..//]`**.........   ......................**oooooo\o[oooooooooo^.....         .....*=Oo/@` ..............                    . . ....O`.....,O@@\^\=//OOOOOOOOOOOOOOOOO@@@@@O/oO^**\O@@@@OOOOOO]=\,oO@@@@@@OOo\,`=oooo`ooooooooooooooooooooo\*,OOOOO@@@@@@@OOO@@@@OOO@@\.....                                . . ....=OO@@
            ..../@`... .         ........*.*...**...*.*......*......................,oo***..=ooooooo*O@.....=OO..*ooooo^**......   .......................=*ooooooo/\ooOooooooo/.. ..        ......*=OoO@...`.............                  .. ......=^......OO@O^\\oOOOOOOOOOOOOOOOOOOO@@@@O\oOo`**oO@@@@OOOOOo\o[oooO@@@@@@O\,o\ooo*o,oooooooooooooooooo*oo\*,OOOOO@@@@@OO@OO@@@@@OO@@\.....                               . ......=OO@@
        .... ..=@^.....         ........****...*.*....*............................*oo`**..*ooOoOOoo.@^.....OO^..=oOOOOoo`*............................ .=^*ooooo/^*,oOooOoooOO^......      .......*oO=OO..=OOO^**..........   .        .............,O......,O@@`.,OOOOOOOOOOOOOOOOOOOO@@@@@OooO\**=OO@@@@OOOOOoo/ooOOOOO@@@@@O`[\/oo^/o`o=oo*******oooo=o^,`\**OOOO@@@@@@@@OOO@@@@@OO@@O.....                            ..........=OOO@
        ......=@^.....          ........***....***.................................,o.**...*ooOoooO*=@^..../OO...ooOOOOOoo^**...........................,o.*oooo.***=oooOOoooOO... ..        ......*oo=@^..=OOOOo\**..........          ............,OO^......OO@\..,OOOOOOOOOOOOOOOOOOOO@@@@@ooOO^**\O@@@@@OOOOOo^ooOOOOOOO@@@@@@]`ooooooo*oo**oooooooooo^=ooo\**OOO@@@@@@@@@OOO@@@@@OO@@\....     .................     .....  ....=OOO@
        .....,@/........        ........*,*...*.*...**... ........................*oo***...=oooOOoo.O@.....OOO..*ooOOOOOooo***................... . ....=/.oooo[****=oOOOooOOO^. ...      ........*=o^=@^..OOOOOOOoo\*...............     ........,/OOOO`.....=OO@`..*,ooOOOOOOOOOOOOOOOOO@@@@OoOOo***OO@@@@@OOOOOooooOOOOOOOO@@@@@@Ooo//[,ooooo`/ooooooo\o/oo^oo*,OOO@@@@@@@@@OOo@@@@@OO@@\..... . .................................*oOO@
     ..  ....@@.....   .        ........*=*...**..*...............................=o***...*=ooooOO^.@/..../OO^..*oooooOooooo`**................... ....=o^.ooo******=ooOoOO@OO^......     ........*=o^/@`.,OOOOOOOOOOOo*............. ........,`,OOOOOOOO\.....OO@O...***,OOOOOOOOOOOOOOOO@@@@@OOOO^*.=OO@@@@@OOOOOooooOOOOOOO@@OO@@@@@\/\`/oooo\ooooooooooOO^o\oo*,OO@@@@@@@@@@OOo@@@@OOO@@\.... ...................................*oOO@
    ........O@`.......          ........**.....*..***............................*o^**....*=oooOOo`=@`...OOOO^..*\ooooOooooo`***.......................oo.*oo*******\OoO@@OOOO......    .........**=o^O@..=OOOOOooOOOOoo\]*...................=OOOOOOO[OOOO`...\OO@...******\OOOOOOOOOOOOOO@@@@OoOOO*.*oO@@@@@@OOOOOooooOOOOOOO@@O`\@@@@@Oo=\/[ooooooooOOOO@@O\oooo*,O@@@@@@@@@@@@Oo@@@@OOO@@^.......................................*oOO@
     .. ...=@^....                . ...***...*.....*.............................=\***....*=ooooo/.O@...=@OOO^.**=oooOo.*********.....................*O/.=o`*******\oO@@OooO^...................**,o*O@..=OOOOOooooOOooooo\**...............*oO@OOOo**=OOO\...=OOOO...*******,oOOOOOOOOOOO@@@@OOOOO\**=OO@@@@@@OOOOOO^oOOOOOOOO@@O*=\O@@@@@\\OOOOOOOOOOOOO@@@Oo\`o\**O@@@@@@@@@@@@OO@@@@@OO@@`......................................*oOO@
    .   ...@@.....                  ...**......*..*....  .......................*o^**.....*=ooooo^.@O...=@@OO`.***`=oO^**********.....................oo^.=o*******.*@@OoooOO^... ...............**,`,@@..OOOOOOoooooooooooooo`**........*,ooooo@@ooo***=OOO`..,OOO@...**********\OOOOOOOOOO@@@@OOOOO^**=O@@@@@@@OOOOOOooOOOOOOOO@O^*,ooO@@@@@@@OOOOOOOOOOO@@@@@O/oo^*=@@@@@@@@@@@@@OO@@@OO@O@@`................................ ....*oOO@
      ....=@`...                    ...**....*.*.**.............................=^***.....*=oooOo^=@^...=@@Oo`*****=oo****.******....................=oo`.\*******,@@@OooooOO^.............*o^..*****=@O.=OOOOOOooooooooooooooooo]*****,ooooooooo@O,o****oOO^...OOO@^..*******..***,oOOOOOOOO@@@OOOOOo***OO@@@@@@@OOOOOOoOOOOOOOOO@O`*oooOO@@@@@@OOOO@@@@@O@@@@@@O\,o^*@@@@O@@@@@@@@@OO@@@@OOO@@................................ ....*oOO@
     ....=@^....                     ..**.....*.*.*.............    ...........*o***.....**=ooOOo.=@^...O@@oo^*****=oo**.....****. ..................oo^..*******O@@@OoOoOoOO.............*oO.....***=@O.=OOOOOOOooooooooooooooooooooooooooooooo`O@\******OOO...=OOOO...*..*........**=OOOOOO@@@@@OOOO^**\O@@@@@@@@OOOOOOoOOOOOOOO@@O**=oOOOoO@@@@@@O@@@@@@OOO@@@@@o/o*@@@@OO@@@@@@@@@OO@@OOOO@@\............................... .....*OO@
     ....@@....                     ...**........*......o.....       ..........=.***.....**=o`=o^.=@...=O@@o[*****=oo^.........*................ ...=oo^..*****]@@@\ooOooooOO.. ........*ooOO....****=@O.OOOOOO@Oo/\************,[[[o/[\/[[******=@O******=OO^..=OOO@^..*..............*,oOOOO@@@@OOOOo*.,oO@@@@@@@@OOOOOOOOOOOOOOO@OO**,oOOOO/O@@@@@@@@@@@@OOOO@@@@O][@@@@OOO@@@@@@@@@OO@@@OOO@@\...................................*=OO@
    . ..=@^...                  .   ...*....*..*......./`.....      ..........*o****......*=^.=o^.@O.../O@@O******=oo*.........*................. .,ooo^..***,@@@@`=ooooooOOO. .........=OOO^......**=@^.OOOOOO@O^********************************=@\******\O\...OOOO^..*.............*****\OO@@@@OOOOO^**=OO@@@@@@@@OOOOOOOOOOOOOOO@@\**\oOOOO/O@@@@@@@@@@@OOOOOO@@@O\@@@@OOOO@@@@@@@@@O@@@OOO@@@`..................................*=OO@
   .....@@.....                    ....*......****....=o.. ..  .  .  .........=.***.......*^..oo^=@^..,OO@@O******oo^*.........*...................=oOo...*.*@@@/.*=ooOoooOO^. ........*oOOO*......**O@^,OOOOoOO@^*********************************O@`******oO`..oOOOO^.*............**.*.***,O@@@OOOOOo***oO@@@@@@@@@OOOOOOOOOOOOOO@@@O**oOOOOOOOO@@@@@@@@@@OOOOOOOO@@@@@@OOOO@@@@@@@@@@O@@@OOO@@@...................................=OO@
   ....@@`.....                 . .....*....****.*...,O^..  .     .. ........*o^***.......**..oo*=@^../.*@@@*****=oo*.............................,oooo...,@@@@`***=oooooOOO^..........*ooOO...***..,O@O=OOOo*\O@O*********************************,O\****..*O\..\OOO@^****.*.................=O@@OOOOOO\**=O@@@@@@@@@@OOOOOOOOOOOOOO@@O\.,OOOOOOOOOO@@@@@@@@@OOOOOOOOO@@@@OOOOO@@@@@@@@@@O@@@OOO@@\........................ .........=OO@
     .=@/......                 .  . ......******..../O*..        ...........=o***............oo*=@^...**@@@^*...*oo*............. .............../Ooo^../@@@.*.***=oooooOOO^.....**...*oooo...*...*=O@OOOOO^*=OO@*********************************.\@^....**,O..=OOOO@^***.*..............*.**\O@OOOOOOO**,OO@@@@@@@@@@OOOOOOOOOOOOO@@@O`.\OOOOOOOOOOO@@@@@@@OOOOOOOOO@@@@^\OOO@@@@@@@@@@@@@@@OOO@@^.................................=OO@
. ....@@......                      ..,....*****.....O^... ..   ............*o^***.... ......,oo*=@..*^*.@@@^...*oo**............................,Oooo*./@@@^*.*..*=ooooOOOO*....***...*oooo.....***=O@OOOOo*.=oO@O.*********************************OO**....*=^.=OOOO@O**...................*.*O@@^\oOOO^**=O@@@@@@@@@@@OOOOOOOOOOOOO@@@O..OOOOOOOOOOOOO@@@@@@@OOOOOOO@@@@\.OOOOO@@@@@@@@@@@@@@OOO@@`......... ......................=OO@
 ....=@^.....                        .=`...*****....=O*... ...   ...........=o^***...........=oo*@@../^*.O@@@*..*oo*............................./Oooo,@@@@`*.*..**=oOooOOOO....**,*...*\]^*..****.*=O@OOOO`*.*oOO@******..*****.*******************.=@^*..*****.=OOOO@@*.*...................**oO@O/^*,oo**,O@@@@@@@@@@@@OOOOOOOOOOOO@@@@\.=OOOOOOOOOOOOOO@@@@@@OOOOOO@@@@@.=OOOO@@@@@@@@@@@@@@@OO@@@................................=OO@
.....@@.... .       ... ....     ..  .*...****.*....O^*.. .. ...............oo****......  ...=oo=@^..o.**=@@@^.,oo,*............. ............*.*Oooo@@@@^*.***..**,ooooOOOO....**=*...******.*.**.*oO@@OO^**..=OO@O.*..............************.**..*O@******.*.=oOOOO@^.....***...........*.**=O@@Oo*.*****\O@@@@@@@@@@@@OOOOOOOOOOOO@@@O^.oOOOOOOOOOOOOOO@@@@@@@OOOO@@@@@\.=OOO@@@@@@@@@@@@@@@@OO@@^. ....................... .....=OOO
....=@^......        ...    ........ .....***.**...=O`*..........   .......=o^****...  ......=oo=@^.,o.*.*@@@^*ooo**..........................*.ooOO@@@@^..**.*..***oooOOOOO....****...*************OO@@O.**..*=OOO@^.*.........*******************.**,@O*..*****=oOOOO@O...****..****......*.***O@@@o***..**=O@@@@@@@@@@@@@OOOOOOOOOOO@@@@O**oOOOOOOOOOOOOO@@@@@@@@OOO@@@@@O^.OOOO@@@@@@@@@@@@@@O@OO@@`..............................=OOO
....@@....          ......................*****....=o**...................*=o*****...........=OoO@^./oo*..@@@@`oo=**.........................,^*ooO@@@@..*.***...**=oOooOOOO...*****...****oO*.****,OO@@o^******oOO@\**********************************\@`*******=oOOOO@O*************.*..*******oO@@O^**.****,O@@@O@@@@@@@@@OOOOOOOOOOO@@@@\*=OOOOOOOOOOOOOO@@@@@@@@@O@@@@@OO*=OOO@@@@@@@@@@@@@@@@OO@@\..............................*oOO
...=@/....  ..      ......................****.....o^*...................@^oo****......... ..=ooO@^.oOo*..\@@@ooo,**.........................=^=o@@@@/..****.....**=ooOOOOOO...*****..****=OO^*****=OO@@^*******=OOO@^*********************************,OO**.****,oOOOOO@*.****=^****************=O@@@^*******.=@@@OOO@@@@@@@@OOOOOOOOOO@@@@Oo*=OOOOOOOOOOOOO@@@@@@@@@@@@@@@OOO*OOOO@@@@@@@@@@@@@@@@@O@@^.............................*oOO
...@@`....        . .....................******....o**..................=@oo^****............=oo@@`=ooo^**=@@@Ooo^*..........................=^o@@@@/.....*.**...**oOOOOOOOo...*.***..****=@O*****.*OO@@`*******=OOO@@^************************.********=@^******=oOOOOO@^*****=o****************=O@@@\*******.*@@@OOOOO@@@@@@@OOOOOOOOOO@@@@O\*\OOOOOOOOOOOOO@@@@O@@@@@@@@@OOO^=OOO@@@@@@@@@@@@@@@@@O@@@.............................*oOO
...@@.....        .. ....................*.****...=^**................../@oo*****....*.......=oO@@*=ooo^*.*@@@@oo^*..........................oO@@@@/....*....*....=ooOOOOOOo...*.***..***.*@O^*****oOO@@^.******=OOO@@@**********************************O@`.****=oOOOOO@O.****=o********.*******=OO@@O*********@@@OOOOOO@@@@@@@OOOOOOOOO@@@@@O^*oOOOOOOOOOOOOO@@@@@@@@@@@@@OOO\.\OOO@@@@@@@@@@@@@@@@@O@@\............................*oOO
..=@^..          . ......................=o****...*`***.................O@o^****......=^.*...=OO@@*oOoo^**.=@@@Oo*..........................*O@@@@O^...........*.*=oOOOOOOOO...*.***..****O@@^*****oOO@@\.*******O@@@@@O*********************************=@\*****ooOOOOO@O*****=O^***************oOO@@O^********O@@OOOOOOOO@@@@@OOOOOOOOOO@@@@Oo*,OOOOOOOOOOOOO@@O@@@O@@@@@@OOOO^,OOO@@@@@@@@@@@@@@@@@@O@@`..................... .....*oOO
..@@....        .. .....*...............*o.****...*****...........**....O@O^****......*o^O...=oo@@=oOoo^***=@@@@o*........................../@@@@Oo`....*.....**.*ooOOOOOOOo...*.***..****O@@^*****OOOO@O********\@@@@@@\*********************************O@^***oooOOOOO@@*****=O^***************oOOO@@^********O@@OOOOOOOOOO@@@@OOOOOOOOO@@@@@O\*\OOOOOOOOOOOOO@@@@@@O@@@@@@@OO\.\OOO@@@@@@@@@@@@@@@@@@@@@...........................*oOO
 .@@.. .     .........*,.*........*.....=o^****...***..............*....O@o*****......*=OO`..=o=@@ooooOo****@@@@^........................../@@@@Oo^*...**...*.*.**oOOOOOOOOo..*..****.****O@@^*****OOOO@@********,O@@@@@@^********************************,@O***oooOOOOOO@^****=O^**************=oOOO@@O********\@@OOOOOOOOOOO@@@@OOOOOOOO@@@@@@O`*oOOOOOOOOOOOO@@@@@@O@@@@@@@@OO^*OOO@@@@@@@@@@@@@@@@@@@@@^..........................*OOO
.=@^..       ..... ../ooo**.*******.....oo****.....*....................O@o****.*.....**oo*..=O=@@ooOOoo*.**O@@@O.........................O@@@@O\o^....**.....*..*oOOOOOOOOo..*..****.****O@Oo****=OOOO@@^********O@@@@@@@^********************************\@^*,oooOOOOOO@^****oO^**************=OOOO@@O^*******=@@OOOOOOOOOOOO@@@OOOOOOOOO@@@@@Oo*,oOOOOOOOOOOOO@@@@@@@@@@@@@@@O\.\OOO@@@@@@@@@@@@@@@@@@@@@.......**,]]]]]]oooooooooOOOO@
.O@^......  .... ....oOOo*********......oo^***....**.................. .O@^****.......***`*..=o=@@oooooo`***O@@@@......................../@@@@/.=.*....**....****=OOOOOOOOOo..*..****.****@@Oo****=@@OO@@^********=@@@@@@@@^********************************OO*,oooOOOOOO@^***.*O^**************=OOOOO@@^*******=@@OOOOOOOOOOOOOO@@OOOOOOOO@@@@@@O`*=OOOOOOOOOOOO@@@@@@@@@@O@@@@@O*,OOO@@@@@@@@@@@@@@@@@O@@@^...*.*oOOOOOOOOOOOOOOOOOO@@@@
.@@... ...  .........oOO^*********.....,o^****....**.................. .O@^****.*....***.**..=o=@@OoOOoo^**oo@@@@^....................../@@@@^..,`*....***..*****oOOOOOOOOOo.**..****.****@@Oo****=@@OO@@O*********O@@@O@@@@^*******************************=@O*ooOOOOOOO@O****oO^**************oOOOOO@@\*******=@@O^.,OOOOOOOOOOO@@OOOOOOOO@@@@@@o**\OOOOOOOOOOOO@@@@@@@@@OOO@@@@@.oOO@@@@@@@@@@@@@@@@@O@@@@...*,oOOOOOO@@@@@@@@@@@@@@@OO
.@@.................*oOO**********.....=o^****....*.....................O@^*****.........***.=o=@@oooooo^.*ooO@@@@....................,/@@@@^...=*.....*.********oOOOOOOOOOo.^*..********=@@Oo****=@@@O@@@^********\@@@OOO@@@]*,****************************,O@,ooOOOOOOO@O****OO^**************oOOOOO@@O*******=@@O`...\OOOOOOOOOO@@/oOOOOO@@@@@@O^**OOOOOOOOOOOO@@@@@@@@@OOOO@@@@@/OO@@@@@@@@@@@@@@@@@@@@@@^..**\oOOOOOO@@@@@@@@@@OOOOOO
=@/...... ..........=OOo*.*.***..*... .=o*****....*.....................O@***...**.......****=o=@Ooooooo^**ooo@@@@^...................@@@@@[*..****......********O@OOOOOOOOo.^*...*******=@@O^****=@@@O@@@^********=@@@OOOO@@@]******************************=@OooOOOOOOO@O****OO`**************oOOOOO@@@^*****.*@@O.....,OOOOOOOOOO@@**\OOO@@@@@@@o**\OOOOOOOOOOO@@@@@@@@@OOOOO@@@@@OO@@@@@@@@@@@@@@@@@@@O@@@`...*,\oOOOOOOOOOOOOO[[[\oOO
=@^...........*.....=OO^*..............oo*****....*.....................O@*.****.*.....*...**oo=@OooOooo^.*oooO@@@@`.................@@@@@**...**.*....**,`*****=O@OOOOOOOOo*^**.********=@OO^****=@@@@@@@\`********@@@OOOOOO@@^,*`**************************=O@oOOOOOOOO@O***=OO**************,oOOOOO@@@^******O@@^.......\OOOOOOOOO@O***\OO@@@@@@O^**oOOOOOOOOOOO@@@@@@@@OOOOOOO@@@@@@@@@@@@@@@@@@@@@@@@@O@@^.......**********.......*,o
=@^...........*.....=OO^*..............o.******.........................O@*.***.....**....*.*o/=@OooOoooo*ooOo`O@@@^................@@@@@`*....**.*....**=o*****=O@OOOOOOOOo=o*..********O@OO^****=@@@@@@@@^********O@@OOOOOOO@@\*****************************=@OoOOOOOOO@O***=OO**************=OOOOO@@@@O******O@O.........,OOOOOOOOO@O****oO@@@@@@o**=OOOOOOOOOOO@@@@@@@@@OOOOOOO@@@@@@@@@@@@@@@@@@@@@@@@@@@@..........................,
@@....... ...**.....=Oo*...............oo******.........................O@*..**.....***.**..,o^O@OoOooooo*oooo^*@@@@`............../@@@@..**...**......**OO*****=O@OOOOOOOOO/o*..********O@OO^****=@@@@@@@@\********O@@OOOOOOOO@@O``**************************,@@OOOOOOOO@^.**=Oo*************^oOOOOO@@@@O******O@^...........OOOOOOOoO@\***=oOOO@@@O^**oOOOOOOOOOO@@@@@@@@@OOOOOOOO@@@@@@@@@@@@@@@@@@@@@@@@@@@^.................  ......*
@@...........**.....=O^................oo******.........................=@..**...*****.....**o^O@OOOoooooooOoo`.\@@@O.............=@@@@...**...*.**....*=@.*****oO@OOO@@OOOOoO*..********O@OO^****=O@@@@@@@@^*******O@@OOOOOOOOO@@@***************************`O@OOOOOOOO@^***=Oo***********=oooOOOOO@@@@O*****=@@.............\OOOooOoO@^**,oOOOO@@@O**=OOOOOOOOOOO@@@@@@@@@OOOOOOO\=@@@@@@@@@@@@@@@@@@@@@@@@@@.... ...........   . ....*
@@....   ...***.....=o^...............*oo*****..........................=@*.**.*..........*.*=^O@OoOoOooooooOo*.,@@@@\...........=@@@@`...**...***....**=@^*****oO@OOO@@OOOOoO^...*******@@OO^****=O@@@@@O@@\**`****=@@OOOOOOOOOOO@@/,*************************o@@OOOOOOO@^*`=O@o**=*********oooOOOOO@@@@@**``^=@^..............=OOooooo@@^**oOOOOO@@O^**oOOOOOOOOOO@@@@@@@@@OOOOOOOo.,@@@@@@@@@@@@@@@@@[@@@@@@@^.............    .. ....*
@/...   ....***.....,o*...............*^o******.........................=@`.***............*.,*O@OooOooooOOoO^...=@@@@..........,@@@@`....*....****...*.=@^*****oO@OO@@@@OOOOO^*..******=@@Oo*****=O@@@@OOO@@*******=@@OOOO^.,OOOOO@@\*`**********************ooO@OOOOOOO@`**=O@^**********.*oooOOOOO@@@@@^*^**O@^...............,OOooooO@@`*=OOOOO@@Oo**=OOOOOOOOOO@@@@@@@@@@OOOOOOo`.=@@@@@@@@@@@@@@@@`,@@@@O@@.............    .. ....*
@^..........***.....*^................*,.******..........................@^..********.......**.O@OooooooooOoo^....\@@@@.........@@@@^.....*....***....*.=@^*****oO@OO@@@@OOOOO^*..******=@@Oo*****,O@@@@OOO@@o*******O@OOOO^...\OOOOO@O*******************`**=ooO@@OOOOOOO*=,=@O*[*=*******=oOoOOOOO@@@@@@^,*`\O@..................oOOoooO@@`=OOOOOO@@o***oOOOOOOOOO@@@@@@@@@@OOOOOOO^.=O@@@@@@@@@@@@@@@^.,@@@O@@^.............    ......*
@^... .. ..***.......`................*=`******..........................@^************........O@OoooooOoooOo^....,@@@@^.......@@@@/......*....***.....*@@^*****oO@OO@@@@OOOOOO*..******=@@Oo******O@@@OOOOO@@``*****O@OOOO^....,OOOOO@@]********************,oooO@OOOOOO*,]`O@O***********oOOoOOOOO@@@@@@o,``=@/...................oOooooO@O*oOOOOO@@O^**=OOOOOOOOO@@@@@@@@@@@OOOOOO^..O@@@@@@@@@@@@@@@^..,@@@@@@`..........      ......*
@^... .....*...*.....*................*=\******..........................@O^*************......O@Oooooooooooo^.....=@@@@^...../@@@@`......*....***.....=@@`*****oO@@@@@@@@OOOOO^..******=@@Oo******O@@@OOOOOO@O******=@@OOO^......=OOOOO@O*******************,ooOO@@OOOO^**=\O@/o********,*oOoOOOOOO@@@@@@O*=*=@^....................\OOoooO@O=OOOOO@@@o^***oOOOOOOO@@@@@@@@@@@OOOOOOo*.\@@@@@@@@@@@@@@@^...,@@@@@^.........      .. ....*
@^... .....***.*.......................=*******..........................@Oo^**************.**.O@OoOoooooooo^*......@@@@@....=@@@@`......,**...***.....=@@******oO@@@@@@@@OOOOO^..*****,@@OO.*.****=@@OOOOOOO@@o*****=@@OOO^........\oooO@O`***\*************=ooOO@@OOOO****=@O`***\oooooooOOOOOOOO@@@@O@@O,`=O@......................=OOooo@@OOOOOOO@@@^*****oOOOOO@@@@@@@@@@@@OOOOOO^.=O@@@@@@@@@@@@@@\.....@@@@@...........  . .......*
@^.. ......*.***.......................*`******..........................@@oo****************..O@OooOooooooo^*.......@@@@\..=@@@@^......=O*...****.....=@^******oO@@@@@@@@OOOOO^...*,**=@@OO..*****=@@OOOOOo.\@O^*,***O@OOO^.........,oooo@@^***************,ooOOOO@@OO^**o,O@o`***\o.**\ooOOOOOOOO@@@OO@@O.*=@^.......................=OOooo@@OOOOOO@@@O******,[[OO@@@@@@@@@@@@OOOOOO^.*O@@@@@@@@@@@@@@O......@@@@\..........     .......
@^........****.**......................*^******..........................O@OO^****************.O@OOOOooooooo^........,@@@@^.@@@@@.......=O*...*****...*=@^******oO@@@@@@@@OOOOOo*..***=@@@Oo`******=@@OOOOO^..=@O,,***o@OOO^...........,oooO@O*[`*]o\[/]]]/o`*oOOOO@@Oo*o/^/OO^=********^=OOOOOOOOO@@OOO@@O*`O@.........................=OOooO@@OOOOO@@@@^*****,***O@@@@@@@@@@@@@OOOOO^..O@@@@@@@@@@@@@@@.......@@@@`..........   .. .....
@^... ....o^**.**......................********..........................O@OOo****************.O@OoooooooOoo^.........=@@@@@@@@@........@O^...****.....=@^******oO@@@@@@@@OOOOOO*..***O@@@Oo*******=@@OOOOO....=@\^*`*=O@OO^.............[ooo@@\`/=//oo\/oooo*oOOOO@@O`/O\/oOO^^****`*o^ooOOOOOOOO@@OOOO@@O[/@/..........................=OOooO@OOOOO@@@@OoOooooo`.=@@@@@@@@@@@@@OOOOOo..O@@@@@@@@@@@@@@@........\@@@..........   .. .....
@^.......=O^*.***......................********..........................=@OOO`***************.O@Ooooooooooo^..........\@@@@@@@^.......=@^*...*****....\@^******oO@@@@@@@@OOOOOO^..**o@@@@O^*******O@OOOOO^.....=@\,**`O@OO^...............\oo@@O]^=***.*.*o/oOOOOOO@O=@o**OO^,\****\o*==OOOOOOOOO@@OOOO@@o*O@............................,OOOO@@OOOO@@@@@^*******.O@@@@@@@@@@@@@@OOOOo*.=@@@@@@@@@@@@@@@.........=@@^..........  .. .....
@^.......=Oo*.****.....................********..........................=@@OOo****************O@OooOoooooOo*...........@@@@@@/........=@^....*****....@@^******\O@@@@@@@@OOOOOO^..*=@@@@@O`******=O@OOOOO`......OO**\`o@OO^.................\\O@@O`*=\/oo^=`oOOOOOOO`@@^o/OOoooo^**^*\]oOOOOOOOO@@OOOOO@@o/@^...............,]]]]]oOOOO@@@@@@OO@@OOO@@@@@O*******.@@@@OOOO@@@@@@@OOOOO*.=@@@@@@@@@@@@@@@..........\@@....... ...   . ....
@^.......OOO`***........................********.........................,@@OOO***************.O@OoOoooooooo*...........,@@@@@^........=@^....*****....@@^******=O@@@@@@@@OOOOOO^.../@@@@@/`******=@OOOOO^........@O`=\/O@O^...................[o@@@Oooo*,o`\OOOOOOO*O@/o^OOo/,\]oo]\o*oOOOOOOOOO@OOOOOO@@o@@@@@@@@@@@@OOOO@@@@@@@@OOOO[/`**,oooO@OOOO@@@@@^***,**.@@@@OOOOOOO@@@@@OOOO^.*@@@@@@@@@@@@@@@...........,@\...  ......  . ....
@^... ...OOO^****.*.....................********..........................@@OOO^***************O@Ooooooooooo............,@@@@@^........=@^....*****....@@^*******O@@@@@@@@OOOOOO\*..O@@@@O^*******O@OoOOO*........,@^o=^O@O\.....................,@@@@O\\]*\=OOOOOO`=@@^o=OOoooooooooooOOOOOOOOO@@OOOOO`O@o@.*,,ooo[[[[[[[[[[[[[[[[***.......*ooo@@OOO@@O@@O*****.=@@@OOOOOOOOOO@@@OOOO^..@@@@@@@@@@@@@@@..... ......,@\..................
@^.......OOOo*..***.....................*^******..........................O@OOOo***************O@Oooooooooo\............/@@@@@@^.......=@^*...*****....@@^*******O@@@@@@@@OOOOOOo*..=OO@@O^******=OOoooO/..........=O^*,/O@o.......................\@@@@OooooOOOOO^*O@Oo\OO\oooooooooooOOOOOOOOO@OOOOO^.OOO/....**********....................,ooO@OOO@@oO@@^,`,*.=@@@OOOOOOOOOOOO@OOOOo..@@@@@@@@@@@@@@@^.............\@.................
@^.......OOOO`*...*.....................********..........................O@@OOO^**************=@OoooooOooO^...........=@@@@@@@@\......=@^*...*****....@@^*******o@@@@@@@@OOOOOOo^`..OO@@O*******o@OoooO`...........=O^o^O@O........................,@@@@@OoOOOOO.*O@OoooOOoooooooooooOOOOOOOOO@@OOOO^..@OO....................................=Oo@@OO@Oo\@@O**,*.O@@@OOOOOOOOOOOOOOOOOo..@@@@@@@@@@@@@@@^..............\@`...............
@\......*OOOO^***.*.....................********..........................=@@OOOo*************.=@OooooooOoo^...........@@@@@=@@@@`.....=@^*...*****....@@^*******=@@@@@@@@OOOOOOO/@^.,O@@^******=O@oooO/.............=Oo\=O@^.........................\@@@@@OOOOO*O@@OooOOoooooooooooOOOOOOOOOO@OoOO/..=@@^.................................../OOO@@@@@@OOO@@o].*.@@@OOOOOOOOOOOOOOOOOOo..@@@@@@@@@@@@@@@\................@^.............*
@@......=OOOOo***.*.....................*********.........................=@@OOOO^*************=@@OoooooOoo^..........=@@@@*.=@@@@^....=@^*...*****....@@^*******=@@@@@@@@@OOOOOO@@^..O@O=******O@Ooooo...............\O/`o@O.............,O/..........=@@@@@OOO.*O@OooOOooooooooooooOOOOOOOOO@@oOO/...=@/.............................]/@@@@@@OOoo@@O@Oooo@@/[***@@@OOOOOOOOOOOOOOOO^,/..@@@@@@@@@@@@@@@@.... ............\\`...........=
@@......=OOOOO*.*.*.....................*********.........................*@@OOOOO^************=@@Ooooooooo^..........@@@@O...\@@@@^...=@^*...******...@@^*******=@@@@@@@@@OOOOO@@@O^.=@O,=****=OOoooO/................OO``O@`........../@/.............,@@@@@O`/O@@oooOO\o]ooooooooOOOOOOOOO@@OoO/....@@.........................,/@@O/[O@@@@@@@@@@@@@OOOOO@O/`*=@@@oOOOOOOOOOOOOOOO^**..@@@@@@@@@@@@@@@@`... .............=@`..... ....=
@@....../OOOOO^*..**....................**********........................*O@@OOOOo************=@@Ooooooooo*.........=@@@@.....O@@@@^..=@^*...******...@@^*******,@@@@@@@@@@OOO@@@OOo..\o****`*O@Ooooo..................OOo=@O......./@@O.................O@@O^/O@@Oo/OOoooooooooooOOOOOOOOOO@Ooo^....=O......................./@@O[*,/@@@@@@@@@@@OO@@@OOO@@@@O\*O@@O=OOOOOOOOOOOOOOOo**..@@@@@@@@@@@@@@@@^..................,@\.........=
=@^.....OOOOOO^*..**.....................*********.........................\@@OOOOO^***********=@@OooooooOo*.........@@@@/......\@@@@`.=@^**..******...@@^********@@@@@@@@@@OOO@@OOOO^..*^***.*OOoooO^...................\O/O@^....O@@/....................\O^/O@@OooO@OooooooooooOOOOOOOOOO@Ooo`.....//..................../@O.**]@@@@@@@@@@@@@@@@@@@@@@@OO@@OO`@@@^/OOOOOOOOOOOOOOOO^*..@@@@@@@@@@@@OO@@O....................\@`.......*
=@^.....OOOOOOo****......................,********.........................=@@OOOOOo***********=@@Ooooooooo*......../@@@@^.......=@@@@\.@O**...*****...O@^********O@@@@@@@@@OO@@OOOOOo*.*****=OOoooo^.....................\OoO@.,@@@O`.......................O@@@OooO@@@ooooooooooOOOOOOOOO@Ooo*.....,O^.................,O@.*.*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\,@@O^oOOOOOOOOOOOOOOOOo*..@@@@@@@@@@@@O\O@@.....................,@\......*
=@^.....OOOOOOO**.*......................**********........................,O@@OOOOO^***********@@Ooooooooo*........@@@@/.........=@@@@^/@**...*****...=@^********O@@@@@@@@@@@@@OOOOOoO`.***.*@Ooooo.......................\@O@@@@O`.........................,@@@o\O@@@@oooooooooOOOOOOOOO@Oo/.......=/................/@O`.,O@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\=@@^***oOOOOOOOOOOOOOOo*.=@@@@@@@@@@@@ooo@@.......................\@`.....
=@O.....OOOOOOO^***......................*`********.........................\@@OOOOOo***********@@Ooooooooo*.......=@@@@^..*.......\@@@@@@^*...*****...=@^********O@@@@@@@@@@@@OOOOOoO@o..**=OOoooO^......................]OO@@@O`............................=@OoO@@@@@ooooooooOOOOOOOOO@Oo`........O`..............,@O`./@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@O=@@OOOOOOO`*,OOOOOOOOOO*.=@@@@@@@@@@@OoooO@^........... ....  ......O\....
.@@.....OOOOOO@O*.**.....................*,*********........................=@@@OOOOO\**********O@Ooooooooo*.......@@@@^...*........=@@@@@^*...******..=@^********=@@@@@@@@@@@OooooooO@^*..*OOoooO^..................../OOoO@O@@@`.........................../OooO@@@@@OoooooooOOOOOOOOO@O^........./^.............,@@`./@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@^...\OOO/oOOOOOOOOOOO..@@@@@@@@@@@@^oooo@^........... ....  .......,O^..
.@@`....OOOOOO@O`.*......................************.......................*O@@OOOOOo**********O@OooooooOo*......=@@@@`...*.........,@@@@@*...******..=O^********=@@@@@@@@@@OoooooooOO***.,@OooO/.................,/OOoO@@O[**\@@..........................=OooO@@@@@@OooooooOOOOOOOOO@O`.........*^............./@O.*@@@@@@@@@@@@@OO@@@@@OOOOOOOO@@@@@@@@@@@@@@@*..*ooO^**oO`*=OOOOOO..@@@@O@@@@@@O.,oo/@@........... .       .......=O`
.O@^....OOOOOO@@^**......................*=`*********........................o@@OOOOOO^*********O@Ooooooooo*......@@@@O....*..........=@@@@@`..******...O^********=@@@@@@@@@@Oooooooo@O****.,oooo...............]OOOO@@@O[`*.....\O......................../OooOO[O@@@@OoooooOOOOOOOO@@/...........,............,@@o=@@@@@@@@@@@@@@@@@@@@@@@@OOOOOO@OO@@@@@@@@@O@O=o**=oO\**]`*]*.*OOOo.=@@@@`\oO@@@^..,o\O@^.......... .            ....\
.=@^....OOOOOOO@O**......................*=^**********.......................=O@OOOOOOo`********=@@Oooooooo*.....=@@@@`....*...........,@@@@@`.******...=^********=@@@@@@@@@OoooooooO@^**,OO.=oo............]OOOOO@@Oo`****.......=\....................../OooOO**.\@@@OooooOOOOOOOO@@`...........*............,OOoO@@@@@@@@@@OOO@@@@@@@@@@@@@OoooOOooooO@@@@@@O@o\ooOOOOOOOOO`,`,OOOO^.O@@@O.=//@@@@...=\O@^...............        ......
..@@....OOOOOOO@@^**.....................*=^**********.......................*O@@OOOOOO^********=@@Oooooooo^...../@@@@.....**............@@@@@^*******..=o********=@@@@@@@@@ooooooooOO^*,OOo\*=`........]OOOOO@OO[****.............\\...................,OOoOO.*....\@@OoooOOOOOOOO@/............*............=OooO@@@@@@@@@@@@@@@@@@@@@@@@@@@@OOOOOOOoooooO@@@OOOooO@OOOOOOOOOOOOOOOO`.@@@@o.O@O@@@@^...=O@@...............     .  ......
..O@....OOOOOOO@@\*.......................=o***********......................*=@@OOOOOOO^*******=@@OoooOooo^....=@@@@`.....*..............@@@@@\******...=`*******=@@@@@@@@Ooooooooo@O`*OOoooo*.....]OOOO@OOO[[**...................=^.................,OO/OO`.......O@oooOOOOOOO@@`..........................,*=OOO@@@@@O@@@@@@@@@@@@@@@@@@@@@`*OOoooooooooo@OO=OOoO@OOOOOOOOOOOOOOO^./@@@O^.O@O@@o@\...*\@@.............. ......   .....
..=@^...OOOOOOOO@O`......................*=o^**********.......................*O@@OOOOOO^*******=@@OoooooOO^....=@@@O.....***..............@@@@@O*****...=^*******=@@@@@@@OooooooooO@^*O@Oooo*.]oOOO@@@Oo[[**........................=^...............=OoOO`.........OOooOOOOOOO@/..............................=OO@@@@@O@O@@@@@@@@@@@@@@@@@@@@@@oo/oooooooo/`/oO@OoO@OOOOOOOOOOOOOOO`.@@@@O*.=@@@O*@@....*O@^............................
...@@...OOOOOOOO@@^*......................*o`**********.......................*\@@OOOOOOO^******=@@Oooooooo^....@@@@^......**..............O@@@@@O*****...********=@@@@@@@OooooooooOO^oOoOOO@@@@@OOo[**...............................**............./OOO`..........=OOoOOOOOO@/................................*=O@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@^....****[[.*\=@@OoO@OOOOOOOOOOOOOO^.=@@@O^..=O@@^*O@^....=@^... .......................*
...@@`..OOOOOOO@@@O*.....................**o^**********.......................*=@@@OOOOOOo******=@@OoooooOo^....@@@@.......**...............@O@@@@@`***...*********O@@@@@Ooooooo/oO@OO@@@@@@O/[*.................,]/O@@@@@@@@@@@@@@@@OO*...........,OO/`............=OOOOOOO@@`..................................=@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@^...........**=@@OO@@OOOOOOOOOOOOOO.,@@@@O`..=O@O**=@@....,@@...........................*
...=@^..OOOOOOO@@@@^.....................**oo***********......................**@@@OOOOOOO^*****=O@@Oooooooo...O@@@^........................\@^@@@@@^**....********O@@@@@OooOOOO@@@@OOooo^................]/O@@@O/[[[[[oo\oOO.****...............,O/................OOOOOO@@^....................................O@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.............\@@OO@@OOOOOOOOOOOOO^./@@@O^...O@@`***O@^....=@`..........................*
...=@@..OOOOOOO@@@@O*....................**oo***********......................**=@@OOOOOOOo*****=O@@OoooooOo...@@@@.........*.....=^........=@^.@@@@@^*....********=@@@@@@OOOOoo/./@O`*o`............]O@@@/[`.........../@@^.....*****.........*[..................O@OOOO@/......................................O@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.............=@@OO@@OOOOOOOOo*\O/.=@@@@o`..==@^.***=@\....=@^..........................*
....\@^.OOOOOOO@@@@O*.....................*=o^***********.....................**=@@@OOOOOOO^****=O@@OOooooOo...@@@^................*.........=@^*O@@@@O*. .****,`o/O@@@@O=o[*oo^.,@O`*.*........,]@@@/`..............]@@@@@@@@@@@@@@@@@Oo*........................=@OOO@O.......................................=@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@^............=@@OO@@@OOOOOOOOOOO.=O@@@O`....O@...**=@@.....@O..........................*
....=@\.OOOOOOO@@@@@^.....................*=oo***********......................*=@@@OOOOOOOo`***=O@@OooooOoo..=@@@`................=.........,@\**=@@@@\....*******=@@@@o^**,o^..O@..*......./@@@.*............]/@@@@@@@@@@@@@@@@@@@@@@@@O]`......................O@O@O`........................................,O@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@^............/@@O@@@OOOOOOOOOOO`,O@@@@o*...=@^......=@^....O@..... ....................*
.....@@`OOOOOOO@@@@@^.....................*=oo^***********.....................*=@@@@OOOOOOO^***,o@@OooOoooo^.O@@@.................=*.........=@`**,@@@@\...*******=@@@@o=ooo^..=@.......,/@@/`..........,]O@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@O/.....................=@@O`...........................................=/..=@@@@@@@@@@@@@@@@@@@@@@@@@@@@^............@@@O@@@OOOOOOOOOO^*O@@@@O`....@/.....*..@O....=@^.........................*
.....=@\OOOOOOO@@@@@^.....................**oo^***********.....................**@@@@@OOOOOOO`***oO@@ooooooo..@@@^.................=*..........O@^***O@@@\...*******O@@@oooo`..=O.....,/@@.*.........,O@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@O`..................,@O...............................................@@@@@@@@@@@@@@@@@@@O@OoOO[[,[O@@............=@@O@@@@OOOOOOOOO^,O@@@@Oo....=O.......*.=@^....@\.........................*
......@@OOOOOOO@@@@@^......................*ooo************....................**O@@@@OOOOOOOo***oO@@OooOooo^.@@@..................=*..........*@O****=@@@@`.*******O@@@ooo...=/..../@@O`........,/@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@[[[OO@@@@O].................[`................................................@@@@@@@@@@@@@@@@@@@O@O*.....................=OO@@@@@OOOOOOOO^,OO@@@@o*...=O........***@@`...@@.........................*
......=@@OOOOOO@@@@@^.....................**=oo^***********....................**=@@@@@OOOOOOO^**=O@@Ooooooo^=@@/..................=`...........\@O****,\@@@]*******O@@@o^...=^..,O@O[........*@@@@@@@@@@@@@@@@@@@@@@@@@OOOOOOOoooo`..............................................................................O@@@@@@@@@@@@@@@@@OOO/......................=O@@@@@@OOOOOOO^=OO@@@@O`...=O`.........**@@@...=@^........................,
.......@@@OOOOO@@@@@O.......................=oo\^***********...................**=@@@@@@OOOOOOo**=O@@Ooooooo^=@@..................../............O@^*****\@@@O`*****=@@@^...*`.,@@O`.......*@@@@@@@@@@@@@@@@@@@@@OO@@@@@@@Ooooooooooo\............................................................................=@@@@@@@@@@@@@@@@@OOOO`.....................oO@@@O@OOOOOOO^/OO@@@@@^....o.............O@@\...@\............**********]/O
.......=@@OOOOO@@@@@O.......................*oooo***********...................**=@@@@@@OOOOOOO^*=O@@OoOooOo^=@@....................o............=@@^******\@@@^*****@@@.....=@OO[....../@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ooooooooooo*..........................................................................=@@@@@@OOO@@@@@@@OOOOOOO`..................=OO@@OO@OOOOOOoOOO@@@@@@\...*..............*@@@^..@@....**]ooOOOOOOOOOOOOOOOO
........@@@OOOO@@@@@@.......................*oooo^***********..............*...**=@@@@@@@OOOOOOO*=OO@@OoooOo^@@`....................=............=O@O******.,@@@\****O@@...,@@@OOOOOOO@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*,/oooooo[o...........................................................................@@@@@@@]OOO@@@OO/`..O@@@@@@O\...........,O@OO@Oo@@OOOOooOOO@@@@@@@@..O................@@@@`.=@^...*oOOOOO@@@@@@@@@@@@@@
........=@@@OOO@@@@@@.......................*oOoo^************.............**..***@@@@@@@@OOOOOOo*oO@@Oooooo^@@.....................=............*oO@O******..=@@O***o@@.,OOOoo`..*O@@@@@@@@@@@@@@@@@@][[O@@@@@@@@@@@@@@@@@@@]O^**.................................................................................\@@@@@@@OoOOO`.......,O@@@@@^..........,/Ooo@OOO@OOOOoOOO@@@@@@@@@@Oo................*=@/\@.=@O..*,oOOOOO@@@@@@@@@@@@@@
.........\@@@@O@@@@@@........................=Oooo************............****..**@@@@@@@@@OOOOOO^oO@@Ooooooo@/.....................=*............,o@@O**`**...,@@@`*o@@OOOoo/`.*O@@@@@@@@@@@@@@@@@@@@@@@@@@@OOOO@@@@@@@@@@@@@@@`..................................................................................=@@@@@@@O^**...........=@@@@...........=OooOOoO@@OOOOOOO@@@@@@@@@@@@^.................=@@.@O.@@...**\oOOOOOOOOOOOOOOOOO
.........,@@@@@@@@@@@........................*oOoo^************............***..**O@@@@@@@@@OOOOOOoO@@OoooooO@^.....................=\............*=O@@O\***....*=@@^*@@Ooooo*,O@@@@@@@@@@@@@@@@@O@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@`..................................................................................=@@@@@@@O`.............=@@^........../OooOOoo@@OOOOOOO@@@@@@@OOO@@@\.................*O@^.@\=@^......**************[\o
..........=@@@@@@@@@@`.......................*oOooo`***********............****.**O@@@@@@@@@@OOOOOoOO@@OooOoO@^.....................*o............**\O@@Oo***...***O@OO@O=/o,/@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@...................................................................................\@@@@@@/............../@^.........*OOooooooO@OOOOOO@@@@@@@@OOO@@@@@^.................=@^.,@/@^......................*
...........\@@@@@@@@@^........................=OOoo^************...........*******O@@@@@@@@@@@OOOOOOO@@OoOoo@@.......................O............***oO@@OO**....\**,O@@O,\O@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@..................................................................................*\@@@@/..............@@O........./ooooooooo@@OOOOO@@@@@@@OOOOOOO@@@O.................=@@..\@@@.......................
............@@@@@@@@@^........................*oOOoo************...*......********\@@@@@@@@@@@OOOOOOO@@Ooo`.=@.......................O............****oO@@OO\*...*`***=@@/@@O@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@`..................................................................................*\@@@^............=@@@`....../ooooooooooo@@OOOO@@@@@@@@OOOOOOOOO@@@^.................@@^..\@@^.. ...................
............,@@@@@@@@^........................*oOOooo***************..*..*********=@@@@@@@@@@@@OOOOOO@@@^..*O@^......................O^............***,OO@@OO\*...o`**=@@@Oo@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@...................................................................................*,@@^........]@@@@@@@@@O\.*o\oooooooo/oO@OOOO@@@@@@@@OOOOOOOOOOO@@@.................=@^..,@@\......................
.............=@@@@@@@^.........................=OOOoo`****************************=@@@@@@@@@@@@@OOOOOO@@Oo*./@^......................=\............****=OOO@OO\...=\`.*@@OO@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@......................................................................................\@@]OO@OO`[[***************,[[=,^==@@OO@@O@@@@@@@OOoOOOOOOOOOO@@^................=@@...=@@......................
..............\@@@@@@^.........................,OOOooo****************************=@@@@@@@@@@@@@@OOOOO@@Oooo@@^...............*]`.....O.............****\OOO@OO^..,O^/OO@oO@@@@@@@@@@@@@@@@@@@@@@@@@@^=@@@@@@@@@@@@@@@@@@@@@@@@@@@@@^............................................................................................***********************,*.*@@O@@/`=@@@@@O`\oooOOOOOOOOO@@@.................@@`...=@^.....................
...............\@@@@@^.........................*oOOOoo\***************************=@@@@@@@@@@@@@@@OOOO@@@ooO@@^..............=oOOO`...O.............*****=OOO@@O`..=@O*O@O@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@O@@@@@@@@@@@@@@@@@^.........................................................................................*.******************]``****=/@@@O[***=@@@@O^.*ooooOOOOOOOO@@@\................=@^....@@.....................
................@@@@@^..........................=OOOOoo`**************************=@@@@@@@@@@@@@@@@OOOO@@Ooo@@^...........*^.=OOOO^...o^..........*.******oOOO@@O`..=O`\@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@O@@^..\@@`......,`........................................................................................*****************,o\`,oo\].*@@O.******=@@@Oo^..\oooOOOOOOOOO@@@`...............=@O....O@`....................
.................@@@@^..........................*OOOOOoo**************************=@@@@@@@@@@@@@@@@@OOO@@OOo@@^.........../^.=OOOOO^..=^..........*..*****,oOOO@@O`..OO=@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@O@@\..=^.................................................................................................****************oo],[ooo\ooO@O*********=@@ooo^..=oooOOOOOOOOO@@@\...............*@@....,@\....................
..................@@@^..........................*=OOOOoo\*************************=@@@@@@@@@@@@@@@@@@OoO@Oo[@@^..........=O^.=OOOOO^..=^..........*..******,oOOOO@\...@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@=O@OoOO^..................................................................................................**]\]**]`***oo\,,[oo\`*,oOo*,\o\******=@@oooo..*ooooOOOOOOOOO@@@................O@^....@@....................
...................@@^...........................=OOOOOoo`************************=@@@@@@@@@@@@@@@@@@@\,@O*.=@..........,OOo.=OOOO^....O`.........*..*******=OOOOO@\..,@@@@@@@@@@@@@@@@@@@@@@@@@@@O@@@@@@@@@@@@@@@OOO@OOOOO`................................................................................................**,oOoo^**oo\***oo\**,ooo`**,[***,\`****\@Oooo*..oooooOOOOOOOOO@@^...............O@^....\@^...................
...................=@O...........................*oOOOOoo\************************=@@@@@@@@@@@@@@@@@@@@/O@o*O@..........=OOo.=OOO`*....O^.........*...*******,OOOOO@\..=@@@@@@@@@OO^....@@@@@@@@@@@OO@@@@@@@@@@@@OOOOOOOOOOOO\....,]........................................................................................****O@O=\o]**\o\**,oo]***,\*************=@@ooo*..\oooooOOOOOOOOO@@`..............=@@....,@^...................
...................=@O....*.......................=OOOOOoo^***********************=@@@@@@@@@@@@@@@@@@@@OO@\.@@..........=OOo.=OO`***...=^.........*...********=OOOOOO^..O@@@@@@@OO\..../@@@@@@@@@/...*OOOOOOOOOOOOOOOOOOOOOO@@O@@@@@.........................................................................................***O@.***,oo`**,o\**,[\****************.=@Ooo^..=oooooOOOOOOOOO@@\..............*@@.....@@...................
...................=@^............................,OOOOOOoo***********************=@@@@@@@@@@@@@@@@@@@@@@@@*@@.....=*...OOOo.=o^****...=O.........*...*********,OOOOOO..,O@@@@@@OOo\@@@@@@@@@@@@@\....oOOOOOOOOO[*..........[[OOO@@^........................................................................................*************[oo`***[^**************.....,@@oo^..,ooooooOOOOOOOOO@@..............*O@^....=@`..................
....................O*....*.......................*oOOOOOooo`**********************@@@@@@@@@@@@@@@@@@@@@@@@O@@.....=O\.=OOO^.=o.*****..,O^........*....*****`**.*OOOOO^..,@@@@@@Oooo/O@@@@@@@@@@@@@OO@@OOOOo`...................@@O..........................................................................................**********************************.......\@Ooo*..oooooooOOOOOOOO@@^.............*=@^....=@^..................
....................*.....**.......................=OOOOOOoo\*********************=@@@@@@@@@@@@@@@@@@@@@@@@@@@.....=OO^=OOO^.=o.*****...=^.............*******o[`,oOOOO\..,@@@@@@ooo`.,@@@@@OOOO@@@@@@@OO/.....................,@@...............................................................................................**********************.*..............@@oo*..=ooooooOOOOOOOO@@O.............*=@@. ...@@..................
...................................................*oOOOOOooo`********************=@@@@@@@@@@@@@@@@@@@@@@@@@@@.....=OO\=OOoo..=.******..=O.............***********,OOOO@\..,O@@@@\o*....\@@@@@OOOO@@@@@O......................,@@`.................................................................................................*...................................\@Oo^..=oooooooOOOOOOOO@O.............**@@.....=@..................
............................*.......................\OOOOOOooo********************=@@@@@@@@@@@@@@@@@@@@@@@@@@@.....=OOO=oooo..*^******..=O*........*...*************oO@@@\..=O@@@@^.......\@@@@OOOOO@@/......................=@/......*******..**......................................................................................................................*@@o^..=ooooooooOOOOOOOOO.............**O@^....=@^.................
....................*......**.......................=OOOOOOooo\*******************=@@@@@@@@@@@@@@@@@@@@@@@@@@@....^=OOOooooo..*********..=^........*....************,O@@@@\..\O@@@@`........\@@@@OOOO^.....................,@@\]].]`*,************......................................................................................................................O@O^..*oooooooooOOOOOOOO.............**=@^....=@^. ...............
..................../^.......*......................*oOOOOOOooo\******************=@@@@@@@@@@@@@@@@@@@@@@@@@@@...=\oOOOOoooo..*********..=O........*....***********,OOoO@@@`..\@O@@@O`........,\@@@@O^.................]@@@@@OOOo[[*****************....................................................................................................................=@Oo..*=oooooooooOOOOOOO.............**=@\.....@O.. ..............
...................=@^....****.......................=OOOOOOOOoo^*****************=@@@@@@@@@@@@@@@@@@@@@@@@@@@...=OOOOOOoooo*.*********..=O........*....**********=O.**\@@@O*..=O@@@Ooo*..........[@@@@@\]]]]....]/@OO@OO/[`**************************..................................................................................................................,@@o*..=ooooooooooOOOOOO^.............*=@@... .=@.................
...................=@^.......*.......................*oOooooooooo*****************=@@@@@@@@@@@@@@@@@@@@@@@@@@@...=OOOOOoooo/^..*********..@^.............*******.*O^***=@@@Oo...,O@@@Ooo].,`..........[\@@@@@@@@@@Oo[`********************************...................................................................................................................@@O*..=ooooooooooOOOOOO^.............*,@@`....=@^................
...................=@^....*..*.......................*=ooooooooooo****************=@@@@@@@@@@@@@@@@@@@@@@@@@@@...=OOOOOooo**`..*********..OO........*......*****OO`****=@@@Oo^...*=O@@@ooO@OO@\]]``.....=OOoo[**************\]************************...................................................................................................................=@O*..=oooooooooooOOOOO^.............**\@^....=@^................
...................=@^......**.......................**ooooooOooooo`**************=@@@@@@OO@@@@@@@@@@@@@@@@@@@...OOOOOoooo`**..****...**..,O........*.......*.*O[*******@@@Oo^*....*,O@@\\o`,[`*,*****************************,\o\]******************....................................................................................................................=@@^..=ooooooooooooOOOO^.............**=@^.....@\................
...................=@^....*.*.*.......................*=oooooOoOOooo`*************=@@OO@OOOOO@@@@@@@@@@@@@@@@@^..OOOoooooo***..****....**..O................,O.*********O@@oo*.*....***O@O`***********************=\]***[\o\]******\oo]***[oo`******.....................................................................................................................=@@o..=oooooooooooOOOOO^..,\.........**=@^.....@@................
...................=@^....*.***.......................**oooooooOOoooo*************=@@OOOOOOOOOO@@@@@@@@@@@@@@@...OOoooooo`***..*********...O`..... ........o[***********=@@Oo*........**,@@\*************************,\o]`***,[oo`*****,\o\`***,o]**.....................................................................................................................=@@o*.*oooooooooooooOOO^...O.........**=@@.....O@................
...................=@^....**...*......................*.*ooooooOOOoooo`***********O@@OOOOOOOOOOOO@@@@@@@@@@@@@...O.**o^\^*****..**********.O^............*......********=@@Oo*............=@@`*****************,\]********\o\`*****[o\`*****\o\*****.....................................................................................................................=@@o*..*oo\ooooooooooOO^...O^........**=@@.....=@^...............
...................=@^....***..**......................*,oooooooOOOoooo***********O@@OOOOOOOOOOOOOO@@@@@@@O@@@...`.***********..**********.=@^.........*........********=@@Oo...............\@\********************,\\`*******[o]]*****[`**********......................................................................................................................=@@O^.*oooooooooooooooo^...OO........***@@.....=@^...............
...................=@^....*.*..**......................**ooooooOOOOOoooo**********O@@OOOOOOOOOOOOOO@OO@@@@O@@@.....***********..************@^................***********@@Oo................*OO\**.*******************,\]`********************..........................................................................................................................O@@Oo**oooo\*oo/ooooooo^...OO^.......***O@^....=@^...............
.................../@.........**........................*oooooooOOOOOOooo`*******=@@@OOOOOOOOOOOOOOOOOOOOO@@@@.....***********...**********.OO.......*......*************@@Oo..................,\O\...***************************************............................................................................................................................@@@@O**oooooooo=ooooooo....O@\.......***=@^....=@O...............
...................O@......*....**......................*oooooooOOOOOOOooo`******=@@@OOOOOOOOOOOOOOOOOOOOOO@@@.....************..**********.=@^......**......************O@O^....................*\O\*....*********************************.............................................................................................................................=@@@@O^*ooooo***\ooooooo....O@O*......***=@^....=@@...............
...................@@......**.............................*oooooooOOOOOOooo^*****o@@@OOOOOOOOOOOOOOOOOOOOOO@@@......***********..***********=@^......*=*....**..*********O@O^......................*,*..*.......*************************...............................................................................................................................@@@@@Oo*ooooo/^**\[ooooo....O@O^......***=@\....=@@...............
...................@O......*.***..*......................*=oooooOOOOOOOOOOooo**`=o@@@OOOOOOOOOOOOOOOOOOOOOO@@@......***********..************OO^......***....************=@@^........................................**************....**..............................................................................................................................=@@@@@@o^oooo=oo*****=oo\....=@@\......***=@O..../@@...............
...................@/......****...........................*ooooooOOOOOOOOOoooo/\=O@@OOOOOOOOOOOOOOOOOOOOOOO@@@......******=****...***********=O^......***....************=@@*..........................................................................................................................................................................................@@OO@@@Oooooo**********o^....=@@O.....****=@@....@@@...............
..................,@^........**...........................*=oooooOOOOOOOOOOOoooo/O@@OOOOOOOOOOOOOOOOOOOOOOO@@@......******\*`***..***********,@O*......=^....*************@@*.........................................................................................................................................................................................@@OoOO@@Ooo^*,************....*O@O`....****,@@...=@@@...............
..................=@^......****............................*ooooooOOOOOOOOOOOOOOOO@@OOOOOOOOOOOOOOOOOOOOOOO@@@......******,o^***..************OO*......*.*...*************@@^......................................................................................................................................................................................../@OoooO@@@OO^**************.....O@@^....*****@@...=@@@...............
..................O@.. ....****.............................,oooooOOOOOOOOOOOOOOOO@@@OOOOOOOOOOOOOOOOOOOOOO@@@......******o\/^**...***********=@\*......*`*.**************O@^......................................................................................................................................................................................./@OooooO@@@OO^**************.. ..=@@\....*****@@..,@O@@...............
..................@@... ....***..............................\ooooOOOOOOOOOOOOOOOO@@OOOOOOOOOOOOOOOOOOOOOOO@@@......******=ooo***..************OO**....*=\*..*************O@O......................................................................................................................................................................................@@OooooooO@@OOo**************.....=@@O....*****@@..=@o@@...............
..................@O.......*.................................*oooooOOOOOOOOOOOOOO@@@OOOOOOOOOOOOOOOOOOOOOOO@@@......******\ooo^**...***********o@^**...*\o^.**************o@O.....................................................................................................................................................................................@@OoooooooO@@OOO^*************.....=@@O....*****@@`,@@=@/...............
.................,@^.......*.**...............................,ooooOOOOOOOOOOOOOO@@@OOOOOOOOOOOOOOOOOOOOOOO@@@......******=ooo^**...************@O***..*\o\***************=@@...................................................................................................................................................................................,@@ooooooooooO@OOo`************......=@@O....*****@@^/@^O@^...............
.................=@^.... ....*.................................=ooooOOOOOOOOOOOOO@@@OOOOOOOOOOOOOOOOOOOOOOO@@@......******=ooo\***..************o@`***..=oo^***************@@..................................................................................................................................................................................=@OoooooooooooO@@Oo*************......=@@O...******@@O@.*@@............    
.................O@..... ....*.................................*ooooOOOOOOOOOOOOO@@@OOOOOOOOOOOOOOOOOOOOOO@@@@......*******oooo\**...***********,@O******ooo***************@@`................................................................................................................................................................................/@OoooooooooooooOOOo,************......=@@@...******O@@O**@/...........  ...
.................@O... ......*..................................=oooOOOOOOOOOOOOO@@@OOOOOOOOOOOOOOOOOOOOOO@@@@......******,ooooo***..************O@^*****=oo\**************O@^...................................................................................................................................../O.......................................,@OoooooooooooooooOOOo=***********.......=@@@^..******O@/...`....... ...  ....
................=@^........*....................................*oooOOOOOOOOOOOOO@@OOOOOOOOOOOOOOOOOOOOOOOO@@@......*******=oooo^**..************=@\******ooo^*************O@^...........................................................................................................................O\.....]\oO`....................................../@OoooooooooooooooooOO/\`**********.......=@@@O..****..,...........  ..........
................=@^.........*...................................*=oOOOOOOOOOOOOOO@@OOOOOOOOOOOOOOOOOOOOOOO@@@@......*******=ooooo***..************O@^*****=ooo*************=@\...........................................................................................................................\@@\/OO/........................................,@@OooooooooooooooooooOO^=^**********.......=@@@O................................
................/@...........*...........*.......................=ooOOOOOOOOOOOO@@@OOOOOOOOOOOOOOOOOOOOOOO@@@@......*******=ooooo^***.************=@O******ooo\************=@@...........................................................................................................................=@/[[........................................../@@@Ooooooooooooooooooooo^=^*********........=@^@O............]...................
................@@...........*...........**.....................*=oOOOOOOOOOOOOO@@@OOOOOOOOOOOOOOOOOOOOOOO@@@@......*******=ooooo\*****************O@^******ooo`***********=@@`..........................................................................................................................=@`..........................................,@@@@@Ooooooooooooooooooooo**^*********........=@^@@^.*******.*@/............*******
...............=@^..........*.............*......................*oOOOOOOOOOOOOO@@@OOOOOOOOOOOOOOOOOOOOOO@@@@@......********\ooOoo^****************=@O******=Oo\************O@^..........................................................................................................................=O........................................../@@@@@@OOooooooooooooooooooo**o********.........=@^O@^*******.*@@....**]/ooOOOOOOOOOO
...............=@^..... ....*.............**.....................*oOOOOOOOOOOOOO@@@OOOOOOOOOOOOOOOOOOOOOO@@@@@^.....********,ooOOOo^****************O@\******ooo^***********O@^....................................................................................................................................................................,@@@@@@@@@Ooo[`******=oooooooo**=*******..........=@^O@\******,@@O..*.*oOOOOO@@@@@@@@@@
...............@@....    ...**...................................*OOOOOOOOOOOOOO@@@OOOOOOOOOOOOOOOOOOOOOO@@@@@^.....*********oooOOO^*****************@@`*****=Ooo***********=@^.................................................................................................................................................................../@`.@@@@@@@O/`**********\ooooo=*.=*******..........@@^O@O*****.*@^..**ooOOOO@@@@@@@@@@@@
..............=@^... .. ....**............**.....................*oOOOOOOOOOOOOO@@OOOOOOOOOOOOOOOOOOOOOOO@@@@@^.....*********oooOOOO`****************=@O******ooo^**********=@\.*,`.***........................................................................................................................................................./@^...\@@@@@@O^**.....*****oooo/^*.=]*****...........O@^=@@*****=@@`..*=oOOOOOO@@@@@@@@@@@
..............=@^..  .. ....**............**......................oOOOOOOOOOOOO@@@OOOOOOOOOOOOOOOOOOOOOO@@@@@@^.....*********=oooOOOo*****************O@^*****=Oo^**********=@@...\OO`***.....................................................................................................................................................=@/.....=@@@@@@O^**.........*,ooooo*.=o^***............O@.=@@`****=@@....**[oooOOOOOOOOOOOoO
..............@@... .  .....*.............*.......................=OOOOOOOOOOOO@@@OOOOOOOOOOOOOOOOOOOOOO@@@@@@^.....**********oooOOOO*****************,O@^*****\O\**********,@@^...,@@@O`,`*................................................................................................................................................,@/........@@@@@@@\**..........*\oo/o*.=o^**.............@@..@@`***.*@^.......................
..............@/.....  .....................****..................=OOOOOOOOOOOO@@@OOOOOOOOOOOOOOOOOOOOOO@@@@@@^.....**********=ooOOOOo*..**************\@\`*****oO`**********@@^...*O@@@@@\]**............................................................................................................................................,@O`.........\@@@@@@O*............*oo\**.=o^**.............@^..@@^***=@@........................
.............=@`....    ....*..............*.****.................*OOOOOOOOOOOO@@@OOOOOOOOOOOOOOOOOOOOOO@@@@@@^.....***********ooOOOOO^*..**************\@O*****,o\**********O@^..*,@@@@@@@@@O]**.......................................................................................................................................,@@`...........,@@@@@@O*.............\o=**.=O`*....*........=@^..@@^**.*@^........................
.............@@....     ....*..............******..................oOOOOOOOOOOO@@OOOOOOOOOOOOOOOOOOOOOO@@@@@@@^....=\**********\ooOOOOo*..***************O@\*****\O\*********=@\*,/@@@OOOO@@@@@@@\**..................................................................................................................................,@O`..............@@@@@@O*.............,O^**.=O*....=^........O@...@@^**,@@.........................
............=@^.....   ......*.............*******.................\OOOOOOOOOOO@@OOOOOOOOOOOOOOOOOOOOOO@@@@@@@^.....O***********ooOOOOO^*..**************,O@\*****oo^********=@@@@@@OOOOOOOOO@@@@@@@\`**............................................................................................................................]@/`................O@@@@@O*..............\^**.=o....,O.........@O..,@@***O@^. .......................
............@@`..... .......................*******................=OOOOOOOOOOO@@OOOOOOOOOOOOOOOOOOOOOO@@@@@@@^.....O^**********oooOOOOo**..**************,O@^****,oo********=@@OOOOOOOOOOOOOOOOO@@@@@@O`**.......................................................................................................................]@/...................=@@@@@O*..............***..*.....O^........=@^..=@O*.*@@..........................
...........=@@......... ....*..............********................,OOOOOOOOOO@@@OOOOOOOOOOOOOOOOOOOOOO@@@@@@@^.....Oo**********=ooOOOOO^*...**************\O@^*****oo********@@@OOOOOOOOOOOOOOOOOOOO@@@@@@o`*..................................................................................................................=@/......................@@@@@O*...............=^.......OO.........@@...O@O**@@`..........................
...........=@^......... ....**.............*********...............*oOOOOOOOOO@@OOOOOOOOOOOOOOOOOOOOOO@@@@@@@@^.....\O***********oooOOOO\**...**************\@@^****=o\*******O@@OOOOOOOOOOOOOOOOOOOOOOOO@@@@@O\`............................................................................................................,O@`........................@@@@@O*...............*^......OO^........=@^...@@..*@^...........................
...........@@.......... ...................**********...............oOOOOOOOOO@@OOOOOOOOOOOOOOOOOOOOOO@@@@@@@@O......O\**********\ooOOOOO^**..**************,\@O^****=o^******O@@OOOOOOOOOOOOOOOOOOOOOOOOOOOO@@@@@@\*......................................................................................................,@@^..........................=@@@@@^....................../OO.........@@....@@^/@^............................
..........=@^. .............**............************..............=ooOOOOOOO@@OOOOOOOOOOOOOOOOOOOOOO@@@@@O@@@......OO***********\ooOOOOo**...**************,\@@\****\o`*****=@@OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@@@@@@\*................................................................................................/@@O@@..........................=@@@@@^.....................*OO`........=@^...=@O/@@.............................
..........=@`...............*.............************..............=oOOOOOOO@@@OOOOOOOOOOOOOOOOOOOOO@@@@@OO@@@......=O^**********,ooOOOOO^**...***************o@@]****oo`****=@@OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@@@@@@@O]*........................................................................................]@@OOOO@@..........................=@@@@@^.................../OOO^........=@/..../@O@/..............................
.........,@@............ ...**............*************.............=ooOOOOOO@@@OOOOOOOOOOOOOOOOOOOOO@@@@@OO@@@.......O^***********=oooOOOo**...****************oO@^****oo`****@@OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@@@@@@@@@@@\`................................................................................,O@@@OOOOO@@^.........................=@@@@O*...................OO@O........,@@....=@@@@...............................
.........=@^.......*... ....**............**************............=ooOOOOOO@@OOOOOOOOOOOOOOOOOOOOOO@@@@@OO@@@.......OO************oooOOOOo**...***************,oO@\`***oo`***O@OoooOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@@@@@@@@@@@@@@@\]`......................................................................./@@@O@@OOOOO@@@.........................,@@@@O*..................=@@O^........@@`....@@@/................................
........,@@... ....*........**............*******************.******,oooOOOOO@@OOOOOOOOOOOOOOOOOOOOOO@@@@OOO@@@.......=@^***********\oooOOOo`**...****************oO@O`***oo***O@@ooooooOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@@@@@@@O@@@@@@@@@@@@@]]`.............................................................]@@@OOOO@@OOOOOO@@..........................@@@@O*..................O@@^........O@^....=@@/.................................
......../@^... ....*.... ...**............*******************...***.*ooOOOOOO@@OOOOOOOOOOOOOOOOOOOOO@@@@@OOO@@@`.......@O^***********=oooOOOo***...****************oOO@]*,,oo**=@@ooooooooOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO@@@@@@@@@@@@@@@OOOOOO@@@@@@@@@O]..................................................../@@@OOOOOO@@OOOOOO@@^.........................@@@@O*.................=@@O........@@^....=@@`..................................
........@@.... ..............*............***************************oOOOOOO@@@OOOOOOOOOOOOOOOOOOOOO@@@@@OOO@@@^.......=@^***********,ooOOOOO^***..*****************\OO@O**=o\**@@OooooooooooOOOOOOOOOOOOOOOOOOOOOOOOOOO@@@@@@@@@@@@@@@OOOOOOOOOOOOOO@@@@@@@@@@@]]]]]....................................]@@@@@OOOOOOO@@@OOOOO@@^.........................O@@@/..................=@@`......./@^....]@/....................................
.......O@^.. .............................***************************oOOOOOO@@@OOOOOOOOOOOOOOOOOOOOO@@@@OOOO@@@^.......=@O************=ooOOOOO^**...*****************=OO@@`**o\*O@OoooooooooooooOOOOOOOOOOOOOOOOOOOOOOOO@@@@@@@@@@O@@@@OO@OOOOOOOOOOOO@@@OO@@@@@@@@@@@@@@@@@@@@O]]]]]]]]]`............]@@@@@@@@OOOOOOO@@@OOOOO@@@.........................=@@@^..................\@/......./@^..../@/.....................................
......=@@..................*.*............***************************oOOOOOO@@OOOOOOOOOOOOOOOOOOOOO@@@@@OOOO@@@O.......=@@^************oOOOOOOo***...*****************,OOO@O*=o\=@OoooooooooooooooOOOOOOOOOOOOOOOOOOOOOO@@@@@@@@@@O@@@OOO@OOOOOOOOOOOO@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@OOO@@OOOOOOO@@@OOOOOO@@.........................=@@@^..................=@^......@@^..../@/......................................
......@@^.........**.......*..............***************************oOOOOOO@@OOOOOOOOOOOOOOOOOOOOO@@@@OOOOO@@@@........@@\************=OOOOOOOo***...******************oOO@@\=o,@@ooooooooooooooooooOOOOOOOOOOOOOOOOOOO@@@@@@@@@OO@@@OO@@OOOOOOOOOOOOO@@@@@@@@@@@@@@@@@@@@@@@@@@OOOOOOOOOOooooOOOOOOOOOOOOOO@@OOOOOOO@@@OOOOOO@@^........................=@@O^..................=O.....,@@`...,@@`.......................................
.....=@@.........**........***............***************************=OOOOO@@@OOOOOOOOOOOOOOOOOOOOO@@@@OOOOO@@@@........@@@*************=OOOOOOO^***...******************oOOO@@^.@@oooooooooooooooooooooOOOOOOOOOOOOOOOOO@@@@@@@@O@@@@OO@@OOOOOOOOOOOO@@@@@@@@@@@@@@@@@@@OO@OOOOOO@@OOOooooooooooooOOOOOOOOOOO@@OOOOOO@@@@OOooO@@^........................=@@O*..................//..../@@....=@/.........................................
....=@@^. . .....*.*.......**.............***************************=OOOOO@@@OOOOOOOOOOOOOOOOOOOO@@@@@OOOOO@@@@........@@@O*************oOOOOOOO^***...******************\OOOO@\O@OooooooooooooooooooooooooOOOOOOOOOOOOO@@@@@@@OO@@@OOO@@OOOOOOOOOOOO@@@@@@@@@@@@@@@@@@@@OOO@OO@OOooooooooooooooooooOoooOOOOO@@OOOOOOO@@@OOooOO@^........................=@@^..................=O^.../@^...,@@`..........................................
..../@O.........*.**.......***............***************************=OOOOO@@OOOOOOOOOOOOOOOOOOOOO@@@@OOOOOO@@@@.......,@@@@`************=OOOOOOOo`***...******************[oOOo[@@OooooooooooooooooooooooooOOOOOOOOOOOOO@@@@@@@OO@@@OOO@@OOOOOOOOOOOO@@@@@@@@@@@@@@@@@@@OOOOOOOooooooooooooooooooooooooooOOOO@@@OOOOOO@@@OOooOO@O........................@@O.................../@*.,@@`..,/@`............................................
...=@@^..........*.........***............***************************=OOOOO@@OOOOOOOOOOOOOOOOOOOO@@@@@OOOOOO@@@@^......=@@@@O*************oOOOOOOOo]***...*******************oOo.=@@OoooooooooooooooooooooOOOOOOOOOOOOOOO@@@@@@@O@@@@OOO@@OOOOOOOOOOO@@@@@@@@@@@@@@@@@OOOOOOooooooooooooooooooooooooooooooooOOO@@OOOOOO@@@OooooO@@........................\/...................=@/.O@/..,O@`..............................................
...@@^..........*.*........***............***************************=OOOOO@@OOOOOOOOOOOOOOOOOOOO@@@@OOOOOOO@@@@^......=@@@@@^************,oOOOOOOOoo`*....*******************\o..@@O@OoooooooooooooooOoOOOOOOOOOOOOOOOOOO@@@@@OO@@@@@@@@@@@@@@@@@@OOO[[[[[[[[[OOO@@@@@@@@@OOOooooooooooooooooooooooooooooooooO@@OOOOOO@@@@ooooO@@^............................................=@o@@`,/O[.................................................
../@O.... .....**..*......****............***************************=OOOO@@@OOOOOOOOOOOOOOOOOOOO@@@@OOOOOOO@@@@^......=@@@@@O`************=OOOOOOOOoo***...********************..=@OoO@@OoooooooooOoOOOOOOOOOOOOoOOOOoOOO@@@@@@@@@@@@[[[...................................[[\@@@@Oooo[ooooooooooooooooooooooO@@@OOOOO@@@@ooooOO@^......................,O@O*.................@@@@O[`....................................................
.,@O^.. . ....***.*.......****............***************************=OOOO@@@OOOOOOOOOOOOOOOOOOO@@@@@OOOOOOO@@@@O......=@@@@@@^*************oOOOOOOOOo\***...*******************..=@OoooO@@OoooOoOoOooooOoooOOOOOO@@@@@@@@@@/[`....................................................[\@@@OooooooooooooooooooooOOO@@OOOOO@@@@OooooO@O........... ..........=@@O*................O@/.........................................................
.@@`.. .. .....*..........***.............***************************=OOOO@@OOOOOOOOOOOOOOOOOOOO@@@@@OOOOOOO@@@@@......@@@@@@@@^************=OOOOOOOOOO^***...******************..=@@ooooooO@@OOOOOOOOOOOO@@@@@@@@@O[[`................................................................,\@@Oo\oooooooooooooooOoO@@@OOOO@@O@@ooooO@@......................=@@/....,]]]]]]]]]]]/@^..........................................................
=@^..... ......*..*......*****............***************************=oOO@@@OOOOOOOOOOOOOOOOOOOO@@@@OOOOOOO@@@@@@.....,@@@@@@@@O*************=OOOOOOOOOO****...*****************...@@OoOOOOOO@@@@@@OOOOO[[[[*[[...........................................................................,\@@OooooooooooooooOOO@@@OOOO@@^@@ooooO@@^.....................@@O^.*OO[[...,[[[[[[[[[\OOO\]]...................................................
@^.....  ....*.**........****............*****************************oOO@@@OOOOOOOOOOOOOOOOOOO@@@@OOOOOOOO@@@@@@.....=@@@@@@@@@O*************oOOOOOOOOOO^***...****************...=@OooooooooooO@@^.........................................................................................,@@OooooooooooooOOO@@@OOOO@@^O@OooooO@^. ............... ..,@@OOO`........................,OO^...............................................


*/

_matrix_t Matrix=
{    
    .Create=MatCreate,
    .Delete=MatDelete,
    .SetVal=MatSetVal,
    .Dump=MatDump,
    .Zeros=MatZeros,
    .Eye=MatEye,

    .Add=MatAdd,
    .Sub=MatSub,
    .Mul=MatMul,
    .Trans=MatTrans,
    .Det=MatDet,
    .Adj=MatAdj,
    .Inv=MatInv,
};
