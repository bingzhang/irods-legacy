/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/
/* 
 * irsync - The irods put utility
*/

#include "rodsClient.h"
#include "parseCommandLine.h"
#include "rodsPath.h"
#include "rsyncUtil.h"
void usage ();


main(int argc, char **argv) {
    int status;
    rodsEnv myEnv;
    rErrMsg_t errMsg;
    rcComm_t *conn;
    rodsArguments_t myRodsArgs;
    char *optStr;
    rodsPathInp_t rodsPathInp;
    objType_t srcType, destType;
    int nArgv;
    int i;
    

    optStr = "ahrR:svV";
   
    status = parseCmdLineOpt (argc, argv, optStr, 0, &myRodsArgs);

    if (status < 0) {
	printf("use -h for help.\n");
        exit (1);
    }

    if (myRodsArgs.help==True) {
       usage();
       exit(0);
    }

    status = getRodsEnv (&myEnv);
    if (status < 0) {
        rodsLogError (LOG_ERROR, status, "main: getRodsEnv error. ");
        exit (1);
    }

    nArgv = argc - optind;

    if (nArgv < 2) {      /* must have at least 2 input */
        usage (argv[0]);
        exit (1);
    }

    for (i = 2; i <= nArgv; i++) {
        if (strcmp (argv[argc-i], "i:") == 0) {
	    srcType = UNKNOWN_OBJ_T;
	    strcpy (argv[argc-i], ".");
        } else if (strncmp (argv[argc-i], "i:", 2) == 0) {
            srcType = UNKNOWN_OBJ_T;
            strcpy (argv[argc-i], argv[argc-i] + 2);
        } else {
            srcType = UNKNOWN_FILE_T;
        }
    }

    if (strcmp (argv[argc-1], "i:") == 0) {
        destType = UNKNOWN_OBJ_T;
        strcpy (argv[argc-1], ".");
    } else if (strncmp (argv[argc-1], "i:", 2) == 0) {
        destType = UNKNOWN_OBJ_T;
        strcpy (argv[argc-1], argv[argc-1] + 2);
    } else {
        destType = UNKNOWN_FILE_T;
    }

    if (srcType == UNKNOWN_FILE_T && destType == UNKNOWN_FILE_T) {
	rodsLog (LOG_ERROR,
	  "Both source and destination cannot be local file type");
	exit (1);
    }

    status = parseCmdLinePath (argc, argv, optind, &myEnv,
      srcType, destType, 0, &rodsPathInp);

    if (status < 0) {
        rodsLogError (LOG_ERROR, status, "main: parseCmdLinePath error. "); 
	printf("use -h for help.\n");
        exit (1);
    }

    conn = rcConnect (myEnv.rodsHost, myEnv.rodsPort, myEnv.rodsUserName,
      myEnv.rodsZone, 1, &errMsg);

    if (conn == NULL) {
        rodsLogError (LOG_ERROR, errMsg.status, "rcConnect failure %s",
	       errMsg.msg);
        exit (2);
    }
   
    status = clientLogin(conn);
    if (status != 0) {
       rcDisconnect(conn);
        exit (7);
    }

    status = rsyncUtil (conn, &myEnv, &myRodsArgs, &rodsPathInp);

    rcDisconnect(conn);

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
"Usage : irsync [-rahsvV] [-R resource] sourceFile|sourceDirectory [....]",
" targetFile|targetDirectory",
" ",
"Synchronize the data between a  local  copy  (local file  system)  and",
"the copy stored in iRODS or between two iRODS copies. The command can be ",
"in one of  the  three  modes  : synchronization of data from the client's",
"local file system to iRODS, from iRODS to the local file system, or from",
"one iRODS path to another iRODS path. The mode is determined by",
"the way the sourceFile|sourceDirectory and targetFile|targetDirectory are",
"specified. Files and directories prepended with 'i:' are iRODS files and",
"collections. Local  files and directories are specified  without any", 
"prependage. For example, the command:",
" ",
"     irsync -r foo1 i:foo2",
" ",
"     synchronizes recursively the data from the  local  directory",
"     foo1 to the iRODS collection foo2 and the command:",
" ",
"     irsync -r i:foo1 foo2",
" ",
"     synchronizes recursively the data from the iRODS collection",
"     foo1 to the local directory foo2.",

"     irsync -r i:foo1 i:foo2",
" ",
"synchronizes recursively the data from  the  iRODS  collection foo1 to another",
" iRODS collection foo2.", 
" ",
"The command compares the checksum values and file sizes of the source"
"and target files to determine whether synchronization is needed. Therefore,"
"the command will run faster if the checksum value for the specific iRODS file,",
"no matter whether it is a source or target, already exists and is registered",
"with  iCAT. This can be achieved by using the -k or -K options of the iput",
"command at the time of  ingestion, or by using the ichksum command after the",
"data have already been ingested into iRODS.",

"If the -s option is used, only the file size (instead of the the size and", 
"checksum value) is used for determining whether synchronization is needed.",
"This  mode  is  gives  a  faster operation but the result is less accurate.",
" ",
"The command accepts  multiple  sourceFiles|sourceDirectories and a single",
"targetFile|targetDirectory. It pretty much follows the syntax of the UNIX",
"cp command with one exception- irsync of a single source  directory to a ",
"single target directory. In UNIX, the command:",
" ",
"     cp -r foo1 foo2",
" ",
"has a different meaning depending on whether the target directory foo2 ",
"exists. If the target directory exists,the content of source directory foo1",
"is copied to the target directory foo2/foo1. But if the target directory",
"does not exist, the content is copied to the target directory foo2.",
" ",
"With the irsync command,",
" ",
"     irsync -r foo1 i:foo2",
" ",
"always means the  synchronization  of  the local directory foo1 to collection",
"foo2, no matter whether foo2 exists or not.",
" ",
" -R  resource - specifies the target resource. This can also be",
"specified in your environment or via a rule set up by the administrator.",
" -r  recursive - store the whole subdirectory",
" -v  verbose",
" -V  Very verbose",
" -h  this help",
" -a   synchronize to all replica if the target is a  iRODS file/collection.",
" -s   use the size instead of the checksum value for determining", 
"      synchronization."
"  -v   verbose mode.", 
""};
   int i;
   for (i=0;;i++) {
      if (strlen(msgs[i])==0) return;
      printf("%s\n",msgs[i]);
   }
}
