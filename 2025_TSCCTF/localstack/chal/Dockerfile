FROM ubuntu:24.04
LABEL maintainer="pwn2ooown"
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -qy xinetd

RUN useradd -m localstack
RUN chown -R root:root /home/localstack
RUN chmod -R 755 /home/localstack

CMD ["/usr/sbin/xinetd", "-dontfork"]