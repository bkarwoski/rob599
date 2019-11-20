#include <lcm/lcm.h>
#include <time.h>
#include "l2g_t.h"

void on_l2g(const lcm_recv_buf_t *rbuf, const char *channel,
            const l2g_t *msg, void *userdata) {
                printf("%.2f %.2f %.2f\n", msg->l2g[0], msg->l2g[1], msg->l2g[2]);
}

int main(void) {
    // we could also pass in a string of settings for special LCM configuration
    // and that would be helpful to configure a multi-robot communication scenario
    lcm_t *lcm = lcm_create(NULL);
    l2g_t message = {0};
    message.utime = 0;
    message.l2g[0] = 1.0;
    message.l2g[1] = 2.0;
    message.l2g[2] = 3.0;
    l2g_t_publish(lcm, "L2G", &message);
    l2g_t_subscription_t *l2g_sub = l2g_t_subscribe(lcm, "L2G", on_l2g, &message);
    clock_t start = clock();
    while (((double)clock() - (double)start) / (double)CLOCKS_PER_SEC < 0.5) {
        lcm_handle_timeout(lcm, 100);
    }
    l2g_t_unsubscribe(lcm, l2g_sub);
    lcm_destroy(lcm);
    return 0;
}
