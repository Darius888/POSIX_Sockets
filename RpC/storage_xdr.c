/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "storage.h"

bool_t
xdr_put_1_argument (XDR *xdrs, put_1_argument *objp)
{
	 if (!xdr_string (xdrs, &objp->topic, 128))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->text, 1024))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_get_1_argument (XDR *xdrs, get_1_argument *objp)
{
	 if (!xdr_string (xdrs, &objp->topic, 128))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->text, 1024))
		 return FALSE;
	return TRUE;
}
