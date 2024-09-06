# Makefile to call other makefiles (TCF and TSL)

TCF_DIR = TCF

.PHONY: all clean tcf

all:
	@$(MAKE) -C $(TCF_DIR)

clean:
	@$(MAKE) -C $(TSL_DIR) clean
	@$(MAKE) -C $(TCF_DIR) clean

tcf:
	@$(MAKE) -C $(TCF_DIR)