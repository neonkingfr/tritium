#include "cst_error.h"
#include "cst_features.h"

CST_VAL_REGISTER_TYPE(features,cst_features)

static cst_featvalpair *feat_find_featpair(const cst_features *f, 
					   const char *name)
{
    cst_featvalpair *n;
    
    if (f == NULL)
	return NULL;
    else
    {
	for (n=f->head; n; n=n->next)
	    if (streq(name,n->name))
		return n;
	return NULL;
    }
}

cst_features *new_features(void)
{
    cst_features *f;

    f = cst_alloc(cst_features,1);
    f->head = NULL;
    f->ctx = NULL;
    return f;
}

cst_features *new_features_local(cst_alloc_context ctx)
{
    cst_features *f;

    f = (cst_features *)cst_local_alloc(ctx, sizeof(*f));
    f->head = NULL;
    f->ctx = ctx;
    return f;
}

void delete_features(cst_features *f)
{
    cst_featvalpair *n, *np;

    if (f)
    {
	for (n=f->head; n; n=np)
	{
	    np = n->next;
	    delete_val(n->val);
	    cst_local_free(f->ctx,n);
	}
	cst_local_free(f->ctx,f);
    }
}

int feat_present(const cst_features *f, const char *name)
{
    return (feat_find_featpair(f,name) != NULL);
}

int feat_length(const cst_features *f)
{
    int i=0;
    cst_featvalpair *p;
    if (f)
	for (i=0,p=f->head; p; p=p->next)
	    i++;
    return i;
}

int feat_remove(cst_features *f, const char *name)
{
    cst_featvalpair *n,*p,*np;
    
    if (f == NULL)
	return FALSE; /* didn't remove it */
    else
    {
	for (p=NULL,n=f->head; n; p=n,n=np)
	{
	    np = n->next;
	    if (streq(name,n->name))
	    {
		if (p == 0)
		    f->head = np;
		else
		    p->next = np;
		delete_val(n->val);
		cst_local_free(f->ctx,n);
		return TRUE;
	    }
	}
	return FALSE;
    }
}

int feat_int(const cst_features *f, const char *name)
{
    return val_int(feat_val(f,name));
}

float feat_float(const cst_features *f, const char *name)
{
    return val_float(feat_val(f,name));
}

const char *feat_string(const cst_features *f, const char *name)
{
    return val_string(feat_val(f,name));
}

const cst_val *feat_val(const cst_features *f, const char *name)
{
    cst_featvalpair *n;
    n = feat_find_featpair(f,name);

    if (n == NULL)
	return NULL;
    else
	return n->val;
}

int get_param_int(const cst_features *f, const char *name,int def)
{
    const cst_val *v;
    
    v = feat_val(f,name);
    if (v != NULL)
        return val_int(v);
    else
	return def;
}

float get_param_float(const cst_features *f, const char *name, float def)
{
    const cst_val *v;
    
    v = feat_val(f,name);
    if (v != NULL)
        return val_float(v);
    else
	return def;
}
const char *get_param_string(const cst_features *f, const char *name, const char *def)
{
    const cst_val *v;
    
    v = feat_val(f,name);
    if (v != NULL)
        return val_string(v);
    else
	return def;
}

const cst_val *get_param_val(const cst_features *f, const char *name, cst_val *def)
{
    const cst_val *v;
    
    v = feat_val(f,name);
    if (v != NULL)
        return v;
    else
	return def;
}

void feat_set_int(cst_features *f, const char *name, int v)
{
    feat_set(f,name,int_val(v));
}

void feat_set_float(cst_features *f, const char *name, float v)
{
    feat_set(f,name,float_val(v));
}

void feat_set_string(cst_features *f, const char *name, const char *v)
{
    feat_set(f,name,string_val(v));
}

void feat_set(cst_features *f, const char* name, const cst_val *val)
{
    cst_featvalpair *n;
    n = feat_find_featpair(f,name);

    if (val == NULL)
    {
	cst_errmsg("cst_val: trying to set a NULL val for feature \"%s\"\n",
		   name);
    }
    else if (n == NULL)
    {   /* first reference to this feature so create new fpair */
	cst_featvalpair *p;
	p = (cst_featvalpair *)cst_local_alloc(f->ctx, sizeof(*p));
	p->next = f->head;
	p->name = name; 
	p->val = val_inc_refcount(val);
	f->head = p;
    }
    else
    {
	delete_val(n->val);
	n->val = val_inc_refcount(val);
    }
}

int feat_copy_into(const cst_features *from,cst_features *to)
{
    /* Copy all features in from into to */
    cst_featvalpair *p;
    int i;
    
    for (i=0,p=from->head; p; p=p->next,i++)
	feat_set(to,p->name,p->val);
    
    return i;
}