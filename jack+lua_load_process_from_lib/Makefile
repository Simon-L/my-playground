OUTDIR=build
LIB_PROCESS=$(OUTDIR)/process.so
LIB_BASE=$(OUTDIR)/base.so

dirs:
	mkdir -p build

$(LIB_PROCESS): dirs
	$(CC) process.c -fPIC -shared -lm -ljack -o $@
	
$(LIB_BASE): dirs
	$(CC) base.c -fPIC -shared -lm -ljack -o $@

libs: $(LIB_PROCESS) $(LIB_BASE)

lua: clean libs 
	luajit main.lua ./build/process.so

clean:
	rm -rf build