/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/
/* This is script-generated code.  */ 
/* See dataObjClose.h for a description of this API call.*/

#include "dataObjClose.h"

int
rcDataObjClose (rcComm_t *conn, dataObjCloseInp_t *fileCloseInp)
{
    int status;
    status = procApiRequest (conn, DATA_OBJ_CLOSE_AN,  fileCloseInp, NULL, 
        (void **) NULL, NULL);

    return (status);
}
