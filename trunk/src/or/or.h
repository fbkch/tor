/* Copyright 2001,2002 Roger Dingledine, Matej Pfajfar. */
/* See LICENSE for licensing information */
/* $Id$ */

#ifndef __OR_H
#define __OR_H

#include "orconfig.h"

#define SEQUENTIAL_ACI

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif
#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif
#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif
#include "../common/torint.h"
#ifdef HAVE_SYS_POLL_H
#include <sys/poll.h>
#elif HAVE_POLL_H
#include <poll.h>
#else
#include "../common/fakepoll.h"
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h> /* Must be included before sys/stat.h for Ultrix */
#endif
#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif
#ifdef HAVE_SYS_FCNTL_H
#include <sys/fcntl.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#ifdef HAVE_ASSERT_H
#include <assert.h>
#endif
#ifdef HAVE_TIME_H
#include <time.h>
#endif
#ifdef HAVE_WINSOCK_H
#include <winsock.h>
#endif
#if _MSC_VER > 1300
#include <winsock2.h>
#include <ws2tcpip.h>
#elif defined(_MSC_VER)
#include <winsock.h>
#endif

#ifdef MS_WINDOWS
#include <io.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define snprintf _snprintf
#endif

#include "../common/crypto.h"
#include "../common/tortls.h"
#include "../common/log.h"
#include "../common/util.h"

#define RECOMMENDED_SOFTWARE_VERSIONS "0.0.2pre8,0.0.2pre9"

#define MAXCONNECTIONS 1000 /* upper bound on max connections.
                              can be lowered by config file */

#define MAX_BUF_SIZE (640*1024)
#define DEFAULT_BANDWIDTH_OP (1024 * 1000)

#define ACI_TYPE_LOWER 0
#define ACI_TYPE_HIGHER 1
#define ACI_TYPE_BOTH 2

#define _CONN_TYPE_MIN 3
#define CONN_TYPE_OR_LISTENER 3
#define CONN_TYPE_OR 4
#define CONN_TYPE_EXIT 5
#define CONN_TYPE_AP_LISTENER 6
#define CONN_TYPE_AP 7
#define CONN_TYPE_DIR_LISTENER 8
#define CONN_TYPE_DIR 9
#define CONN_TYPE_DNSWORKER 10
#define CONN_TYPE_CPUWORKER 11
#define _CONN_TYPE_MAX 11

#define LISTENER_STATE_READY 0

#define DNSWORKER_STATE_IDLE 0
#define DNSWORKER_STATE_BUSY 1

#define _CPUWORKER_STATE_MIN 0
#define CPUWORKER_STATE_IDLE 0
#define CPUWORKER_STATE_BUSY_ONION 1
#define CPUWORKER_STATE_BUSY_HANDSHAKE 2
#define _CPUWORKER_STATE_MAX 2

#define CPUWORKER_TASK_ONION CPUWORKER_STATE_BUSY_ONION

#define _OR_CONN_STATE_MIN 0
#define OR_CONN_STATE_CONNECTING 0 /* waiting for connect() to finish */
#define OR_CONN_STATE_HANDSHAKING 1 /* SSL is handshaking, not done yet */
#define OR_CONN_STATE_OPEN 2 /* ready to send/receive cells. */
#define _OR_CONN_STATE_MAX 2

#define _EXIT_CONN_STATE_MIN 0
#define EXIT_CONN_STATE_RESOLVING 0 /* waiting for response from dns farm */
#define EXIT_CONN_STATE_CONNECTING 1 /* waiting for connect() to finish */
#define EXIT_CONN_STATE_OPEN 2
#define _EXIT_CONN_STATE_MAX 2
#if 0
#define EXIT_CONN_STATE_CLOSE 3 /* flushing the buffer, then will close */
#define EXIT_CONN_STATE_CLOSE_WAIT 4 /* have sent a destroy, awaiting a confirmation */
#endif

#define _AP_CONN_STATE_MIN 3
#define AP_CONN_STATE_SOCKS_WAIT 3
#define AP_CONN_STATE_OR_WAIT 4
#define AP_CONN_STATE_OPEN 5
#define _AP_CONN_STATE_MAX 5

#define _DIR_CONN_STATE_MIN 0
#define DIR_CONN_STATE_CONNECTING_FETCH 0
#define DIR_CONN_STATE_CONNECTING_UPLOAD 1
#define DIR_CONN_STATE_CLIENT_SENDING_FETCH 2
#define DIR_CONN_STATE_CLIENT_SENDING_UPLOAD 3
#define DIR_CONN_STATE_CLIENT_READING_FETCH 4
#define DIR_CONN_STATE_CLIENT_READING_UPLOAD 5
#define DIR_CONN_STATE_SERVER_COMMAND_WAIT 6
#define DIR_CONN_STATE_SERVER_WRITING 7
#define _DIR_CONN_STATE_MAX 7

#define CIRCUIT_STATE_BUILDING 0 /* I'm the OP, still haven't done all my handshakes */
#define CIRCUIT_STATE_ONIONSKIN_PENDING 1 /* waiting to process the onionskin */
#define CIRCUIT_STATE_OR_WAIT 2 /* I'm the OP, my firsthop is still connecting */
#define CIRCUIT_STATE_OPEN 3 /* onionskin(s) processed, ready to send/receive cells */

#define RELAY_COMMAND_BEGIN 1
#define RELAY_COMMAND_DATA 2
#define RELAY_COMMAND_END 3
#define RELAY_COMMAND_CONNECTED 4
#define RELAY_COMMAND_SENDME 5
#define RELAY_COMMAND_EXTEND 6
#define RELAY_COMMAND_EXTENDED 7
#define RELAY_COMMAND_TRUNCATE 8
#define RELAY_COMMAND_TRUNCATED 9

#define RELAY_HEADER_SIZE 8

/* default cipher function */
#define DEFAULT_CIPHER CRYPTO_CIPHER_AES_CTR
/* Used to en/decrypt onion skins */
#define ONION_CIPHER      DEFAULT_CIPHER
/* Used to en/decrypt RELAY cells */
#define CIRCUIT_CIPHER    DEFAULT_CIPHER

#define CELL_DIRECTION_IN 1
#define CELL_DIRECTION_OUT 2
#define EDGE_EXIT CONN_TYPE_EXIT
#define EDGE_AP CONN_TYPE_AP
#define CELL_DIRECTION(x) ((x) == EDGE_EXIT ? CELL_DIRECTION_IN : CELL_DIRECTION_OUT)

#define CIRCWINDOW_START 1000
#define CIRCWINDOW_INCREMENT 100

#define STREAMWINDOW_START 500
#define STREAMWINDOW_INCREMENT 50

/* cell commands */
#define CELL_PADDING 0
#define CELL_CREATE 1
#define CELL_CREATED 2
#define CELL_RELAY 3
#define CELL_DESTROY 4

#define CELL_PAYLOAD_SIZE 248
#define CELL_NETWORK_SIZE 256

/* legal characters in a filename */
#define CONFIG_LEGAL_FILENAME_CHARACTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-_/"

/* structure of a socks client operation */
typedef struct {
   unsigned char version;     /* socks version number */
   unsigned char command;     /* command code */
   uint16_t destport; /* destination port, host order */
   uint32_t destip;   /* destination address, host order */
   /* userid follows, terminated by a \0 */
   /* dest host follows, terminated by a \0 */
} socks4_t;

#define SOCKS4_NETWORK_LEN 8

typedef uint16_t aci_t;

/* cell definition */
typedef struct { 
  aci_t aci; /* Anonymous Connection Identifier */
  unsigned char command;
  unsigned char length; /* of payload if relay cell */
  uint32_t seq; /* sequence number */

  unsigned char payload[CELL_PAYLOAD_SIZE];
} cell_t;
#define CELL_RELAY_COMMAND(c)         (*(uint8_t*)((c).payload))
#define SET_CELL_RELAY_COMMAND(c,cmd) (*(uint8_t*)((c).payload) = (cmd))
#define STREAM_ID_SIZE 7
#define SET_CELL_STREAM_ID(c,id)      memcpy((c).payload+1,(id),STREAM_ID_SIZE)

#define ZERO_STREAM "\0\0\0\0\0\0\0\0"

typedef struct buf_t buf_t;

struct connection_t { 

  uint8_t type;
  uint8_t state;
  uint8_t wants_to_read; /* should we start reading again once
                          * the bandwidth throttler allows it?
                          */
  uint8_t wants_to_write; /* should we start writing again once
                           * the bandwidth throttler allows reads?
                           */
  int s; /* our socket */
  int poll_index; /* index of this conn into the poll_array */
  int marked_for_close; /* should we close this conn on the next
                         * iteration of the main loop?
                         */

  buf_t *inbuf;
  int inbuf_reached_eof; /* did read() return 0 on this conn? */
  long timestamp_lastread; /* when was the last time poll() said we could read? */

  buf_t *outbuf;
  int outbuf_flushlen; /* how much data should we try to flush from the outbuf? */
  long timestamp_lastwritten; /* when was the last time poll() said we could write? */

  long timestamp_created; /* when was this connection_t created? */

  uint32_t bandwidth; /* connection bandwidth. Set to -1 for non-OR conns. */
  int receiver_bucket; /* when this hits 0, stop receiving. Every second we
                        * add 'bandwidth' to this, capping it at 10*bandwidth.
			* Set to -1 for non-OR conns.
                        */

  uint32_t addr; /* these two uniquely identify a router. Both in host order. */
  uint16_t port; /* if non-zero, they identify the guy on the other end
                  * of the connection. */
  char *address; /* FQDN (or IP) of the guy on the other end.
                  * strdup into this, because free_connection frees it
                  */
  crypto_pk_env_t *onion_pkey; /* public RSA key for the other side's onions */
  crypto_pk_env_t *link_pkey; /* public RSA key for the other side's TLS */
  crypto_pk_env_t *identity_pkey; /* public RSA key for the other side's signing */

/* Used only by OR connections: */
  tor_tls *tls;
#ifdef SEQUENTIAL_ACI
  uint16_t next_aci; /* Which ACI do we try to use next on this connection? 
                      * This is always in the range 0..1<<15-1.*/
#endif

/* Used only by edge connections: */
  char stream_id[STREAM_ID_SIZE];
  struct connection_t *next_stream; /* points to the next stream at this edge, if any */
  struct crypt_path_t *cpath_layer; /* a pointer to which node in the circ this conn exits at */
  int package_window; /* how many more relay cells can i send into the circuit? */
  int deliver_window; /* how many more relay cells can end at me? */

  int done_sending; /* for half-open connections; not used currently */
  int done_receiving;
};

typedef struct connection_t connection_t;

#define EXIT_POLICY_ACCEPT 1
#define EXIT_POLICY_REJECT 2

struct exit_policy_t {
  char policy_type;
  char *string;
  char *address;
  char *port;

  struct exit_policy_t *next;
};

/* config stuff we know about the other ORs in the network */
typedef struct {
  char *address;
  char *nickname;
 
  uint32_t addr; /* all host order */
  uint16_t or_port;
  uint16_t ap_port;
  uint16_t dir_port;
 
  crypto_pk_env_t *onion_pkey; /* public RSA key for onions */
  crypto_pk_env_t *link_pkey;  /* public RSA key for TLS */
  crypto_pk_env_t *identity_pkey;  /* public RSA key for signing */
 
  /* link info */
  uint32_t bandwidth;
  struct exit_policy_t *exit_policy;
} routerinfo_t;

#define MAX_ROUTERS_IN_DIR 1024
typedef struct {
  routerinfo_t **routers;
  int n_routers;
  char *software_versions;
} directory_t;

struct crypt_path_t { 

  /* crypto environments */
  crypto_cipher_env_t *f_crypto;
  crypto_cipher_env_t *b_crypto;

  crypto_dh_env_t *handshake_state;

  uint32_t addr;
  uint16_t port;

  uint8_t state;
#define CPATH_STATE_CLOSED 0
#define CPATH_STATE_AWAITING_KEYS 1
#define CPATH_STATE_OPEN 2
  struct crypt_path_t *next;
  struct crypt_path_t *prev; /* doubly linked list */

  int package_window;
  int deliver_window;
};

#define DH_KEY_LEN CRYPTO_DH_SIZE
#define DH_ONIONSKIN_LEN DH_KEY_LEN+16

typedef struct crypt_path_t crypt_path_t;

/* struct for a path (circuit) through the network */
struct circuit_t {
  uint32_t n_addr;
  uint16_t n_port;
  connection_t *p_conn;
  connection_t *n_conn; /* for the OR conn, if there is one */
  connection_t *p_streams;
  connection_t *n_streams;
  int package_window;
  int deliver_window;

  aci_t p_aci; /* circuit identifiers */
  aci_t n_aci;

  crypto_cipher_env_t *p_crypto; /* used only for intermediate hops */
  crypto_cipher_env_t *n_crypto;

  crypt_path_t *cpath;

  char onionskin[DH_ONIONSKIN_LEN]; /* for storage while onionskin pending */
  long timestamp_created;
  uint8_t dirty; /* whether this circuit has been used yet */

  uint8_t state;

  void *next;
};

typedef struct circuit_t circuit_t;

typedef struct {
   char *LogLevel;
   char *DataDirectory;
   char *RouterFile;
   char *Nickname;
   double CoinWeight;
   int Daemon;
   int ORPort;
   int APPort;
   int DirPort;
   int MaxConn;
   int OnionRouter;
   int TrafficShaping;
   int LinkPadding;
   int IgnoreVersion;
   int DirRebuildPeriod;
   int DirFetchPeriod;
   int KeepalivePeriod;
   int MaxOnionsPending;
   int NewCircuitPeriod;
   int TotalBandwidth;
   int NumCpus;
   int Role;
   int loglevel;
} or_options_t;

    /* all the function prototypes go here */

/********************************* buffers.c ***************************/

buf_t *buf_new();
buf_t *buf_new_with_capacity(size_t size);
void buf_free(buf_t *buf);

size_t buf_datalen(const buf_t *buf);
size_t buf_capacity(const buf_t *buf);
const char *_buf_peek_raw_buffer(const buf_t *buf);

int read_to_buf(int s, int at_most, buf_t *buf, int *reached_eof);
int read_to_buf_tls(tor_tls *tls, int at_most, buf_t *buf);

int flush_buf(int s, buf_t *buf, int *buf_flushlen);
int flush_buf_tls(tor_tls *tls, buf_t *buf, int *buf_flushlen);

int write_to_buf(char *string, int string_len, buf_t *buf);
int fetch_from_buf(char *string, int string_len, buf_t *buf);
int fetch_from_buf_http(buf_t *buf,
                        char *headers_out, int max_headerlen,
                        char *body_out, int max_bodylen);
int fetch_from_buf_socks(buf_t *buf,
                         char *addr_out, int max_addrlen,
                         uint16_t *port_out);
int find_on_inbuf(char *string, int string_len, buf_t *buf);

/********************************* circuit.c ***************************/

void circuit_add(circuit_t *circ);
void circuit_remove(circuit_t *circ);
circuit_t *circuit_new(aci_t p_aci, connection_t *p_conn);
void circuit_free(circuit_t *circ);

circuit_t *circuit_enumerate_by_naddr_nport(circuit_t *start, uint32_t naddr, uint16_t nport);
circuit_t *circuit_get_by_aci_conn(aci_t aci, connection_t *conn);
circuit_t *circuit_get_by_conn(connection_t *conn);
circuit_t *circuit_get_newest_open(void);

int circuit_deliver_relay_cell(cell_t *cell, circuit_t *circ,
                               int cell_direction, crypt_path_t *layer_hint);
int relay_crypt(circuit_t *circ, char *in, int inlen, char cell_direction,
                crypt_path_t **layer_hint, char *recognized, connection_t **conn);
int relay_check_recognized(circuit_t *circ, int cell_direction, char *stream, connection_t **conn);

void circuit_resume_edge_reading(circuit_t *circ, int edge_type, crypt_path_t *layer_hint);
int circuit_consider_stop_edge_reading(circuit_t *circ, int edge_type, crypt_path_t *layer_hint);
int circuit_consider_sending_sendme(circuit_t *circ, int edge_type, crypt_path_t *layer_hint);

void circuit_close(circuit_t *circ);
void circuit_about_to_close_connection(connection_t *conn);

void circuit_dump_by_conn(connection_t *conn);

void circuit_expire_unused_circuits(void);
void circuit_launch_new(int failure_status);
int circuit_establish_circuit(void);
void circuit_n_conn_open(connection_t *or_conn);
int circuit_send_next_onion_skin(circuit_t *circ);
int circuit_extend(cell_t *cell, circuit_t *circ);
int circuit_finish_handshake(circuit_t *circ, char *reply);
int circuit_truncated(circuit_t *circ, crypt_path_t *layer);

void assert_cpath_ok(const crypt_path_t *c);
void assert_cpath_layer_ok(const crypt_path_t *c);
void assert_circuit_ok(const circuit_t *c);

/********************************* command.c ***************************/

void command_process_cell(cell_t *cell, connection_t *conn);

/********************************* config.c ***************************/

int getconfig(int argc, char **argv, or_options_t *options);

/********************************* connection.c ***************************/

connection_t *connection_new(int type);
void connection_free(connection_t *conn);

int connection_create_listener(struct sockaddr_in *bindaddr, int type);
int connection_handle_listener_read(connection_t *conn, int new_type);

int connection_tls_start_handshake(connection_t *conn, int receiving);

int connection_connect(connection_t *conn, char *address, uint32_t addr, uint16_t port);
int retry_all_connections(uint16_t or_listenport, uint16_t ap_listenport, uint16_t dir_listenport);

int connection_handle_read(connection_t *conn);
int connection_read_to_buf(connection_t *conn);

int connection_fetch_from_buf(char *string, int len, connection_t *conn);
int connection_find_on_inbuf(char *string, int len, connection_t *conn);

int connection_wants_to_flush(connection_t *conn);
int connection_outbuf_too_full(connection_t *conn);
int connection_flush_buf(connection_t *conn);
int connection_handle_write(connection_t *conn);
int connection_write_to_buf(char *string, int len, connection_t *conn);

int connection_receiver_bucket_should_increase(connection_t *conn);

#define connection_speaks_cells(conn) ((conn)->type == CONN_TYPE_OR)
int connection_is_listener(connection_t *conn);
int connection_state_is_open(connection_t *conn);

int connection_send_destroy(aci_t aci, connection_t *conn);

int connection_process_inbuf(connection_t *conn);
int connection_finished_flushing(connection_t *conn);

void assert_connection_ok(connection_t *conn, time_t now);

/********************************* connection_edge.c ***************************/

int connection_edge_process_inbuf(connection_t *conn);
int connection_edge_send_command(connection_t *fromconn, circuit_t *circ, int relay_command);
int connection_edge_process_relay_cell(cell_t *cell, circuit_t *circ, connection_t *conn,
                                       int edge_type, crypt_path_t *layer_hint);
int connection_edge_finished_flushing(connection_t *conn);

int connection_package_raw_inbuf(connection_t *conn);
int connection_consider_sending_sendme(connection_t *conn, int edge_type);

int connection_exit_connect(connection_t *conn);

/********************************* connection_or.c ***************************/

int connection_or_process_inbuf(connection_t *conn);
int connection_or_finished_flushing(connection_t *conn);

connection_t *connection_or_connect(routerinfo_t *router);

int connection_write_cell_to_buf(const cell_t *cellp, connection_t *conn);
int connection_process_cell_from_inbuf(connection_t *conn);

/********************************* cpuworker.c *****************************/

void cpu_init(void);
int connection_cpu_finished_flushing(connection_t *conn);
int connection_cpu_process_inbuf(connection_t *conn);
int cpuworker_main(void *data);
int assign_to_cpuworker(connection_t *cpuworker, unsigned char question_type,
                        void *task);

/********************************* directory.c ***************************/

void directory_initiate_command(routerinfo_t *router, int command);
void directory_set_dirty(void);
int connection_dir_process_inbuf(connection_t *conn);
int connection_dir_finished_flushing(connection_t *conn);

/********************************* dns.c ***************************/

void dns_init(void);
int connection_dns_finished_flushing(connection_t *conn);
int connection_dns_process_inbuf(connection_t *conn);
void dns_cancel_pending_resolve(char *question, connection_t *onlyconn);
int dns_resolve(connection_t *exitconn);

/********************************* main.c ***************************/

void set_onion_key(crypto_pk_env_t *k);
crypto_pk_env_t *get_onion_key(void);
void set_identity_key(crypto_pk_env_t *k);
crypto_pk_env_t *get_identity_key(void);
int connection_add(connection_t *conn);
int connection_remove(connection_t *conn);
void connection_set_poll_socket(connection_t *conn);

connection_t *connection_twin_get_by_addr_port(uint32_t addr, uint16_t port);
connection_t *connection_exact_get_by_addr_port(uint32_t addr, uint16_t port);

connection_t *connection_get_by_type(int type);
connection_t *connection_get_by_type_state(int type, int state);
connection_t *connection_get_by_type_state_lastwritten(int type, int state);

void connection_watch_events(connection_t *conn, short events);
int connection_is_reading(connection_t *conn);
void connection_stop_reading(connection_t *conn);
void connection_start_reading(connection_t *conn);
void connection_stop_writing(connection_t *conn);
void connection_start_writing(connection_t *conn);

int dump_signed_directory_to_string(char *s, int maxlen, 
                                    crypto_pk_env_t *private_key);
/* Exported for debugging */
int dump_signed_directory_to_string_impl(char *s, int maxlen, 
                                         directory_t *dir, 
                                         crypto_pk_env_t *private_key); 
const char *router_get_my_descriptor(void);

int main(int argc, char *argv[]);

/********************************* onion.c ***************************/

int decide_aci_type(uint32_t local_addr, uint16_t local_port,
                    uint32_t remote_addr, uint16_t remote_port);

int onion_pending_add(circuit_t *circ);
circuit_t *onion_next_task(void);
void onion_pending_remove(circuit_t *circ);

int onionskin_answer(circuit_t *circ, unsigned char *payload, unsigned char *keys);

crypt_path_t *onion_generate_cpath(routerinfo_t **firsthop);

int onion_skin_create(crypto_pk_env_t *router_key,
                      crypto_dh_env_t **handshake_state_out,
                      char *onion_skin_out); /* Must be DH_ONIONSKIN_LEN bytes long */

int onion_skin_server_handshake(char *onion_skin, /* DH_ONIONSKIN_LEN bytes long */
                                crypto_pk_env_t *private_key,
                                char *handshake_reply_out, /* DH_KEY_LEN bytes long */
                                char *key_out,
                                int key_out_len);

int onion_skin_client_handshake(crypto_dh_env_t *handshake_state,
                             char *handshake_reply,/* Must be DH_KEY_LEN bytes long*/
                             char *key_out,
                             int key_out_len);

/********************************* routers.c ***************************/

int learn_my_address(struct sockaddr_in *me);
void router_retry_connections(void);
routerinfo_t *router_pick_directory_server(void);
routerinfo_t *router_get_by_addr_port(uint32_t addr, uint16_t port);
routerinfo_t *router_get_by_link_pk(crypto_pk_env_t *pk);
#if 0
routerinfo_t *router_get_by_identity_pk(crypto_pk_env_t *pk);
#endif
void router_get_directory(directory_t **pdirectory);
int router_is_me(uint32_t addr, uint16_t port);
void router_forget_router(uint32_t addr, uint16_t port);
int router_get_list_from_file(char *routerfile);
int router_get_router_hash(char *s, char *digest);

/* Reads a list of known routers, unsigned. */
int router_get_list_from_string(char *s);
/* Exported for debugging */
int router_get_list_from_string_impl(char **s, directory_t **dest);
/* Reads a signed directory. */
int router_get_dir_from_string(char *s, crypto_pk_env_t *pkey);
/* Exported or debugging */
int router_get_dir_from_string_impl(char *s, directory_t **dest,
                                    crypto_pk_env_t *pkey);
routerinfo_t *router_get_entry_from_string(char **s);
int router_compare_to_exit_policy(connection_t *conn);
void routerinfo_free(routerinfo_t *router);

#endif

/*
  Local Variables:
  mode:c
  indent-tabs-mode:nil
  c-basic-offset:2
  End:
*/
