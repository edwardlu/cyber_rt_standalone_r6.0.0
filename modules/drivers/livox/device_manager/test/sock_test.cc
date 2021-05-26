#include "../sock_util.h"

int main()
{
        uint32_t ip;
        uint8_t *ptr;
        socket_util sk_util;
        sk_util.get_local_addr();
        ip = sk_util.get_ip_by_name("enp3s0f3");

        ptr = (uint8_t *)&ip;
        printf("%d\n",*ptr);
        printf("%d\n",*(ptr+1));
        printf("%d\n",*(ptr+2));
        printf("%d\n",*(ptr+3));

        ip = sk_util.get_ip_by_name("eno1");
        ptr = (uint8_t *)&ip;
        printf("%d\n",*ptr);
        printf("%d\n",*(ptr+1));
        printf("%d\n",*(ptr+2));
        printf("%d\n",*(ptr+3));

        return 0;
}

