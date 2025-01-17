FROM ubuntu:20.04@sha256:8e5c4f0285ecbb4ead070431d29b576a530d3166df73ec44affc1cd27555141b
LABEL maintainer="pwn2ooown"
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -qy xinetd

RUN useradd -m globalstack
RUN chown -R root:root /home/globalstack
RUN chmod -R 755 /home/globalstack

CMD ["/usr/sbin/xinetd", "-dontfork"]