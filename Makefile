UID := $(shell id -u)
GID := $(shell id -g)
DOCKERFILE ?= Dockerfile.u24
DOCKERFILEPATH = dockerfiles/${DOCKERFILE}

BUILD = build/${DOCKERFILE}
IMAGE_NAME = dmenu_$(shell echo ${DOCKERFILEPATH} | cut -d'.' -f2)
BUILD_UNIT_GCC = ${BUILD}/unit_gcc
BUILD_GCC = ${BUILD}/gcc


help:
	@echo "Help - TBD"

build:
	mkdir -p ${BUILD}
	docker build --build-arg UID=${UID} --build-arg GID=${GID} -f ${DOCKERFILEPATH} -t ${IMAGE_NAME} .

gcc: build
	mkdir -p ${BUILD_GCC}
	docker run --rm -v "${PWD}:/ws" --name ${IMAGE_NAME} ${IMAGE_NAME} /bin/bash -c \
	"cd /ws/${BUILD_GCC} && \
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc /ws  &&\
	make -j"

--clean_gcda:
	find ${BUILD_UNIT_GCC} -iname "*.gcda" | xargs -d"\n" rm | true

unit_gcc: build --clean_gcda
	mkdir -p ${BUILD_UNIT_GCC}
	docker run --rm -v "${PWD}:/ws" --name ${IMAGE_NAME} ${IMAGE_NAME} /bin/bash -c \
	"cd /ws/${BUILD_UNIT_GCC} && \
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -DBUILD_TESTING=True /ws  &&\
	make -j && \
	env CTEST_OUTPUT_ON_FAILURE=1 \
	make test && \
	./tests/drw_tests"

coverage: unit_gcc
	docker run --rm -v "${PWD}:/ws" --name ${IMAGE_NAME} ${IMAGE_NAME} /bin/bash -c \
	"cd /ws/${BUILD_UNIT_GCC} && \
	make coverage"

.PHONY: build

