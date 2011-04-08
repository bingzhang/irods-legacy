/* For copyright information please refer to files in the COPYRIGHT directory
 */
#ifndef RULES_H
#define RULES_H
#include "parser.h"
#include "utils.h"
#include "arithmetics.h"
#include "typing.h"
#define MAX_NUM_RULES 50000
#define MAX_NUM_APP_RULES 10000

typedef struct ruleSet {
	int len;
	Node* rules[MAX_NUM_RULES];
} RuleSet;

extern RuleSet coreRules, appRules;

int setLocalVarValue(char* varName, ruleExecInfo_t *rei, Res* res, char* errmsg, Region *r);
int readRuleSetFromFile(char *ruleBaseName, RuleSet *ruleSet, int* errloc, rError_t *errmsg, Region *r);
int readRuleSetFromLocalFile(char *ruleBaseName, char *fileName, RuleSet *ruleSet, int *errloc, rError_t *errmsg, Region *r);
Res *parseAndComputeExpression(char * expr,Env *env, ruleExecInfo_t *rei, int reiSaveFlag, rError_t *errmsg, Region *r);
int computeRule( char *expr, ruleExecInfo_t *rei, int reiSaveFlag, msParamArray_t *msParamArray, rError_t *errmsg, Region *r);
Res *computeExpressionWithParams(char *actionName, char** params, int paramCount, ruleExecInfo_t *rei, int reiSaveFlag, msParamArray_t *vars, rError_t *errmsg, Region *r);
Res *computeExpressionNode(Node *expr, Env *env, ruleExecInfo_t *rei, int reiSaveFlag , rError_t *errmsg, Region *r);

ExprType *typeRule(Node *ruleNode, Hashtable *funcDesc, Hashtable *varTypes, List *typingConstraints, rError_t *errmsg, Node **errnode, Region *r);
ExprType *typeRuleSet(RuleSet *ruleset, rError_t *errmsg, Node **errnode, Region *r);
int initializeEnv(Node *ruleHead, Res **args, int argc, Hashtable *env, Region *r);
void addCmdExecOutToEnv(Hashtable *global, Region *r);
Node *getRuleNode(int ri);
int generateRuleTypes(RuleSet *inRuleSet, Hashtable *symbol_type_table, Region *r);
void copyFromEnv(Res**params, char **paramNames, int paramsCount, Hashtable *env, Region *r);
int actionTableLookUp (char *action);
rError_t *newRError();
Res *parseAndComputeExpressionNewEnv(char *inAction, msParamArray_t *inMsParamArray,
		  ruleExecInfo_t *rei, int reiSaveFlag, Region *r);
#endif
