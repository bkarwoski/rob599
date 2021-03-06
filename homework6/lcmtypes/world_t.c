// THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
// BY HAND!!
//
// Generated by lcm-gen

#include <string.h>
#include "world_t.h"

static int __world_t_hash_computed;
static uint64_t __world_t_hash;

uint64_t __world_t_hash_recursive(const __lcm_hash_ptr *p)
{
    const __lcm_hash_ptr *fp;
    for (fp = p; fp != NULL; fp = fp->parent)
        if (fp->v == __world_t_get_hash)
            return 0;

    __lcm_hash_ptr cp;
    cp.parent =  p;
    cp.v = __world_t_get_hash;
    (void) cp;

    uint64_t hash = (uint64_t)0xecb7acc198c7cbe0LL
         + __agent_t_hash_recursive(&cp)
         + __agent_t_hash_recursive(&cp)
        ;

    return (hash<<1) + ((hash>>63)&1);
}

int64_t __world_t_get_hash(void)
{
    if (!__world_t_hash_computed) {
        __world_t_hash = (int64_t)__world_t_hash_recursive(NULL);
        __world_t_hash_computed = 1;
    }

    return __world_t_hash;
}

int __world_t_encode_array(void *buf, int offset, int maxlen, const world_t *p, int elements)
{
    int pos = 0, element;
    int thislen;

    for (element = 0; element < elements; element++) {

        thislen = __agent_t_encode_array(buf, offset + pos, maxlen - pos, &(p[element].runner), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __agent_t_encode_array(buf, offset + pos, maxlen - pos, &(p[element].chaser), 1);
        if (thislen < 0) return thislen; else pos += thislen;

    }
    return pos;
}

int world_t_encode(void *buf, int offset, int maxlen, const world_t *p)
{
    int pos = 0, thislen;
    int64_t hash = __world_t_get_hash();

    thislen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;

    thislen = __world_t_encode_array(buf, offset + pos, maxlen - pos, p, 1);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

int __world_t_encoded_array_size(const world_t *p, int elements)
{
    int size = 0, element;
    for (element = 0; element < elements; element++) {

        size += __agent_t_encoded_array_size(&(p[element].runner), 1);

        size += __agent_t_encoded_array_size(&(p[element].chaser), 1);

    }
    return size;
}

int world_t_encoded_size(const world_t *p)
{
    return 8 + __world_t_encoded_array_size(p, 1);
}

int __world_t_decode_array(const void *buf, int offset, int maxlen, world_t *p, int elements)
{
    int pos = 0, thislen, element;

    for (element = 0; element < elements; element++) {

        thislen = __agent_t_decode_array(buf, offset + pos, maxlen - pos, &(p[element].runner), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __agent_t_decode_array(buf, offset + pos, maxlen - pos, &(p[element].chaser), 1);
        if (thislen < 0) return thislen; else pos += thislen;

    }
    return pos;
}

int __world_t_decode_array_cleanup(world_t *p, int elements)
{
    int element;
    for (element = 0; element < elements; element++) {

        __agent_t_decode_array_cleanup(&(p[element].runner), 1);

        __agent_t_decode_array_cleanup(&(p[element].chaser), 1);

    }
    return 0;
}

int world_t_decode(const void *buf, int offset, int maxlen, world_t *p)
{
    int pos = 0, thislen;
    int64_t hash = __world_t_get_hash();

    int64_t this_hash;
    thislen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &this_hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;
    if (this_hash != hash) return -1;

    thislen = __world_t_decode_array(buf, offset + pos, maxlen - pos, p, 1);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

int world_t_decode_cleanup(world_t *p)
{
    return __world_t_decode_array_cleanup(p, 1);
}

int __world_t_clone_array(const world_t *p, world_t *q, int elements)
{
    int element;
    for (element = 0; element < elements; element++) {

        __agent_t_clone_array(&(p[element].runner), &(q[element].runner), 1);

        __agent_t_clone_array(&(p[element].chaser), &(q[element].chaser), 1);

    }
    return 0;
}

world_t *world_t_copy(const world_t *p)
{
    world_t *q = (world_t*) malloc(sizeof(world_t));
    __world_t_clone_array(p, q, 1);
    return q;
}

void world_t_destroy(world_t *p)
{
    __world_t_decode_array_cleanup(p, 1);
    free(p);
}

int world_t_publish(lcm_t *lc, const char *channel, const world_t *p)
{
      int max_data_size = world_t_encoded_size (p);
      uint8_t *buf = (uint8_t*) malloc (max_data_size);
      if (!buf) return -1;
      int data_size = world_t_encode (buf, 0, max_data_size, p);
      if (data_size < 0) {
          free (buf);
          return data_size;
      }
      int status = lcm_publish (lc, channel, buf, data_size);
      free (buf);
      return status;
}

struct _world_t_subscription_t {
    world_t_handler_t user_handler;
    void *userdata;
    lcm_subscription_t *lc_h;
};
static
void world_t_handler_stub (const lcm_recv_buf_t *rbuf,
                            const char *channel, void *userdata)
{
    int status;
    world_t p;
    memset(&p, 0, sizeof(world_t));
    status = world_t_decode (rbuf->data, 0, rbuf->data_size, &p);
    if (status < 0) {
        fprintf (stderr, "error %d decoding world_t!!!\n", status);
        return;
    }

    world_t_subscription_t *h = (world_t_subscription_t*) userdata;
    h->user_handler (rbuf, channel, &p, h->userdata);

    world_t_decode_cleanup (&p);
}

world_t_subscription_t* world_t_subscribe (lcm_t *lcm,
                    const char *channel,
                    world_t_handler_t f, void *userdata)
{
    world_t_subscription_t *n = (world_t_subscription_t*)
                       malloc(sizeof(world_t_subscription_t));
    n->user_handler = f;
    n->userdata = userdata;
    n->lc_h = lcm_subscribe (lcm, channel,
                                 world_t_handler_stub, n);
    if (n->lc_h == NULL) {
        fprintf (stderr,"couldn't reg world_t LCM handler!\n");
        free (n);
        return NULL;
    }
    return n;
}

int world_t_subscription_set_queue_capacity (world_t_subscription_t* subs,
                              int num_messages)
{
    return lcm_subscription_set_queue_capacity (subs->lc_h, num_messages);
}

int world_t_unsubscribe(lcm_t *lcm, world_t_subscription_t* hid)
{
    int status = lcm_unsubscribe (lcm, hid->lc_h);
    if (0 != status) {
        fprintf(stderr,
           "couldn't unsubscribe world_t_handler %p!\n", hid);
        return -1;
    }
    free (hid);
    return 0;
}

