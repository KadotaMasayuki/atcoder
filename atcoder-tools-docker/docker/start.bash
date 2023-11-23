#!/bin/bash
docker run --rm -it -u $UID --mount type=bind,src=${HOME}/atcoder/src,target=/atcoder-workspace -w /atcoder-workspace atcoder-tools-cpp /bin/bash

