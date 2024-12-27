MODULE_DEST_DIR=../bin
MODULES = interceptor serial-bridge

buildmodules:
	@echo "Build Kernel Modules"
	@for m in $(MODULES); do \
		$(MAKE) -C $$m || exit $$?; \
		cp $$m/*.ko $(MODULE_DEST_DIR); \
	done

cleanmodules:
	@echo "Clean Kernel Modules"
	@for m in $(MODULES); do \
		$(MAKE) -C $$m clean || exit $$?; \
		rm -rf $(MODULE_DEST_DIR)/*.ko; \
	done

all: buildmodules

clean: cleanmodules
