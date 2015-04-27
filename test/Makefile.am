check_PROGRAMS = mem register

register_SOURCES = register.c
register_CFLAGS = -Wall $(SOFIA_CFLAGS) $(CHECK_CFLAGS)
register_LDADD = $(top_builddir)/src/evsip-evsip_mem.o $(SOFIA_LIBS) $(CHECK_LIBS)

mem_SOURCES = mem.c
mem_CFLAGS = -Wall $(SOFIA_CFLAGS) $(CHECK_CFLAGS)
mem_LDADD = $(top_builddir)/src/evsip-evsip_mem.o $(SOFIA_LIBS) $(CHECK_LIBS)


TESTS = $(check_PROGRAMS)
