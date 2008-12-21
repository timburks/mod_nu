/* 
**  mod_nu.c -- Apache sample nu module
**  [Autogenerated via ``apxs -n nu -g'']
**
**  To play with this sample module first compile it into a
**  DSO file and install it into Apache's modules directory 
**  by running:
**
**    $ apxs -c -i mod_nu.c
**
**  Then activate it in Apache's httpd.conf file for instance
**  for the URL /nu in as follows:
**
**    #   httpd.conf
**    LoadModule nu_module modules/mod_nu.so
**    <Location /nu>
**    SetHandler nu
**    </Location>
**
**  Then after restarting Apache via
**
**    $ apachectl restart
**
**  you immediately can request the URL /nu and watch for the
**  output of this module. This can be achieved for instance via:
**
**    $ lynx -mime_header http://localhost/nu 
**
**  The output should be similar to the following one:
**
**    HTTP/1.1 200 OK
**    Date: Tue, 31 Mar 1998 14:42:22 GMT
**    Server: Apache/1.3.4 (Unix)
**    Connection: close
**    Content-Type: text/html
**  
**    The sample page from mod_nu.c
*/ 
#import <Foundation/Foundation.h>
#import <Nu/Nu.h>

#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"

@interface ApacheRequest : NSObject {
  request_rec *request;
}
@end


NSString *stringWithCString(char *cString) {
   if (cString) {
	return [NSString stringWithCString:cString encoding:NSUTF8StringEncoding];
   } else {
	return nil;
   }
}

int extract_table_entry(void *rec, const char *key, const char *value) {
  [((NSMutableDictionary *) rec) setObject:stringWithCString(value) forKey:stringWithCString(key)];
  return 1;
}

@implementation ApacheRequest
- (void) setRequest:(request_rec *) r {request = r;}
- (NSString *) unparsed_uri {return stringWithCString(request->unparsed_uri);}
- (NSString *) uri 	    {return stringWithCString(request->uri);}
- (NSString *) filename     {return stringWithCString(request->filename);}
- (NSString *) path_info    {return stringWithCString(request->path_info);}
- (NSString *) args 	    {return stringWithCString(request->args);}

// this isn't quite right -- apache tables can have multiple entries per key.
- (NSMutableDictionary *) headers {
   NSMutableDictionary *dict = [NSMutableDictionary dictionary];
   apr_table_do(&extract_table_entry, (void *)dict, request->headers_in, NULL);
   return dict;
}
@end

typedef struct {
  char *string;
} modnu_config;

module AP_MODULE_DECLARE_DATA nu_module;

#ifndef DEFAULT_MODTUT2_STRING
#define DEFAULT_MODTUT2_STRING "/home/tim/work/mod_nu/apache.nu"
#endif

/* The sample content handler */
static int nu_handler(request_rec *r)
{
    if (strcmp(r->handler, "nu")) {
        return DECLINED;
    }
    r->content_type = "text/html";      
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSError *error;

    // Get the module configuration
    NSString *path = nil;

    modnu_config *perdir_cfg = ap_get_module_config(r->per_dir_config, &nu_module);
    if (perdir_cfg && perdir_cfg->string) {
        path = [[NSString alloc] initWithCString:perdir_cfg->string encoding:NSUTF8StringEncoding];
    } else {
        modnu_config *s_cfg = ap_get_module_config(r->server->module_config, &nu_module);
        if (s_cfg && s_cfg->string) {
            path = [[NSString alloc] initWithCString:s_cfg->string encoding:NSUTF8StringEncoding];
        }
    }
    NSString *string = path ? [NSString stringWithContentsOfFile:path] : @"\"Server error: Nu module not configured\"";

    ApacheRequest *request = [[ApacheRequest alloc] init];
    [request setRequest:r];

    NSString *result;
    if (!string) {
        result = [error description];
    } else {
        id parser = [Nu parser];
	[parser setValue:request forKey:@"request"];
        result = [parser parseEval:string];
    }

    if (!r->header_only) {
        ap_rputs([result cStringUsingEncoding:NSUTF8StringEncoding], r);
    }

    [request release];
    [pool release];
    return OK;
}

static void nu_register_hooks(apr_pool_t *p)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NuInit();
    [pool release];
    ap_hook_handler(nu_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

static void *create_modnu_perdir_config(apr_pool_t *p, server_rec *s)
{
  modnu_config *newcfg = (modnu_config *) apr_pcalloc(p, sizeof(modnu_config));
  newcfg->string = 0;
  return (void *) newcfg;
}

static void *create_modnu_config(apr_pool_t *p, server_rec *s)
{
  modnu_config *newcfg = (modnu_config *) apr_pcalloc(p, sizeof(modnu_config));
  newcfg->string = 0;
  return (void *) newcfg;
}

static const char *set_modnu_perdir_string(cmd_parms *parms, void *mconfig, const char *arg)
{
  modnu_config *s_cfg = (modnu_config *) mconfig;
  s_cfg->string = (char *) arg;
  return NULL;
}

static const char *set_modnu_string(cmd_parms *parms, void *mconfig, const char *arg)
{
  modnu_config *s_cfg = ap_get_module_config(parms->server->module_config, &nu_module);
  s_cfg->string = (char *) arg;
  return NULL;
}

static const command_rec mod_nu_cmds[] =
{
  AP_INIT_TAKE1(
    "NuDefaultHandler",
    set_modnu_string,
    NULL,
    RSRC_CONF,
    "NuDefaultHandler (string) Path to the Nu request handler."
  ),
  AP_INIT_TAKE1(
    "NuHandler",
    set_modnu_perdir_string,
    NULL,
    ACCESS_CONF,
    "NuHandler (string) Path to the Nu request handler."
  ),
  {NULL}
};

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA nu_module = {
    STANDARD20_MODULE_STUFF, 
    create_modnu_perdir_config, /* create per-dir    config structures */
    NULL,                       /* merge  per-dir    config structures */
    create_modnu_config,        /* create per-server config structures */
    NULL,                       /* merge  per-server config structures */
    mod_nu_cmds,              /* table of config file commands       */
    nu_register_hooks           /* register hooks                      */
};

