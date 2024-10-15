FROM ubuntu:24.04
LABEL maintainer="pwn2ooown"
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -qy xinetd

RUN useradd -m yaupc-revenge
RUN chown -R root:root /home/yaupc-revenge
RUN chmod -R 755 /home/yaupc-revenge

CMD ["/usr/sbin/xinetd", "-dontfork"]