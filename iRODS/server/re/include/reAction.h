/* reAction.h - header file for Actions that are 'called' when executing the rukes by 
 * the rule engine modules
 */
#ifndef RE_ACTION_H
#define RE_ACTION_H

#include "rodsUser.h"
#include "rods.h"
#include "rcGlobalExtern.h"
#include "reDefines.h"
#include "objInfo.h"
#include "regExpMatch.h"
#include "reSysDataObjOpr.h"
#include "reDataObjOpr.h"
#include "reNaraMetaData.h"
#include "reIn2p3SysRule.h"

typedef struct {
  char action[MAX_ACTION_SIZE];
  int numberOfStringArgs;
  funcPtr callAction;
} microsdef_t;


int print_hello(ruleExecInfo_t *c);
int print_bye(ruleExecInfo_t *c);
int print_eol(ruleExecInfo_t  *c);
int print_hello_arg(msParam_t *s, ruleExecInfo_t  *c);  /* void * is always at the end!!! */
int whileExec(msParam_t* condition, msParam_t* whileBody,
	      msParam_t* recoverWhileBody, ruleExecInfo_t *rei);
int forExec(msParam_t* initial, msParam_t* condition, msParam_t* step, 
	    msParam_t* forBody, msParam_t* recoverForBody, ruleExecInfo_t *rei);
int delayExec(msParam_t* condition, msParam_t* workflow, 
	      msParam_t* recoverWorkFlow, ruleExecInfo_t *rei);
int remoteExec(msParam_t* hostName, msParam_t* condition, msParam_t* workflow, 
	      msParam_t* recoverWorkFlow, ruleExecInfo_t *rei);
int forEachExec(msParam_t* list, msParam_t* setOfWFS, msParam_t* recoverSetOfWFS,
	      ruleExecInfo_t *rei);
int writeString(msParam_t* where, msParam_t* inString, ruleExecInfo_t *rei);
int writeLine(msParam_t* where, msParam_t* inString, ruleExecInfo_t *rei);
int  msiSetResource(msParam_t* xrescName, ruleExecInfo_t *rei);
int sendStdoutAsEmail(msParam_t* xtoAddr, msParam_t* xsubjectLine, ruleExecInfo_t *rei);
int msiPrintKeyValPair(msParam_t* where, msParam_t* inKVPair,  ruleExecInfo_t *rei);
int msiGetValByKey(msParam_t* inKVPair,  msParam_t* inKey, msParam_t* outVal,  ruleExecInfo_t *rei);
int assign(msParam_t* var, msParam_t* value, ruleExecInfo_t *rei);
int ifExec(msParam_t* condition, msParam_t* thenC, msParam_t* recoverThen, 
	   msParam_t* elseC, msParam_t* recoverElse, ruleExecInfo_t *rei);
int msiExecStrCondQuery(msParam_t* queryParam, msParam_t* genQueryOutParam, ruleExecInfo_t *rei);
int msiExecGenQuery(msParam_t* genQueryInParam, msParam_t* genQueryOutParam, ruleExecInfo_t *rei);
int msiMakeQuery(msParam_t* selectListParam, msParam_t* conditionsParam,
		 msParam_t* queryOutParam, ruleExecInfo_t *rei);

int msiVacuum(ruleExecInfo_t *rei);
int msiGoodFailure(ruleExecInfo_t *rei);
int msiRegisterData(ruleExecInfo_t *rei);
int msiCheckPermission(msParam_t *perm, ruleExecInfo_t *rei);
int msiCheckOwner(ruleExecInfo_t *rei);
int msiCreateUser(ruleExecInfo_t *rei);
int msiCreateCollByAdmin(msParam_t *parColl, msParam_t *childName, ruleExecInfo_t *rei);
int msiCommit(ruleExecInfo_t *rei);
int msiDeleteCollByAdmin(msParam_t *parColl, msParam_t *childName, ruleExecInfo_t *rei);
int msiDeleteUser(ruleExecInfo_t *rei);
int msiSendMail(msParam_t *toAddr, msParam_t *subjectLine, msParam_t *body, ruleExecInfo_t *rei);
int admChangeCoreIRB(msParam_t *newF, ruleExecInfo_t *rei);
int admShowIRB(msParam_t *bufP, ruleExecInfo_t *rei);
int admShowDVM(msParam_t *bufP, ruleExecInfo_t *rei);
int admShowFNM(msParam_t *bufP, ruleExecInfo_t *rei);
int admAppendToTopOfCoreIRB(msParam_t *bufP, ruleExecInfo_t *rei);
int admClearAppRuleStruct(ruleExecInfo_t *rei);
int admAddAppRuleStruct(msParam_t *irbFilesParam, msParam_t *dvmFilesParam, 
			msParam_t *fnmFilesParam,  ruleExecInfo_t *rei);
int msiGetObjType(msParam_t *objNameP, msParam_t *objTypeP,
	      ruleExecInfo_t *rei);
int msiAssociateKeyValuePairsToObj(msParam_t *mDP, msParam_t* objP,  msParam_t* typP,
				   ruleExecInfo_t *rei);
int msiExtractTemplateMDFromBuf(msParam_t* sOP, msParam_t* tSP, 
			   msParam_t *mDP, ruleExecInfo_t *rei);
int msiReadMDTemplateIntoTagStruct(msParam_t* sOP, msParam_t* tSP, ruleExecInfo_t *rei);
int msiFreeBuffer(msParam_t* memP, ruleExecInfo_t *rei);
int msiGetIcatTime(msParam_t* timeOutParam,  msParam_t* typeInParam, ruleExecInfo_t *rei);

int msiGetDataObjAVUs(msParam_t *inpParam, msParam_t *outParam, ruleExecInfo_t *rei);
int msiGetDataObjPSmeta(msParam_t *inpParam, msParam_t *outParam, ruleExecInfo_t *rei);
int msiGetCollectionPSmeta(msParam_t *inpParam, msParam_t *outParam, ruleExecInfo_t *rei);
int msiGetDataObjAIP(msParam_t *inpParam, msParam_t *outParam, ruleExecInfo_t *rei);
int msiLoadMetadataFromFile(msParam_t *inpParam, msParam_t *outParam, ruleExecInfo_t *rei);
int msiExportRecursiveCollMeta(msParam_t *inpParam, msParam_t *outParam, ruleExecInfo_t *rei);
int msiSaveBufferToLocalFile(msParam_t *inpParam1, msParam_t *inpParam2, msParam_t *outParam, ruleExecInfo_t *rei);

int msitest1(msParam_t *A, msParam_t *B, msParam_t* C, ruleExecInfo_t *rei);
int msitest2(msParam_t *A, msParam_t *B, msParam_t* C, ruleExecInfo_t *rei);
int msitest3(msParam_t *A, msParam_t *B, msParam_t* C, ruleExecInfo_t *rei);
int recover_print_hello(ruleExecInfo_t *c);
int recover_print_bye(ruleExecInfo_t *c);
int recover_print_eol(ruleExecInfo_t *c);
int recover_print_hello_arg(msParam_t *s, ruleExecInfo_t *c);

int recover_msiCreateUser(ruleExecInfo_t *rei);
int recover_msiCreateCollByAdmin(msParam_t *parColl, msParam_t *childName, ruleExecInfo_t *rei);

microsdef_t MicrosTable[] = {
  {"print_hello", 0, (funcPtr) print_hello},
  {"print_bye", 0, (funcPtr) print_bye},
  {"print_eol", 0, (funcPtr) print_eol},
  {"print_hello_arg", 1, (funcPtr) print_hello_arg},
  {"msiVacuum", 0, (funcPtr) msiVacuum},
  {"msitest1", 3, (funcPtr) msitest1},
  {"msitest2", 3, (funcPtr) msitest2},
  {"msitest3", 3, (funcPtr) msitest3},
  {"msiGoodFailure", 0, (funcPtr) msiGoodFailure},
  {"msiSetResource", 1,(funcPtr)  msiSetResource},
  {"msiCheckPermission", 1,(funcPtr)  msiCheckPermission},
  {"msiCheckOwner", 0, (funcPtr) msiCheckOwner},
  {"msiCreateUser", 0, (funcPtr) msiCreateUser},
  {"msiCreateCollByAdmin",2, (funcPtr) msiCreateCollByAdmin},
  {"msiSendMail",3, (funcPtr) msiSendMail},
  {"recover_print_hello", 0, (funcPtr) recover_print_hello},
  {"recover_print_bye", 0, (funcPtr) recover_print_bye},
  {"recover_print_eol", 0, (funcPtr) recover_print_eol},
  {"recover_print_hello_arg", 1, (funcPtr) recover_print_hello_arg},
  {"msiCommit", 0, (funcPtr) msiCommit},
  {"msiRollback", 0, (funcPtr) msiRollback},
  {"msiDeleteCollByAdmin",2, (funcPtr) msiDeleteCollByAdmin},
  {"msiDeleteUser", 0, (funcPtr) msiDeleteUser},
  {"msiSetDefaultResc", 2, (funcPtr) msiSetDefaultResc},
  {"msiSetRescSortScheme", 1, (funcPtr) msiSetRescSortScheme},
  {"msiSysReplDataObj", 2, (funcPtr) msiSysReplDataObj},
  {"msiStageDataObj", 1, (funcPtr) msiStageDataObj},
  {"msiSetDataObjPreferredResc", 1, (funcPtr) msiSetDataObjPreferredResc},
  {"msiSetDataObjAvoidResc", 1, (funcPtr) msiSetDataObjAvoidResc},
  {"msiSortDataObj", 1, (funcPtr) msiSortDataObj},
  {"msiSysChksumDataObj", 0, (funcPtr) msiSysChksumDataObj},
  {"msiSetDataTypeFromExt", 0, (funcPtr) msiSetDataTypeFromExt},
  {"msiSetNoDirectRescInp",1,(funcPtr) msiSetNoDirectRescInp},
  {"msiSetNumThreads", 3,(funcPtr) msiSetNumThreads},
  {"msiDeleteDisallowed",0,(funcPtr) msiDeleteDisallowed},
  {"msiDataObjCreate",3,(funcPtr) msiDataObjCreate},
  {"msiDataObjOpen",2,(funcPtr) msiDataObjOpen},
  {"msiDataObjClose",2,(funcPtr) msiDataObjClose},
  {"msiDataObjLseek",4,(funcPtr) msiDataObjLseek},
  {"msiDataObjRead",3,(funcPtr) msiDataObjRead},
  {"msiDataObjWrite",3,(funcPtr) msiDataObjWrite},
  {"msiDataObjUnlink",2,(funcPtr) msiDataObjUnlink},
  {"msiDataObjRepl",3,(funcPtr) msiDataObjRepl},
  {"msiDataObjCopy",4,(funcPtr) msiDataObjCopy},
  {"msiExtractNaraMetadata",0,(funcPtr) msiExtractNaraMetadata},
  {"msiSetMultiReplPerResc",0,(funcPtr) msiSetMultiReplPerResc},
  {"admChangeCoreIRB",1,(funcPtr) admChangeCoreIRB},
  {"admShowIRB",1,(funcPtr) admShowIRB},
  {"admShowDVM",1,(funcPtr) admShowDVM},
  {"admShowFNM",1,(funcPtr) admShowFNM},
  {"admAppendToTopOfCoreIRB",1,(funcPtr) admAppendToTopOfCoreIRB},
  {"admClearAppRuleStruct",0,(funcPtr) admClearAppRuleStruct},
  {"admAddAppRuleStruct",3,(funcPtr) admAddAppRuleStruct},
  {"msiGetObjType",2,(funcPtr) msiGetObjType},
  {"msiAssociateKeyValuePairsToObj",3,(funcPtr) msiAssociateKeyValuePairsToObj},
  {"msiExtractTemplateMDFromBuf",3,(funcPtr) msiExtractTemplateMDFromBuf},
  {"msiReadMDTemplateIntoTagStruct",2,(funcPtr) msiReadMDTemplateIntoTagStruct},
  {"msiDataObjPut",4,(funcPtr) msiDataObjPut},
  {"msiDataObjGet",3,(funcPtr) msiDataObjGet},
  {"msiDataObjChksum",3,(funcPtr) msiDataObjChksum},
  {"msiDataObjPhymv",6,(funcPtr) msiDataObjPhymv},
  {"msiDataObjRename",4,(funcPtr) msiDataObjRename},
  {"msiDataObjTrim",6,(funcPtr) msiDataObjTrim},
  {"msiCollCreate",2,(funcPtr) msiCollCreate},
  {"msiRmColl",3,(funcPtr) msiRmColl},
  {"msiPhyPathReg",5,(funcPtr) msiPhyPathReg},
  {"msiObjStat",2,(funcPtr) msiObjStat},
  {"msiDataObjRsync",5,(funcPtr) msiDataObjRsync},
  {"msiFreeBuffer",1,(funcPtr) msiFreeBuffer},
  {"msiNoChkFilePathPerm",0,(funcPtr) msiNoChkFilePathPerm},
  {"msiNoTrashCan",0,(funcPtr) msiNoTrashCan},
  {"msiSetPublicUserOpr",1,(funcPtr) msiSetPublicUserOpr},
  {"whileExec",3,(funcPtr) whileExec},
  {"forExec",5,(funcPtr) forExec},
  {"delayExec",3,(funcPtr) delayExec},
  {"remoteExec",4,(funcPtr) remoteExec},
  {"forEachExec",3,(funcPtr) forEachExec},
  {"writeString",2,(funcPtr) writeString},
  {"writeLine",2,(funcPtr) writeLine},
  {"sendStdoutAsEmail",2,(funcPtr) sendStdoutAsEmail},
  {"msiPrintKeyValPair",2,(funcPtr) msiPrintKeyValPair},
  {"msiGetValByKey",3,(funcPtr) msiGetValByKey},
  {"assign",2,(funcPtr) assign},
  {"ifExec",5,(funcPtr) ifExec},
  {"msiExecStrCondQuery",2,(funcPtr) msiExecStrCondQuery},
  {"msiExecGenQuery",2,(funcPtr) msiExecGenQuery},
  {"msiMakeQuery",3,(funcPtr) msiMakeQuery},
  {"msiExecCmd",6,(funcPtr) msiExecCmd},
  {"msiSetGraftPathScheme",2,(funcPtr) msiSetGraftPathScheme},
  {"msiSetRandomScheme",0,(funcPtr) msiSetRandomScheme},
  {"msiCheckHostAccessControl",0,(funcPtr) msiCheckHostAccessControl},
  {"msiGetIcatTime",2,(funcPtr) msiGetIcatTime},
  {"msiGetDataObjAVUs", 2, (funcPtr) msiGetDataObjAVUs},
  {"msiGetDataObjPSmeta", 2, (funcPtr) msiGetDataObjPSmeta},
  {"msiGetCollectionPSmeta", 2, (funcPtr) msiGetCollectionPSmeta},
  {"msiGetDataObjAIP", 2, (funcPtr) msiGetDataObjAIP},
  {"msiLoadMetadataFromFile", 2, (funcPtr) msiLoadMetadataFromFile},
  {"msiExportRecursiveCollMeta", 2, (funcPtr) msiExportRecursiveCollMeta },
  {"msiSaveBufferToLocalFile", 3, (funcPtr) msiSaveBufferToLocalFile }
};

int NumOfAction = sizeof(MicrosTable) / sizeof(microsdef_t);

#endif	/* RE_ACTION_H */

