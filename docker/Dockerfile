FROM ubuntu:xenial

RUN apt-get update && \
    apt-get install --no-install-recommends -q -y grub-common grub-pc-bin xorriso && \
    rm -rf /var/lib/apt/lists/*

ENTRYPOINT ["/usr/bin/grub-mkrescue"]

# docker build -t=grub:latest .
# docker run -it --rm -v $(pwd):$(pwd) -w $(pwd) -u `id -u $USER` grub:latest -o kernel.iso iso
