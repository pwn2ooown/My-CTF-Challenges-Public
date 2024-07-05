FROM alpine

RUN apk add --no-cache python3 py3-pip socat

RUN pip3 install --upgrade pip && pip3 install --upgrade pycryptodome

RUN mkdir -p /chal
WORKDIR /chal

COPY ./src /chal

CMD ["socat", "TCP-LISTEN:8787,fork", "EXEC:'timeout 300 python3 server.py'"]