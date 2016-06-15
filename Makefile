TOOLS=bsc-probe i2ccat i2ccatChar i2ccatHex i2csetparm

all: $(TOOLS)
clean:
	$(RM) $(patsubst %,%.o,$(TOOLS)) \
    rPodI2C.o rI2CTX.o \
    $(TOOLS)

bsc-probe: bsc-probe.o
i2ccat: i2ccat.o rPodI2C.o
i2ccatChar: i2ccatChar.o rPodI2C.o
i2ccatHex: i2ccatHex.o rPodI2C.c
i2csetparm: i2csetparm.o rI2CTX.o
