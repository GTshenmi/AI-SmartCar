/*
 * file.c
 *
 *  Created on: 2021��1��19��
 *      Author: 936305695
 */
#include "file.h"

static FATFS fs[2];

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
    res = f_mount(&fs[0],"0:",1);
    if(res==0X0D)
    {
        res=f_mkfs("0:",1,4096);
        if (res == 0)
        {
            f_setlabel((const TCHAR*) "0:SDCard");
        }

    }
    return res;
}

uint FileSys_PathIsExists(sint8_t *path)
{
    uint8_t res = 0;
    bool is_exist = false;

    FIL fp;

    res += f_open(&fp,path,FA_OPEN_EXISTING);

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

    f_close(&fp);

    return is_exist;
}

uint FileSys_Open(FIL *fp,sint8_t *path,sint8_t *flags)
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
uint FileSys_Close(FIL *fp)
{
    return !!f_close(fp);
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

uint FileSys_FastWrite (sint8_t *path, sint8_t *str)
{
    static _Bool firstOpen = 0;
    static FIL fp;

    if(!firstOpen){
        uint8_t res = f_open(&fp,path,FA_WRITE | FA_CREATE_NEW);
        if(res)
            res = f_open(&fp,path,FA_WRITE);
        if(res)
            return 0;
    }

    if(!firstOpen)
        f_lseek(&fp,fp.fsize);

    uint result = f_puts(str,&fp);

    f_sync(&fp);

    firstOpen = 1;
    return result;
}

uint FileSys_FastRead(sint8_t *path,sint8_t *buf,uint n)
{
    FIL fp;

    uint8_t res = f_open(&fp, path, FA_READ);
    if(res)
        return 1;

    uint wlen = 0;

    f_read(&fp, buf, n, &wlen);

    f_close(&fp);

    return wlen;

}

uint FileSys_MKDir(sint8_t *dir)
{
    f_mkdir(dir);
    return 0;
}

uint FileSys_CreateFile(sint8_t *path,sint8_t *file)
{
    FIL fp;

    uint8_t res = f_open(&fp, path, FA_CREATE_ALWAYS);
    if (res != 0)
        return 1;

    f_close(&fp);

    return 0;
}

file_t SD = {
        .init = FileSys_Init,
        .open = FileSys_Open,
        .close = FileSys_Close,
        .read = FileSys_Read,
        .write = FileSys_Write,
        .mkdir = FileSys_MKDir,

        .fastRead = FileSys_FastRead,
        .fastWrite = FileSys_FastWrite,

};