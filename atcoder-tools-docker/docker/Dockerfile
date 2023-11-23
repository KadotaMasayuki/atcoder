# for atcoder-tools (https://github.com/kyuridenamida/atcoder-tools/blob/stable/README.md)
# 2023/11/19(Sun)
#
# 1) put files with marked '*' below.
#   ~/
#       atcoder/(*)
#           docker/(*)
#               .vimrc(*)
#               Dockerfile(*)
#           src/(*)
#               template/(*)
#                   template.cpp(*)
#               AGC001/
#                   A/
#                       in_1.txt
#                       out_1.txt
#                       main.cpp
#                       a.out
#                   B/
#
# 2) in Dockerfile, change UID and GID to same value at your host. (shown in 'id' command)
#
# 3) and create image with following command
#
#   $ cd ~/atcoder/docker
#   $ cp ~/.vimrc ./
#   $ docker build -t atcoder-tools-cpp .  # need last '.' , do not forget !
#
# 4) and invoke container with folowing command
#
#   $ docker run --rm -it -u $UID --mount type=bind,src=${HOME}/atcoder/src,target=/atcoder-workspace -w /atcoder-workspace atcoder-tools-cpp /bin/bash
#
# 5) and do some works
#
#   /atcoder-workspace $ ac-gen AGC001
#                         username:
#                         password:
#                         * once authorized, there is no need for the next input untill exit container.
#   /atcoder-workspace $ cd AGC001/A
#   /atcoder-workspace $ vi main.cpp
#   /atcoder-workspace $ ac-g++ main.cpp
#   /atcoder-workspace $ ac-test
#   /atcoder-workspace $ ac-submit
#   /atcoder-workspace $ exit
#
#   * /atcoder-workspace at container is bind to ~/atcoder/src at host
#
# *) sample template, algorithm
#
#RUN curl -s -S https://gist.githubusercontent.com/gghatano/1aab64239be88181d0fc91069c6fe9b4/raw/625a707e7b0c38777e5b8e9984871481243a8597/template.cpp >> /root/.atcodertools/template/template.cpp
#RUN curl -s -S https://gist.githubusercontent.com/gghatano/1aab64239be88181d0fc91069c6fe9b4/raw/625a707e7b0c38777e5b8e9984871481243a8597/zzz_algorithm.cpp >> /root/atcoder-workspace/algorighm.cpp
#


FROM ubuntu:20.04


RUN apt-get update

# set time-zone because of apt-get needs correct time-stamp
RUN apt-get install -y tzdata
ENV TZ='Asia/Tokyo'

# and get some package
RUN apt-get install -y --no-install-recommends apt-utils git iproute2 procps lsb-release
RUN apt-get install -y sudo wget curl vim
RUN apt-get install -y gcc-9 g++-9 gdb
RUN apt-get install -y python3.9 python3-pip
RUN apt-get clean -y
RUN rm -rf /var/lib/apt/lists/*

# AtCoder Library
RUN cd /tmp \
    && git clone https://github.com/atcoder/ac-library.git 


# markupsafe 2.1.3 is not good for 'atcoder-tools gen' command, it do not work. delete this version and install 2.0.1
RUN pip3 install markupsafe==2.0.1 --force-reinstall

# atcoder-tools
RUN pip3 install atcoder-tools


# user & group
# set UID and GID to same value to your value at host os if you care about security.
# do not worry about USERNAME and GROUPNAME. keep it as it is.
ARG USERNAME=acuser
ARG GROUPNAME=acgroup
ARG UID=1000
ARG GID=1000
RUN groupadd -g $GID $GROUPNAME && \
    useradd -m -s /bin/bash -u $UID -g $GID $USERNAME
USER $USERNAME
WORKDIR /home/${USERNAME}/


# alias
RUN echo 'alias ac-gen="atcoder-tools gen --workspace=/atcoder-workspace --template /atcoder-workspace/template/template.cpp --lang=cpp"' >> /home/${USERNAME}/.bashrc
RUN echo 'alias ac-gen-nologin="atcoder-tools gen --workspace=/atcoder-workspace --template /atcoder-workspace/template/template.cpp --lang=cpp --without-login"' >> /home/${USERNAME}/.bashrc
RUN echo 'alias ac-g++="g++-9 -O0 -std=c++17 -Wall -Wextra -Wconversion -Wshadow -Wfloat-equal -ftrapv -fsanitize=undefined,address -fno-omit-frame-pointer -fno-sanitize-recover -g -I /tmp/ac-library"' >> /home/${USERNAME}/.bashrc
RUN echo 'alias ac-test="atcoder-tools test"' >> /home/${USERNAME}/.bashrc
RUN echo 'alias ac-submit="atcoder-tools submit -f -u"' >> /home/${USERNAME}/.bashrc


# copy my vimrc
COPY .vimrc /home/${USERNAME}/.vimrc


CMD ["/bin/bash"]

