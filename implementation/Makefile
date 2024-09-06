# Makefile to call other makefiles (TCF and TSL)

TSL_DIR = TSL
TCF_DIR = TCF

.PHONY: all clean tsl tcf

all:
	@$(MAKE) -C $(TSL_DIR)
	@$(MAKE) -C $(TCF_DIR)

clean:
	@$(MAKE) -C $(TSL_DIR) clean
	@$(MAKE) -C $(TCF_DIR) clean

tsl:
	@$(MAKE) -C $(TSL_DIR)

tcf:
	@$(MAKE) -C $(TCF_DIR)