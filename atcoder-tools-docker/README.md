# atcoder-toolsをdockerで使う

atcoder-tools

https://github.com/kyuridenamida/atcoder-tools/blob/stable/README.md


## docker内にatcoder-toolsを構成して、以下のように使う。

コンテナ起動し中に入る

genコマンドでコンテストの問題を取得する
コンテナを起動した最初のときにログイン情報を入力する。ログインに成功すれば、コンテナを終了するまで再ログイン不要

取得したコンテストのフォルダに移動する

各問題のフォルダに移動する

main.cppをvimで編集する

testコマンドでテストする

submitコマンドで提出する


## 確認環境

Windows11 > WSL2 > Ubuntu > Docker

Windows11 Home 22H2 22621.2715

uname -a → Linux 5.15.133.1-microsoft-standard-WSL2 #1 SMP Thu Oct 5 21:02:42 UTC 2023 x86_64 x86_64 x86_64 GNU/Linux

Docker version 24.0.7, build afdd53b


## ホスト側（dockerを起動する側）のファイル構成

下記のように配置する

```
~/atcoder/docker/.vimrc
~/atcoder/docker/Dockerfile
~/atcoder/docker/start.bash
~/atcoder/src/template/template.cpp
```

`.vimrc`は、vimでソースコードを書きたい場合に必要。有無チェックしないので、空ファイルで良いので作っておく。

`template.cpp`は、問題を取得した際に各問題のディレクトリにmain.cppの名前で配置される。

配置を図で示すと以下の通り。srcディレクトリの中に各コンテストのディレクトリが取得される。

```
   ~/
       atcoder/(*)
           docker/(*)
               .vimrc(*)
               Dockerfile(*)
               start.bash(*)
           src/(*)
               template/(*)
                   template.cpp(*)
               AGC001/
                   A/
                       in_1.txt
                       out_1.txt
                       main.cpp
                       a.out
                   B/
```


## dockerイメージを作る

`~/atcoder/docker`ディレクトリに移動して、`Dockerfile`のUIDとGIDを変更する。`UID`と`GID`は`id`コマンドでわかる。
※コンテナ内とホスト内とでユーザーID/グループIDを合わせることで、root権限の乱用を避けたい。
UIDとGIDがわからなければ、初期値のままで良い。

```
ARG UID=1000
ARG GID=1000
```

以下のコマンドを打つ

```
docker build -t atcoder-tools-cpp .
```

インストールログが流れる。

```
hostuser@host:~/atcoder/docker$ docker build -t atcoder-tools-cpp .
[+] Building 2.1s (25/25) FINISHED                                                                    docker:default
 => [internal] load build definition from Dockerfile                                                            0.0s
 => => transferring dockerfile: 3.97kB                                                                          0.0s
 => [internal] load .dockerignore                                                                               0.0s
 => => transferring context: 2B                                                                                 0.0s
 => [internal] load metadata for docker.io/library/ubuntu:20.04                                                 0.0s
 => [ 1/20] FROM docker.io/library/ubuntu:20.04                                                                 0.0s
 => [internal] load build context                                                                               0.0s
 => => transferring context: 6.52kB                                                                             0.0s
 => CACHED [ 2/20] RUN apt-get update                                                                           0.0s
 => CACHED [ 3/20] RUN apt-get install -y tzdata                                                                0.0s
 => CACHED [ 4/20] RUN apt-get install -y --no-install-recommends apt-utils git iproute2 procps lsb-release     0.0s
 => CACHED [ 5/20] RUN apt-get install -y sudo wget curl vim                                                    0.0s
 => CACHED [ 6/20] RUN apt-get install -y gcc-9 g++-9 gdb                                                       0.0s
 => CACHED [ 7/20] RUN apt-get install -y python3.9 python3-pip                                                 0.0s
 => CACHED [ 8/20] RUN apt-get clean -y                                                                         0.0s
 => CACHED [ 9/20] RUN rm -rf /var/lib/apt/lists/*                                                              0.0s
 => CACHED [10/20] RUN cd /tmp     && git clone https://github.com/atcoder/ac-library.git                       0.0s
 => CACHED [11/20] RUN pip3 install markupsafe==2.0.1 --force-reinstall                                         0.0s
 => CACHED [12/20] RUN pip3 install atcoder-tools                                                               0.0s
 => CACHED [13/20] RUN groupadd -g 1000 acgroup &&     useradd -m -s /bin/bash -u 1000 -g 1000 acuser           0.0s
 => CACHED [14/20] WORKDIR /home/acuser/                                                                        0.0s
 => [15/20] RUN echo 'alias ac-gen="atcoder-tools gen --workspace=/atcoder-workspace --template /atcoder-works  0.2s
 => [16/20] RUN echo 'alias ac-gen-nologin="atcoder-tools gen --workspace=/atcoder-workspace --template /atcod  0.4s
 => [17/20] RUN echo 'alias ac-g++="g++-9 -O0 -std=c++17 -Wall -Wextra -Wconversion -Wshadow -Wfloat-equal -ft  0.4s
 => [18/20] RUN echo 'alias ac-test="atcoder-tools test"' >> /home/acuser/.bashrc                               0.4s
 => [19/20] RUN echo 'alias ac-submit="atcoder-tools submit -f -u"' >> /home/acuser/.bashrc                     0.5s
 => [20/20] COPY .vimrc /home/acuser/.vimrc                                                                     0.0s
 => exporting to image                                                                                          0.1s
 => => exporting layers                                                                                         0.1s
 => => writing image sha256:7eecad31ced052468796d3802d665c17efabea1de6ad4ce3597ceda975dce6f5                    0.0s
 => => naming to docker.io/library/atcoder-tools-cpp                                                            0.0s
```

成功したら、`docker image list`で`atcoder-tools-cpp`が出来てるか確認する。

```
hostuser@host:~/atcoder/docker$ docker image list
REPOSITORY             TAG       IMAGE ID       CREATED             SIZE
atcoder-tools-cpp      latest    7eecad31ced0   About an hour ago   676MB
```

できていたら、次のコマンドを打ってコンテナを起動する

```
docker run --rm -it -u $UID --mount type=bind,src=${HOME}/atcoder/src,target=/atcoder-workspace -w /atcoder-workspace atcoder-tools-cpp /bin/bash
```

または、スクリプトで起動する

```
hostuser@host:~/atcoder/docker$ bash start.bash
```

コマンドプロンプトが次のようになっていたら成功

```
acuser@xxxxxxxxxx:/atcoder-workspace$ 
```


## コンテナ内の構成

`/atcoder-workspace`がホストの`~/atcoder/src`を示す。


## コンテナ内のコマンド

コマンドが長ったらしいのでalias設定している。

```
alias ac-g++='g++-9 -O0 -std=c++17 -Wall -Wextra -Wconversion -Wshadow -Wfloat-equal -ftrapv -fsanitize=undefined,address -fno-omit-frame-pointer -fno-sanitize-recover -g -I /tmp/ac-library'
alias ac-gen='atcoder-tools gen --workspace=/atcoder-workspace --template /atcoder-workspace/template/template.cpp --lang=cpp'
alias ac-gen-nologin='atcoder-tools gen --workspace=/atcoder-workspace --template /atcoder-workspace/template/template.cpp --lang=cpp --without-login'
alias ac-submit='atcoder-tools submit -f -u'
alias ac-test='atcoder-tools test'
```


### `gen`コマンドでコンテストの問題を取得する

`ac-gen`または`ac-gen-nologin`を使う。基本は`ac-gen`で良いと思う。

```
acuser@xxxxxxxxxx:/atcoder-workspace$ ac-gen abc326
2023-11-23 11:15:36,209 INFO: Going to load /usr/local/lib/python3.8/dist-packages/atcodertools/tools/atcodertools-default.toml as config
AtCoder username: atcoder-user-name
AtCoder password:
2023-11-23 11:15:45,679 INFO: Saved session into /home/acuser/.local/share/atcoder-tools/cookie.txt
2023-11-23 11:15:45,679 INFO: Login successful.
=================================================
2023-11-23 11:15:45,911 INFO: Problem A: /atcoder-workspace/template/template.cpp is used for template
2023-11-23 11:15:46,023 INFO: Problem A: Created examples.
2023-11-23 11:15:46,023 INFO: Problem A: Format prediction succeeded
2023-11-23 11:15:46,099 INFO: Problem A: Saved code to /atcoder-workspace/abc326/A/main.cpp
2023-11-23 11:15:46,099 INFO: Problem A: Saved metadata to /atcoder-workspace/abc326/A/metadata.json
=================================================
2023-11-23 11:15:46,099 INFO: Problem B: /atcoder-workspace/template/template.cpp is used for template
2023-11-23 11:15:46,225 INFO: Problem B: Created examples.
2023-11-23 11:15:46,225 INFO: Problem B: Format prediction succeeded
2023-11-23 11:15:46,306 INFO: Problem B: Saved code to /atcoder-workspace/abc326/B/main.cpp
2023-11-23 11:15:46,306 INFO: Problem B: Saved metadata to /atcoder-workspace/abc326/B/metadata.json
=================================================
2023-11-23 11:15:46,306 INFO: Problem C: /atcoder-workspace/template/template.cpp is used for template
2023-11-23 11:15:46,448 INFO: Problem C: Created examples.
2023-11-23 11:15:46,449 INFO: Problem C: Format prediction succeeded
2023-11-23 11:15:46,527 INFO: Problem C: Saved code to /atcoder-workspace/abc326/C/main.cpp
2023-11-23 11:15:46,527 INFO: Problem C: Saved metadata to /atcoder-workspace/abc326/C/metadata.json
=================================================
2023-11-23 11:15:46,527 INFO: Problem D: /atcoder-workspace/template/template.cpp is used for template
2023-11-23 11:15:46,641 INFO: Problem D: Created examples.
2023-11-23 11:15:46,642 INFO: Problem D: Format prediction succeeded
2023-11-23 11:15:46,738 INFO: Problem D: Saved code to /atcoder-workspace/abc326/D/main.cpp
2023-11-23 11:15:46,738 INFO: Problem D: Saved metadata to /atcoder-workspace/abc326/D/metadata.json
=================================================
2023-11-23 11:15:46,738 INFO: Problem E: /atcoder-workspace/template/template.cpp is used for template
2023-11-23 11:15:46,871 INFO: Problem E: Created examples.
2023-11-23 11:15:46,872 INFO: Problem E: Format prediction succeeded
2023-11-23 11:15:46,958 INFO: Problem E: Saved code to /atcoder-workspace/abc326/E/main.cpp
2023-11-23 11:15:46,958 INFO: Problem E: Saved metadata to /atcoder-workspace/abc326/E/metadata.json
=================================================
2023-11-23 11:15:46,958 INFO: Problem F: /atcoder-workspace/template/template.cpp is used for template
2023-11-23 11:15:47,090 INFO: Problem F: Created examples.
2023-11-23 11:15:47,091 INFO: Problem F: Format prediction succeeded
2023-11-23 11:15:47,197 INFO: Problem F: Saved code to /atcoder-workspace/abc326/F/main.cpp
2023-11-23 11:15:47,197 INFO: Problem F: Saved metadata to /atcoder-workspace/abc326/F/metadata.json
=================================================
2023-11-23 11:15:47,197 INFO: Problem G: /atcoder-workspace/template/template.cpp is used for template
2023-11-23 11:15:47,331 INFO: Problem G: Created examples.
2023-11-23 11:15:47,333 INFO: Problem G: Format prediction succeeded
2023-11-23 11:15:47,426 INFO: Problem G: Saved code to /atcoder-workspace/abc326/G/main.cpp
2023-11-23 11:15:47,426 INFO: Problem G: Saved metadata to /atcoder-workspace/abc326/G/metadata.json
=================================================
acuser@xxxxxxxxxx:/atcoder-workspace$ ls
abc326  template
acuser@xxxxxxxxxx:/atcoder-workspace$ ls abc001/
A  B  C  D  E  F  G
```


### ソースコードを編集する

```
acuser@xxxxxxxxxx:/atcoder-workspace/abc326/A$ vim main.cpp
```

または、ホストPCの`~/atcoder/src/agc326/A/main.cpp`を好みのエディタで編集する。


### コンパイルする

`ac-g++`でソースコードを指定してコンパイルする。

```
acuser@xxxxxxxxxx:/atcoder-workspace/abc326/A$ ac-g++ main.cpp
acuser@xxxxxxxxxx:/atcoder-workspace/abc326/A$ ./a.out
Yes
```


### テストする

上記の`Yes`としか出力しない`main.cpp`をそのままコンパイルしてテストしてみると以下の通り。

```
acuser@167d78aeb71f:/atcoder-workspace/abc326/A$ ac-test
2023-11-23 12:10:04,376 INFO: Inferred exec file: ./a.out
# in_1.txt ... WA
[Input]
1 4
[Expected]
No
[Received]
Yes


# in_2.txt ... PASSED 9 ms
# in_3.txt ... WA
[Input]
100 1
[Expected]
No
[Received]
Yes
```


### 提出する

動作未確認


