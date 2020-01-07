/*---------------------------------------------------------------------------
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version. See also the license.txt file for
 *	additional informations.
 *
 */

/* input.h: interface for the input class. */

#if !defined(AFX_INPUT_H__59701BE2_A97A_11D3_8645_00A0241D2A65__INCLUDED_)
#define AFX_INPUT_H__59701BE2_A97A_11D3_8645_00A0241D2A65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif /* _MSC_VER > 1000 */

extern unsigned char	ngpInputState;

BOOL InitInput(void);
void UpdateInputState(void);
void FreeInput(void);

#endif /* !defined(AFX_INPUT_H__59701BE2_A97A_11D3_8645_00A0241D2A65__INCLUDED_) */
