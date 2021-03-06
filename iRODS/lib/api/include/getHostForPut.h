/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/
/* dataObjPut.h - This dataObj may be generated by a program or script
 */

#ifndef GET_HOST_FOR_PUT_H
#define GET_HOST_FOR_PUT_H

/* This is a Object File I/O call */

#include "rods.h"
#include "rcMisc.h"
#include "procApiRequest.h"
#include "apiNumber.h"
#include "initServer.h"
#include "dataObjWrite.h"
#include "dataObjClose.h"

#define THIS_ADDRESS	"thisAddress"	/* a returned value for outHost.
					 * Just use the address of this conn */
#if defined(RODS_SERVER)
#define RS_GET_HOST_FOR_PUT rsGetHostForPut
/* prototype for the server handler */
int
rsGetHostForPut (rsComm_t *rsComm, dataObjInp_t *dataObjInp, 
char **outHost);
#else
#define RS_GET_HOST_FOR_PUT NULL
#endif

#ifdef  __cplusplus
extern "C" {
#endif

/* prototype for the client call */
/* rcGetHostForPut - get the best host the put operation.
 * Input -
 *   rcComm_t *conn - The client connection handle.
 *   dataObjInp_t *dataObjInp - generic dataObj input. Relevant items are:
 *      objPath - Optional, can be the path of the target collection.
 *      condInput - conditional Input
 *          FORCE_FLAG_KW - overwrite an existing data object
 *	    ALL_KW - update all copies.
 *          REPL_NUM_KW  - "value" = The replica number of the copy to
 *              upload.
 *          DEST_RESC_NAME_KW - "value" = The destination Resource. 
 *	    DEF_RESC_NAME_KW - "value" - The default dest resource. Only used
 *            to create a new file but no overwite existing file.
 *   return value - The status of the operation.
 *	    char **outHost - the address of the best host
 */

int
rcGetHostForPut (rcComm_t *conn, dataObjInp_t *dataObjInp,
char **outHost);
#ifdef  __cplusplus
}
#endif

#endif	/* GET_HOST_FOR_PUT_H */
