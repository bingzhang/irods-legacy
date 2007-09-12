/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/
/* dataObjWrite.h - This dataObj may be generated by a program or script
 */

#ifndef DATA_OBJ_WRITE_H
#define DATA_OBJ_WRITE_H

/* This is a high level type API call */

#include "rods.h"
#include "procApiRequest.h"
#include "apiNumber.h"
#include "initServer.h"
#include "fileWrite.h"

typedef struct {
    int l1descInx;
    int len;
} dataObjWriteInp_t;
    
#define dataObjWriteInp_PI "int dataObjInx; int len;"

#if defined(RODS_SERVER)
#define RS_DATA_OBJ_WRITE rsDataObjWrite
/* prototype for the server handler */
int
rsDataObjWrite (rsComm_t *rsComm, dataObjWriteInp_t *dataObjWriteInp, 
bytesBuf_t *dataObjWriteInpBBuf);
int
l3Write (rsComm_t *rsComm, int l1descInx, int len,                       
bytesBuf_t *dataObjWriteInpBBuf);
int
_l3Write (rsComm_t *rsComm, int destRescTypeInx, int l3descInx,
void *buf, int len);
#else
#define RS_DATA_OBJ_WRITE NULL
#endif

#ifdef  __cplusplus
extern "C" {
#endif

/* prototype for the client call */
int
rcDataObjWrite (rcComm_t *conn, dataObjWriteInp_t *dataObjWriteInp,
bytesBuf_t *dataObjWriteInpBBuf);

#ifdef  __cplusplus
}
#endif

#endif	/* DATA_OBJ_WRITE_H */
