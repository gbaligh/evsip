/**
 * @file evsip_cli_cmdlst.c
 * @brief
 */

#include "evsip_glob.h"
#include "evsip_evsofia.h"
#include "evsip_log.h"
#include "evsip_types.h"
#include <sofia-sip/rbtree.h>
#include <assert.h>
#include "evsip_cli_cmdlst.h"

/**
 * @brief Magic used for command Object
 */
#define EVSIP_CMD_MAGIC	0x20150324

/**
 * @brief Command Object flags
 */
enum {
	EVSIP_CMD_FLAGS_RED 		= (1 << 0),     //!< EVSIP_CMD_FLAGS_RED
	EVSIP_CMD_FLAGS_INSERTED	= (1 << 1),  	//!< EVSIP_CMD_FLAGS_INSERTED
	EVSIP_CMD_FLAGS_PRIVILEGED	= (1 << 2),		//!< EVSIP_CMD_FLAGS_PRIVILEGED
};

/**
 * @brief Default filter available for the CLI
 */
struct evsip_cli_cmd_filters_str {
	const char *cmd; 	//!< Command name
	const char *help;	//!< Command help message
};

/**
 * Default filter available for the CLI
 */
static struct evsip_cli_cmd_filters_str filter_cmds[] =
{
		{ "begin",   "Begin with lines that match" },
		{ "between", "Between lines that match" },
		{ "count",   "Count of lines"   },
		{ "exclude", "Exclude lines that match" },
		{ "include", "Include lines that match" },
		{ "grep",    "Include lines that match regex (options: -v, -i, -e)" },
		{ "egrep",   "Include lines that match extended regex" },
		{ NULL, NULL}
};

/**
 * @brief Command Object
 */
struct evsip_cli_cmd_str {
	unsigned int magic;					//!< Magic
	char *cmd;							//!< Command string
	char *short_cmd;					//!< Short of the command
	char *help_msg;						//!< Help message
	unsigned int flags;					//!< Flags of the commands object.
	unsigned int privilege;				//!< Access right for this command
	unsigned int mode;					//!< Configuration or Status command

	struct evsip_cli_cmd_str *parent;	//!< RBTree parent
	struct evsip_cli_cmd_str *left;		//!< RBTree left child
	struct evsip_cli_cmd_str *right;	//!< RBTree right child
};

/** @addtogroup CmdRbTree Commands RBTree
 *  @{
 */
/**
 * Left node
 */
#define LEFT(node) \
	((node)->left)
/**
 * Reght node
 */
#define RIGHT(node) \
	((node)->right)
/**
 * Parent node
 */
#define PARENT(node) \
	((node)->parent)
/**
 * Set node red
 */
#define SET_RED(node) \
	((node)->flags |= EVSIP_CMD_FLAGS_RED)
/**
 * set node black
 */
#define SET_BLACK(node) \
	((node)->flags &= ~EVSIP_CMD_FLAGS_RED)
/**
 * compare two nodes
 */
#define CMP(a, b) \
	(strcmp((a)->cmd, (b)->cmd))
/**
 * Check if node is red
 */
#define IS_RED(node) \
	((node) && ((node)->flags & EVSIP_CMD_FLAGS_RED))
/**
 * Check if node is black
 */
#define IS_BLACK(node) \
	((node) && ((node)->flags & EVSIP_CMD_FLAGS_RED))
/**
 * Copy color from one node to other
 */
#define CP_COLOR(d, s) \
	((d)->flags &= ((s)->flags & EVSIP_CMD_FLAGS_RED))
/**
 * Insert node
 */
#define INSERT(node) \
	((node)->flags |= EVSIP_CMD_FLAGS_INSERTED)
/**
 * Remove node
 */
#define REMOVE(node) \
	((node)->left = (node)->right = (node)->parent = NULL, \
	(node)->flags &= ~EVSIP_CMD_FLAGS_INSERTED)
/**
 * RBTree Functions Prototypes declaration
 */
RBTREE_PROTOS(su_inline, evsip_cmd_rbtree, struct evsip_cli_cmd_str);

/**
 * RBTree Functions implementation
 */
RBTREE_BODIES(su_inline, evsip_cmd_rbtree, struct evsip_cli_cmd_str ,
		LEFT, RIGHT, PARENT,
		IS_RED, SET_RED,
		IS_BLACK, SET_BLACK,
		CP_COLOR, CMP, INSERT, REMOVE);

/** @} */

static evsip_cli_cmd_t *pGlobCmdCtxTree = (evsip_cli_cmd_t *)0;

unsigned int evsip_cli_cmd_init(evsip_cli_cmd_t **pCtx,
		const char *cmd,
		const char *help,
		const unsigned int privilege,
		const unsigned int mode)
{
	EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_DEBUG, "Enter: %s", __FUNCTION__);

	struct evsip_cli_cmd_str *_pCmdCtx = (struct evsip_cli_cmd_str *)0;

	_pCmdCtx = su_alloc(evSipGlobCtx->memPage, sizeof (struct evsip_cli_cmd_str));
	if (_pCmdCtx == (struct evsip_cli_cmd_str *)0) {
		EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "Alloc");
		return (EVSIP_ERROR_OUTOFRESOURCES);
	}

	memset(_pCmdCtx, 0, sizeof (struct evsip_cli_cmd_str));
	_pCmdCtx->magic = EVSIP_CMD_MAGIC;

	_pCmdCtx->cmd = su_strdup(evSipGlobCtx->memPage, cmd);
	if (_pCmdCtx->cmd == (char *)0) {
		EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "strdup cmd");
		su_free(evSipGlobCtx->memPage, _pCmdCtx);
		return (EVSIP_ERROR_OUTOFRESOURCES);
	}

	_pCmdCtx->help_msg = su_strdup(evSipGlobCtx->memPage, help);
	if (_pCmdCtx->help_msg == (char *)0) {
		EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "strdup help msg");
		su_free(evSipGlobCtx->memPage, _pCmdCtx->cmd);
		su_free(evSipGlobCtx->memPage, _pCmdCtx);
		return (EVSIP_ERROR_OUTOFRESOURCES);
	}

	_pCmdCtx->mode = mode;
	_pCmdCtx->privilege = privilege;

	*pCtx = _pCmdCtx;

	EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_DEBUG, "End.");
	return (EVSIP_SUCCESS);
}

unsigned int evsip_cli_cmd_register(evsip_cli_cmd_t *pCmdCtx)
{
	EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_DEBUG, "Enter: %s", __FUNCTION__);

	struct evsip_cli_cmd_str *_pCmdCtx = (struct evsip_cli_cmd_str *)pCmdCtx;
	struct evsip_cli_cmd_str *_pCmdOldCtx = (struct evsip_cli_cmd_str *)0;

	if (_pCmdCtx == (struct evsip_cli_cmd_str *)0) {
		EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "INVALID handler");
		return (EVSIP_ERROR_BADPARAM);
	}

	if (_pCmdCtx->magic != EVSIP_CMD_MAGIC) {
		EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "magic");
		return (EVSIP_ERROR_INVALID_HANDLE);
	}

	EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_DEBUG, "Register %s", _pCmdCtx->cmd);
	evsip_cmd_rbtree_insert(&pGlobCmdCtxTree, _pCmdCtx, &_pCmdOldCtx);

	EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_DEBUG, "End.");
	return (EVSIP_SUCCESS);
}

unsigned int evsip_cli_cmd_find(const char *pCmd, evsip_cli_cmd_t **pCmdCtx)
{
	EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_DEBUG, "Enter: %s", __FUNCTION__);

	struct evsip_cli_cmd_str *_pCmdCtx = (struct evsip_cli_cmd_str *)0;

	if (pCmdCtx == (evsip_cli_cmd_t **)0) {
		EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "NULL pointer given");
		return EVSIP_ERROR_BADPARAM;
	}

	if (pCmd == (char *)0) {
		EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "NULL pointer given");
		return EVSIP_ERROR_BADPARAM;
	}

	for (_pCmdCtx = evsip_cmd_rbtree_first(pGlobCmdCtxTree); _pCmdCtx; _pCmdCtx = _pCmdCtx->left) {
		if (_pCmdCtx->magic != EVSIP_CMD_MAGIC) {
			EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "address %p [%ud != %ud]", _pCmdCtx->magic, EVSIP_CMD_MAGIC);
			continue;
		}
		if (strncmp(_pCmdCtx->cmd, pCmd, strlen(_pCmdCtx->cmd)) == 0) {
			*pCmdCtx = _pCmdCtx;
			return (EVSIP_SUCCESS);
		}
	}

	EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_DEBUG, "End.");
	return (EVSIP_ERROR_NOT_FOUND);
}

const char *evsip_cli_cmd_str(evsip_cli_cmd_t *pCmdCtx)
{
	struct evsip_cli_cmd_str *_pCmdCtx = (struct evsip_cli_cmd_str *)pCmdCtx;

	if (_pCmdCtx == (struct evsip_cli_cmd_str *)0) {
		EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "INVALID handler");
		goto EXIT;
	}

	if (_pCmdCtx->magic != EVSIP_CMD_MAGIC) {
		EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "address %p [%ud != %ud]", _pCmdCtx->magic, EVSIP_CMD_MAGIC);
		goto EXIT;
	}

	return _pCmdCtx->cmd;
	EXIT:
	return (const char *)0;
}

unsigned int evsip_cli_cmd_execute(evsip_cli_cmd_t *pCmdCtx)
{
	struct evsip_cli_cmd_str *_pCmdCtx = (struct evsip_cli_cmd_str *)pCmdCtx;

	if (_pCmdCtx == (struct evsip_cli_cmd_str *)0) {
		EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "INVALID handler");
		return EVSIP_ERROR_BADPARAM;
	}

	if (_pCmdCtx->magic != EVSIP_CMD_MAGIC) {
		EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "address %p [%ud != %ud]", _pCmdCtx->magic, EVSIP_CMD_MAGIC);
		return EVSIP_ERROR_INVALID_HANDLE;
	}

	fprintf(stderr, "Not implemented yet: (%s[%s])\n", _pCmdCtx->cmd, _pCmdCtx->help_msg);

	return EVSIP_SUCCESS;
}

void evsip_cli_cmd_destroy(evsip_cli_cmd_t *pCmdCtx)
{
	EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_DEBUG, "Enter: %s", __FUNCTION__);

	struct evsip_cli_cmd_str *_pCmdCtx = (struct evsip_cli_cmd_str *)pCmdCtx;

	if (_pCmdCtx == (struct evsip_cli_cmd_str *)0) {
		EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "INVALID handler");
		goto EXIT;
	}

	if (_pCmdCtx->magic != EVSIP_CMD_MAGIC) {
		EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "address %p [%ud != %ud]", _pCmdCtx->magic, EVSIP_CMD_MAGIC);
		goto EXIT;
	}

	evsip_cmd_rbtree_remove(&pGlobCmdCtxTree, _pCmdCtx);

	su_free(evSipGlobCtx->memPage, _pCmdCtx->cmd);
	su_free(evSipGlobCtx->memPage, _pCmdCtx->help_msg);
	memset(_pCmdCtx, 0, sizeof (struct evsip_cli_cmd_str));
	su_free(evSipGlobCtx->memPage, _pCmdCtx);
	EXIT:
	EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_DEBUG, "End.");
}
