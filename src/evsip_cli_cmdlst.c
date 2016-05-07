/*
EasyVision SIP Server (EvSip), a test Server for SIP protocol.
Copyright (C) 2016  Baligh Guesmi

This file is part of EvSip.

EvSip is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

EvSip is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

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
 * @brief Command Object
 */
struct evsip_cli_cmd_str {
  unsigned int magic;					//!< Magic
  su_home_t home[1]; 
  char *cmd;							//!< Command string
  char *help_msg;						//!< Help message
  unsigned int flags;					//!< Flags of the commands object.
  unsigned int privilege;				//!< Access right for this command
  unsigned int mode;					//!< Configuration or Status command
  unsigned int (*exec_f)(void *exec_arg); //!<
  void *exec_arg; 

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
  struct evsip_cli_cmd_str *_pCmdCtx = (struct evsip_cli_cmd_str *)0;

  _pCmdCtx = su_home_clone(evSipGlobCtx->memPage, sizeof (struct evsip_cli_cmd_str));
  if (_pCmdCtx == (struct evsip_cli_cmd_str *)0) {
    EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "Alloc");
    return (EVSIP_ERROR_OUTOFRESOURCES);
  }

  memset(_pCmdCtx, 0, sizeof (struct evsip_cli_cmd_str));
  _pCmdCtx->magic = EVSIP_CMD_MAGIC;

  _pCmdCtx->cmd = su_strdup(_pCmdCtx->home, cmd);
  if (_pCmdCtx->cmd == (char *)0) {
    EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "strdup cmd");
    su_home_unref(_pCmdCtx->home);
    return (EVSIP_ERROR_OUTOFRESOURCES);
  }

  _pCmdCtx->help_msg = su_strdup(_pCmdCtx->home, help);
  if (_pCmdCtx->help_msg == (char *)0) {
    EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "strdup help msg");
    su_home_unref(_pCmdCtx->home);
    return (EVSIP_ERROR_OUTOFRESOURCES);
  }

  _pCmdCtx->mode = mode;
  _pCmdCtx->privilege = privilege;

  *pCtx = _pCmdCtx;

  return (EVSIP_SUCCESS);
}

unsigned int evsip_cli_cmd_register(evsip_cli_cmd_t *pCmdCtx, unsigned int (*exec_f)(void *), void *arg)
{
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

  if (exec_f == NULL) {
    EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "callback function not set");
    return (EVSIP_ERROR_INVALID_HANDLE);
  }

  _pCmdCtx->exec_f = exec_f;
  _pCmdCtx->exec_arg = arg;

  EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_DEBUG, "Registering <%s:%p> command into tree(%p)", _pCmdCtx->cmd, _pCmdCtx, pGlobCmdCtxTree);

  evsip_cmd_rbtree_insert(&pGlobCmdCtxTree, _pCmdCtx, &_pCmdOldCtx);

  EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_DEBUG, "Registered <%s:%p> command into tree(%p) with old(%p)", _pCmdCtx->cmd, _pCmdCtx, pGlobCmdCtxTree, _pCmdOldCtx);

  return (EVSIP_SUCCESS);
}

unsigned int evsip_cli_cmd_find(const char *pCmd, evsip_cli_cmd_t **pCmdCtx)
{
  struct evsip_cli_cmd_str *_pCmdCtx = (struct evsip_cli_cmd_str *)0;

  if (pCmdCtx == (evsip_cli_cmd_t **)0) {
    EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "NULL pointer given");
    return EVSIP_ERROR_BADPARAM;
  }

  if (pCmd == (char *)0) {
    EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "NULL pointer given");
    return EVSIP_ERROR_BADPARAM;
  }

  for (_pCmdCtx = pGlobCmdCtxTree; _pCmdCtx; _pCmdCtx = _pCmdCtx->left) {
    if (_pCmdCtx->magic != EVSIP_CMD_MAGIC) {
      EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "address %p [%ud != %ud]", _pCmdCtx->magic, EVSIP_CMD_MAGIC);
      continue;
    }
    if (strncmp(_pCmdCtx->cmd, pCmd, strlen(_pCmdCtx->cmd)) == 0) {
      *pCmdCtx = _pCmdCtx;
      return (EVSIP_SUCCESS);
    }
  }

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

  if ( _pCmdCtx->exec_f) {
    _pCmdCtx->exec_f(_pCmdCtx->exec_arg);
  } else
    fprintf(stderr, "==> Callbacks for commands are not implemented yet: %s[%s]\n", _pCmdCtx->cmd, _pCmdCtx->help_msg);

  return EVSIP_SUCCESS;
}

void evsip_cli_cmd_destroy(evsip_cli_cmd_t *pCmdCtx)
{
  struct evsip_cli_cmd_str *_pCmdCtx = (struct evsip_cli_cmd_str *)pCmdCtx;

  if (_pCmdCtx == (struct evsip_cli_cmd_str *)0) {
    EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "INVALID handler");
    return;
  }

  if (_pCmdCtx->magic != EVSIP_CMD_MAGIC) {
    EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "address %p [%ud != %ud]", _pCmdCtx->magic, EVSIP_CMD_MAGIC);
    return;
  }

  evsip_cmd_rbtree_remove(&pGlobCmdCtxTree, _pCmdCtx);
  su_home_unref(_pCmdCtx->home);
}


void evsip_cli_cmd_destroy_all()
{
	struct evsip_cli_cmd_str *_pCmdCtx = (struct evsip_cli_cmd_str *)0;

	for (_pCmdCtx = pGlobCmdCtxTree; _pCmdCtx; _pCmdCtx = _pCmdCtx->left) {
		if (_pCmdCtx->magic != EVSIP_CMD_MAGIC) {
			EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "address %p [%ud != %ud]", _pCmdCtx->magic, EVSIP_CMD_MAGIC);
			continue;
		}
		evsip_cmd_rbtree_remove(&pGlobCmdCtxTree, _pCmdCtx);
		su_home_unref(_pCmdCtx->home);
	}
}

static unsigned int evsip_cli_cmd_help_clbk(void *pRef)
{
  struct evsip_cli_cmd_str *_pCmdCtx = (struct evsip_cli_cmd_str *)0;

  for (_pCmdCtx = pGlobCmdCtxTree; _pCmdCtx; _pCmdCtx = _pCmdCtx->left) {
    if (_pCmdCtx->magic != EVSIP_CMD_MAGIC) {
      EVSIP_LOG(EVSIP_CLI, EVSIP_LOG_ERROR, "address %p [%ud != %ud]", _pCmdCtx->magic, EVSIP_CMD_MAGIC);
      continue;
    }
    
    /* TODO: CLI API to print messages */
    printf("\t%s: %s\n", _pCmdCtx->cmd, _pCmdCtx->help_msg);
  }

  return EVSIP_SUCCESS;
}


unsigned int evsip_cli_cmd_register_help_cmd()
{
  unsigned int _ret = EVSIP_SUCCESS;
  evsip_cli_cmd_t *_pCmdCtx = (evsip_cli_cmd_t *)0;

  _ret = evsip_cli_cmd_init(&_pCmdCtx, "help", "Help and list all available commands", 1, 1);
  if ( _ret != EVSIP_SUCCESS) {
    return _ret;
  }

  _ret = evsip_cli_cmd_register(_pCmdCtx, evsip_cli_cmd_help_clbk, (void*)0);
  if (_ret != EVSIP_SUCCESS) {
    return _ret;
  }

  return (EVSIP_SUCCESS);
}

//vim: noai:ts=2:sw=2
