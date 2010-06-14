/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/
/* procStat.h - This dataObj may be generated by a program or script
 */

#ifndef PROC_STAT_H
#define PROC_STAT_H

/* This is a Object File I/O API call */

#include "rods.h"
#include "procApiRequest.h"
#include "apiNumber.h"
#include "initServer.h"

typedef struct {
    char addr[LONG_NAME_LEN];       /* if non empty, stat at this addr */
    char rodsZone[NAME_LEN];	    /* the zone */ 
    keyValPair_t condInput;
} procStatInp_t;

#define ProcStatInp_PI "str addr[LONG_NAME_LEN];str rodsZone[NAME_LEN];struct KeyValPair_PI;"

#if defined(RODS_SERVER)
#define RS_PROC_STAT rsProcStat
/* prototype for the server handler */
int
rsProcStat (rsComm_t *rsComm, procStatInp_t *procStatInp, 
genQueryOut_t **procStatOut);
int
_rsProcStat (rsComm_t *rsComm, procStatInp_t *procStatInp,
genQueryOut_t **procStatOut);
int
_rsProcStatAll (rsComm_t *rsComm, procStatInp_t *procStatInp, 
genQueryOut_t **procStatOut);
int
localProcStat (rsComm_t *rsComm, procStatInp_t *procStatInp,
genQueryOut_t **procStatOut);
int
remoteProcStat (rsComm_t *rsComm, procStatInp_t *procStatInp,
genQueryOut_t **procStatOut, rodsServerHost_t *rodsServerHost);
int
initProcStatOut (genQueryOut_t **procStatOut, int numProc);
#else
#define RS_PROC_STAT NULL
#endif

#ifdef  __cplusplus
extern "C" {
#endif

/* prototype for the client call */
/* rcProcStat - Get the stat of irods agents running in the federation.
 * By default, the stat of the irods agents on the icat server is listed.
 * 
 * Input -
 *   rcComm_t *conn - The client connection handle.
 *   procStatInp_t *procStatInp :
 *      addr - the IP address of the server where the stat should be done.
 * 	    A zero len addr means no input.
 *      rodsZone - the zone name for this stat.  A zero len rodsZone means
 *          the stat is to be done in the local zone.
 *      condInput - conditional Input
 *          RESC_NAME_KW - "value" - do the stat on the server where the 
 *	    Resource is located. 
 *	    ALL_KW (and zero len value) - stat for all servers in the
 *	     fedration.  
 *   return value - The status of the operation.
 */

int
rcProcStat (rcComm_t *conn, procStatInp_t *procStatInp,
genQueryOut_t **procStatOut);
#ifdef  __cplusplus
}
#endif

#endif	/* PROC_STAT_H */
