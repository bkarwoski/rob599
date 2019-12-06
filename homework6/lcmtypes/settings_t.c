// THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
// BY HAND!!
//
// Generated by lcm-gen

#include <string.h>
#include "settings_t.h"

static int __settings_t_hash_computed;
static uint64_t __settings_t_hash;

uint64_t __settings_t_hash_recursive(const __lcm_hash_ptr *p)
{
    const __lcm_hash_ptr *fp;
    for (fp = p; fp != NULL; fp = fp->parent)
        if (fp->v == __settings_t_get_hash)
            return 0;

    __lcm_hash_ptr cp;
    cp.parent =  p;
    cp.v = __settings_t_get_hash;
    (void) cp;

    uint64_t hash = (uint64_t)0xa5c17cd2bfaee412LL
         + __int32_t_hash_recursive(&cp)
         + __int32_t_hash_recursive(&cp)
         + __double_hash_recursive(&cp)
         + __int32_t_hash_recursive(&cp)
         + __double_hash_recursive(&cp)
         + __int32_t_hash_recursive(&cp)
         + __double_hash_recursive(&cp)
        ;

    return (hash<<1) + ((hash>>63)&1);
}

int64_t __settings_t_get_hash(void)
{
    if (!__settings_t_hash_computed) {
        __settings_t_hash = (int64_t)__settings_t_hash_recursive(NULL);
        __settings_t_hash_computed = 1;
    }

    return __settings_t_hash;
}

int __settings_t_encode_array(void *buf, int offset, int maxlen, const settings_t *p, int elements)
{
    int pos = 0, element;
    int thislen;

    for (element = 0; element < elements; element++) {

        thislen = __int32_t_encode_array(buf, offset + pos, maxlen - pos, &(p[element].initial_runner_idx), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __int32_t_encode_array(buf, offset + pos, maxlen - pos, &(p[element].delay_every), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __double_encode_array(buf, offset + pos, maxlen - pos, &(p[element].to_goal_magnitude), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __int32_t_encode_array(buf, offset + pos, maxlen - pos, &(p[element].to_goal_power), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __double_encode_array(buf, offset + pos, maxlen - pos, &(p[element].avoid_obs_magnitude), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __int32_t_encode_array(buf, offset + pos, maxlen - pos, &(p[element].avoid_obs_power), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __double_encode_array(buf, offset + pos, maxlen - pos, &(p[element].max_vel), 1);
        if (thislen < 0) return thislen; else pos += thislen;

    }
    return pos;
}

int settings_t_encode(void *buf, int offset, int maxlen, const settings_t *p)
{
    int pos = 0, thislen;
    int64_t hash = __settings_t_get_hash();

    thislen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;

    thislen = __settings_t_encode_array(buf, offset + pos, maxlen - pos, p, 1);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

int __settings_t_encoded_array_size(const settings_t *p, int elements)
{
    int size = 0, element;
    for (element = 0; element < elements; element++) {

        size += __int32_t_encoded_array_size(&(p[element].initial_runner_idx), 1);

        size += __int32_t_encoded_array_size(&(p[element].delay_every), 1);

        size += __double_encoded_array_size(&(p[element].to_goal_magnitude), 1);

        size += __int32_t_encoded_array_size(&(p[element].to_goal_power), 1);

        size += __double_encoded_array_size(&(p[element].avoid_obs_magnitude), 1);

        size += __int32_t_encoded_array_size(&(p[element].avoid_obs_power), 1);

        size += __double_encoded_array_size(&(p[element].max_vel), 1);

    }
    return size;
}

int settings_t_encoded_size(const settings_t *p)
{
    return 8 + __settings_t_encoded_array_size(p, 1);
}

int __settings_t_decode_array(const void *buf, int offset, int maxlen, settings_t *p, int elements)
{
    int pos = 0, thislen, element;

    for (element = 0; element < elements; element++) {

        thislen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &(p[element].initial_runner_idx), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &(p[element].delay_every), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __double_decode_array(buf, offset + pos, maxlen - pos, &(p[element].to_goal_magnitude), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &(p[element].to_goal_power), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __double_decode_array(buf, offset + pos, maxlen - pos, &(p[element].avoid_obs_magnitude), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &(p[element].avoid_obs_power), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __double_decode_array(buf, offset + pos, maxlen - pos, &(p[element].max_vel), 1);
        if (thislen < 0) return thislen; else pos += thislen;

    }
    return pos;
}

int __settings_t_decode_array_cleanup(settings_t *p, int elements)
{
    int element;
    for (element = 0; element < elements; element++) {

        __int32_t_decode_array_cleanup(&(p[element].initial_runner_idx), 1);

        __int32_t_decode_array_cleanup(&(p[element].delay_every), 1);

        __double_decode_array_cleanup(&(p[element].to_goal_magnitude), 1);

        __int32_t_decode_array_cleanup(&(p[element].to_goal_power), 1);

        __double_decode_array_cleanup(&(p[element].avoid_obs_magnitude), 1);

        __int32_t_decode_array_cleanup(&(p[element].avoid_obs_power), 1);

        __double_decode_array_cleanup(&(p[element].max_vel), 1);

    }
    return 0;
}

int settings_t_decode(const void *buf, int offset, int maxlen, settings_t *p)
{
    int pos = 0, thislen;
    int64_t hash = __settings_t_get_hash();

    int64_t this_hash;
    thislen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &this_hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;
    if (this_hash != hash) return -1;

    thislen = __settings_t_decode_array(buf, offset + pos, maxlen - pos, p, 1);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

int settings_t_decode_cleanup(settings_t *p)
{
    return __settings_t_decode_array_cleanup(p, 1);
}

int __settings_t_clone_array(const settings_t *p, settings_t *q, int elements)
{
    int element;
    for (element = 0; element < elements; element++) {

        __int32_t_clone_array(&(p[element].initial_runner_idx), &(q[element].initial_runner_idx), 1);

        __int32_t_clone_array(&(p[element].delay_every), &(q[element].delay_every), 1);

        __double_clone_array(&(p[element].to_goal_magnitude), &(q[element].to_goal_magnitude), 1);

        __int32_t_clone_array(&(p[element].to_goal_power), &(q[element].to_goal_power), 1);

        __double_clone_array(&(p[element].avoid_obs_magnitude), &(q[element].avoid_obs_magnitude), 1);

        __int32_t_clone_array(&(p[element].avoid_obs_power), &(q[element].avoid_obs_power), 1);

        __double_clone_array(&(p[element].max_vel), &(q[element].max_vel), 1);

    }
    return 0;
}

settings_t *settings_t_copy(const settings_t *p)
{
    settings_t *q = (settings_t*) malloc(sizeof(settings_t));
    __settings_t_clone_array(p, q, 1);
    return q;
}

void settings_t_destroy(settings_t *p)
{
    __settings_t_decode_array_cleanup(p, 1);
    free(p);
}

int settings_t_publish(lcm_t *lc, const char *channel, const settings_t *p)
{
      int max_data_size = settings_t_encoded_size (p);
      uint8_t *buf = (uint8_t*) malloc (max_data_size);
      if (!buf) return -1;
      int data_size = settings_t_encode (buf, 0, max_data_size, p);
      if (data_size < 0) {
          free (buf);
          return data_size;
      }
      int status = lcm_publish (lc, channel, buf, data_size);
      free (buf);
      return status;
}

struct _settings_t_subscription_t {
    settings_t_handler_t user_handler;
    void *userdata;
    lcm_subscription_t *lc_h;
};
static
void settings_t_handler_stub (const lcm_recv_buf_t *rbuf,
                            const char *channel, void *userdata)
{
    int status;
    settings_t p;
    memset(&p, 0, sizeof(settings_t));
    status = settings_t_decode (rbuf->data, 0, rbuf->data_size, &p);
    if (status < 0) {
        fprintf (stderr, "error %d decoding settings_t!!!\n", status);
        return;
    }

    settings_t_subscription_t *h = (settings_t_subscription_t*) userdata;
    h->user_handler (rbuf, channel, &p, h->userdata);

    settings_t_decode_cleanup (&p);
}

settings_t_subscription_t* settings_t_subscribe (lcm_t *lcm,
                    const char *channel,
                    settings_t_handler_t f, void *userdata)
{
    settings_t_subscription_t *n = (settings_t_subscription_t*)
                       malloc(sizeof(settings_t_subscription_t));
    n->user_handler = f;
    n->userdata = userdata;
    n->lc_h = lcm_subscribe (lcm, channel,
                                 settings_t_handler_stub, n);
    if (n->lc_h == NULL) {
        fprintf (stderr,"couldn't reg settings_t LCM handler!\n");
        free (n);
        return NULL;
    }
    return n;
}

int settings_t_subscription_set_queue_capacity (settings_t_subscription_t* subs,
                              int num_messages)
{
    return lcm_subscription_set_queue_capacity (subs->lc_h, num_messages);
}

int settings_t_unsubscribe(lcm_t *lcm, settings_t_subscription_t* hid)
{
    int status = lcm_unsubscribe (lcm, hid->lc_h);
    if (0 != status) {
        fprintf(stderr,
           "couldn't unsubscribe settings_t_handler %p!\n", hid);
        return -1;
    }
    free (hid);
    return 0;
}

