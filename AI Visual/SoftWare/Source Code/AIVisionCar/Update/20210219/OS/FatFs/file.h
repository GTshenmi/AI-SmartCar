/*
 * file.h
 *
 *  Created on: 2021Äê1ÔÂ19ÈÕ
 *      Author: 936305695
 */

#ifndef OS_FATFS_FILE_H_
#define OS_FATFS_FILE_H_

#include "ff.h"
#include "device.h"

typedef struct
{
    uint (*exist)(sint8_t *path);
    uint (*isfile)(sint8_t *path);
    uint (*isdir)(sint8_t *path);
    uint (*join)(char *buf,uint buflen,uint pathnum,...);
}path_t;

typedef struct
{
    uint8_t (*init)(void);
    uint (*close)(FIL *fp);
    void (*getcwd)(void);
    void (*listdir)(sint8_t *path);
    void (*lseek)(FIL *fp,uint32_t pos,uint8_t how);
    uint (*mkdir)(sint8_t *path,uint8_t mode);
    uint (*open)(FIL *fp,sint8_t *path,sint8_t *flags);
    uint (*read)(FIL *fp,sint8_t *buf,uint n);
    uint (*remove)(sint8_t *path);
    uint (*rename)(sint8_t *src,sint8_t *dst);
    uint (*rmdir)(sint8_t *path);
    uint (*walk)(sint8_t *top,...);
    uint (*write)(FIL *fp,sint8_t *str);

    uint (*fastWrite)(sint8_t *path,sint8_t *str);
    uint (*fastRead)(sint8_t *path,sint8_t *buf,uint n);

}file_t;

uint8_t FileSys_Init(void);
uint FileSys_PathIsExists(sint8_t *path);


#endif /* OS_FATFS_FILE_H_ */
