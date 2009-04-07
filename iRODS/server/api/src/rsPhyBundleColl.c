/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/
/* rsPhyBundleColl.c. See phyBundleColl.h for a description of
 * this API call.*/

#include "phyBundleColl.h"
#include "objMetaOpr.h"
#include "miscServerFunct.h"
#include "openCollection.h"
#include "readCollection.h"
#include "closeCollection.h"
#include "dataObjRepl.h"
#include "dataObjCreate.h"
#include "syncMountedColl.h"
#include "regReplica.h"

static rodsLong_t OneGig = (1024*1024*1024);

int
rsPhyBundleColl (rsComm_t *rsComm, structFileExtAndRegInp_t *phyBundleCollInp)
{
    char *destRescName;
    int status; 
    rodsServerHost_t *rodsServerHost;
    int remoteFlag;
    rodsHostAddr_t rescAddr;
    rescGrpInfo_t *rescGrpInfo = NULL;

    if ((destRescName = getValByKey (&phyBundleCollInp->condInput, 
      DEST_RESC_NAME_KW)) == NULL) {
	return USER_NO_RESC_INPUT_ERR;
    }

    if (isLocalZone (phyBundleCollInp->collection) == 0) { 
	/* can only do local zone */
	return SYS_INVALID_ZONE_NAME;
    }
    status = _getRescInfo (rsComm, destRescName, &rescGrpInfo);
    if (status < 0) {
	 rodsLog (LOG_ERROR,
          "rsPhyBundleColl: _getRescInfo of %s error for %s. stat = %d",
          destRescName, phyBundleCollInp->collection, status);
	return status;
    }

    bzero (&rescAddr, sizeof (rescAddr));
    rstrcpy (rescAddr.hostAddr, rescGrpInfo->rescInfo->rescLoc, NAME_LEN);
    remoteFlag = resolveHost (&rescAddr, &rodsServerHost);

    if (remoteFlag == LOCAL_HOST) {
        status = _rsPhyBundleColl (rsComm, phyBundleCollInp, rescGrpInfo);
    } else if (remoteFlag == REMOTE_HOST) {
        status = remotePhyBundleColl (rsComm, phyBundleCollInp, rodsServerHost);
    } else if (remoteFlag < 0) {
	    status = remoteFlag;
    }

    return status;
}

int
_rsPhyBundleColl (rsComm_t *rsComm, structFileExtAndRegInp_t *phyBundleCollInp,
rescGrpInfo_t *rescGrpInfo)
{
    rescInfo_t *myRescInfo;
    char *myRescName;
    collInp_t collInp;
    collEnt_t *collEnt;
    char objPath[MAX_NAME_LEN];
    char collName[MAX_NAME_LEN];
    char dataName[MAX_NAME_LEN];
    rodsLong_t dataId;
    char phyBunDir[MAX_NAME_LEN];
    char subPhyPath[MAX_NAME_LEN];
    char cachePhyPath[MAX_NAME_LEN];
    int cacheReplNum;
    rodsLong_t subFileSize;
    int bundled;
    int handleInx;
    int status, l1descInx;
    dataObjInp_t dataObjInp;
    dataObjInfo_t dataObjInfo;
    bunReplCacheHeader_t bunReplCacheHeader;

    myRescInfo = rescGrpInfo->rescInfo;
    myRescName = myRescInfo->rescName;
    bzero (&collInp, sizeof (collInp));
    rstrcpy (collInp.collName, phyBundleCollInp->collection, MAX_NAME_LEN);
    collInp.flags = RECUR_QUERY_FG | VERY_LONG_METADATA_FG;
    handleInx = rsOpenCollection (rsComm, &collInp);
    if (handleInx < 0) {
        rodsLog (LOG_ERROR,
          "_rsPhyBundleColl: rsOpenCollection of %s error. status = %d",
          collInp.collName, handleInx);
        return (handleInx);
    }

    if (CollHandle[handleInx].rodsObjStat->specColl != NULL) {
        rodsLog (LOG_ERROR,
          "_rsPhyBundleColl: unable to bundle mounted collection %s",
          collInp.collName);
        rsCloseCollection (rsComm, &handleInx);
        return (0);
    }

    /* greate the bundle file */ 
    l1descInx = createPhyBundleDataObj (rsComm, phyBundleCollInp->collection,
      rescGrpInfo, &dataObjInp);

    if (l1descInx < 0) return l1descInx;

    createPhyBundleDir (rsComm, L1desc[l1descInx].dataObjInfo, phyBunDir);

    bzero (&bunReplCacheHeader, sizeof (bunReplCacheHeader));
    objPath[0] = '\0';
    collName[0] = '\0';
    dataName[0] = '\0';
    subPhyPath[0] = '\0';
    cachePhyPath[0] = '\0';
    bundled = 0;
    while ((status = rsReadCollection (rsComm, &handleInx, &collEnt)) >= 0) {
	if (collEnt->objType == DATA_OBJ_T) {
	    if (collName[0] == '\0') {
		/* a new dataObj.  */
		rstrcpy (collName, collEnt->collName, MAX_NAME_LEN);
		rstrcpy (dataName, collEnt->dataName, MAX_NAME_LEN);
		dataId = strtoll (collEnt->dataId, 0, 0);
	    } else if (strcmp (collName, collEnt->collName) != 0 ||
	      strcmp (collName, collEnt->collName) != 0) {
		if (bunReplCacheHeader.numSubFiles >= MAX_SUB_FILE_CNT ||
		  bunReplCacheHeader.totSubFileSize + collEnt->dataSize > 
		  MAX_BUNDLE_SIZE * OneGig) {
		    status = bundlleAndRegSubFiles (rsComm, l1descInx,
		      phyBunDir, phyBundleCollInp->collection,
		      &bunReplCacheHeader);
                    if (status < 0) {
                        /* XXXX need to handle error */
                        rodsLog (LOG_ERROR,
                        "_rsPhyBundleColl:bunAndRegSubFiles err for %s,stst=%d",
                          phyBundleCollInp->collection, status);
                    } else {
                        /* create a new bundle file */
                        l1descInx = createPhyBundleDataObj (rsComm,
                          phyBundleCollInp->collection, rescGrpInfo,
                          &dataObjInp);

                        /* XXXXX need to handle error */
                        if (l1descInx < 0) return l1descInx;

                        createPhyBundleDir (rsComm,
                          L1desc[l1descInx].dataObjInfo, phyBunDir);
                    }
		}
		if (bundled == 0) {
		    /* next dataObj. See if we need to replicate */
		    if (subPhyPath[0] == '\0') {
		        /* don't have a good cache copy yet. make one */
		        status = replDataObjForBundle (rsComm, collName, 
			  dataName, myRescName, &dataObjInfo);
		        if (status >= 0) {
		            setSubPhyPath (phyBunDir, collEnt->dataId, 
		              subPhyPath);
			    rstrcpy (cachePhyPath, dataObjInfo.filePath, 
		              MAX_NAME_LEN);
			    cacheReplNum = dataObjInfo.replNum;
			    subFileSize = dataObjInfo.dataSize;
		        }
		    }
		    /* add sub file to be bundled */
		    status = addSubFileToDir (subPhyPath, cachePhyPath, 
		      cacheReplNum, dataId, collName, dataName, subFileSize,
		      &bunReplCacheHeader);
		    if (status < 0) { 
			/* XXXX need to handle error */
        		rodsLog (LOG_ERROR,
          		"_rsPhyBundleColl:addSubFileToDir error for %s,stst=%d",
			    subPhyPath, status);
		    }
		} else {
		    /* already bundled. nothing to do */
		    bundled = 0;
		}
                rstrcpy (collName, collEnt->collName, MAX_NAME_LEN);
                rstrcpy (dataName, collEnt->dataName, MAX_NAME_LEN);
		dataId = strtoll (collEnt->dataId, 0, 0);
	    }
 
	    if (bundled > 0) {
		/* already bundled. skip */
	    } else if (isDataObjBundled (rsComm, collEnt)) {
		/* already bundled, skip */
		bundled = 1;
    		subPhyPath[0] = '\0';
    		cachePhyPath[0] = '\0';
	    } else if (collEnt->replStatus > 0 && 
	      strcmp (collEnt->resource, myRescName) == 0) {
		/* have a good copy in cache resource */
                setSubPhyPath (phyBunDir, collEnt->dataId, subPhyPath);
                rstrcpy (cachePhyPath, collEnt->phyPath, MAX_NAME_LEN);
                cacheReplNum = collEnt->replNum;
		subFileSize = collEnt->dataSize;
	    }
	}
	free (collEnt);     /* just free collEnt but not content */
    }
    /* handle any remaining */
    status = bundlleAndRegSubFiles (rsComm, l1descInx, phyBunDir, 
      phyBundleCollInp->collection, &bunReplCacheHeader);
    if (status < 0) {
        /* XXXX need to handle error */
        rodsLog (LOG_ERROR,
          "_rsPhyBundleColl:bunAndRegSubFiles err for %s,stst=%d",
          phyBundleCollInp->collection, status);
    }
    return status;
}

int
bundlleAndRegSubFiles (rsComm_t *rsComm, int l1descInx, char *phyBunDir, 
char *collection, bunReplCacheHeader_t *bunReplCacheHeader)
{
    int status;
    openedDataObjInp_t dataObjCloseInp;
    bunReplCache_t *tmpBunReplCache, *nextBunReplCache;
    regReplica_t regReplicaInp;

    bzero (&dataObjCloseInp, sizeof (dataObjCloseInp));
    status = phyBundle (rsComm, L1desc[l1descInx].dataObjInfo, phyBunDir,
      collection);
    if (status < 0) {
	/* XXXXX need to handle error */
        rodsLog (LOG_ERROR,
          "rsStructFileBundle: rsStructFileSync of %s error. stat = %d",
          L1desc[l1descInx].dataObjInfo->objPath, status);
	return status;
    } else {
        /* mark it was written so the size would be adjusted */
        L1desc[l1descInx].bytesWritten = 1;
    }

    dataObjCloseInp.l1descInx = l1descInx;
    rsDataObjClose (rsComm, &dataObjCloseInp);

    /* now register a replica for each subfile */
    tmpBunReplCache = bunReplCacheHeader->bunReplCacheHead;

    bzero (&regReplicaInp, sizeof (regReplicaInp));
    regReplicaInp.srcDataObjInfo = malloc (sizeof (dataObjInfo_t));
    regReplicaInp.destDataObjInfo = malloc (sizeof (dataObjInfo_t));
    bzero (regReplicaInp.srcDataObjInfo, sizeof (dataObjInfo_t));
    bzero (regReplicaInp.destDataObjInfo, sizeof (dataObjInfo_t));
    addKeyVal (&regReplicaInp.condInput, IRODS_ADMIN_KW, "");
    rstrcpy (regReplicaInp.destDataObjInfo->rescName, BUNDLE_RESC, NAME_LEN);
    rstrcpy (regReplicaInp.destDataObjInfo->filePath, 
      L1desc[l1descInx].dataObjInfo->filePath, MAX_NAME_LEN);
    
    while (tmpBunReplCache != NULL) {
	char subPhyPath[MAX_NAME_LEN];

	nextBunReplCache = tmpBunReplCache->next;
	/* rm the hard link here */
	snprintf (subPhyPath, MAX_NAME_LEN, "%s/%lld", phyBunDir, 
	  tmpBunReplCache->dataId);
	unlink (subPhyPath);
	/* register the replica */
        rstrcpy (regReplicaInp.srcDataObjInfo->objPath, 
	  tmpBunReplCache->objPath, MAX_NAME_LEN);
	regReplicaInp.srcDataObjInfo->dataId = 
	  regReplicaInp.destDataObjInfo->dataId =
	  tmpBunReplCache->dataId; 
	regReplicaInp.srcDataObjInfo->replNum = tmpBunReplCache->srcReplNum;
	status = rsRegReplica (rsComm, &regReplicaInp);
	if (status < 0) {
            /* XXXXX need to handle error */
            rodsLog (LOG_ERROR,
              "rsStructFileBundle: rsRegReplica error for %s. stat = %d",
              tmpBunReplCache->objPath, status);
	}
	free (tmpBunReplCache);
        tmpBunReplCache = nextBunReplCache;
    }
    clearKeyVal (&regReplicaInp.condInput);
    free (regReplicaInp.srcDataObjInfo);
    free (regReplicaInp.destDataObjInfo);
    bzero (bunReplCacheHeader, sizeof (bunReplCacheHeader_t)); 
    rmdir (phyBunDir);

    return status;
}

int
phyBundle (rsComm_t *rsComm, dataObjInfo_t *dataObjInfo, char *phyBunDir,
char *collection)
{
    structFileOprInp_t structFileOprInp;
    int status;

    bzero (&structFileOprInp, sizeof (structFileOprInp));

    structFileOprInp.specColl = malloc (sizeof (specColl_t));
    memset (structFileOprInp.specColl, 0, sizeof (specColl_t));
    structFileOprInp.specColl->type = TAR_STRUCT_FILE_T;

    /* collection and objPath are only important for reg CollInfo2 */
    rstrcpy (structFileOprInp.specColl->collection,
      collection, MAX_NAME_LEN);
    rstrcpy (structFileOprInp.specColl->objPath,
      dataObjInfo->objPath, MAX_NAME_LEN);
    structFileOprInp.specColl->collClass = STRUCT_FILE_COLL;
    rstrcpy (structFileOprInp.specColl->resource, dataObjInfo->rescName,
      NAME_LEN);
    rstrcpy (structFileOprInp.specColl->phyPath,
      dataObjInfo->filePath, MAX_NAME_LEN);
    rstrcpy (structFileOprInp.addr.hostAddr, dataObjInfo->rescInfo->rescLoc,
      NAME_LEN);
    rstrcpy (structFileOprInp.specColl->cacheDir, phyBunDir, MAX_NAME_LEN);
    structFileOprInp.specColl->cacheDirty = 1;
    /* don't reg CollInfo2 */
    structFileOprInp.oprType = NO_REG_COLL_INFO;

    status = rsStructFileSync (rsComm, &structFileOprInp);

    free (structFileOprInp.specColl);

    if (status < 0) {
        rodsLog (LOG_ERROR,
          "rsStructFileBundle: rsStructFileSync of %s error. stat = %d",
          dataObjInfo->objPath, status);
    }

    return status;
}

int
addSubFileToDir (char *subPhyPath, char *cachePhyPath, int cacheReplNum, 
rodsLong_t dataId, char *collName, char *dataName, rodsLong_t subFileSize,
bunReplCacheHeader_t *bunReplCacheHeader)
{
    int status;
    bunReplCache_t *bunReplCache;

    /* add a link */
    status = link (cachePhyPath, subPhyPath);
    if (status < 0) {
        rodsLog (LOG_ERROR,
          "addSubFileToDir: link error %s to %s. errno = %d",
	  cachePhyPath, subPhyPath, errno);
	return (UNIX_FILE_LINK_ERR - errno);
    }
    bunReplCache = malloc (sizeof (bunReplCache_t));
    bzero (bunReplCache, sizeof (bunReplCache_t));
    bunReplCache->dataId = dataId;
    snprintf (bunReplCache->objPath, MAX_NAME_LEN, "%s/%s", collName, dataName);
    bunReplCache->srcReplNum = cacheReplNum;
    bunReplCache->next = bunReplCacheHeader->bunReplCacheHead;
    bunReplCacheHeader->bunReplCacheHead = bunReplCache;
    bunReplCacheHeader->numSubFiles++;
    bunReplCacheHeader->totSubFileSize += subFileSize;

    subPhyPath[0] = cachePhyPath[0] = '\0';

    return 0;
}
    
int
setSubPhyPath (char *phyBunDir, char *dataId, char *subPhyPath)
{
    snprintf (subPhyPath, MAX_NAME_LEN, "%s/%s", phyBunDir, dataId);
    return 0;
}

int
isDataObjBundled (rsComm_t *rsComm, collEnt_t *collEnt)
{
    if (strcmp (collEnt->resource, BUNDLE_RESC) == 0) {
	if (collEnt->replStatus > 0) { 
	    return 1;
	} else {
	    /* XXXXXX need to remove this outdated copy */
	    return 0;
	}
    } else {
	return 0;
    }
}

int
replDataObjForBundle (rsComm_t *rsComm, char *collName, char *dataName,
char *rescName, dataObjInfo_t *outCacheObjInfo)
{
    transStat_t transStat;
    dataObjInp_t dataObjInp;
    int status;

    if (outCacheObjInfo != NULL)
        memset (outCacheObjInfo, 0, sizeof (dataObjInfo_t));
    memset (&dataObjInp, 0, sizeof (dataObjInp_t));
    memset (&transStat, 0, sizeof (transStat));

    snprintf (dataObjInp.objPath, MAX_NAME_LEN, "%s/%s", collName, dataName);
    addKeyVal (&dataObjInp.condInput, BACKUP_RESC_NAME_KW, rescName);
    addKeyVal (&dataObjInp.condInput, IRODS_ADMIN_KW, "");

    status = rsDataObjReplWithOutDataObj (rsComm, &dataObjInp, &transStat,
      outCacheObjInfo);
    clearKeyVal (&dataObjInp.condInput);
    return status;
}

int
createPhyBundleDir (rsComm_t *rsComm, dataObjInfo_t *dataObjInfo, 
char *outPhyBundleDir)
{
    /* the dir where we put the files to bundle is in phyPath.dir */
    snprintf (outPhyBundleDir, MAX_NAME_LEN, "%s.dir",  dataObjInfo->filePath);
    mkdirR ("/", outPhyBundleDir, 0750);
    return (0);
}

int
createPhyBundleDataObj (rsComm_t *rsComm, char *collection, 
rescGrpInfo_t *rescGrpInfo, dataObjInp_t *dataObjInp)
{
    int myRanNum;
    int l1descInx;
    int status;
    int rescTypeInx = rescGrpInfo->rescInfo->rescTypeInx;

    /* XXXXXX We do bundle only with UNIX_FILE_TYPE for now */
    if (RescTypeDef[rescTypeInx].driverType != UNIX_FILE_TYPE) {
        rodsLog (LOG_ERROR,
          "createPhyBundleFile: resource %s is not UNIX_FILE_TYPE",
          rescGrpInfo->rescInfo->rescName);
        return SYS_INVALID_RESC_TYPE;
    }
	
    do {
        bzero (dataObjInp, sizeof (dataObjInp_t));
        myRanNum = random ();
        status = rsMkBundlePath (rsComm, collection, dataObjInp->objPath, 
	  myRanNum);
        if (status < 0) {
            rodsLog (LOG_ERROR,
              "createPhyBundleFile: getPhyBundlePath error for %s. status = %d",
              collection, status);
            return status;
        }

        addKeyVal (&dataObjInp->condInput, NO_OPEN_FLAG_KW, "");
        addKeyVal (&dataObjInp->condInput, DATA_TYPE_KW, "tar bundle");

        l1descInx = _rsDataObjCreateWithRescInfo (rsComm, dataObjInp,
          rescGrpInfo->rescInfo, rescGrpInfo->rescGroupName);

        clearKeyVal (&dataObjInp->condInput);
    } while (l1descInx == OVERWITE_WITHOUT_FORCE_FLAG);

    return l1descInx;
}

/* rsMkBundlePath - set the BundlePath to 
 * /zone/bundle/home/.../collection.myRanNum. Make all the necessary 
 * parent collections. The output is put in bundlePath.
 */

int
rsMkBundlePath (rsComm_t *rsComm, char *collection, char *bundlePath, 
int myRanNum)
{
    int status;
    char *tmpStr;
    char startBundlePath[MAX_NAME_LEN];
    char destBundleColl[MAX_NAME_LEN], myFile[MAX_NAME_LEN];
    char *bundlePathPtr;

    bundlePathPtr = bundlePath;
    *bundlePathPtr = '/';
    bundlePathPtr++;
    tmpStr = collection + 1;
    /* copy the zone */
    while (*tmpStr != '\0') {
        *bundlePathPtr = *tmpStr;
        bundlePathPtr ++;
        if (*tmpStr == '/') {
            tmpStr ++;
            break;
        }
        tmpStr ++;
    }

    if (*tmpStr == '\0') {
        rodsLog (LOG_ERROR,
          "rsMkBundlePath: input path %s too short", collection);
        return (USER_INPUT_PATH_ERR);
    }

    /* cannot bundle trash and bundle */
    if (strncmp (tmpStr, "trash/", 6) == 0 || 
      strncmp (tmpStr, "bundle/", 7) == 0) {
        rodsLog (LOG_ERROR,
          "rsMkBundlePath: cannot bundle trash or bundle path %s", collection);
        return (USER_INPUT_PATH_ERR);
    }


    /* don't want to go back beyond /myZone/bundle/home */
    *bundlePathPtr = '\0';
    rstrcpy (startBundlePath, bundlePath, MAX_NAME_LEN);

    snprintf (bundlePathPtr, MAX_NAME_LEN, "bundle/%s.%d", tmpStr, myRanNum);

    if ((status = splitPathByKey (bundlePath, destBundleColl, myFile, '/')) 
      < 0) {
        rodsLog (LOG_ERROR,
          "rsMkBundlePath: splitPathByKey error for %s ", bundlePath);
        return (USER_INPUT_PATH_ERR);
    }

    status = rsMkCollR (rsComm, startBundlePath, destBundleColl);

    if (status < 0) {
        rodsLog (LOG_ERROR,
          "rsMkBundlePath: rsMkCollR error for startPath %s, destPath %s ",
          startBundlePath, destBundleColl);
    }

    return (status);
}

int
remotePhyBundleColl (rsComm_t *rsComm, 
structFileExtAndRegInp_t *phyBundleCollInp, rodsServerHost_t *rodsServerHost)
{
    int status;

    if (rodsServerHost == NULL) {
        rodsLog (LOG_NOTICE,
          "remotePhyBundleColl: Invalid rodsServerHost");
        return SYS_INVALID_SERVER_HOST;
    }

    if ((status = svrToSvrConnect (rsComm, rodsServerHost)) < 0) {
        return status;
    }

    status = rcPhyBundleColl (rodsServerHost->conn, phyBundleCollInp);
    return status;
}

