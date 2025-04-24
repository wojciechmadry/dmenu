UID := $(shell id -u)
GID := $(shell id -g)
DOCKERFILE ?= Dockerfile.u24
DOCKERFILEPATH = dockerfiles/${DOCKERFILE}
CXX ?= g++
CC ?= gcc

BUILD_ARTIFACTS = build/${DOCKERFILE}
IMAGE_NAME = dmenu_$(shell echo ${DOCKERFILEPATH} | cut -d'.' -f2)
BUILD_UNIT = ${BUILD_ARTIFACTS}/unit_${CXX}
BUILD = ${BUILD_ARTIFACTS}/${CXX}
WORKDIR=dmenu

help:
	@echo "Help - TBD"

docker_build:
	mkdir -p ${BUILD}
	docker build --build-arg UID=${UID} --build-arg GID=${GID} -f ${DOCKERFILEPATH} -t ${IMAGE_NAME} .

build: docker_build
	mkdir -p ${BUILD}
	docker run --rm -v "${PWD}:/${WORKDIR}" --name ${IMAGE_NAME} ${IMAGE_NAME} /bin/bash -c \
	"cd /${WORKDIR}/${BUILD} && \
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=${CXX} -DCMAKE_C_COMPILER=${CC} /${WORKDIR}  &&\
	make -j"

--clean_gcda:
	find ${BUILD_UNIT} -iname "*.gcda" | xargs -d"\n" rm | true

unit: docker_build --clean_gcda
	mkdir -p ${BUILD_UNIT}
	docker run --rm -v "${PWD}:/dmenu" --name ${IMAGE_NAME} ${IMAGE_NAME} /bin/bash -c \
	"cd /${WORKDIR}/${BUILD_UNIT} && \
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=${CXX} -DCMAKE_C_COMPILER=${CC} -DBUILD_TESTING=True /${WORKDIR}  &&\
	make -j && \
	env CTEST_OUTPUT_ON_FAILURE=1 \
	make test && \
	./tests/drw_tests"

coverage: unit
	docker run --rm -v "${PWD}:/${WORKDIR}" --name ${IMAGE_NAME} ${IMAGE_NAME} /bin/bash -c \
	"cd /${WORKDIR}/${BUILD_UNIT} && \
	make coverage"

valgrind: unit
	docker run --rm -v "${PWD}:/${WORKDIR}" --name ${IMAGE_NAME} ${IMAGE_NAME} /bin/bash -c \
	"cd /${WORKDIR}/${BUILD_UNIT} && \
	valgrind --tool=memcheck --error-exitcode=1 --trace-children=yes ./tests/drw_tests &&\"
	valgrind --tool=memcheck --error-exitcode=1 --trace-children=yes ./tests/dmenu_tests"

.PHONY: build valgrind coverage unit --clean_gcda docker_build

