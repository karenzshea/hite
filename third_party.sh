#!/usr/bin/env bash

set -eu
set -x
set -o pipefail

DIR="`dirname $0`/third_party"

CATCH_HEADER="catch.hpp"
CATCH_VERSION="v2.3.0"

mkdir -p ${DIR}/catch2
curl -L "https://github.com/catchorg/Catch2/releases/download/${CATCH_VERSION}/${CATCH_HEADER}" > ${DIR}/catch2/${CATCH_HEADER}
