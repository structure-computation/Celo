MCP = metil_comp -O3 -g3 --no-sep-libs -Isrc
PRG = tests/test_Http.cpp
INSTALL_DIR = `pwd`

all: cmp

cmp: inst_dir ext/Sipe
	make -C ext/Sipe
	${MCP} -ne -o -DSCEEN listener_generator src/Celo/listener_generator.cpp

test_%:
	${MCP} tests/$@.cpp

ext/Sipe:
	mkdir -p ext; cd ext; test -e Sipe || ( test -e ../../Sipe && ln -s `pwd`/../../Sipe . ) || git clone git@github.com:structure-computation/Sipe.git

exe:
	${MCP} ${PRG}

inst_dir:
	export D="#define INSTALL_DIR \"${INSTALL_DIR}\""; grep "$$D" src/Celo/InstallDir.h || echo "$$D" > src/Celo/InstallDir.h

clean:
	rm listener_generator
	rm src/Celo/compilations/*

.PHONY: inst_dir

