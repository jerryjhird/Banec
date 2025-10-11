PREFIX := $(CURDIR)/dist
BUILD_DIR := build
COMPILER ?= clang
LINKER ?= lld
ENVV := CC=$(COMPILER) CC_LD=$(LINKER)
DEBUG_MODE := true

.PHONY: all setup build release fix clean strip

all: build

setup:
	rm -rf $(BUILD_DIR)
	$(ENVV) meson setup $(BUILD_DIR) --prefix=$(PREFIX) -Ddebug_mode=$(DEBUG_MODE)

build:
	$(ENVV) meson compile -C $(BUILD_DIR)
	$(ENVV) meson install -C $(BUILD_DIR)

	@bin_dir="$(PREFIX)/bin"; \
	testutil_path="$${bin_dir}/testutil"; \
	if [ -f "$$testutil_path" ]; then \
		mv "$$testutil_path" "$${bin_dir}/test"; \
	fi


release:
	@$(MAKE) build DEBUG_MODE=false

reconf:
	rm -rf $(BUILD_DIR)
	$(MAKE) setup DEBUG_MODE=$(DEBUG_MODE)

clean:
	rm -rf $(BUILD_DIR) $(PREFIX)

strip:
	@find "$(PREFIX)/bin" -type f -perm -111 2>/dev/null | while read -r f; do \
		strip "$$f" >/dev/null 2>&1; \
		printf "%s : %d\n" "$$f" $$?; \
	done

lint:
	@run-clang-tidy -p build -quiet
