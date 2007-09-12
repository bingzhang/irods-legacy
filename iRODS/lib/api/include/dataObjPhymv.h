/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/
/* dataObjPhymv.h - This dataObj may be generated by a program or script
 */

#ifndef DATA_OBJ_PHYMV_H
#define DATA_OBJ_PHYMV_H

/* This is a Object File I/O API call */

#include "rods.h"
#include "rcMisc.h"
#include "procApiRequest.h"
#include "apiNumber.h"
#include "initServer.h"
#include "dataObjWrite.h"
#include "dataObjClose.h"
#include "dataCopy.h"

#if defined(RODS_SERVER)
#define RS_DATA_OBJ_PHYMV rsDataObjPhymv
/* prototype for the server handler */
int
rsDataObjPhymv (rsComm_t *rsComm, dataObjInp_t *dataObjInp, 
transStat_t **transStat);
int
_rsDataObjPhymv (rsComm_t *rsComm, dataObjInp_t *dataObjInp,
dataObjInfo_t *srcDataObjInfoHead, rescGrpInfo_t *destRescGrpInfo,
transStat_t *transStat, int multiCopyFlag);
#else
#define RS_DATA_OBJ_PHYMV NULL
#endif

#ifdef  __cplusplus
extern "C" {
#endif

/* prototype for the client call */
int
rcDataObjPhymv (rcComm_t *conn, dataObjInp_t *dataObjInp);

#ifdef  __cplusplus
}
#endif

#endif	/* DATA_OBJ_PHYMV_H */
