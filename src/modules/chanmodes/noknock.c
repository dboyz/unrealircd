/*
 * Disallow knocks UnrealIRCd Module (Channel Mode +K)
 * (C) Copyright 2014 Travis McArthur (Heero) and the UnrealIRCd team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 1, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "config.h"
#include "struct.h"
#include "common.h"
#include "sys.h"
#include "numeric.h"
#include "msg.h"
#include "proto.h"
#include "channel.h"
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <io.h>
#endif
#include <fcntl.h>
#include "h.h"
#ifdef _WIN32
#include "version.h"
#endif

ModuleHeader MOD_HEADER(noknock)
  = {
	"chanmodes/noknock",
	"$Id$",
	"Channel Mode +V",
	"3.2-b8-1",
	NULL 
    };

Cmode_t EXTCMODE_NOKNOCK;

#define IsNoKnock(chptr)    (chptr->mode.extmode & EXTCMODE_NOKNOCK)

DLLFUNC int noknock_check (aClient *sptr, aChannel *chptr);
DLLFUNC int noknock_mode_allow(aClient *cptr, aChannel *chptr, char mode, char *para, int checkt, int what);
DLLFUNC int noknock_mode_del (aChannel *chptr, int modeChar);

DLLFUNC int MOD_TEST(noknock)(ModuleInfo *modinfo)
{
	return MOD_SUCCESS;
}

DLLFUNC int MOD_INIT(noknock)(ModuleInfo *modinfo)
{
CmodeInfo req;

	memset(&req, 0, sizeof(req));
	req.paracount = 0;
	req.flag = 'K';
	req.is_ok = noknock_mode_allow;
	CmodeAdd(modinfo->handle, req, &EXTCMODE_NOKNOCK);
	
	HookAddEx(modinfo->handle, HOOKTYPE_PRE_KNOCK, noknock_check);
	HookAddEx(modinfo->handle, HOOKTYPE_MODECHAR_DEL, noknock_mode_del);

	
	MARK_AS_OFFICIAL_MODULE(modinfo);
	return MOD_SUCCESS;
}

DLLFUNC int MOD_LOAD(noctcp)(int module_load)
{
	return MOD_SUCCESS;
}

DLLFUNC int MOD_UNLOAD(noctcp)(int module_unload)
{
	return MOD_SUCCESS;
}


DLLFUNC int noknock_check (aClient *sptr, aChannel *chptr)
{
	if (MyClient(sptr) && IsNoKnock(chptr))
	{
		sendto_one(sptr, err_str(ERR_CANNOTKNOCK),
				    me.name,
				    sptr->name, chptr->chname, "No knocks are allowed! (+K)");
		return HOOK_DENY;
	}

	return HOOK_CONTINUE;
}

DLLFUNC int noknock_mode_del (aChannel *chptr, int modeChar)
{
	// Remove noknock when we're removing invite only
	if (modeChar == 'i')
		chptr->mode.extmode &= ~EXTCMODE_NOKNOCK;

	return 0;
}

DLLFUNC int noknock_mode_allow(aClient *cptr, aChannel *chptr, char mode, char *para, int checkt, int what)
{

	if (!(chptr->mode.mode & MODE_INVITEONLY))
	{
		sendto_one(cptr, err_str(ERR_CANNOTCHANGECHANMODE),
						me.name, cptr->name, 'K', "+i must be set");
		return EX_DENY;
	}

	return extcmode_default_requirechop(cptr,chptr,mode,para,checkt,what);
}

