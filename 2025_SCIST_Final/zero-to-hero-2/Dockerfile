FROM ubuntu@sha256:8feb4d8ca5354def3d8fce243717141ce31e2c428701f6682bd2fafe15388214 AS base

FROM pwn.red/jail

COPY --from=base / /srv
COPY chal /srv/app/run
COPY flag /srv/app/flag
RUN chmod 755 /srv/app/run
RUN chmod 444 /srv/app/flag
ENV JAIL_TIME=120 JAIL_MEM=20M JAIL_CPU=100
