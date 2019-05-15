/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _STORAGE_H_RPCGEN
#define _STORAGE_H_RPCGEN

#include <rpc/rpc.h>

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif


struct put_1_argument {
	char *topic;
	char *text;
};
typedef struct put_1_argument put_1_argument;

struct get_1_argument {
	char *topic;
	char *text;
};
typedef struct get_1_argument get_1_argument;

#define STORAGE 99
#define STORAGEVER 1

#if defined(__STDC__) || defined(__cplusplus)
#define INIT 1
extern  enum clnt_stat init_1(int *, CLIENT *);
extern  bool_t init_1_svc(int *, struct svc_req *);
#define PUT 2
extern  enum clnt_stat put_1(char *, char *, int *, CLIENT *);
extern  bool_t put_1_svc(char *, char *, int *, struct svc_req *);
#define GET 3
extern  enum clnt_stat get_1(char *, char *, char **, CLIENT *);
extern  bool_t get_1_svc(char *, char *, char **, struct svc_req *);
extern int storage_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define INIT 1
extern  enum clnt_stat init_1();
extern  bool_t init_1_svc();
#define PUT 2
extern  enum clnt_stat put_1();
extern  bool_t put_1_svc();
#define GET 3
extern  enum clnt_stat get_1();
extern  bool_t get_1_svc();
extern int storage_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_put_1_argument (XDR *, put_1_argument*);
extern  bool_t xdr_get_1_argument (XDR *, get_1_argument*);

#else /* K&R C */
extern bool_t xdr_put_1_argument ();
extern bool_t xdr_get_1_argument ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_STORAGE_H_RPCGEN */