/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/

/* irodsFs.c - The main program of the iRods/Fuse server. It is to be run to
 * serve a single client 
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <assert.h>
#include "irodsFs.h"
#include "iFuseOper.h"
#include "iFuseLib.h"

/* some global variables */
iFuseDesc_t IFuseDesc[MAX_IFUSE_DESC];

iFuseConn_t DefConn;
rodsEnv MyRodsEnv;

static struct fuse_operations irodsOper =
{
  .getattr = irodsGetattr,
  .readlink = irodsReadlink,
  .readdir = irodsReaddir,
  .mknod = irodsMknod,
  .mkdir = irodsMkdir,
  .symlink = irodsSymlink,
  .unlink = irodsUnlink,
  .rmdir = irodsRmdir,
  .rename = irodsRename,
  .link = irodsLink,
  .chmod = irodsChmod,
  .chown = irodsChown,
  .truncate = irodsTruncate,
  .utimens = irodsUtimens,
  .open = irodsOpen,
  .read = irodsRead,
  .write = irodsWrite,
  .statfs = irodsStatfs,
  .release = irodsRelease,
  .fsync = irodsFsync,
  .flush = irodsFlush,
};

void usage ();

/* Note - fuse_main parses command line options 
 * static const struct fuse_opt fuse_helper_opts[] = {
    FUSE_HELPER_OPT("-d",          foreground),
    FUSE_HELPER_OPT("debug",       foreground),
    FUSE_HELPER_OPT("-f",          foreground),
    FUSE_HELPER_OPT("-s",          singlethread),
    FUSE_HELPER_OPT("fsname=",     nodefault_subtype),
    FUSE_HELPER_OPT("subtype=",    nodefault_subtype),

    FUSE_OPT_KEY("-h",          KEY_HELP),
    FUSE_OPT_KEY("--help",      KEY_HELP),
    FUSE_OPT_KEY("-ho",         KEY_HELP_NOHEADER),
    FUSE_OPT_KEY("-V",          KEY_VERSION),
    FUSE_OPT_KEY("--version",   KEY_VERSION),
    FUSE_OPT_KEY("-d",          FUSE_OPT_KEY_KEEP),
    FUSE_OPT_KEY("debug",       FUSE_OPT_KEY_KEEP),
    FUSE_OPT_KEY("fsname=",     FUSE_OPT_KEY_KEEP),
    FUSE_OPT_KEY("subtype=",    FUSE_OPT_KEY_KEEP),
    FUSE_OPT_END
};

static void usage(const char *progname)
{
    fprintf(stderr,
            "usage: %s mountpoint [options]\n\n", progname);
    fprintf(stderr,
            "general options:\n"
            "    -o opt,[opt...]        mount options\n"
            "    -h   --help            print help\n"
            "    -V   --version         print version\n"
            "\n");
}

*/

int 
main (int argc, char **argv)
{

    int status;
    rErrMsg_t errMsg;
    rodsArguments_t myRodsArgs;
    char *optStr;
    rodsPathInp_t rodsPathInp;
    pthread_t connManagerThr;

    optStr = "h";

    status = parseCmdLineOpt (argc, argv, optStr, 0, &myRodsArgs);

    if (status < 0) {
        printf("Use -h for help.\n");
        exit (1);
    }
    if (myRodsArgs.help==True) {
       usage();
       exit(0);
    }

    status = getRodsEnv (&MyRodsEnv);

    if (status < 0) {
        rodsLogError(LOG_ERROR, status, "main: getRodsEnv error. ");
        exit (1);
    }

    memset (&DefConn, 0, sizeof (DefConn));

    pthread_mutex_init (&DefConn.lock, NULL);

    status = getIFuseConn (&DefConn, &MyRodsEnv);
    relIFuseConn (&DefConn);

    if (status < 0) {
        rodsLogError (LOG_ERROR, status, "rcConnect failure");
        exit (2);
    }

    initIFuseDesc ();

    status = fuse_main (argc, argv, &irodsOper, NULL);

    rcDisconnect (DefConn.conn);

    if (status < 0) {
        exit (3);
    } else {
        exit(0);
    }
}

void
usage ()
{
   char *msgs[]={
   "Usage : irodsFs [-h]",
"Single user iRods/Fuse server",
"Options are:",
" -h  this help",
""};
    int i;
    for (i=0;;i++) {
        if (strlen(msgs[i])==0) return;
         printf("%s\n",msgs[i]);
    }
}


