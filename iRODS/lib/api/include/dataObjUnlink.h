/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/
/* dataObjUnlink.h
 */

#ifndef DATA_OBJ_UNLINK_H
#define DATA_OBJ_UNLINK_H

/* This is a high level type API call */

#include "rods.h"
#include "rcMisc.h"
#include "procApiRequest.h"
#include "apiNumber.h"
#include "initServer.h"
#include "dataObjInpOut.h"
#include "fileUnlink.h"

#if defined(RODS_SERVER)
#define RS_DATA_OBJ_UNLINK rsDataObjUnlink
/* prototype for the server handler */
int
rsDataObjUnlink (rsComm_t *rsComm, dataObjInp_t *dataObjUnlinkInp);
int
dataObjUnlinkS (rsComm_t *rsComm, dataObjInp_t *dataObjUnlinkInp,
dataObjInfo_t *dataObjInfo);
int
l3Unlink (rsComm_t *rsComm, dataObjInfo_t *dataObjInfo);
int
_rsDataObjUnlink (rsComm_t *rsComm, dataObjInp_t *dataObjUnlinkInp,
dataObjInfo_t *dataObjInfoHead);
int
rsMvDataObjToTrash (rsComm_t *rsComm, dataObjInp_t *dataObjInp,
dataObjInfo_t *dataObjInfoHead);
#else
#define RS_DATA_OBJ_UNLINK NULL
#endif

#ifdef  __cplusplus
extern "C" {
#endif

/* prototype for the client call */
int
rcDataObjUnlink (rcComm_t *conn, dataObjInp_t *dataObjUnlinkInp);

/* rcDataObjUnlink - Unlink a iRods data object.
 * Input - 
 *   rcComm_t *conn - The client connection handle.
 *   dataObjInp_t *dataObjInp - generic dataObj input. Relevant items are:
 *	objPath - the path of the data object.
 *
 * OutPut - 
 *   int status - The status of the operation.   
 */

#ifdef  __cplusplus
}
#endif

#endif	/* DATA_OBJ_UNLINK_H */
