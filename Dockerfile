# Gunakan image dasar Alpine
FROM alpine:latest

# Install dependensi
RUN apk add --no-cache \
    g++ \
    boost-dev \
    file \
    clamav \
    clamav-daemon \
    && freshclam

# Salin file sumber ke dalam kontainer
COPY avscan.cpp /usr/src/avscan.cpp

# Compile aplikasi
RUN g++ -o /usr/local/bin/avscan /usr/src/avscan.cpp -lboost_system -lboost_filesystem -lmagic

# Set direktori kerja
WORKDIR /tmp

# Perintahkan kontainer untuk menjalankan server
CMD ["avscan"]
