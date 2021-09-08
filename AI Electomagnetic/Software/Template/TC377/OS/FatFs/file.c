/*
 * file.c
 *
 *  Created on: 2021Äê1ÔÂ19ÈÕ
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

    if(res == 0)
    {
        SD.isInitFlag = true;
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

#define FileSysPathLen 50

typedef struct {
    FIL fp;
    char fileName[FileSysPathLen];
} File;

uint FileSys_FastWrite(const sint8_t * path, sint8_t *str) {

    static File file[10];

    _Bool firstOpen = 1;
    char finalName[FileSysPathLen];

    int filePointer = 0;
    while (filePointer < 10) {
        if (file[filePointer].fileName[0]) {
            if (!strcmp(file[filePointer].fileName, path)) {
                firstOpen = 0;
                break;
            }
        }
        filePointer++;
    }

    if (firstOpen) {
        char myPathBuffer[FileSysPathLen];
        char * myPath = myPathBuffer;
        strcpy(myPath,path);
        uint8_t res;
        char suffix[10];
        const char *name = myPath;

        filePointer = 0;
        while (filePointer < 10) {
            if (!file[filePointer].fileName[0]) {
                break;
            }
            filePointer++;
        }
        strcpy(file[filePointer].fileName, myPath);

        uint8_t counter = 0;

        do {
            myPath++;
        } while (*myPath != '.');
        strcpy(suffix, myPath + 1);
        *myPath = '\0';

        uint8_t test = 100;
        do {
            sprintf(finalName, "%s-%05d.%s", name, counter++, suffix);
            res = f_open(&file[filePointer].fp, finalName, FA_WRITE | FA_CREATE_NEW);
        } while (test-- && res);

        if (res)
            res = f_open(&file[filePointer].fp, myPath, FA_WRITE);

        if (res)
            return 1;

        f_lseek(&file[filePointer].fp, file[filePointer].fp.fsize);
    }

    uint result = f_puts(str, &file[filePointer].fp);

    f_sync(&file[filePointer].fp);

    return result;
}


//uint FileSys_FastWrite (sint8_t *path, sint8_t *str)
//{
//    static _Bool firstOpen = 1;
//    static FIL fp;
//    char finalName[25];
//
//    if (firstOpen)
//    {
//        uint8_t test = 100;
//        uint8_t res;
//        char suffix[10];
//        const char * name = path;
//
//        uint8_t counter = 0;
//
//        do
//        {
//            path++;
//        }while (*path != '.');
//        strcpy(suffix, path + 1);
//        *path = '\0';
//
//        do
//        {
//            sprintf(finalName, "%s-%05d.%s", name, counter++, suffix);
//            res = f_open(&fp, finalName, FA_WRITE | FA_CREATE_NEW);
//        }while (test-- && res);
//
//        if (res)
//            res = f_open(&fp, path, FA_WRITE);
//        firstOpen = 0;
//
//        if (res)
//            return 0;
//
//        f_lseek(&fp, fp.fsize);
//    }
//
//    uint result = f_puts(str, &fp);
//
//    f_sync(&fp);
//
//    return result;
//}

uint FileSys_FastRead(const sint8_t *path,sint8_t *buf,uint n)
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

uint8_t FileSys_IsInit()
{
    return SD.isInitFlag;
}

file_t SD = {
        .isInitFlag = false,
        .isInit = FileSys_IsInit,
        .init = FileSys_Init,
        .open = FileSys_Open,
        .close = FileSys_Close,
        .read = FileSys_Read,
        .write = FileSys_Write,
        .mkdir = FileSys_MKDir,

        .fastRead = FileSys_FastRead,
        .fastWrite = FileSys_FastWrite,

};
