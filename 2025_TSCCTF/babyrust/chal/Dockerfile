FROM ubuntu:24.04
LABEL maintainer="pwn2ooown"
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -qy xinetd

RUN useradd -m babyrust
RUN chown -R root:root /home/babyrust
RUN chmod -R 755 /home/babyrust

CMD ["/usr/sbin/xinetd", "-dontfork"]