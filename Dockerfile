FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt update
RUN apt install -y python3.9 python3-pip
RUN apt install -y zlib1g-dev

COPY . /app

WORKDIR /app/HostMiddleware
RUN make clean
RUN pip install -r requirements.txt
RUN make -j4 dist


# WORKDIR /app

# RUN make dist
RUN cp BPMMiddleware /BPMMiddleware
RUN chmod +x /BPMMiddleware

WORKDIR /
RUN rm -rf /app
RUN rm -rf /root/.pyarmor

CMD ["./BPMMiddleware"]
