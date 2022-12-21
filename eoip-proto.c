#include "eoip-proto.h"
#include "eoip.h"

///
/// populate_eoiphdr: populate an IPv4 eoip_hdr with given tid.
///
void populate_eoiphdr(int tid, void *dst) {
  union eoip_hdr *eoip_hdr = (union eoip_hdr *) dst;
  eoip_hdr->eoip.tid = tid;
  memcpy(&eoip_hdr->header, EOIP_MAGIC, 4);
}

///
/// populate_eoip6hdr: populate an IPv6 eoip_hdr with given tid.
///
void populate_eoip6hdr(int tid, void *dst) {
  union eoip_hdr *eoip6_hdr = (union eoip_hdr *) dst;
  eoip6_hdr->header_v = htons(EIPHEAD(tid));
  eoip6_hdr->eoip6.head_p1 = eoip6_hdr->eoip6.head_p1;
}

///
/// eoip_header: populate an eoip_hdr with given AF and tid.
///
void eoip_header(int af, int tid, void *dst) {
  if (af == AF_INET) populate_eoiphdr(tid, dst);
  if (af == AF_INET6) populate_eoip6hdr(tid, dst);
}
