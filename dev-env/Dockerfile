FROM ubuntu:24.04

ENV DEBIAN_FRONTEND noninteractive
ENV TZ Europe/Moscow

RUN apt-get update && \
    apt-get install -y \
        apt-utils tzdata whiptail \
        man-db less \
        unminimize && \
    yes | unminimize

COPY install_deps.sh /
RUN bash install_deps.sh && rm install_deps.sh

WORKDIR /root

COPY .vimrc /root/.vimrc

EXPOSE 22
RUN echo 'root:root' | chpasswd && \
    sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config && \
    mkdir /var/run/sshd

CMD ["/usr/sbin/sshd", "-D"]

