#!/bin/bash
TOP_DIR="$(cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd -P)"
export APOLLO_ROOT_DIR=${PWD}
export APOLLO_BAZEL_DIST_DIR="${APOLLO_CACHE_DIR}/distdir"
export CYBER_PATH="${APOLLO_ROOT_DIR}/cyber"

export PYTHONPATH=${bazel_bin_path}/cyber/python/internal:${PYTHONPATH}

export CYBER_DOMAIN_ID=80
export CYBER_IP=127.0.0.1

echo ${APOLLO_ROOT_DIR}
export GLOG_log_dir="/tmp/data/log"
export GLOG_alsologtostderr=1
export GLOG_colorlogtostderr=1
export GLOG_minloglevel=0

export sysmo_start=0

# for DEBUG log
#export GLOG_v=4
