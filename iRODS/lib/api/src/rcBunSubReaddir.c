/* This is script-generated code.  */ 
/* See bunSubReaddir.h for a description of this API call.*/

#include "bunSubReaddir.h"

int
rcBunSubReaddir (rcComm_t *conn, bunSubFdOprInp_t *bunSubReaddirInp,
rodsDirent_t **rodsDirent)
{
    int status;
    status = procApiRequest (conn, BUN_SUB_READDIR_AN, bunSubReaddirInp, NULL, 
        (void **) rodsDirent, NULL);

    return (status);
}
