cmake_minimum_required(VERSION 3.4.1)

project(re)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -ggdb")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_OPENSSL")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_OPENSSL_DTLS")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_OPENSSL_SRTP")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLINUX")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DOS='\"linux\"'")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DARCH='\"arm\"'")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_TLS")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_DTLS")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_DTLS_SRTP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_ZLIB")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_PTHREAD")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_STRERROR_R")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_GETOPT")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_INTTYPES_H")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_NET_ROUTE_H")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_STDBOOL_H")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_INET6")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_SYSLOG")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_FORK")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_INET_NTOP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_PWD_H")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_POLL")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_INET_PTON")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_SELECT")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_SELECT_H")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_SETRLIMIT")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_SIGNAL")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_SYS_TIME_H")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_EPOLL")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_UNAME")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_UNISTD_H")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_STRINGS_H")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_GAI_STRERROR")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVERSION='\"0.5.5\"'")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_ROUTE_LIST")

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_EPOLL")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_UNAME")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_UNISTD_H")

set(source_list
    ../../src/main/cpp/re/src/sip/addr.c
    ../../src/main/cpp/re/src/sip/auth.c
    ../../src/main/cpp/re/src/sip/contact.c
    ../../src/main/cpp/re/src/sip/cseq.c
    ../../src/main/cpp/re/src/sip/ctrans.c
    ../../src/main/cpp/re/src/sip/dialog.c
    ../../src/main/cpp/re/src/sip/keepalive.c
    ../../src/main/cpp/re/src/sip/keepalive_udp.c
    ../../src/main/cpp/re/src/sip/msg.c
    ../../src/main/cpp/re/src/sip/reply.c
    ../../src/main/cpp/re/src/sip/request.c
    ../../src/main/cpp/re/src/sip/sip.c
    ../../src/main/cpp/re/src/sip/strans.c
    ../../src/main/cpp/re/src/sip/transp.c
    ../../src/main/cpp/re/src/sip/via.c
    ../../src/main/cpp/re/src/sipevent/listen.c
    ../../src/main/cpp/re/src/sipevent/msg.c
    ../../src/main/cpp/re/src/sipevent/notify.c
    ../../src/main/cpp/re/src/sipevent/subscribe.c
    ../../src/main/cpp/re/src/sipreg/reg.c
    ../../src/main/cpp/re/src/sipsess/sess.c
    ../../src/main/cpp/re/src/sipsess/accept.c
    ../../src/main/cpp/re/src/sipsess/ack.c
    ../../src/main/cpp/re/src/sipsess/close.c
    ../../src/main/cpp/re/src/sipsess/connect.c
    ../../src/main/cpp/re/src/sipsess/info.c
    ../../src/main/cpp/re/src/sipsess/listen.c
    ../../src/main/cpp/re/src/sipsess/modify.c
    ../../src/main/cpp/re/src/sipsess/reply.c
    ../../src/main/cpp/re/src/sipsess/request.c
    ../../src/main/cpp/re/src/uri/uri.c
    ../../src/main/cpp/re/src/uri/ucmp.c
    ../../src/main/cpp/re/src/uri/uric.c
    ../../src/main/cpp/re/src/http/auth.c
    ../../src/main/cpp/re/src/http/client.c
    ../../src/main/cpp/re/src/http/msg.c
    ../../src/main/cpp/re/src/http/server.c
    ../../src/main/cpp/re/src/httpauth/digest.c
    ../../src/main/cpp/re/src/msg/ctype.c
    ../../src/main/cpp/re/src/msg/param.c
    ../../src/main/cpp/re/src/websock/websock.c
    ../../src/main/cpp/re/src/stun/addr.c
    ../../src/main/cpp/re/src/stun/attr.c
    ../../src/main/cpp/re/src/stun/ctrans.c
    ../../src/main/cpp/re/src/stun/dnsdisc.c
    ../../src/main/cpp/re/src/stun/hdr.c
    ../../src/main/cpp/re/src/stun/ind.c
    ../../src/main/cpp/re/src/stun/keepalive.c
    ../../src/main/cpp/re/src/stun/msg.c
    ../../src/main/cpp/re/src/stun/rep.c
    ../../src/main/cpp/re/src/stun/req.c
    ../../src/main/cpp/re/src/stun/stun.c
    ../../src/main/cpp/re/src/stun/stunstr.c
    ../../src/main/cpp/re/src/turn/chan.c
    ../../src/main/cpp/re/src/turn/perm.c
    ../../src/main/cpp/re/src/turn/turnc.c
    ../../src/main/cpp/re/src/ice/cand.c
    ../../src/main/cpp/re/src/ice/candpair.c
    ../../src/main/cpp/re/src/ice/chklist.c
    ../../src/main/cpp/re/src/ice/comp.c
    ../../src/main/cpp/re/src/ice/connchk.c
    ../../src/main/cpp/re/src/ice/icem.c
    ../../src/main/cpp/re/src/ice/icesdp.c
    ../../src/main/cpp/re/src/ice/icestr.c
    ../../src/main/cpp/re/src/ice/stunsrv.c
    ../../src/main/cpp/re/src/ice/util.c
    ../../src/main/cpp/re/src/natbd/filtering.c
    ../../src/main/cpp/re/src/natbd/genalg.c
    ../../src/main/cpp/re/src/natbd/hairpinning.c
    ../../src/main/cpp/re/src/natbd/lifetime.c
    ../../src/main/cpp/re/src/natbd/mapping.c
    ../../src/main/cpp/re/src/natbd/natstr.c
    ../../src/main/cpp/re/src/rtp/fb.c
    ../../src/main/cpp/re/src/rtp/member.c
    ../../src/main/cpp/re/src/rtp/ntp.c
    ../../src/main/cpp/re/src/rtp/pkt.c
    ../../src/main/cpp/re/src/rtp/rr.c
    ../../src/main/cpp/re/src/rtp/rtcp.c
    ../../src/main/cpp/re/src/rtp/rtp.c
    ../../src/main/cpp/re/src/rtp/sdes.c
    ../../src/main/cpp/re/src/rtp/sess.c
    ../../src/main/cpp/re/src/rtp/source.c
    ../../src/main/cpp/re/src/sdp/attr.c
    ../../src/main/cpp/re/src/sdp/format.c
    ../../src/main/cpp/re/src/sdp/media.c
    ../../src/main/cpp/re/src/sdp/msg.c
    ../../src/main/cpp/re/src/sdp/session.c
    ../../src/main/cpp/re/src/sdp/str.c
    ../../src/main/cpp/re/src/sdp/util.c
    ../../src/main/cpp/re/src/jbuf/jbuf.c
    ../../src/main/cpp/re/src/telev/telev.c
    ../../src/main/cpp/re/src/dns/client.c
    ../../src/main/cpp/re/src/dns/cstr.c
    ../../src/main/cpp/re/src/dns/dname.c
    ../../src/main/cpp/re/src/dns/hdr.c
    ../../src/main/cpp/re/src/dns/ns.c
    ../../src/main/cpp/re/src/dns/rr.c
    ../../src/main/cpp/re/src/dns/rrlist.c
    ../../src/main/cpp/re/src/md5/wrap.c
    ../../src/main/cpp/re/src/hmac/hmac_sha1.c
    ../../src/main/cpp/re/src/hmac/openssl/hmac.c
    ../../src/main/cpp/re/src/base64/b64.c
    ../../src/main/cpp/re/src/udp/udp.c
    ../../src/main/cpp/re/src/udp/mcast.c
    ../../src/main/cpp/re/src/sa/ntop.c
    ../../src/main/cpp/re/src/sa/printaddr.c
    ../../src/main/cpp/re/src/sa/pton.c
    ../../src/main/cpp/re/src/sa/sa.c
    ../../src/main/cpp/re/src/net/if.c
    ../../src/main/cpp/re/src/net/net.c
    ../../src/main/cpp/re/src/net/netstr.c
    ../../src/main/cpp/re/src/net/rt.c
    ../../src/main/cpp/re/src/net/sock.c
    ../../src/main/cpp/re/src/net/sockopt.c
    ../../src/main/cpp/re/src/net/posix/pif.c
    ../../src/main/cpp/re/src/net/linux/rt.c
    ../../src/main/cpp/re/src/tcp/tcp.c
    ../../src/main/cpp/re/src/tcp/tcp_high.c
    ../../src/main/cpp/re/src/tls/openssl/tls.c
    ../../src/main/cpp/re/src/tls/openssl/tls_tcp.c
    ../../src/main/cpp/re/src/tls/openssl/tls_udp.c
    ../../src/main/cpp/re/src/list/list.c
    ../../src/main/cpp/re/src/mbuf/mbuf.c
    ../../src/main/cpp/re/src/hash/hash.c
    ../../src/main/cpp/re/src/hash/func.c
    ../../src/main/cpp/re/src/fmt/ch.c
    ../../src/main/cpp/re/src/fmt/hexdump.c
    ../../src/main/cpp/re/src/fmt/pl.c
    ../../src/main/cpp/re/src/fmt/print.c
    ../../src/main/cpp/re/src/fmt/prm.c
    ../../src/main/cpp/re/src/fmt/regex.c
    ../../src/main/cpp/re/src/fmt/str.c
    ../../src/main/cpp/re/src/fmt/str_error.c
    ../../src/main/cpp/re/src/fmt/time.c
    ../../src/main/cpp/re/src/fmt/unicode.c
    ../../src/main/cpp/re/src/tmr/tmr.c
    ../../src/main/cpp/re/src/main/init.c
    ../../src/main/cpp/re/src/main/main.c
    ../../src/main/cpp/re/src/main/method.c
    ../../src/main/cpp/re/src/main/epoll.c
    ../../src/main/cpp/re/src/main/openssl.c
    ../../src/main/cpp/re/src/mem/mem.c
    ../../src/main/cpp/re/src/dbg/dbg.c
    ../../src/main/cpp/re/src/sys/daemon.c
    ../../src/main/cpp/re/src/sys/endian.c
    ../../src/main/cpp/re/src/sys/fs.c
    ../../src/main/cpp/re/src/sys/rand.c
    ../../src/main/cpp/re/src/sys/sleep.c
    ../../src/main/cpp/re/src/sys/sys.c
    ../../src/main/cpp/re/src/lock/rwlock.c
    ../../src/main/cpp/re/src/mqueue/mqueue.c
    ../../src/main/cpp/re/src/mod/mod.c
    ../../src/main/cpp/re/src/mod/dl.c
    ../../src/main/cpp/re/src/conf/conf.c
    ../../src/main/cpp/re/src/bfcp/attr.c
    ../../src/main/cpp/re/src/bfcp/conn.c
    ../../src/main/cpp/re/src/bfcp/msg.c
    ../../src/main/cpp/re/src/bfcp/reply.c
    ../../src/main/cpp/re/src/bfcp/request.c
    ../../src/main/cpp/re/src/aes/openssl/aes.c
    ../../src/main/cpp/re/src/srtp/misc.c
    ../../src/main/cpp/re/src/srtp/replay.c
    ../../src/main/cpp/re/src/srtp/srtcp.c
    ../../src/main/cpp/re/src/srtp/srtp.c
    ../../src/main/cpp/re/src/srtp/stream.c
    ../../src/main/cpp/re/src/odict/entry.c
    ../../src/main/cpp/re/src/odict/odict.c
    ../../src/main/cpp/re/src/odict/type.c
    ../../src/main/cpp/re/src/json/decode.c
    ../../src/main/cpp/re/src/json/decode_odict.c
    ../../src/main/cpp/re/src/json/encode.c
)

include_directories(../../src/main/cpp/re/include)
include_directories(../../src/main/cpp/openssl/include)

add_library(re STATIC ${source_list})

target_link_libraries(re ssl crypto z)
