
set -e -x

apt-get install -y \
    openssh-server \
    vim htop tree acl glibc-doc bash-completion \
    clang clangd lldb lld \
    make gcc python3 python3-pip python3-venv libpcre3-dev \
    gcc-aarch64-linux-gnu qemu-user gdb-multiarch \
    gdb binutils strace valgrind linux-tools-generic systemd-coredump \
    iproute2 netcat-openbsd tcpdump lsof iputils-ping bind9-dnsutils whois curl wget arp-scan nmap

wget https://go.dev/dl/go1.25.2.linux-amd64.tar.gz
tar -C /usr/local -xzf go1.25.2.linux-amd64.tar.gz
rm -f go1.25.2.linux-amd64.tar.gz
echo 'export PATH=$PATH:/usr/local/go/bin' >> /etc/profile
