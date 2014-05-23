REPO		?= doric
PKG_REVISION    ?= $(shell git describe --tags)
PKG_VERSION	?= $(shell git describe --tags | tr - .)
PKG_ID           = doric-$(PKG_VERSION)
PKG_BUILD        = 1
BASE_DIR         = $(shell pwd)
ERLANG_BIN       = $(shell dirname $(shell which erl))
REBAR           ?= $(BASE_DIR)/rebar
OVERLAY_VARS    ?=
HTTP_PORT    ?= 8080

.PHONY: rel stagedevrel deps test depgraph graphviz all compile

all: compile

compile: deps
	@(./rebar compile)

deps:
	@./rebar get-deps

##
## Lock Targets
##
##  see https://github.com/seth/rebar_lock_deps_plugin
lock: deps compile
	./rebar lock-deps

locked-all: locked-deps compile

locked-deps:
	@echo "Using rebar.config.lock file to fetch dependencies"
	./rebar -C rebar.config.lock get-deps

clean:
	@./rebar clean

distclean: clean
	@./rebar delete-deps
	@rm -rf $(PKG_ID).tar.gz

##
## Release targets
##
rel: deps compile
	@./rebar generate skip_deps=true $(OVERLAY_VARS)

relclean:
	rm -rf rel/doric

##
## Developer targets
##
stage : rel
	$(foreach dep,$(wildcard deps/*), rm -rf rel/doric/lib/$(shell basename $(dep))-* && ln -sf $(abspath $(dep)) rel/doric/lib;)
	$(foreach app,$(wildcard apps/*), rm -rf rel/doric/lib/$(shell basename $(app))-* && ln -sf $(abspath $(app)) rel/doric/lib;)

devclean: clean
	rm -rf dev

DIALYZER_APPS = kernel stdlib sasl erts ssl tools os_mon runtime_tools crypto inets \
	webtool eunit syntax_tools compiler
PLT ?= $(HOME)/.doric_dialyzer_plt

##
## Packaging targets
##
.PHONY: package
export PKG_VERSION PKG_ID PKG_BUILD BASE_DIR ERLANG_BIN REBAR OVERLAY_VARS RELEASE

package.src: deps
	mkdir -p package
	rm -rf package/$(PKG_ID)
	git archive --format=tar --prefix=$(PKG_ID)/ $(PKG_REVISION)| (cd package && tar -xf -)
	cp rebar.config.script package/$(PKG_ID)
	make -C package/$(PKG_ID) deps
	mkdir -p package/$(PKG_ID)/priv
	git --git-dir=.git describe --tags >package/$(PKG_ID)/priv/vsn.git
	for dep in package/$(PKG_ID)/deps/*; do \
             echo "Processing dep: $${dep}"; \
             mkdir -p $${dep}/priv; \
             git --git-dir=$${dep}/.git describe --tags >$${dep}/priv/vsn.git; \
        done
	find package/$(PKG_ID) -depth -name ".git" -exec rm -rf {} \;
	tar -C package -czf package/$(PKG_ID).tar.gz $(PKG_ID)

dist: package.src
	cp package/$(PKG_ID).tar.gz .

package: package.src
	make -C package -f $(PKG_ID)/deps/node_package/Makefile

pkgclean: distclean
	rm -rf package

include tools.mk
