#pragma once

#if MG_ARCH == MG_ARCH_FREERTOS_TCP

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <FreeRTOS.h>
#include <FreeRTOS_IP.h>
#include <FreeRTOS_Sockets.h>
#include <task.h>

#define MG_INT64_FMT "%lld"
#define MG_DIRSEP '/'

#define IPPROTO_TCP FREERTOS_IPPROTO_TCP
#define IPPROTO_UDP FREERTOS_IPPROTO_UDP
#define AF_INET FREERTOS_AF_INET
#define SOCK_STREAM FREERTOS_SOCK_STREAM
#define SOCK_DGRAM FREERTOS_SOCK_DGRAM
#define SO_BROADCAST 0
#define SO_ERROR 0
#define SOL_SOCKET 0
#define SO_REUSEADDR 0
#define sockaddr_in freertos_sockaddr
#define sockaddr freertos_sockaddr
#define accept(a, b, c) FreeRTOS_accept((a), (b), (c))
#define connect(a, b, c) FreeRTOS_connect((a), (b), (c))
#define bind(a, b, c) FreeRTOS_bind((a), (b), (c))
#define listen(a, b) FreeRTOS_listen((a), (b))
#define socket(a, b, c) FreeRTOS_socket((a), (b), (c))
#define send(a, b, c, d) FreeRTOS_send((a), (b), (c), (d))
#define recv(a, b, c, d) FreeRTOS_recv((a), (b), (c), (d))
#define setsockopt(a, b, c, d, e) FreeRTOS_setsockopt((a), (b), (c), (d), (e))
#define sendto(a, b, c, d, e, f) FreeRTOS_sendto((a), (b), (c), (d), (e), (f))
#define recvfrom(a, b, c, d, e, f) \
  FreeRTOS_recvfrom((a), (b), (c), (d), (e), (f))
#define closesocket(x) FreeRTOS_closesocket(x)
#define gethostbyname(x) FreeRTOS_gethostbyname(x)

#ifdef MG_ENABLE_FF
#include <ff_stdio.h>

#undef FILE
#define FILE FF_FILE
#define stat(a, b) ff_stat((a), (b))
#define fopen(a, b) ff_fopen((a), (b))
#define fclose(a) ff_fclose(a)
#define fread(a, b, c, d) ff_fread((a), (b), (c), (d))
#define fwrite(a, b, c, d) ff_fwrite((a), (b), (c), (d))
#define vfprintf ff_vfprintf
#define fprintf ff_fprintf
#define remove(a) ff_remove(a)
#define rename(a, b) ff_rename((a), (b), 1)

static inline int ff_vfprintf(FF_FILE *fp, const char *fmt, va_list ap) {
  char *buf = NULL;
  int n = mg_vasprintf(&buf, 0, fmt, ap);
  if (buf != NULL) ff_fwrite(buf, 1, n, fp), free(buf);
  return n;
}
#endif  // MG_ENABLE_FF

#endif  // MG_ARCH == MG_ARCH_FREERTOS_TCP
