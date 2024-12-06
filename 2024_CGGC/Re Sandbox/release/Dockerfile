FROM ubuntu:24.04
LABEL maintainer="pwn2ooown"

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -qy python3 socat

COPY flag /flag
COPY readflag /readflag

RUN chown root:root /flag && chmod 400 /flag
RUN chown root:root /readflag && chmod 4755 /readflag

RUN useradd -m ctf

USER ctf

CMD socat tcp-listen:1337,reuseaddr,fork exec:/home/ctf/run.py,pty,echo=0,raw,iexten=0