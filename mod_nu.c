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

/* The sample content handler */
static int nu_handler(request_rec *r)
{
    if (strcmp(r->handler, "nu")) {
        return DECLINED;
    }
    r->content_type = "text/html";      

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSError *error;
    NSString *string = [NSString stringWithContentsOfFile:@"/Users/tim/mod_nu/apache.nu" encoding:NSUTF8StringEncoding error:&error];

    NSString *result;
    if (!string) {
        result = [error description];
    } else {
        id parser = [Nu parser];
        result = [parser parseEval:string];
    }

    if (!r->header_only) {
        ap_rputs([result cStringUsingEncoding:NSUTF8StringEncoding], r);
    }

    [pool release];
    return OK;
}

static void nu_register_hooks(apr_pool_t *p)
{
    NuInit();
    ap_hook_handler(nu_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA nu_module = {
    STANDARD20_MODULE_STUFF, 
    NULL,                  /* create per-dir    config structures */
    NULL,                  /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    NULL,                  /* table of config file commands       */
    nu_register_hooks  /* register hooks                      */
};

