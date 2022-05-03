#pragma once

#include <stdint.h>

struct Request
{
    uint32_t view_size, ip;
    uint16_t port;
    uint32_t attack_id;
};
struct Response
{
    uint32_t virtualIp, realIp, view_size, malicious_view_size;
};