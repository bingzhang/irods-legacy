/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/
/* regDataObj.c
 */

#include "regDataObj.h"
#include "icatHighLevelRoutines.h"

int
rsRegDataObj (rsComm_t *rsComm, dataObjInfo_t *dataObjInfo, 
dataObjInfo_t **outDataObjInfo)
{
    int status;
    rodsServerHost_t *rodsServerHost = NULL;


    status = getAndConnRcatHost (rsComm, MASTER_RCAT, dataObjInfo->objPath,
      &rodsServerHost);
    if (status < 0) {
       return(status);
    }
    
    *outDataObjInfo = (dataObjInfo_t *) malloc (sizeof (dataObjInfo_t));

    memset (*outDataObjInfo, 0, sizeof (dataObjInfo_t));

    if (rodsServerHost->localFlag == LOCAL_HOST) {
#ifdef RODS_CAT
        status = _rsRegDataObj (rsComm, dataObjInfo);
	**outDataObjInfo = *dataObjInfo;
#else
        status = SYS_NO_RCAT_SERVER_ERR;
#endif
    } else {
        status = rcRegDataObj (rodsServerHost->conn, dataObjInfo, 
	  outDataObjInfo);
    }

    return (status);
}

int
_rsRegDataObj (rsComm_t *rsComm, dataObjInfo_t *dataObjInfo)
{
    int status;
#ifdef RODS_CAT
    status = chlRegDataObj (rsComm, dataObjInfo);
    return (status);
#else
    return (SYS_NO_RCAT_SERVER_ERR);
#endif

}

int
svrRegDataObj (rsComm_t *rsComm, dataObjInfo_t *dataObjInfo)
{
    int status;
    rodsServerHost_t *rodsServerHost = NULL;

    if (dataObjInfo->specColl != NULL) {
	rodsLog (LOG_NOTICE,
	 "svrRegDataObj: Reg path %s is in spec coll",
	  dataObjInfo->objPath);
	return (SYS_REG_OBJ_IN_SPEC_COLL);
    }

    status = getAndConnRcatHost (rsComm, MASTER_RCAT, dataObjInfo->objPath,
      &rodsServerHost);
    if (status < 0) {
       return(status);
    }

    if (rodsServerHost->localFlag == LOCAL_HOST) {
#ifdef RODS_CAT
        status = _rsRegDataObj (rsComm, dataObjInfo);
#else
        status = SYS_NO_RCAT_SERVER_ERR;
#endif
    } else {
        dataObjInfo_t *outDataObjInfo = NULL;
        status = rcRegDataObj (rodsServerHost->conn, dataObjInfo,
          &outDataObjInfo);
        if (status >= 0) {
            dataObjInfo->dataId = outDataObjInfo->dataId;
            free (outDataObjInfo);
        }
    }

    return (status);
}

