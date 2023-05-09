#!/bin/bash

TZ=America/Sao_Paulo
ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

apt update
apt-get install openjdk-11-jdk -y

source /opt/esp/idf/export.sh

SONAR_SERVER_URL="https://sonarcloud.io"
SONAR_SCANNER_VERSION="4.8.0.2856"
SONAR_SCANNER_DOWNLOAD_URL="https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-$SONAR_SCANNER_VERSION-linux.zip"
SONAR_WRAPPER_DOWNLOAD_URL="$SONAR_SERVER_URL/static/cpp/build-wrapper-linux-x86.zip"
SONAR_WRAPPER_OUTPUT_DIR="build_wrapper_output"

mkdir -p $HOME/.sonar

curl -sSLo $HOME/.sonar/sonar-scanner.zip $SONAR_SCANNER_DOWNLOAD_URL
unzip -o $HOME/.sonar/sonar-scanner.zip -d $HOME/.sonar/
echo "$HOME/.sonar/sonar-scanner-$SONAR_SCANNER_VERSION-linux/bin"
export PATH="$HOME/.sonar/sonar-scanner-$SONAR_SCANNER_VERSION-linux/bin":$PATH

curl -sSLo $HOME/.sonar/build-wrapper-linux-x86.zip $SONAR_WRAPPER_DOWNLOAD_URL
unzip -o $HOME/.sonar/build-wrapper-linux-x86.zip -d $HOME/.sonar/
echo "$HOME/.sonar/build-wrapper-linux-x86"
export PATH="$HOME/.sonar/build-wrapper-linux-x86":$PATH

build-wrapper-linux-x86-64 --out-dir $SONAR_WRAPPER_OUTPUT_DIR idf.py build

if [ $? -eq 0 ]
then
  sonar-scanner --define sonar.host.url="$SONAR_SERVER_URL" \
    --define sonar.organization=$SONAR_ORGANIZATION_NAME \
    --define sonar.login=$SONAR_ORGANIZATION_TOKEN

  if [ $? -eq 0 ]
  then
    exit 0
  else
    echo "Failure scanning source code"
    exit $?
  fi
else
  echo "Failure building on Docker"
  exit $?
fi
