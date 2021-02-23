FROM ubuntu:18.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get -y update && apt-get -y upgrade
RUN apt-get -y install php-cgi git make clang

COPY ./entrypoint.sh	.

ENTRYPOINT sh entrypoint.sh

EXPOSE 8080 8888
