/*
 * file.c
 *
 *  Created on: 2021Äê1ÔÂ19ÈÕ
 *      Author: 936305695
 */
#include "file.h"

//static FATFS fs[2];

int f_scanf (
    FIL* fp,            /* Pointer to the file object */
    const TCHAR* fmt,   /* Pointer to the format string */
    ...                 /* Optional arguments... */
){
    return 0;
}

uint8_t FileSys_Init()
{
    uint8_t res = 0;
    //res = f_mount(&fs[0],"0",1); /*¹ÒÔØSD¿¨*/
    return res;
}

uint FileSys_PathIsExists(sint8_t *path)
{
    uint8_t res = 0;
    bool is_exist = false;

    FIL *fp = NULL;

    res += f_open(fp,path,FA_OPEN_ALWAYS);

    if(res == FR_OK)
        is_exist = true;
    else
    {
       DIR *dp = NULL;
       res = f_opendir(dp,path);
       if(res == FR_OK)
           is_exist = true;
       else
           is_exist = false;

    }

    return is_exist;
}

bool FileSys_Open(FIL *fp,sint8_t *path,sint8_t *flags)
{
    sint8_t *p = flags;
    uint8_t mode = 0;


    while(*p)
    {
        switch(*p)
        {
            case 'r':
                mode |= FA_READ;
            break;
            case 'w':
                mode |= FA_WRITE;
                break;
            case '+':
                mode |= FA_CREATE_ALWAYS;
                break;
            default:
                break;
        }
    }

    bool res = f_open(fp,path,mode);

    return !res;

}
bool FileSys_Close(FIL *fp)
{
    return !f_close(fp);
}

uint FileSys_Write(FIL *fp,sint8_t *str)
{
    uint slen = 0;

    sint8_t *p = str;

    while(*p++)
        slen++;

    uint wlen = 0;
    f_write(fp,str,slen,&wlen);

    return wlen;
}
uint FileSys_Read(FIL *fp,sint8_t *buf,uint n)
{
    uint wlen = 0;
    f_read(fp,buf,n,&wlen);
    return true;
}

uint FileSys_FastWrite(sint8_t *path,sint8_t *str)
{
    FIL *fp = NULL;
    if(FileSys_PathIsExists(path))
    {
        f_open(fp,path,1);//...

        sint8_t *p = str;
        uint slen = 0;

        while(*p++)
            slen++;

        uint wlen = 0;

        f_write(fp,str,slen,&wlen);

        f_close(fp);

        return wlen;
    }
    else
        return 1;
}
uint FileSys_FastRead(sint8_t *path,sint8_t *buf,uint n)
{
    FIL *fp = NULL;
    if(FileSys_PathIsExists(path))
    {
        f_open(fp,path,1);//...

        uint wlen = 0;

        f_read(fp,buf,n,&wlen);

        f_close(fp);

        return wlen;
    }
    else
        return 1;
}
uint FileSys_MKDir(sint8_t *dir)
{
    return 0;
}
uint FileSys_CreateFile(sint8_t *path,sint8_t *file)
{
    return 0;
}
