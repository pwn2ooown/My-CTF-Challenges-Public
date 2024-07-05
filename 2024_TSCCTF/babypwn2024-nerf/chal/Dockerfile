FROM ubuntu:22.04
MAINTAINER TWNWAKing

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -qy xinetd python3

RUN useradd -m chal
RUN chown -R root:root /home/chal
RUN chmod -R 755 /home/chal

CMD ["/usr/sbin/xinetd", "-dontfork"]
