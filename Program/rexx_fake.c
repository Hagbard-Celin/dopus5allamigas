/*

Directory Opus 5
Original APL release version 5.82
Copyright 1993-2012 Jonathan Potter & GP Software

This program is free software; you can redistribute it and/or
modify it under the terms of the AROS Public License version 1.1.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
AROS Public License for more details.

The release of Directory Opus 5 under the GPL in NO WAY affects
the existing commercial status of Directory Opus for Windows.

For more information on Directory Opus for Windows please see:

                 http://www.gpsoft.com.au

*/

#include "dopus.h"

// Generate a fake rexx message
long rexx_fake_msg(char *command,char **result)
{
	long count=0;
	struct RexxMsg *msg;
	struct MsgPort *port;

	// Clear result pointer
	if (result) *result=0;

	// Create message port
	if (!(port=CreateMsgPort()))
		return 0;

	// Create the message
	if (!(msg=CreateRexxMsg(port,0,0)))
	{
		DeleteMsgPort(port);
		return 0;
	}

	// Fill in argument
	
	// Want result?
	if (result) msg->rm_Action|=RXFF_RESULT;

	// Process it
	if (rexx_process_msg(msg,port,&count))
	{
		// Wait for reply
		WaitPort(port);
	}

	// Result string set?
	if (result && msg->rm_Result2)
	{
		// Allocate copy
		if (*result=AllocVec(strlen((char *)msg->rm_Result2)+1,0))
			strcpy(*result,(char *)msg->rm_Result2);
	}

