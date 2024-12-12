FROM ubuntu:24.04

WORKDIR /root

ENV DEBIAN_FRONTEND noninteractive
ENV TZ Europe/Moscow

RUN apt-get update && \
    apt-get install -y apt-utils && \
    apt-get install -y man-db && \
    apt-get install -y unminimize && \
    yes | unminimize && \
    apt-get install -y openssh-server && \
    apt-get install -y vim htop glibc-doc && \
    apt-get install -y make gcc python3 python3-pip libpcre3-dev && \
    apt-get install -y gcc-aarch64-linux-gnu qemu-user gdb-multiarch && \
    apt-get install -y gdb binutils strace valgrind && \
    apt-get install -y iproute2 netcat-openbsd tcpdump lsof iputils-ping bind9-dnsutils whois curl wget arp-scan nmap

COPY .vimrc /root/.vimrc

EXPOSE 22
RUN echo 'root:root' | chpasswd && \
    sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config && \
    mkdir /var/run/sshd

CMD ["/usr/sbin/sshd", "-D"]

