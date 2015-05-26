/* -*- Mode: C; tab-width: 8; c-basic-offset: 2; indent-tabs-mode: nil; -*- */

#include "rrutil.h"

extern int capget(cap_user_header_t header, cap_user_data_t data);

int main(int argc, char* argv[]) {
  struct __user_cap_header_struct* hdr;
  struct __user_cap_data_struct* data;

  ALLOCATE_GUARD(hdr, 'a');
  hdr->version = 0;
  hdr->pid = 0;
  test_assert(0 == capget(hdr, NULL));
  test_assert(hdr->version > 0);
  VERIFY_GUARD(hdr);

  ALLOCATE_GUARD(hdr, 'a');
  hdr->version = _LINUX_CAPABILITY_VERSION_1;
  hdr->pid = 0;
  ALLOCATE_GUARD(data, 'b');
  test_assert(0 == capget(hdr, data));
  VERIFY_GUARD(hdr);
  VERIFY_GUARD(data);

  test_assert(0 == data[0].effective);
  test_assert(0 == data[0].permitted);
  test_assert(0 == data[0].inheritable);

  ALLOCATE_GUARD(hdr, 'c');
  hdr->version = _LINUX_CAPABILITY_VERSION_3;
  hdr->pid = 0;
  data = allocate_guard(sizeof(*data) * 2, 'd');
  test_assert(0 == capget(hdr, data));
  VERIFY_GUARD(hdr);
  verify_guard(sizeof(*data) * 2, data);

  test_assert(0 == data[0].effective);
  test_assert(0 == data[0].permitted);
  test_assert(0 == data[0].inheritable);

  test_assert(0 == data[1].effective);
  test_assert(0 == data[1].permitted);
  test_assert(0 == data[1].inheritable);

  atomic_puts("EXIT-SUCCESS");
  return 0;
}
