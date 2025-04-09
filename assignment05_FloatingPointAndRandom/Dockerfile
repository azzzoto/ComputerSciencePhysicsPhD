FROM almalinux:latest

# Install dependencies
RUN dnf update -y && \
    dnf groupinstall -y "Development Tools" && \
    dnf install -y --allowerasing \
    wget \
    git \
    make \
    gcc \
    gcc-c++ \
    curl \
    tar \
    gzip \
    which \
    nano \
    vim

# Install GSL 2.7.1
WORKDIR /usr/local/src
RUN curl -LO https://ftp.gnu.org/gnu/gsl/gsl-2.7.1.tar.gz && \
    tar -xzf gsl-2.7.1.tar.gz && \
    cd gsl-2.7.1 && \
    ./configure --prefix=/usr/local/gsl-2.7.1 && \
    make -j$(nproc) && \
    make install

# Configure environment variables for GSL
ENV PATH=/usr/local/gsl-2.7.1/bin:$PATH
ENV LD_LIBRARY_PATH=/usr/local/gsl-2.7.1/lib:$LD_LIBRARY_PATH
ENV C_INCLUDE_PATH=/usr/local/gsl-2.7.1/include:$C_INCLUDE_PATH
ENV LIBRARY_PATH=/usr/local/gsl-2.7.1/lib:$LIBRARY_PATH

# Install Julia
RUN curl -LO https://julialang-s3.julialang.org/bin/linux/x64/1.9/julia-1.9.3-linux-x86_64.tar.gz && \
    tar -xzf julia-1.9.3-linux-x86_64.tar.gz -C /opt && \
    ln -s /opt/julia-1.9.3/bin/julia /usr/local/bin/julia

# Clone the repository and go to the correct folder
WORKDIR /root
RUN git clone https://github.com/azzzoto/ComputerSciencePhysicsPhD.git
WORKDIR /root/ComputerSciencePhysicsPhD/assignment05_FloatingPointAndRandom

# Make the run_programs.sh script executable
RUN chmod +x run_programs.sh

# Default command when the container is started
CMD ["./run_programs.sh"] 