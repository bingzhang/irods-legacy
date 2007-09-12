/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/

#ifndef RE_FUNCDEFS_H
#define RE_FUNCDEFS_H

int initRuleStruct(char *ruleSet, char *dvmSet, char *fnmSet);

int readRuleStructFromFile(char *ruleBaseName, ruleStruct_t *inRuleStrct);

int clearRuleStruct(ruleStruct_t *inRuleStrct);

int readDVarStructFromFile(char *dvarBaseName,rulevardef_t *inRuleVarDef);

int clearDVarStruct(rulevardef_t *inRuleVarDef);

int readFuncMapStructFromFile(char *fmapBaseName, rulefmapdef_t* inRuleFuncMapDef);

int clearFuncMapStruct( rulefmapdef_t* inRuleFuncMapDef);

int findNextRule (char *action,  int *ruleInx);

int getRule(int ri, char *ruleBase, char *ruleHead, char *ruleCondition, 
	char *ruleAction, char *ruleRecovery, int rSize);

int executeRuleBody(char *action, char *ruleAction, char *ruleRecovery, 
		char *args[MAX_NUM_OF_ARGS_IN_ACTION], int argc,
		ruleExecInfo_t *rei, int reiSaveFlag);

int getActionRecoveryList(char *ruleAction, char *ruleRecovery, 
		      char *actionArray[],char *recoveryArray[]);

int executeRuleRecovery(char *ruleRecovery, ruleExecInfo_t *rei, int reiSaveFlag);

int parseAction(char *inAction,char *action, char *args[], int *argc);

int executeRuleAction(char *inAction, ruleExecInfo_t *rei, int reiSaveFlag);

int actionTableLookUp (char *action);

int getNextAction(char *listOfAction, char *action, char **restPtr);

int checkRuleHead(char *ruleHead, char *args[MAX_NUM_OF_ARGS_IN_ACTION], int argc);

int checkRuleCondition(char *action, char *incond, char *args[MAX_NUM_OF_ARGS_IN_ACTION], 
		   int argc, ruleExecInfo_t *rei, int reiSaveFlag);

int computeExpression( char *expr, ruleExecInfo_t *rei, int reiSaveFlag, char *res);

int replaceVariables(char *action, char *inStr, char *args[MAX_NUM_OF_ARGS_IN_ACTION], int argc,
		   ruleExecInfo_t *rei );
int replaceDollarParam(char *action, char *dPtr, int len, 
		   char *args[MAX_NUM_OF_ARGS_IN_ACTION], int argc,
		   ruleExecInfo_t *rei);

int replaceArgVar( char *start, int size, 
	       char *args[MAX_NUM_OF_ARGS_IN_ACTION], int argc);

int replaceSessionVar(char *action,  char *start, int size,  ruleExecInfo_t *rei);

int getVarMap(char *action, char *varName, char **varMap, int index);

int getVarValue(char *varMap, ruleExecInfo_t *rei, char **varValue);

int replaceDataVar( char *start, int size,  dataObjInfo_t *doi);

int replaceUserVar( char *start, int size,  userInfo_t *uoic,  userInfo_t *uoip);

int replaceCollVar( char *start, int size,  collInfo_t *coi);

int replaceRescVar( char *start, int size,  rescInfo_t *roi);

int isNumber(char *s);

int isLogical(char *s);

int isAFunction(char *s);

int splitExpression(char *expr, char *expr1, char *expr2, char *oper);

int goodExpr(char *expr);

int replaceStrValue(char *start, int size, char *val, int paramLen);

int replaceIntValue(char *start, int size, int inval, int paramLen);

int replaceLongValue(char *start, int size, rodsLong_t inval, int paramLen);

int replaceULongValue(char *start, int size, rodsULong_t inval, int paramLen);

void *mallocAndZero(int s);

int copyRuleExecInfo(ruleExecInfo_t *from, ruleExecInfo_t *to);

int freeRuleExecInfoStruct(ruleExecInfo_t *rs, int freeMsParamFlag);

int freeRuleExecInfoInternals(ruleExecInfo_t *rs, int freeMsParamFlag);

int copyDataObjInfo(dataObjInfo_t *from, dataObjInfo_t *to);

int copyCollInfo(collInfo_t *from, collInfo_t *to);

int freeCollInfo(collInfo_t *rs);

int copyUserInfo(userInfo_t *from, userInfo_t *to);

int freeUserInfo(userInfo_t *rs);

int copyRescInfo(rescInfo_t *from, rescInfo_t *to);

int freeRescInfo(rescInfo_t *rs);

int copyRescGrpInfo(rescGrpInfo_t *from, rescGrpInfo_t *to);

int freeRescGrpInfo(rescGrpInfo_t *rs);

int copyKeyValPairStruct(keyValPair_t *from, keyValPair_t *to);

int freeKeyValPairStruct(keyValPair_t *rs);

int reREMatch(char *pat, char *str);

int trimWS (char *s);

int getValFromRuleExecInfo(char *varMap, ruleExecInfo_t *rei, char **varValue);

int getValFromRsComm(char *varMap, rsComm_t *ptr, char **varValue);

int getValFromDataObjInfo(char *varMap, dataObjInfo_t *ptr, char **varValue);

int getValFromRescInfo(char *varMap, rescInfo_t *ptr, char **varValue);

int getValFromUserInfo(char *varMap, userInfo_t *ptr, char **varValue);

int getValFromCollInfo(char *varMap, collInfo_t *ptr, char **varValue);

int getValFromKeyValPair(char *varMap, keyValPair_t *ptr, char **varValue);

int getValFromVersion(char *varMap, version_t *ptr, char **varValue);

int getValFromAuthInfo(char *varMap, authInfo_t *ptr, char **varValue);

int getValFromUserOtherInfo(char *varMap, userOtherInfo_t *ptr, char **varValue);

int getValFromDataObjInp(char *varMap, dataObjInp_t *ptr, char **varValue);

int getValFromDataOprInp(char *varMap, dataOprInp_t *ptr, char **varValue);

int getValFromFileOpenInp(char *varMap, fileOpenInp_t *ptr, char **varValue);

int getValFromRescGrpInfo(char *varMap, rescGrpInfo_t *ptr, char **varValue);

int getValFromRodsHostAddr(char *varMap, rodsHostAddr_t *ptr, char **varValue);

int mapExternalFuncToInternalProc( char *funcName);

int zeroRuleExecInfoStruct(ruleExecInfo_t *rei);
int initReiWithDataObjInp (ruleExecInfo_t *rei, rsComm_t *rsComm,
    dataObjInp_t *dataObjIn);
int initReiWithDataObjInp (ruleExecInfo_t *rei, rsComm_t *rsComm,
    dataObjInp_t *dataObjInp);
int packRei (rsComm_t *rsComm, ruleExecInfo_t *rei, bytesBuf_t **packedReiBBuf);
int unpackRei (rsComm_t *rsComm, ruleExecInfo_t **rei, bytesBuf_t *packedReiBBuf);
int touchupPackedRei (rsComm_t *rsComm, ruleExecInfo_t *myRei);
int packReiAndArg (rsComm_t *rsComm, ruleExecInfo_t *rei, char *myArgv[],
    int myArgc, bytesBuf_t **packedReiAndArgBBuf);
int unpackReiAndArg (rsComm_t *rsComm, ruleExecInfoAndArg_t **reiAndArg,
    bytesBuf_t *packedReiAndArgBBuf);

int pushStack(strArray_t *strArray, char *value);
int popStack(strArray_t *strArray, char *value);
int copyTaggedValue(char *str, char *tag, char *buf, int bufLen);
/***  causing trouble in compiling clientLogin.c
int
fillSubmitConditions (char *action, char *inDelayCondition, bytesBuf_t *packedReiAndArgBBuf,   ruleExecSubmitInp_t *ruleSubmitInfo);
***/
int print_uoi(userInfo_t *uoi);
int print_doi(dataObjInfo_t *doi);
int execMyRule(char * ruleDef,  msParamArray_t *inMsParamArray,
	  ruleExecInfo_t *rei);
int executeMyRuleBody(char *action, char *ruleAction, char *ruleRecovery, 
		msParamArray_t *inMsParamArray,
		ruleExecInfo_t *rei, int reiSaveFlag );
int executeMyRuleRecovery(char *ruleRecovery, msParamArray_t *inMsParamArray, ruleExecInfo_t *rei, int reiSaveFlag);
int executeMyRuleAction(char *inAction, msParamArray_t *inMsParamArray, ruleExecInfo_t *rei, int reiSaveFlag);
int executeMicroService (char *action, char *args[MAX_NUM_OF_ARGS_IN_ACTION], int argc,
	  ruleExecInfo_t *rei);
int applyActionCall(char *actionCall,  ruleExecInfo_t *rei, int reiSaveFlag);

int applyRuleArg(char *action, char *args[MAX_NUM_OF_ARGS_IN_ACTION], int argc,
	       ruleExecInfo_t *rei, int reiSaveFlag);
int applyRuleArgPA(char *action, char *args[MAX_NUM_OF_ARGS_IN_ACTION], int argc,
		      msParamArray_t *inMsParamArray, ruleExecInfo_t *rei, int reiSaveFlag);
int applyRule(char *inAction, msParamArray_t *inMsParamArray,
	  ruleExecInfo_t *rei, int reiSaveFlag);

int clearMsparamInRei (ruleExecInfo_t *rei);

int _delayExec(char *inActionCall, char *recoveryActionCall, 
	       char *delayCondition,  ruleExecInfo_t *rei);

int msiRollback(ruleExecInfo_t *rei);

int executeRuleActionNew(char *inAction,  msParamArray_t *inMsParamArray, 
    ruleExecInfo_t *rei, int reiSaveFlag);

int executeRuleRecoveryNew(char *ruleRecovery, msParamArray_t *inMsParamArray, 
    ruleExecInfo_t *rei, int reiSaveFlag);
int executeMicroServiceNew(char *inAction,  msParamArray_t *inMsParamArray,
                       ruleExecInfo_t *rei);
int makeAction(char *outAction,char *action, char *args[], int argc, int len);
int executeRuleBodyNew(char *action, char *ruleAction, char *ruleRecovery,
                msParamArray_t *inMsParamArray,
                ruleExecInfo_t *rei, int reiSaveFlag );
int replaceStarVar(char *action,  char *start, int size,   
     msParamArray_t *inMsParamArray);
int freeRuleArgs (char *args[], int argc);
#endif  /* RE_FUNCDEFS_H */
