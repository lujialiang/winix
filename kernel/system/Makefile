INCLUDE_DIR = ../../include
INCLUDES = $(wildcard $(INCLUDE_DIR)/*.h $(INCLUDE_DIR)/winix/*.h \
				$(INCLUDE_DIR)/sys/*.h $(INCLUDE_DIR)/kernel/*.h)


PROGS = do_alarm do_brk do_execve do_exit do_vfork do_fork \
		do_sigaction do_sigreturn do_wait do_kill do_times\
		do_winfo do_printf do_getc do_getpid do_sysconf
		
SOURCES = $(PROGS:=.c)
ASSEMBLY = $(PROGS:=.s)
OBJECT = $(PROGS:=.o)

MAKEDEPEND = gcc -M $(CPPFLAGS) -o $*.Td $<

system: $(OBJECT)
$(ASSEMBLY): %.s: %.c $(INCLUDES)
	$(Q)wcc $(CFLAGS) -S -N -I$(INCLUDE_DIR) -o $@ $<
ifeq ($(KBUILD_VERBOSE),0)
	@echo "CC \t $<"
endif

$(OBJECT): %.o: %.s
	$(Q)wasm $< -o $@
ifeq ($(KBUILD_VERBOSE),0)
	@echo "AS \t $<"
endif

depend:
	makedepend -I$(INCLUDE_DIR) $(SOURCES)

clean:
	$(Q)-rm -f $(OBJECT) $(ASSEMBLY)
ifeq ($(KBUILD_VERBOSE),0)
	@echo "RM \t KERNEL/SYSTEM"
endif

.DELETE_ON_ERROR:# DO NOT DELETE

