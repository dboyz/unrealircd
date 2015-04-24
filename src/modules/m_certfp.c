/*   IRC - Internet Relay Chat, src/modules/m_certfp.c
 *   (C) 2015 The UnrealIRCd Team
 *
 *   Server to server CERTFP command
 *
 *   See file AUTHORS in IRC package for additional names of
 *   the programmers.
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 1, or (at your option)
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include "config.h"
#include "struct.h"
#include "common.h"
#include "sys.h"
#include "numeric.h"
#include "msg.h"
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
#include "proto.h"
#ifdef STRIPBADWORDS
#include "badwords.h"
#endif
#ifdef _WIN32
#include "version.h"
#endif

DLLFUNC int m_certfp(aClient *cptr, aClient *sptr, int parc, char *parv[]);
static Command* cmdCertfp = NULL;

#define MSG_CERTFP "CERTFP"

ModuleHeader MOD_HEADER(m_certfp)
  = {
	"m_certfp",
	"$Id$",
	"Server to Server CERTFP command",
	"3.2-b8-1",
	NULL 
    };

DLLFUNC int MOD_INIT(m_certfp)(ModuleInfo *modinfo)
{
	cmdCertfp = CommandAdd(modinfo->handle, MSG_CERTFP, m_certfp, MAXPARA, 0);
	MARK_AS_OFFICIAL_MODULE(modinfo);
	return MOD_SUCCESS;
}

DLLFUNC int MOD_LOAD(m_certfp)(int module_load)
{
	return MOD_SUCCESS;
}

DLLFUNC int MOD_UNLOAD(m_certfp)(int module_unload)
{
	if (cmdCertfp)
	{
		CommandDel(cmdCertfp);
		cmdCertfp = NULL;
	}
	return MOD_SUCCESS;
}
/*
 * m_certfp
 * parv[1] = nickname
 * parv[2] = certfp string
 *
*/

int m_certfp(aClient *cptr, aClient *sptr, int parc, char *parv[])
{
#ifdef USE_SSL
	aClient *acptr;

	if (!IsServer(sptr))
		return 0;

	if (parc < 3)
		return 0;

	acptr = find_person(parv[1], (aClient *)NULL);
       if (!acptr)
		return 0;

	acptr->certfp = strdup(parv[2]);
	
#endif
	return 0;
}