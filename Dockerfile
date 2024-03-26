FROM ubuntu as base

RUN apt update -y && apt install -y cmake && apt install -y build-essential && apt install -y g++